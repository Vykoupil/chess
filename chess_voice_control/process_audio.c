#include "voice_controls.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "vosk_api.h"

void process_audio(void) {
    if (!recognizer) return;

    short pcm_buffer[4096];
    int samples_read = 0;


    samples_read = ziskej_data_z_mikrofonu(pcm_buffer, 4096);


    if (samples_read > 0) {

        int accepted = vosk_recognizer_accept_waveform_s(recognizer, pcm_buffer, samples_read);

        if (accepted) {
            
            const char *result = vosk_recognizer_result(recognizer);
            printf("Rozpoznáno: %s\n", result);
            
            
        } else {

            const char *partial = vosk_recognizer_partial_result(recognizer);
        }
    }
}