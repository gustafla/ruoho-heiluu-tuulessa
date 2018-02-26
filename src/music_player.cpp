#include "music_player.h"
#include "stb_vorbis.c"
#include "util.h"
#include <iostream>
#include <cstdlib>

MusicPlayer::MusicPlayer(): m_audio(nullptr), m_audioDevice(0) {}

MusicPlayer::MusicPlayer(std::string filename) {
  // Load vorbis file
  short *buffer;
  int channels, sampleRate;
  m_samples = stb_vorbis_decode_filename(filename.c_str(),
      &channels, &sampleRate, &buffer);
  if (m_samples < 1) {
    std::cerr << filename << ": Failed to open vorbis file" << std::endl;
    die(EXIT_FAILURE); // exceptions are a pain, not needed in a demo
  }

  // SDL audio device specs
  SDL_AudioSpec want;
  want.freq = sampleRate;
  want.format = AUDIO_S16LSB;
  want.channels = channels;
  want.samples = 2048;
  want.callback = callback;
  want.userdata = (void*)this;

  // Init SDL audio device
  m_audioDevice = SDL_OpenAudioDevice(nullptr, 0, &want, &m_audioSpec,
      SDL_AUDIO_ALLOW_FORMAT_CHANGE);
  if (m_audioDevice == 0) {
    std::cerr << filename << ": Failed to open audio device" << std::endl;
    die(EXIT_FAILURE);
  }

  // Setup playback variable, must cast, stb_vorbis doesn't load to uint8
  m_audio = reinterpret_cast<Uint8*>(buffer);
  m_position = m_audio;
}

MusicPlayer::~MusicPlayer() {
  SDL_CloseAudioDevice(m_audioDevice);
  if (m_audio != nullptr) {
    free(reinterpret_cast<short *>(m_audio));
    // free() instead of delete[] because stb_vorbis uses malloc
  }
}

void MusicPlayer::pause(bool pause) {
  SDL_PauseAudioDevice(m_audioDevice, pause);
}

void MusicPlayer::setPosition(double time) {
  int samples = m_audioSpec.freq * time;
  // Callback will stop us if trying to play over buffer
  m_position = m_audio + samples * m_audioSpec.channels * 2;
}

bool MusicPlayer::isPlaying() {
  return SDL_GetAudioDeviceStatus(m_audioDevice) == SDL_AUDIO_PLAYING;
}

double MusicPlayer::getTime() const {
  double since = static_cast<double>(SDL_GetTicks() - m_callbackTicks) / 1000.;
  int bytes = static_cast<int>(m_position - m_audio);
  int samples = (bytes / 2) / m_audioSpec.channels;
  double time = static_cast<double>(samples)
    / static_cast<double>(m_audioSpec.freq);
  double bufferDuration = static_cast<double>(m_audioSpec.samples)
    / static_cast<double>(m_audioSpec.freq);
  return time + (since > bufferDuration ? bufferDuration : since);
}

void MusicPlayer::callback(void *userData, Uint8 *stream, int len) {
  MusicPlayer *musicPlayer = (MusicPlayer*)userData;

  SDL_memset(stream, 0, len); // In case nothing to play

  int audioSize = musicPlayer->m_samples * musicPlayer->m_audioSpec.channels * 2;
  int bytesLeft = audioSize
    - static_cast<int>(musicPlayer->m_position - musicPlayer->m_audio);

  // Stop after end
  if (bytesLeft <= 0) {
    musicPlayer->pause(true);
    return;
  }

  // Limit to whats available
  len = (len > bytesLeft ? bytesLeft : len);

  // Put bytes
  SDL_memcpy(stream, musicPlayer->m_position, len);

  // Update position
  musicPlayer->m_position += len;

  // Get time
  musicPlayer->m_callbackTicks = SDL_GetTicks();
}
