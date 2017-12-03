#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "structs.h"
#include "image_to_pixels.h"
#include "helpers.h"
#include "jpeg_reader.h"
#include "bitstream.h"
#include "huffman.h"
#include "de_quantization.h"
#include "de_zig_zag.h"
#include "decompress.h"
#include "rgb.h"
#include "idct.h"
//#include "fast_idct.h"
#include "ppm.h"



int main(int argc, char const * argv[]) {

    // Date de début d'execution
    clock_t start;
    start = clock();

    // On récupère le fichier si il est valide
    // on renvoie un message d'erreur sinon
    const char * filename = valid_filename(argc, argv);

    // On cree un jpeg_desc qui permettra de lire ce fichier.
    struct jpeg_desc * jdesc = read_jpeg(filename);

    // On recupere le flux des donnees brutes a partir du descripteur.
    struct bitstream * stream = get_bitstream(jdesc);


    //─────────────────────────────────────────────────────────────


    // On crée l'image
    image * my_image = decompress(jdesc, stream);

    // On de-quantifie
    de_quantization (jdesc, my_image);

    // On de_zig_zag
    de_zig_zag(my_image);

    // On passe du domaine fréquentiel au domaine spatial
    idct(my_image);
    //fast_idct(my_image);

    // On copie les pixels des block dans un tableau contenant tous les pixels
    image_to_pixels(my_image);

    // On convertit l'image de YCbCr à RGB
    rgb(my_image);

    // On cré le fichier .ppm ou .pgm
    ppm(filename, my_image);


    //─────────────────────────────────────────────────────────────


    print_execution_time(filename, start);
    free_image(my_image);
    close_jpeg(jdesc);

    return 0;
}
