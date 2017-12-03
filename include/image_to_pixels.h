#ifndef __IMAGE_TO_PIXELS__
#define __IMAGE_TO_PIXELS__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "structs.h"

// Remplissage du tableau de pixels de l'image
extern void image_to_pixels(image * my_image);

// Remplissage du tableau de pixels à partir d'une mcu
extern void mcu_to_pixels(image * my_image, mcu * my_mcu);

// Remplissage du tableau de pixels à partir d'un block
extern void block_to_pixels(image * my_image, block * my_block,
                         uint32_t position, uint8_t ** sampling);


#endif
