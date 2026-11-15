#include "voice_controls.h"
#include "vosk_api.h"
#include <alsa/asoundlib.h>
#include <stdio.h>
#include <string.h>

static snd_pcm_t *microphone = NULL;
static VoskModel *model = NULL;
static VoskRecognizer *recognizer = NULL;

static char grammar[65536];

static void build_grammar(void) {
    const char *pieces[] = {
        "pawn", "knight", "bishop", "rook", "queen", "king"
    };
    const char *files[] = {
        "a", "b", "c", "d", "e", "f", "g", "h"
    };
    const char *numbers[] = {
        "one", "two", "three", "four",
        "five", "six", "seven", "eight"
    };

    size_t pos = 0;
    size_t capacity = sizeof(grammar);
    grammar[pos++] = '[';

    for (int p = 0; p < 6; p++) {
        for (int f = 0; f < 8; f++) {
            for (int n = 0; n < 8; n++) {
                int written = snprintf(
                    grammar + pos, capacity - pos,
                    "%s\"%s to %s %s\"",
                    pos > 1 ? "," : "",
                    pieces[p], files[f], numbers[n]
                );

                if (written < 0 ||
                    (size_t)written >= capacity - pos) {
                    grammar[0] = '\0';
                    return;
                }
                pos += (size_t)written;
            }
        }
    }

    int written = snprintf(
        grammar + pos, capacity - pos,
        ",\"start game\",\"end game\","
        "\"long castle\",\"short castle\",\"resign\","
        "\"queen\",\"knight\",\"bishop\",\"rook\"]"
    );

    if (written < 0 ||
        (size_t)written >= capacity - pos) {
        grammar[0] = '\0';
    }
}

void free_voice(void) {
    if (microphone) {
        snd_pcm_close(microphone);
        microphone = NULL;
    }
    if (recognizer) {
        vosk_recognizer_free(recognizer);
        recognizer = NULL;
    }
    if (model) {
        vosk_model_free(model);
        model = NULL;
    }
}

bool init_voice(const char *path) {
    vosk_set_log_level(-1);

    model = vosk_model_new(path);
    if (!model) {
        fprintf(stderr, "Voice: cannot load model\n");
        return false;
    }

    build_grammar();
    if (!grammar[0]) {
        free_voice();
        return false;
    }

    recognizer = vosk_recognizer_new_grm(
        model, 16000.0f, grammar
    );

    if (!recognizer) {
        free_voice();
        return false;
    }

    int err = snd_pcm_open(
        &microphone,
        "plughw:CARD=Wireless,DEV=0",
        SND_PCM_STREAM_CAPTURE,
        SND_PCM_NONBLOCK
    );

    if (err < 0) {
        fprintf(stderr, "Voice: %s\n", snd_strerror(err));
        free_voice();
        return false;
    }

    err = snd_pcm_set_params(
        microphone,
        SND_PCM_FORMAT_S16_LE,
        SND_PCM_ACCESS_RW_INTERLEAVED,
        1, 16000, 1, 100000
    );

    if (err < 0) {
        fprintf(stderr, "Voice: %s\n", snd_strerror(err));
        free_voice();
        return false;
    }

    return true;
}

bool process_audio(char *out, size_t capacity) {
    if (!microphone || !recognizer || !out || capacity < 2)
        return false;

    out[0] = '\0';

    short audio[2048];
    snd_pcm_sframes_t count =
        snd_pcm_readi(microphone, audio, 2048);

    if (count == -EAGAIN)
        return false;

    if (count < 0) {
        snd_pcm_recover(microphone, (int)count, 1);
        return false;
    }

    if (count == 0)
        return false;

    if (vosk_recognizer_accept_waveform_s(
            recognizer, audio, (int)count) != 1)
        return false;

    const char *json = vosk_recognizer_result(recognizer);
    const char *p = json ? strstr(json, "\"text\"") : NULL;

    if (!p || !(p = strchr(p, ':')) ||
        !(p = strchr(p, '"')))
        return false;

    p++;
    size_t n = 0;

    while (p[n] && p[n] != '"' && n + 1 < capacity) {
        out[n] = p[n];
        n++;
    }
    out[n] = '\0';

    if (n) {
        printf("Voice: %s\n", out);
        return true;
    }

    return false;
}
