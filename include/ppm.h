#ifndef __PPM_H__
#define __PPM_H__

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "structs.h"
#include "helpers.h"

// On écrit le code .ppm ou .pgm dans le fichier filename
// selon les informations données par my_image
extern void write_ppm(const char * filename, image * my_image);

// On crée le nom du fichier ppm / pgm dans lequel on va écrire
extern char *filename_ppm(const char * filename, image * my_image);

extern void ppm(const char * filename, image * my_image);

#endif
