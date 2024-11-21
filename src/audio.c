#include "audio.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <AL/alut.h>  // You can use alut for easier file loading

audio_manager* audio_manager_init() {
    audio_manager *manager = calloc(1, sizeof(audio_manager));
    
    manager->device = alcOpenDevice(NULL);  // Open the default audio device
    if (!manager->device) {
        fprintf(stderr, "Failed to open audio device.\n");
        return NULL;
    }

    manager->context = alcCreateContext(manager->device, NULL);
    alcMakeContextCurrent(manager->context);

    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);  // Listener position for 3D audio (if needed)

    manager->buffers = NULL;
    manager->sources = NULL;
    manager->buffer_count = 0;
    manager->source_count = 0;
    alutInit(NULL, 0);

    return manager;
}

void audio_manager_load_sound(audio_manager *manager, const char *file_path, const char *sound_name) {
    ALuint buffer;
    alGenBuffers(1, &buffer);
    
    // Use alut for simplicity in loading .wav files
    buffer = alutCreateBufferFromFile(file_path);
    if (buffer == AL_NONE) {
        fprintf(stderr, "Failed to load audio file: %s\n", file_path);
        return;
    }

    manager->buffers = realloc(manager->buffers, sizeof(audio_buffer) * (manager->buffer_count + 1));
    manager->buffers[manager->buffer_count].buffer = buffer;
    strncpy(manager->buffers[manager->buffer_count].name, sound_name, sizeof(manager->buffers[manager->buffer_count].name));
    manager->buffer_count++;
}

ALuint audio_manager_get_buffer_by_name(audio_manager *manager, const char *sound_name) {
    for (int i = 0; i < manager->buffer_count; i++) {
        if (strcmp(manager->buffers[i].name, sound_name) == 0) {
            return manager->buffers[i].buffer;
        }
    }
    return AL_NONE;
}

audio_source* audio_manager_create_source(audio_manager *manager, const char *sound_name) {
    ALuint buffer = audio_manager_get_buffer_by_name(manager, sound_name);
    if (buffer == AL_NONE) {
        fprintf(stderr, "Sound not found: %s\n", sound_name);
        return NULL;
    }

    audio_source *source = calloc(1, sizeof(audio_source));
    alGenSources(1, &source->source);
    alSourcei(source->source, AL_BUFFER, buffer);

    manager->sources = realloc(manager->sources, sizeof(audio_source) * (manager->source_count + 1));
    manager->sources[manager->source_count] = *source;
    manager->source_count++;

    return source;
}

void audio_manager_play_source(audio_source *source, int loop) {
    source->loop = loop;
    alSourcePlay(source->source);
    source->is_playing = 1;
}

void audio_manager_stop_source(audio_source *source) {
    alSourceStop(source->source);
    source->is_playing = 0;
}



void audio_manager_cleanup(audio_manager *manager) {
    for (int i = 0; i < manager->source_count; i++) {
        alDeleteSources(1, &manager->sources[i].source);
    }
    for (int i = 0; i < manager->buffer_count; i++) {
        alDeleteBuffers(1, &manager->buffers[i].buffer);
    }

    alcMakeContextCurrent(NULL);
    alcDestroyContext(manager->context);
    alcCloseDevice(manager->device);

    free(manager->sources);
    free(manager->buffers);
    free(manager);
}


void audio_manager_play_by_name(audio_manager *manager, const char *sound_name) {
    ALuint buffer = audio_manager_get_buffer_by_name(manager, sound_name);
    if (buffer == AL_NONE) {
        fprintf(stderr, "Sound not found: %s\n", sound_name);
        return;
    }

    ALuint source;
    alGenSources(1, &source);
    if (alGetError() != AL_NO_ERROR) {
        fprintf(stderr, "Failed to generate source for sound: %s\n", sound_name);
        return;
    }
    alSourcei(source, AL_BUFFER, buffer);

    alSourcePlay(source);
}



void audio_manager_update(audio_manager *manager) {
    for (int i = 0; i < manager->source_count; i++) {
        ALint state;
        alGetSourcei(manager->sources[i].source, AL_SOURCE_STATE, &state);
        if (state != AL_PLAYING && manager->sources[i].is_playing) {
            if (manager->sources[i].loop) {
                alSourcePlay(manager->sources[i].source);
                printf("looping!\n");
            } else {
                // Stop playback and clean up
                manager->sources[i].is_playing = 0;
                alDeleteSources(1, &manager->sources[i].source);

                // Remove the source from the array and shift the rest down
                for (int j = i; j < manager->source_count - 1; j++) {
                    manager->sources[j] = manager->sources[j + 1];
                }
                manager->source_count--;
                i--;
            }
        }
    }
}