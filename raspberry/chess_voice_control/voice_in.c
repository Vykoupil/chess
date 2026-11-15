
#include "voice_controls.h"
#include "vosk_api.h"
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <string.h>

#define AUDIO_BUFFERS 4
#define SAMPLES 2048

static HMODULE dll;
static HWAVEIN microphone;
static WAVEHDR headers[AUDIO_BUFFERS];
static short audio[AUDIO_BUFFERS][SAMPLES];

static VoskModel *model;
static VoskRecognizer *recognizer;

static VoskModel *(*load_model)(const char *);
static void (*release_model)(VoskModel *);
static VoskRecognizer *(*make_recognizer)(VoskModel *, float, const char *);
static void (*release_recognizer)(VoskRecognizer *);
static int (*accept_audio)(VoskRecognizer *, const short *, int);
static const char *(*get_result)(VoskRecognizer *);
static void (*set_log)(int);

static char chess_grammar[65536];

static void build_chess_grammar(void) {
    const char *pieces[] = {
        "pawn", "knight", "bishop", "rook", "queen", "king"
    };

    const char *files[] = {
        "a", "b", "c", "d",
        "e", "f", "g", "h"
    };

    const char *numbers[] = {
        "one", "two", "three", "four",
        "five", "six", "seven", "eight"
    };

    size_t pos = 0;
    size_t capacity = sizeof(chess_grammar);

    chess_grammar[pos++] = '[';

    for (int p = 0; p < 6; p++) {
        for (int f = 0; f < 8; f++) {
            for (int n = 0; n < 8; n++) {
                int written = snprintf(
                    chess_grammar + pos,
                    capacity - pos,
                    "%s\"%s to %s %s\"",
                    pos > 1 ? "," : "",
                    pieces[p],
                    files[f],
                    numbers[n]
                );

                if (written < 0 ||
                    (size_t)written >= capacity - pos) {
                    chess_grammar[0] = '\0';
                    return;
                }

                pos += (size_t)written;
            }
        }
    }

    int written = snprintf(chess_grammar + pos, capacity - pos, ",\"start game\",\"end game\"");

    if(written < 0 || (size_t)written >= capacity - pos) {
        chess_grammar[0] = '\0';
        return;
    }

    pos += (size_t)written;

    int written2 = snprintf(chess_grammar + pos, capacity - pos, ",\"long castle\",\"short castle\",\"resign\",\"queen\",\"knight\",\"bishop\",\"rook\"");

    if(written2 < 0 || (size_t)written2 >= capacity - pos) {
        chess_grammar[0] = '\0';
        return;
    }

    pos += (size_t)written2;

    chess_grammar[pos++] = ']';
    chess_grammar[pos] = '\0';
}

bool init_voice(const char *path) {
    dll = LoadLibraryA("libvosk.dll");

    if (!dll) {
        fprintf(stderr, "Voice: missing libvosk.dll\n");
        return false;
    }

#define LOAD(dst, name) do { \
    *(FARPROC *)&(dst) = GetProcAddress(dll, name); \
    if (!(dst)) { \
        fprintf(stderr, "Voice: missing symbol %s\n", name); \
        free_voice(); \
        return false; \
    } \
} while (0)

    LOAD(load_model, "vosk_model_new");
    LOAD(release_model, "vosk_model_free");
    LOAD(make_recognizer, "vosk_recognizer_new_grm");
    LOAD(release_recognizer, "vosk_recognizer_free");
    LOAD(accept_audio, "vosk_recognizer_accept_waveform_s");
    LOAD(get_result, "vosk_recognizer_result");
    LOAD(set_log, "vosk_set_log_level");

#undef LOAD

    set_log(-1);

    model = load_model(path);

    if (!model) {
        fprintf(stderr, "Voice: model not found: %s\n", path);
        free_voice();
        return false;
    }

    build_chess_grammar();

    if (!chess_grammar[0]) {
        fprintf(stderr, "Voice: grammar too large\n");
        free_voice();
        return false;
    }

    recognizer = make_recognizer(
        model,
        16000.0f,
        chess_grammar
    );

    if (!recognizer) {
        fprintf(stderr, "Voice: recognizer failed\n");
        free_voice();
        return false;
    }

    WAVEFORMATEX format = {
        WAVE_FORMAT_PCM, 1, 16000, 32000, 2, 16, 0
    };

    MMRESULT err = waveInOpen(
        &microphone,
        WAVE_MAPPER,
        &format,
        0,
        0,
        CALLBACK_NULL
    );

    if (err != MMSYSERR_NOERROR) {
        fprintf(
            stderr,
            "Voice: microphone cannot open (code %u)\n",
            (unsigned)err
        );
        free_voice();
        return false;
    }

    for (int i = 0; i < AUDIO_BUFFERS; i++) {
        headers[i].lpData = (LPSTR)audio[i];
        headers[i].dwBufferLength = sizeof(audio[i]);

        if (waveInPrepareHeader(
                microphone,
                &headers[i],
                sizeof(WAVEHDR)
            ) != MMSYSERR_NOERROR ||
            waveInAddBuffer(
                microphone,
                &headers[i],
                sizeof(WAVEHDR)
            ) != MMSYSERR_NOERROR) {
            free_voice();
            return false;
        }
    }

    if (waveInStart(microphone) != MMSYSERR_NOERROR) {
        free_voice();
        return false;
    }

    return true;
}

bool process_audio(char *out, size_t capacity) {
    if (!microphone || !recognizer || !out || capacity < 2) {
        return false;
    }

    out[0] = 0;

    for (int i = 0; i < AUDIO_BUFFERS; i++) {
        if (!(headers[i].dwFlags & WHDR_DONE)) {
            continue;
        }

        int count = (int)(
            headers[i].dwBytesRecorded / sizeof(short)
        );

        int completed = count > 0
            ? accept_audio(recognizer, audio[i], count)
            : 0;

        if (completed == 1) {
            const char *json = get_result(recognizer);
            const char *p = json
                ? strstr(json, "\"text\"")
                : NULL;

            if (p) {
                p = strchr(p, ':');

                if (p) {
                    p = strchr(p, '"');

                    if (p) {
                        p++;
                        size_t n = 0;

                        while (
                            p[n] &&
                            p[n] != '"' &&
                            n + 1 < capacity
                        ) {
                            out[n] = p[n];
                            n++;
                        }

                        out[n] = 0;

                        if (n) {
                            printf("Voice: %s\n", out);
                        }
                    }
                }
            }
        }

        headers[i].dwFlags &= ~WHDR_DONE;
        headers[i].dwBytesRecorded = 0;

        waveInAddBuffer(
            microphone,
            &headers[i],
            sizeof(WAVEHDR)
        );

        if (completed == 1 && out[0]) {
            return true;
        }
    }

    return false;
}

void free_voice(void) {
    if (microphone) {
        waveInStop(microphone);
        waveInReset(microphone);

        for (int i = 0; i < AUDIO_BUFFERS; i++) {
            if (headers[i].dwFlags & WHDR_PREPARED) {
                waveInUnprepareHeader(
                    microphone,
                    &headers[i],
                    sizeof(WAVEHDR)
                );
            }
        }

        waveInClose(microphone);
        microphone = NULL;
    }

    if (recognizer && release_recognizer) {
        release_recognizer(recognizer);
    }
    recognizer = NULL;

    if (model && release_model) {
        release_model(model);
    }
    model = NULL;

    if (dll) {
        FreeLibrary(dll);
    }
    dll = NULL;
}
