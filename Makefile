# Repertoires du projet

BIN_DIR = bin
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
IMG_DIR = images

# Options de compilation/édition des liens

CC = clang
LD = clang
INC = -I$(INC_DIR)
SYSLIB = -lm

CFLAGS += $(INC) -Wall -std=c99 -O0 -g -Wextra
LDFLAGS =


# Liste des fichiers objet

OBJ_FILES =  $(OBJ_DIR)/main.o
OBJ_FILES = $(OBJ_DIR)/jpeg_reader.o
OBJ_FILES += $(OBJ_DIR)/helpers.o $(OBJ_DIR)/structs.o
OBJ_FILES += $(OBJ_DIR)/de_quantization.o
OBJ_FILES += $(OBJ_DIR)/idct.o
#OBJ_FILES += $(OBJ_DIR)/fast_idct.o
OBJ_FILES += $(OBJ_DIR)/de_zig_zag.o $(OBJ_DIR)/decompress.o
OBJ_FILES += $(OBJ_DIR)/rgb.o $(OBJ_DIR)/ppm.o
OBJ_FILES += $(OBJ_DIR)/image_to_pixels.o
OBJ_FILES += $(OBJ_DIR)/bitstream.o
OBJ_FILES += $(OBJ_DIR)/huffman.o

# cible par défaut
TARGET = $(BIN_DIR)/jpeg2ppm



all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(LD) $(LDFLAGS) $(OBJ_FILES) -o $(TARGET) $(SYSLIB)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/*.h
	$(CC) $(CFLAGS) -c $< -o $@



.PHONY: clean

clean:
	rm -f $(TARGET) $(OBJ_FILES) $(IMG_DIR)/*.ppm $(IMG_DIR)/*.pgm
	rm -f $(IMG_DIR)/*.txt $(IMG_DIR)/*.blabla

test:
	@rm -f  images/*.ppm images/*.pgm

	@for file in ./$(IMG_DIR)/*.jpg ./$(IMG_DIR)/*.jpeg; do \
		$(TARGET) $$file; \
	done
