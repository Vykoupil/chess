#include "voice_controls.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "vosk_api.h"

void free_voice(void) {
    if (recognizer) {
        vosk_recognizer_free(recognizer);
        recognizer = NULL;
    }
    
    if (model) {
        vosk_model_free(model);
        model = NULL;
    }
}