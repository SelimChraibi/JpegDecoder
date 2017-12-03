#ifndef __decompress_H__
#define __decompress_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include "jpeg_reader.h"
#include "bitstream.h"
#include "huffman.h"
#include "structs.h"
#include "helpers.h"


// On récupère l'image
extern image *decompress(struct jpeg_desc * jdesc, struct bitstream * stream);

// On récupère une mcu
extern void decompress_mcu(struct jpeg_desc * jdesc, struct bitstream * stream,
                        mcu *my_mcu, uint16_t * last_dc_value);

// On récupere un block
extern void decompress_block(struct jpeg_desc * jdesc, struct bitstream * stream, block * my_block, uint16_t * last_dc_value);


#endif
