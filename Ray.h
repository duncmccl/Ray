
#ifndef __RAY_H
#define __RAY_H

#include "primative.h"

void render_image(const camera_t * Camera, const primative_t * primative_list, const unsigned long primative_count, unsigned int * PixelBuffer);

#endif