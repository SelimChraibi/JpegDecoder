#include "de_zig_zag.h"

// de-zigzagation d'une image
void de_zig_zag (image * my_image)
{
    for (uint32_t m = 0; m < my_image->number_of_mcus; m++) {
        de_zig_zag_mcu (my_image->mcu_table[m]);
    }
};

// de-zigzagation d'une mcu
void de_zig_zag_mcu (mcu * my_mcu)
{
    for (uint32_t b = 0; b < my_mcu->number_of_blocks; b++) {
        de_zig_zag_block (my_mcu->block_table[b]);
    }
};

// de_zig_zagation d'un block
void de_zig_zag_block (block * my_block)
{
    // zigzag_array se lit de la façon suivante :
    // l'élément d'indice p du block de-zigzagué se trouve
    // à l'indice zigzag_array[p] dans le bloc ziguzagué...
    static int zigzag_array[64] =
        {   0,  1,  5,  6,  14, 15, 27, 28,
            2,  4,  7,  13, 16, 26, 29, 42,
            3,  8,  12, 17, 25, 30, 41, 43,
            9,  11, 18, 24, 31, 40, 44, 53,
            10, 19, 23, 32, 39, 45, 52, 54,
            20, 22, 33, 38, 46, 51, 55, 60,
            21, 34, 37, 47, 50, 56, 59, 61,
            35, 36, 48, 49, 57, 58, 62, 63   };

    // (block temporaire)
    block temp = * my_block;

    // ...on parcours donc le block selon les valeurs
    // stocker dans zigzag_array
    for(int8_t p = 0; p < 64; p++)
       {
           my_block->pixels[p] = temp.pixels[zigzag_array[p]];
       }
};
