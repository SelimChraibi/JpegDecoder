# Jpeg Decoder

*First year C project at Ensimag - Grenoble INP*

![Screen Shot 2017-12-03 at 12.12.23](https://i.imgur.com/08F1EoC.jpg)

Conversion of .jpg or .jpeg images into .ppm images.
Works with all horizontal and vertical downsampling ratios.


![Decoding steps](https://i.imgur.com/ieIiYRy.png)

See [JPEG encoding](https://en.wikipedia.org/wiki/JPEG#Encoding) to understand the different steps.

## Getting started

*Note: To avoid architecture compatibility issues, the use of a linux virtual machine environment is recommanded. ([Vagrant](https://www.vagrantup.com/downloads.html))*

### Compilation

```sh
make all
```

### Execution

To test the program on a specific image:

```sh
./bin/jpeg2ppm ./images/image.jp[e]g
```

To test the program on all images of the `./images` directory:

```sh
make test
```

### Cleaning

Cleaning (`jpeg2ppm`, `.ppm`, `.pgm`, `.o`)
```sh
make clean
```

## Code structure

### `main.c`

```c
// Creation of the image (Variable Length Decoding using the Huffman tables)
image * my_image = decompress(jdesc, stream);

// De-quantization using the Quantization tables
de_quantization (jdesc, my_image);

// Unraveling of the zigzag ordering
de_zig_zag(my_image);

// Reverse Discrete Cosine Transform
idct(my_image);

// Decoded image reassembled from blocks
image_to_pixels(my_image);

// Conversion of the image from YCbCr to RGB
rgb(my_image);

// Creation of the .ppm or .pgm file (depending on if its a B&W or RGB image)
ppm(filename, my_image);
```

### Structs used
```
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
```
