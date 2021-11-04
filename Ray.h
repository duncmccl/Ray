
#ifndef __RAY_H
#define __RAY_H

#include "primitive.h"

void render_image(const camera_t * Camera, const primitive_t * primitive_list, const unsigned long primitive_count, unsigned int * PixelBuffer);

#endif