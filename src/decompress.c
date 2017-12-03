#include "decompress.h"


// On récupère l'image
image * decompress(struct jpeg_desc * jdesc, struct bitstream * stream)
{
    // On cré une nouvelle image
    image * my_image = new_image(jdesc);

    // last_dc_value permettra le calcul de la
    // prochaine composante continue (cf 2.4.2 DPCM)
    uint16_t * last_dc_value = calloc(3, sizeof(uint16_t));

    // On récupère les mcus une par une
    for (uint32_t m = 0; m < my_image->number_of_mcus; m++) {
            decompress_mcu(jdesc, stream, my_image->mcu_table[m], last_dc_value);
        }
    free(last_dc_value);
    return my_image;
}


//─────────────────────────────────────────────────────────────



// On récupère une mcu
void decompress_mcu(struct jpeg_desc * jdesc, struct bitstream *stream,
                mcu *my_mcu, uint16_t * last_dc_value)
{
    // On récupère les blocks un par un
    for (uint8_t b = 0; b < my_mcu->number_of_blocks; b++) {
        decompress_block(jdesc, stream, my_mcu->block_table[b], last_dc_value);
    }
}


//─────────────────────────────────────────────────────────────


// On récupere un block
void decompress_block(struct jpeg_desc * jdesc, struct bitstream * stream, block * my_block, uint16_t * last_dc_value)
{
    if (stream == NULL) {
		fprintf(stderr, "%s", cat("decompress_block :", "bitstream non valide"));
		exit(1);
	} else if (my_block == NULL) {
		fprintf(stderr, "%s", cat("decompress_block :", "block non valide"));
		exit(1);
	}

    // On récupère la table de huffman 0
    // si le block est de couleur Y
    // sinon on récupère la table 1
    uint8_t huff_index = (my_block->color_index == 0)? 0: 1;

    // Lecture des tables de huffman huff_index (AC et DC)
    struct huff_table * huff_table_ac = get_huffman_table(jdesc, AC, huff_index);
    struct huff_table * huff_table_dc = get_huffman_table(jdesc, DC, huff_index);

    if (huff_table_ac == NULL) {
		fprintf(stderr, "%s", cat("decompress_block :", "impossible de récupérer huff_table_ac"));
		exit(1);
	} if (huff_table_dc == NULL) {
		fprintf(stderr, "%s", cat("decompress_block :", "impossible de récupérer huff_table_dc"));
		exit(1);
	}

    uint8_t magnitude;
    uint32_t index = 0;

    uint8_t rle;
    int8_t nb_zeros;

    //────────────────────────────

    // On lit la valeur DC correspondante au block :

    // On récupère la magnitude
    magnitude = next_huffman_value(huff_table_dc, stream);
    // On lit l'index qui se trouve alors dans
    // les magnitude prochains bits
    read_bitstream(stream, magnitude, &index, true);
    // On peut en déduire la valeur de DC
    // (on oublie pas d'ajouter last_dc_value pour la DPCM)
    my_block->pixels[0] = value(magnitude, index)
                        + last_dc_value[my_block->color_index];

    //────────────────────────────

    // On lit les valeur AC correspondantes au block :

    // Pour chaque pixel du block
    for (uint8_t p = 1; p < 64; p++) {
        // On récupère le code RLE
        rle = next_huffman_value(huff_table_ac, stream);

        // On sépare les bits de poids fort/faibles
        nb_zeros = (rle & 0xF0)>>4; // 4 bits de poids fort
        magnitude = (rle & 0x0F); // 4 bits de poids faible

        // si RLE = 0xF0
        if (nb_zeros == 15 && magnitude == 0) {
            // on laisse 16 composantes nulles
            p += 15;
        // si RLE = 0x00
        } else if (nb_zeros == 0 && magnitude == 0) {
            // on a atteind "End Of Block"
            p = 63;
        // si on a pas un symbol interdit 0x?0
        } else if (magnitude > 0 && magnitude < 11) {
                // on laisse nb_zeros composantes nulles
                p += nb_zeros;
                // On lit l'index qui se trouve alors dans
                // les magnitude prochains bits
                read_bitstream(stream, magnitude, &index, true);
                // On peut en déduire la valeur AC pour ce pixel
                my_block->pixels[p] = value(magnitude, index);
        }
    }

    // On oublie pas de renvoyer last_dc_value qui
    // permettra le calcul de laprochaine
    // composante continue (cf 2.4.2 DPCM)
    last_dc_value[my_block->color_index] = my_block->pixels[0];
}
