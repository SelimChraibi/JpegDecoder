#ifndef __HELPERS__
#define __HELPERS__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <libgen.h>
#include "structs.h"
#define CLAMPED(value) (value < 0 ? 0 : value > 255 ? 255 : value)

// Récupère le nom du fichier s'il est valide, exit sinon
extern const char * valid_filename(int argc, char const * argv[]);

// valeur(m, i) comme définie à la table 2.1
extern int16_t value(uint8_t magnitude, uint32_t index);

// Imprime le temps d'execution
// extern void print_execution_time(image * my_image, const char *filename, clock_t start);
extern void print_execution_time(const char *filename, clock_t start);

// Formatage de l'impression de 2 lignes de texte (pour les erreurs)
extern char *cat(char * line1, char * line2);

#endif
