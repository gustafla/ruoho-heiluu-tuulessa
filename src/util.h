#pragma once

#include <SDL.h>
#include <string>

void die(int rc);
std::string loadFile(std::string filename);
Uint32 fastrand();
