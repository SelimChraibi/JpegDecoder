#include "rgb.h"

// Conversion en RBG d'une image
// (remplace Y Cb Cr par R G B dans pixels)
void rgb(image * my_image)
{
    // Si l'image est en noir et blanc
    if (my_image->color == 1)
        // il n'y a rien à faire...
        return;

    // On initialise quelques valeurs temporaires
    // qui nous serons utiles
    uint8_t ** temp = my_image->pixels;
    float y;
    float cb;
    float cr;
    float r;
    float g;
    float b;

    // Pour chaque pixel de my_image->pixels
    for (uint32_t p = 0; p < my_image->pixels_size; p++) {

        // On récupère les valeurs en YCbCr
        y = my_image->pixels[p][0];
        cb = my_image->pixels[p][1];
        cr = my_image->pixels[p][2];

        // On calcule les valeurs correspondentes en RGB
        r = y + 1.402 * (cr - 128);
        g = y - 0.34414 * (cb - 128) - 0.71414 * (cr - 128);
        b = y + 1.772 * (cb - 128);

        // On les clampe et
        // on les écrit dans l'image temporaire
        temp[p][0] = CLAMPED(r);
        temp[p][1] = CLAMPED(g);
        temp[p][2] = CLAMPED(b);
    }

    // On peut maintenant copier temp dans pixels
    for (uint32_t p = 0; p < my_image->size; p++) {
        for (uint8_t color_index = 0; color_index < 3; color_index++) {
            my_image->pixels[p][color_index] = temp[p][color_index];
        }
    }
}
