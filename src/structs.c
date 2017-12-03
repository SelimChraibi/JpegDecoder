#include "structs.h"

/*
    ┌───────────────────────────────────────────────────────────┐
    │                                                           │
    │              Constructeurs et destructeurs                │
    │                                                           │
    └───────────────────────────────────────────────────────────┘
*/

/*						      block
 ─────────────────────────────────────────────────────────────
*/

// Constructeur de la structure de block
block * new_block(uint8_t color_index)
{
    block * my_block = calloc(1, sizeof(block));
    my_block->color_index = color_index;

    return my_block;
}

// Destructeur de la structure de block
void free_block(block * my_block)
{
    if (my_block == NULL)
    	return;
    free(my_block);
}


/*						       mcu
  ─────────────────────────────────────────────────────────────
*/

// Constructeur de la structure de mcu
mcu	* new_mcu(uint8_t ** sampling, uint8_t color, uint32_t x_mcu, uint32_t y_mcu)
{
    mcu * my_mcu = malloc(sizeof(mcu));

    my_mcu->sampling = sampling;
    my_mcu->x_mcu = x_mcu;
    my_mcu->y_mcu = y_mcu;


    // Le nombre de blocks est donné par sampling
    my_mcu->number_of_blocks = 0;
    for (uint8_t color_index = 0; color_index < color; color_index++) {
        my_mcu->number_of_blocks += sampling[color_index][0]
                                   * sampling[color_index][1];
    }
    my_mcu->block_table = malloc(sizeof(block *) * my_mcu->number_of_blocks);


    // On cré les blocks et on les ajoute à block_table :
    uint8_t b = 0; // b = indice du block courant
    for (uint8_t color_index = 0; color_index < color; color_index++) {
        for (uint8_t i = 0;
                i < sampling[color_index][0] * sampling[color_index][1];
                i++) {
            my_mcu->block_table[b] = new_block(color_index);
            b += 1;
        }
    }

    return my_mcu;
}

// Destructeur de la structure de mcu
void free_mcu(mcu * my_mcu)
{
    if (my_mcu == NULL)
    	return;

    // On detruit les blocks
    for (uint8_t b = 0; b < my_mcu->number_of_blocks; b++) {
        free_block(my_mcu->block_table[b]);
    }

    // On detruit block_table
    free(my_mcu->block_table);

    // Puis on détruit la mcu
    free(my_mcu);
}

/*						       image
  ─────────────────────────────────────────────────────────────
*/

// Constructeur de la structure d'image
image * new_image(struct jpeg_desc * jdes)
{
    image * my_image = malloc(sizeof(image));

    my_image->color = get_nb_components(jdes);
    my_image->height = get_image_size(jdes, DIR_V);
    my_image->width = get_image_size(jdes, DIR_H);
    my_image->size = my_image->height * my_image->width;

    //──────────────────────────────────────

    // On alloue et on initialise sampling
    my_image->sampling = malloc(my_image->color * sizeof(uint8_t *));
    for (uint8_t color_index = 0; color_index < my_image->color; color_index++) {
        my_image->sampling[color_index] = malloc(2 * sizeof(uint8_t));

        my_image->sampling[color_index][0] =
                    get_frame_component_sampling_factor(jdes, DIR_H, color_index);
        my_image->sampling[color_index][1] =
                    get_frame_component_sampling_factor(jdes, DIR_V, color_index);
    }

    //──────────────────────────────────────

    uint32_t height_in_mcus = (uint32_t) ceil((double)
                    (my_image->height) / (8.0 * my_image->sampling[0][1]));
    uint32_t width_in_mcus = (uint32_t) ceil((double)
                    (my_image->width) / (8.0 * my_image->sampling[0][0]));

    my_image->number_of_mcus = height_in_mcus * width_in_mcus;
    my_image->mcu_height = my_image->sampling[0][1] * 8;
    my_image->mcu_width = my_image->sampling[0][0] * 8;

    //──────────────────────────────────────

    // On alloue et on initialise mcu_table
    my_image->mcu_table = malloc(sizeof(mcu *) * my_image->number_of_mcus);
    uint32_t x_mcu;
 	uint32_t y_mcu;

    for (uint32_t h = 0; h < width_in_mcus; h++) {
        for (uint32_t v = 0; v < height_in_mcus; v++) {
            x_mcu = h * my_image->mcu_width;
         	y_mcu = v * my_image->mcu_height;
            my_image->mcu_table[h + width_in_mcus * v]
                        = new_mcu(my_image->sampling, my_image->color, x_mcu, y_mcu);
        }
    }

    my_image->pixels_height = height_in_mcus * my_image->mcu_height;
    my_image->pixels_width = width_in_mcus * my_image->mcu_width;
    my_image->pixels_size = my_image->pixels_height * my_image->pixels_width;

    //──────────────────────────────────────

    // On alloue pixels
    my_image->pixels = malloc(sizeof(uint8_t *) * my_image->pixels_size);
    for (uint32_t p = 0; p < my_image->pixels_size; p++) {
        my_image->pixels[p] = malloc(sizeof(uint8_t) * my_image->color);
    }

    return my_image;
}

// Destructeur de la structure image
void free_image(image * my_image)
{
    if (my_image == NULL)
    	return;

    // On detruit pixels
    for (uint32_t p = 0; p < my_image->pixels_size; p++) {
         free(my_image->pixels[p]);
    }
    free(my_image->pixels);

    // On détruit mcu_table
    for (uint32_t m = 0; m < my_image->number_of_mcus; m++) {
        free_mcu(my_image->mcu_table[m]);
    }
    free(my_image->mcu_table);

    // On détruit sampling
    for (uint8_t color_index = 0; color_index < my_image->color; color_index++) {
        free(my_image->sampling[color_index]);
    }
    free(my_image->sampling);

    // Puis on detruit l'image
    free(my_image);
}
