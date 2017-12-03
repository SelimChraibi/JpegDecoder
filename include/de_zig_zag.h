#ifndef __DE_ZIG_ZAG_H__
#define __DE_ZIG_ZAG_H__

#include <stdio.h>
#include <stdint.h>
#include "structs.h"
#include "helpers.h"

// de-zigzagation d'une image
extern void de_zig_zag (image * my_image);

// de-zigzagation d'une mcu
extern void de_zig_zag_mcu (mcu * my_mcu);

// de_zig_zagation d'un block
extern void de_zig_zag_block (block * my_block);


#endif
