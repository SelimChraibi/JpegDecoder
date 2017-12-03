#include "idct.h"

// calcul de la transformée inverse de l'image
void idct (image * my_image)
{
    for (uint32_t m = 0; m < my_image->number_of_mcus; m++) {
        idct_mcu (my_image->mcu_table[m]);
    }
};

// calcul de la transformée inverse d'une mcu
void idct_mcu (mcu * my_mcu)
{
    for (uint8_t b = 0; b < my_mcu->number_of_blocks; b++) {
        idct_block (my_mcu->block_table[b]);
    }
}

// calcul de la transformée inverse d'un block
void idct_block (block * my_block)
{
    // (block temporaire)
    block temp = *my_block;
    int16_t s_xy;
    // Pour tout point (x, y) du bloc
    // On calcul la trasformée inverse S(x, y)
    for (uint8_t x = 0; x < 8; x++) {
        for (uint8_t y = 0; y < 8; y++) {
            s_xy = s(x, y, &temp);
            my_block->pixels[x + 8 * y] = CLAMPED(s_xy);
        }
    }
};


// calcul de la transformée inverse en un point
int16_t s(int8_t x, int8_t y, block * my_block) {

    double sum = 0;
    for (int8_t lambda = 0; lambda < 8; lambda++) {
        for (int8_t mu = 0; mu < 8; mu++) {
            sum += constant(lambda) * constant(mu) * cos(angle(x, lambda)) * cos(angle(y, mu)) * my_block->pixels[lambda + 8 * mu];
        }
    }
    return sum / 4.0 + 128.0;
};



//─────────────────────────────────────────────────────────────



// angle des cosinus
double angle(int8_t x, int8_t lambda)
{
    const double pi = acos(-1);
    return ((2 * x + 1) * lambda * pi)/16.0;
};

// C(xi)
double constant(int8_t xi)
{
    double c = 1;
    if (xi == 0) {
        c = 1 / sqrt(2);
    }
    return c;
};
