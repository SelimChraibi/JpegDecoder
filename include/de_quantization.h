#ifndef __DE_QUANTIZATION_H__
#define __DE_QUANTIZATION_H__

#include <stdio.h>
#include <stdint.h>
#include "jpeg_reader.h"
#include "structs.h"
#include "helpers.h"

// de-quantification d'une image
extern void de_quantization (struct jpeg_desc * jdesc, image * my_image);

// de-quantification d'une mcu
extern void de_quantization_mcu (struct jpeg_desc * jdesc, mcu * my_mcu);

// de-quantification d'un block
extern void de_quantization_block (struct jpeg_desc * jdesc, block * my_block);

#endif
