#include "de_quantization.h"


// de-quantification d'une image
void de_quantization (struct jpeg_desc * jdesc, image *my_image)
{
    for (uint32_t m = 0; m < my_image->number_of_mcus; m++) {
        de_quantization_mcu (jdesc, my_image->mcu_table[m]);
    }
};

// de-quantification d'une mcu
void de_quantization_mcu (struct jpeg_desc * jdesc, mcu *my_mcu)
{
    for (uint32_t b = 0; b < my_mcu->number_of_blocks; b++) {
        de_quantization_block (jdesc, my_mcu->block_table[b]);
    }
};

// de-quantification d'un block
void de_quantization_block (struct jpeg_desc * jdesc, block *my_block)
{
    // On récupère la table de quantification 0
    // si le block est de couleur Y
    // sinon on récupère la table 1
    uint8_t * quant_table = get_quantization_table(jdesc, (my_block->color_index == 0)? 0: 1);

    if (quant_table == NULL) {
		fprintf(stderr, "%s", cat("de_quantization_block :", "impossible de récupérer quant_table"));
		exit(1);
	}

    // On multiplie chaque élément de my_block->pixels
    // par la valeur donnée par la table de quantification
    for (int8_t p = 0; p < 64; p++) {
        my_block->pixels[p] *= quant_table[p];
    }
};
