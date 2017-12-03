#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "jpeg_reader.h"


/*
	┌───────────────────────────────────────────────────────────┐
	│                                                           │
	│                        Structures                         │
	│                                                           │
	└───────────────────────────────────────────────────────────┘
*/


// L'image est constituée de
// MCUs qui elles même sont constituées de blocks
// ...comme dans le sujet !

/*
	┌───────────────────────────────────────────────────────────┐
	│                           image                           │
	│                                                           │
	│                                                           │
	│  ┌ mcu_table ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ┐  │
	│    ┌─────────────────────────────────────────────────┐    │
	│  │ │                       mcu                       │ │  │
	│    │                                                 │    │
	│  │ │ ┌ ─block_table─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ┐ │ │  │
	│    │   ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┐   │    │
	│  │ │ │ │Block│Block│Block│Block│Block│Block│ ... │ │ │ │  │
	│    │   │  Y  │  Y  │  Y  │  Y  │ Cb  │ Cb  │     │   │    │
	│  │ │ │ └─────┴─────┴─────┴─────┴─────┴─────┴─────┘ │ │ │  │
	│    │  ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─  │    │
	│  │ └─────────────────────────────────────────────────┘ │  │
	│    ┌─────────────────────────────────────────────────┐    │
	│  │ │                       mcu                       │ │  │
	│    │                                                 │    │
	│  │ │ ┌ ─block_table─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ┐ │ │  │
	│    │   ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┐   │    │
	│  │ │ │ │Block│Block│Block│Block│Block│Block│ ... │ │ │ │  │
	│    │   │  Y  │  Y  │  Y  │  Y  │ Cb  │ Cb  │     │   │    │
	│  │ │ │ └─────┴─────┴─────┴─────┴─────┴─────┴─────┘ │ │ │  │
	│    │  ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─  │    │
	│  │ └─────────────────────────────────────────────────┘ │  │
	│   ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─   │
	│                                                           │
	└───────────────────────────────────────────────────────────┘
*/

/*						      block
 ─────────────────────────────────────────────────────────────
*/

typedef struct {
	// color_index = 0 pour Y    puis pour R
	// 			   = 1 pour Cb   puis pour G
	//		       = 2 pour Cr   puis pour B
	uint8_t color_index;

	// Un tableau de 8x8 pixels
	int16_t pixels[64];
}block;

/*						       mcu
  ─────────────────────────────────────────────────────────────
*/

typedef struct{
	// Sampling = {{h_Y,v_Y},{h_Cb,v_Cr},{h_Cb,v_Cr}}
	uint8_t ** sampling;

	// Un tableau de blocks
	block ** block_table;

	// nombre de blocks dans block_table
	uint8_t number_of_blocks;

	// position de la mcu (pixel en haut à gauche)
	// dans l'image (non tronquée)
	uint32_t x_mcu;
	uint32_t y_mcu;
}mcu;


/*						       image
  ─────────────────────────────────────────────────────────────
*/

typedef struct {
	// color = 1 si l'image est en noir et blanc,
	// 	     = 3 si l'image est en couleurs
	uint8_t color;
	uint32_t height;
	uint32_t width;
	uint32_t size; // taille réelle de l'image (tronquée)

	// Sampling = {{h_Y,v_Y},{h_Cb,v_Cr},{h_Cb,v_Cr}}
	uint8_t ** sampling;

	// Un tableau de mcu
	mcu ** mcu_table;
	uint32_t number_of_mcus;
	uint8_t mcu_height; // en pixels
	uint8_t mcu_width; // en pixel

	// Un tableau de pixels dans lequel
	// sont stockés tout les pixels de l'image.
	// pixels[p][color_index]
	uint8_t ** pixels;
	uint32_t pixels_height;
	uint32_t pixels_width;
	uint32_t pixels_size; // taille non tronquée de l'image
}image;

/*
	┌───────────────────────────────────────────────────────────┐
	│                                                           │
	│               Constructeurs et destructeurs               │
	│                                                           │
	└───────────────────────────────────────────────────────────┘
*/


// Constructeur de la structure de block
extern block * new_block(uint8_t color_index);

// Destructeur de la structure de block
extern void free_block(block * my_block);

// Constructeur de la structure de mcu
extern mcu *new_mcu(uint8_t ** sampling, uint8_t color, uint32_t x_mcu, uint32_t y_mcu);

// Destructeur de la structure de mcu
extern void free_mcu(mcu * my_mcu);

// Constructeur de la structure d'image
extern image *new_image(struct jpeg_desc * jdesc);

// Destructeur de la structure image
extern void free_image(image * my_image);

#endif
