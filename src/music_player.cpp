#include "music_player.h"
#include "stb_vorbis.h"
#include <cstdlib>

MusicPlayer::MusicPlayer(std::string filename) {
  // Load vorbis file
  short *buffer;
  int channels, sampleRate;
  samples = stb_vorbis_decode_filename(filename.c_str(),
      &channels, &sampleRate, &buffer);

  // SDL audio device specs
  SDL_AudioSpec want;
  want.freq = sampleRate;
  want.format = AUDIO_S16LSB;
  want.channels = channels;
  want.samples = 2048;
  want.callback = callback;
  want.userdata = (void*)this;

  // Init SDL audio device
  audioDevice = SDL_OpenAudioDevice(nullptr, 0, &want, &audioSpec,
      SDL_AUDIO_ALLOW_FORMAT_CHANGE);
  if (audioDevice == 0) {
    throw "Failed to open audio device";
  }

  // Setup playback variable, must cast, stb_vorbis doesn't load to uint8
  audio = position = reinterpret_cast<Uint8*>(buffer);
}

MusicPlayer::~MusicPlayer() {
  SDL_CloseAudioDevice(audioDevice);
  // Cast back to original type necessary?
  free(reinterpret_cast<short*>(audio));
  // free() instead of delete[] because stb_vorbis uses malloc
}

void MusicPlayer::pause(bool pause) {
  SDL_PauseAudioDevice(audioDevice, pause);
}

void MusicPlayer::setPosition(float time) {
  int samples = audioSpec.freq * time;
  // Callback will stop us if trying to play over buffer
  position = audio + samples * audioSpec.channels * 2;
}

void MusicPlayer::callback(void *userData, Uint8 *stream, int len) {
  MusicPlayer *musicPlayer = (MusicPlayer*)userData;

  SDL_memset(stream, 0, len); // In case nothing to play

  int audioSize = musicPlayer->samples * musicPlayer->audioSpec.channels * 2;
  int bytesLeft = audioSize
    - static_cast<int>(musicPlayer->position - musicPlayer->audio);

  // Stop after end
  if (bytesLeft <= 0) {
    musicPlayer->pause(true);
    return;
  }

  // Limit to whats available
  len = (len > bytesLeft ? bytesLeft : len);

  // Push bytes
  SDL_memcpy(stream, musicPlayer->position, len);

  // Update position
  musicPlayer->position += len;
}
