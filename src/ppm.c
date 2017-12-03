#include "ppm.h"

// On écrit le code .ppm ou .pgm dans le fichier filename
// selon les informations données par my_image
void write_ppm(const char * filename, image * my_image)
{
	FILE * fichier = NULL;
	fichier = fopen(filename, "w");

	// Si l'image est en couleur on écrit l'entête "P6"
	if (my_image->color == 3) {
		fprintf(fichier, "P6\n");
	// Sinon on écrit l'entête "P5"
	} else {
		fprintf(fichier, "P5\n");
	}

	fprintf(fichier, "%d %d\n", my_image->width, my_image->height);
	fprintf(fichier, "255\n");

	// Pour chaque pixel du tableau de pixels de l'image
	for (uint32_t p = 0; p < my_image->pixels_size; p++) {

		// (x, y) = position du point courant dans pixels
		uint32_t x = p % my_image->pixels_width;
		uint32_t y = p / my_image->pixels_width;

		// Tronquage :
		// On test si (x, y) est bien dans l'image.
		// S'il n'y est pas, il n'est pas écrit
		if (x < my_image->width && y < my_image->height) {
			for (size_t color_index = 0; color_index < my_image->color; color_index++) {
				fwrite(&my_image->pixels[p][color_index], sizeof(uint8_t), 1, fichier);
			}
		}
	}
	fclose(fichier);
}

// On crée le nom du fichier ppm / pgm dans lequel on va écrire
char * filename_ppm(const char * filename, image * my_image)
{
	char * name = malloc(strlen(filename) + 1);
	strcpy(name, filename);

	// On récupère un pointeur vers le début de l'extention...
	char * extension = strrchr(name, '.');

	// ...on peut donc la remplacer par ppm ou pgm
	extension[1] = 'p';
	extension[3] = 'm';
	extension[4] = '\0';
	if (my_image->color == 3) {
		extension[2] = 'p';
	} else {
		extension[2] = 'g';
	}
	return name;
}

// On cré le fichier .ppm ou .pgm et on écrit dedant
void ppm(const char * filename, image * my_image)
{
	char * new_filename = filename_ppm(filename, my_image);
	write_ppm(new_filename, my_image);
	free(new_filename);
}
