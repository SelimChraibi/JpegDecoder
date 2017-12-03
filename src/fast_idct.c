#include "fast_idct.h"


// calcul de la transformée inverse rapide de l'image
void fast_idct (image * my_image)
{
    for (uint32_t m = 0; m < my_image->number_of_mcus; m++) {
        fast_idct_mcu (my_image->mcu_table[m]);
    }
}

// calcul de la transformée inverse rapide d'une mcu
void fast_idct_mcu (mcu * my_mcu)
{
    for (uint8_t b = 0; b < my_mcu->number_of_blocks; b++) {
        fast_idct_block (my_mcu->block_table[b]);
    }
}

// calcul de la transformée inverse rapide d'un block
void fast_idct_block(block * my_block)
{
    // On crée une matrice 8x8...
    float **pixels_matrix = malloc(sizeof(float *) * 8);
    for (uint8_t line = 0; line < 8; line++) {
        pixels_matrix[line] = malloc(sizeof(float) * 8);
    }
    // ...dans laquelle on copie le block dans une matrice
    for (uint8_t line = 0; line < 8; line++) {
        for (uint8_t row = 0; row < 8; row++) {
            pixels_matrix[line][row] = sqrt(8) * my_block->pixels[line * 8 + row];
        }

    }
    // On execute l'idct rapidesur les lignes...
    fast_idct_vector(pixels_matrix);
    transpose(pixels_matrix);

    // ...puis sur les colonnes
    fast_idct_vector(pixels_matrix);
    transpose(pixels_matrix);

    // On copie la matrice contenant les résultats
    // de l'iDCT rapide dans le block (après avoir convertit
    // les valeurs en entier et après les avoir clampés)
    for (uint8_t line = 0; line < 8; line++) {
        for (uint8_t row = 0; row < 8; row++) {
            my_block->pixels[line * 8 + row] =  CLAMPED(128 + (int16_t) (pixels_matrix[line][row]));
        }
        free(pixels_matrix[line]);
    }
    free(pixels_matrix);
}


// calcul de la transformée inverse rapide sur les vecteurs lignes
void fast_idct_vector(float **pixels_matrix)
{
    // On commence par changer l'ordre des lignes
    change_order(pixels_matrix);
    // On effectue ensuites les opérations unes par une...

    // Stage 1
    for (uint8_t row = 0; row < 8; row++) {
        pixels_matrix[5][row] /= sqrt(2);
        pixels_matrix[6][row] /= sqrt(2);
    }
    butterfly(4, 7, pixels_matrix);

    // Stage 2
    butterfly(1, 0, pixels_matrix);
    rotator(2, 3, pixels_matrix, sqrt(2), 6.0);
    butterfly(6, 4, pixels_matrix);
    butterfly(5, 7, pixels_matrix);

    // Stage 3
    butterfly(3, 0, pixels_matrix);
    butterfly(2, 1, pixels_matrix);
    rotator(4, 7, pixels_matrix, 1.0, 3.0);
    rotator(5, 6, pixels_matrix, 1.0, 1.0);

    // Stage 4
    butterfly(4, 3, pixels_matrix);
    butterfly(5, 2, pixels_matrix);
    butterfly(6, 1, pixels_matrix);
    butterfly(7, 0, pixels_matrix);
}

// On change l'ordre des lignes
void change_order(float **pixels_matrix)
{
    int8_t new_order[8] = {0, 4, 2, 6, 7, 3, 5, 1};
    float **temp = pixels_matrix;
    // Pout chaque ligne line
    for (uint8_t line = 0; line < 8; line++) {
        // On échange la ligne line et la ligne new_order[line]
        for (uint8_t row = 0; row < 8; row++) {
            pixels_matrix[line][row] = temp[new_order[line]][row];
        }
    }
}

// Transpose le tableau de pixel
void transpose(float **pixels_matrix)
{
    float **temp = pixels_matrix;
    for (uint8_t line = 0; line < 8; line++) {
        for (uint8_t row = 0; row < 8; row++) {
            pixels_matrix[line][row] = temp[row][line];
        }
    }
}

void butterfly(uint8_t plus_index, uint8_t minus_index, float **pixels_matrix)
{
    float **temp = pixels_matrix;
    for (uint8_t row = 0; row < 8; row++) {
        pixels_matrix[plus_index][row] =
                        (temp[plus_index][row] + temp[minus_index][row])/2;
        pixels_matrix[minus_index][row] =
                        (temp[plus_index][row] - temp[minus_index][row])/2;
    }
}

void rotator(uint8_t index_0, uint8_t index_1, float **pixels_matrix, float k, float n)
{
    const float pi = acos(-1);
    float **temp = pixels_matrix;
    float c = cos((n * pi) / 16.0) / k;
    float s = sin((n * pi) / 16.0) / k;
    for (uint8_t row = 0; row < 8; row++) {
        pixels_matrix[index_0][row] = c * temp[index_0][row] - s * temp[index_1][row];
        pixels_matrix[index_1][row] = c * temp[index_1][row] + s * temp[index_0][row];
    }
}
