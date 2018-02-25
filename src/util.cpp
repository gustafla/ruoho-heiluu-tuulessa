#include "util.h"
#include <cstdlib>
#include <fstream>

void die(int rc) {
  SDL_Quit();
  exit(rc);
}

std::string loadFile(std::string filename) {
  std::ifstream ifs(filename, std::ifstream::in);
  return std::string((std::istreambuf_iterator<char>(ifs)),
      (std::istreambuf_iterator<char>()));
}

Uint32 g_seed = 1998;
Uint32 fastrand() {
  g_seed = (214013*g_seed+2531011);
  return (g_seed>>16)&0x7FFF;
}
