#include "util.h"
#include <cstdlib>

void die(int rc) {
  SDL_Quit();
  exit(rc);
}

Uint32 g_seed = 1998;
Uint32 fastrand() {
  g_seed = (214013*g_seed+2531011);
  return (g_seed>>16)&0x7FFF;
}
