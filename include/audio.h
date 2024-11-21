#ifndef AUDIO_H
#define AUDIO_H

#include <AL/al.h>
#include <AL/alc.h>

typedef struct {
    ALuint buffer;
    char name[256];
} audio_buffer;

typedef struct {
    ALuint source;
    int is_playing;
    int loop;
} audio_source;

typedef struct {
    audio_buffer *buffers; 
    audio_source *sources; 
    int buffer_count;       
    int source_count;       
    ALCdevice *device;      
    ALCcontext *context;    
} audio_manager;

audio_manager* audio_manager_init();

void audio_manager_load_sound(audio_manager *manager, const char *file_path, const char *sound_name);

audio_source* audio_manager_create_source(audio_manager *manager, const char *sound_name);

void audio_manager_play_source(audio_source *source, int loop);

void audio_manager_stop_source(audio_source *source);

void audio_manager_cleanup(audio_manager *manager);

void audio_manager_play_by_name(audio_manager *manager, const char *sound_name);
void audio_manager_update(audio_manager *manager);


#endif
