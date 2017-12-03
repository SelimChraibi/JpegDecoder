#include "image_to_pixels.h"

 /*
   ┌────────────────────────────────────────────────────────────────┐
   │                                                                │
   │  C'est ici que se passe l'upsampling en cas d'échantillonnage  │
   │                                                                │
   └────────────────────────────────────────────────────────────────┘
 */

// Remplissage du tableau de pixels de l'image
void image_to_pixels(image * my_image)
{
    // On remplis le tableau de pixels
    // à partir des mcu, une par une
    for (size_t m = 0; m < my_image->number_of_mcus; m++) {
        mcu_to_pixels(my_image, my_image->mcu_table[m]);
    }
}

// Remplissage du tableau de pixels à partir d'une mcu
void mcu_to_pixels(image * my_image, mcu * my_mcu)
{
    // (x_block, y_block) = Position du premier pixel
    //                      du block Y courant dans my_image->pixels
    uint32_t x_block;
    uint32_t y_block;

    // b = Index du block courant
    uint8_t b = 0;

    // Pour chaque couleur de l'image
    for (uint8_t color_index = 0; color_index < my_image->color; color_index++) {

        // Pour chaque block de cette couleur
        for (uint8_t v = 0; v < my_image->sampling[color_index][1]; v++) {
            for (uint8_t h = 0; h < my_image->sampling[color_index][0]; h++) {

                // (v, h) = Position du block dans la mcu

                // On calcul la position du block dans my_image->pixels
                x_block = my_mcu->x_mcu + h * 8;
                y_block = my_mcu->y_mcu + v * 8;

                // On remplis le tableau de pixels à partir du block b
                // qui est situé à la position
                // x_block + y_block * my_image->pixels_width
                // dans l'image
                block_to_pixels(my_image, my_mcu->block_table[b],
                                x_block + y_block * my_image->pixels_width,
                                my_mcu->sampling);
                // On incrémente l'indice du block
                b +=1;
            }
        }
    }
}

// Remplissage du tableau de pixels à partir d'un block
void block_to_pixels(image * my_image, block * my_block,
                        uint32_t position, uint8_t ** sampling)
{
    if (my_block == NULL) {
		fprintf(stderr, "%s", cat("block_to_pixels :", "block non valide"));
		exit(1);
	}

    // Coefficiant de sampling horizontal
    //     = hauteur (en block) sur laquelle s'étendra le block
    uint8_t h_sampling = sampling[0][0] / sampling[my_block->color_index][0];

    // Coefficiant de sampling vertical
    //     = largeur (en block) sur laquelle s'étendra le block
    uint8_t v_sampling = sampling[0][1] / sampling[my_block->color_index][1];


    // (x, y) = position du block dans la mcu
    uint32_t x;
    uint32_t y;

    // On parcourt en parallèle le bloc 8x8      :    (indice i)
    //                       et le bloc upsamplé :    (indice p)
    uint8_t p = 0;
    uint8_t i;

    // Pour tout les élément du block 8x8
    for (i = 0; i < 64; i++) {

        // On parcours tout les pixels (x, y) de la mcu
        // dans lequel on va recopier le pixel i du block 8x8
        for (uint8_t v = 0; v < v_sampling ; v++) {
            for (uint8_t h = 0; h < h_sampling ; h++) {
                x = p % (8 * h_sampling) + h;
                y = p / (8 * h_sampling) + v;

                // On copie le pixels i du block 8x8
                // dans le pixels de l'image à l'indice
                // position + x + my_image->pixels_width * y
                my_image->pixels[position + x + my_image->pixels_width * y]
                                [my_block->color_index]
                                                        = my_block->pixels[i];

            }
        }
        // On incremante p selon le sampling horizontal :
        p += h_sampling;
        // Si on arrive en fin de ligne de la mcu...
        if (p % my_image->mcu_width == 0)
            // ...on incrémente p selon le sampling vertical
            p += (v_sampling - 1) * my_image->mcu_width;
    }
}
