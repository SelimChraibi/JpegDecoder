#ifndef __FAST_IDCT_H__
#define __FAST_IDCT_H__

#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include "structs.h"
#include "helpers.h"

// calcul de la transformée inverse rapide de l'image
extern void fast_idct (image * my_image);

// calcul de la transformée inverse rapide d'une mcu
extern void fast_idct_mcu (mcu * my_mcu);

// calcul de la transformée inverse rapide d'un block
extern void fast_idct_block(block * my_block);

// calcul de la transformée inverse rapide sur les vecteurs lignes
extern void fast_idct_vector(float **pixels_matrix);

// On change l'ordre des lignes
extern void change_order(float **pixels_matrix);

// Transpose le tableau de pixel
extern void transpose(float **pixels_matrix);

extern void butterfly(uint8_t plus_index, uint8_t minus_index, float **pixels_matrix);

extern void rotator(uint8_t index_0, uint8_t index_1, float **pixels_matrix, float k, float n);

#endif
