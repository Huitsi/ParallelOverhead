/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#include "common.h"

//Struct for holding information about an audio file
struct audio
{
	SDL_AudioSpec spec;
	Uint8 *buffer;
	Uint32 length;
};

//Global struct to store audio data and state
struct
{
	SDL_AudioDeviceID music_device;
	SDL_AudioDeviceID sound_device;

	struct audio music;
	struct audio move;
	struct audio death;

	char priority_sound_played;
}
AudioData;

/**
 * Load the given audio file.
 * @param file The path of the file to load.
 * @return The audio file info.
 */
struct audio load_wav(const char* file)
{
	struct audio audio;
	if (SDL_LoadWAV(file, &audio.spec, &audio.buffer, &audio.length) == NULL)
	{
		report_SDL_error("loading wav");
	}
	return audio;
}

//SDL_AudioSpec.callback function
void loop_music(void *userdata, unsigned char *stream, int len)
{
	static int place = 0;

	if (place + len < AudioData.music.length)
	{
		memmove(stream, AudioData.music.buffer + place, len);
		place += len;
		return;
	}

	memmove(stream, AudioData.music.buffer + place, AudioData.music.length - place);
	place = 0;
}

/**
 * Load the sound files and start the music.
 */
void init_audio()
{
	AudioData.music = load_wav("data/fast_pulse.wav");
	AudioData.move = load_wav("data/move.wav");
	AudioData.death = load_wav("data/death.wav");

	SDL_AudioSpec obt;

	SDL_AudioSpec music_spec = AudioData.music.spec;
	music_spec.callback = loop_music;

	AudioData.music_device = SDL_OpenAudioDevice(NULL, 0, &music_spec, &obt, SDL_AUDIO_ALLOW_ANY_CHANGE);
	if (!AudioData.music_device)
	{
		report_SDL_error("open audio device (music)");
	}

	AudioData.sound_device = SDL_OpenAudioDevice(NULL, 0, &AudioData.death.spec, &obt, SDL_AUDIO_ALLOW_ANY_CHANGE);
	if (!AudioData.sound_device)
	{
		report_SDL_error("open audio device (sound)");
	}

	SDL_QueueAudio(AudioData.music_device, AudioData.music.buffer, AudioData.music.length);
	SDL_PauseAudioDevice(AudioData.music_device, 0);
	SDL_PauseAudioDevice(AudioData.sound_device, 0);
}

/**
 * Free the space used for the sound files.
 */
void free_audio()
{
	SDL_FreeWAV(AudioData.music.buffer);
	SDL_FreeWAV(AudioData.move.buffer);
	SDL_FreeWAV(AudioData.death.buffer);
}

void play_move_sound()
{
	if (AudioData.priority_sound_played && SDL_GetQueuedAudioSize(AudioData.sound_device))
	{
		return;
	}
	SDL_ClearQueuedAudio(AudioData.sound_device);
	if (SDL_QueueAudio(AudioData.sound_device, AudioData.move.buffer, AudioData.move.length))
	{
		report_SDL_error("queue sound (move)");
	}
	AudioData.priority_sound_played = 0;
}

void play_death_sound()
{
	SDL_ClearQueuedAudio(AudioData.sound_device);
	if (SDL_QueueAudio(AudioData.sound_device, AudioData.death.buffer, AudioData.death.length))
	{
		report_SDL_error("queue sound (death)");
	}
	AudioData.priority_sound_played = 1;
}
