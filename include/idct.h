#ifndef __IDCT_H__
#define __IDCT_H__

#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include "structs.h"
#include "helpers.h"

// calcul de la transformée inverse de l'image
extern void idct (image * my_image);

// calcul de la transformée inverse d'une mcu
extern void idct_mcu (mcu * my_mcu);

// calcul de la transformée inverse d'un block
extern void idct_block (block * my_block);

// calcul de la transformée inverse en un point
extern int16_t s(int8_t x, int8_t y, block * my_block);

// angle des cosinus
extern double angle(int8_t x, int8_t lambda);

extern double constant(int8_t xi); // C(xi)

#endif
