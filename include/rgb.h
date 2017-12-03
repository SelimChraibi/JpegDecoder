#ifndef __YCBCR_TO_RGB_H__
#define __YCBCR_TO_RGB_H__

#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include "structs.h"
#include "helpers.h"


// Conversion en RBG d'une image
// (remplace Y Cb Cr par R G B dans pixels)
extern void rgb(image * my_image);

#endif
