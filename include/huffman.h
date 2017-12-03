#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "bitstream.h"


struct huff_table;

extern struct huff_table * load_huffman_table(struct bitstream * stream,
                                             uint16_t * nb_byte_read);

extern int8_t next_huffman_value(struct huff_table * table,
                                 struct bitstream * stream);

extern void free_huffman_table(struct huff_table * table);

#endif
