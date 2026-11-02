#ifndef VOICE_CONTROLS_H
#define VOICE_CONTROLS_H
#include <stdbool.h>
#include <stddef.h>
bool init_voice(const char *model_path);
void free_voice(void);
bool process_audio(char *out, size_t capacity);
int voice_move(const char *command); /* 0 invalid, 1 played, 2+ ambiguous */
#endif
