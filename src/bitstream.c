#include "bitstream.h"


struct bitstream {
	FILE * file;

	// bits_left est le nombre de bits restants à extraire du buffer
	// à la prochaine lecture
	//
	// Exemple :
	// ┌───┬───┬───┬───┬───┬───┬───┬───┐
	// │ 7 │ 6 │ 5 │ 4 │ 3 │ 2 │ 1 │ 0 │ = buffer
	// └───┴───┴───┴───┴───┴───┴───┴───┘
	//             ─────────────────────
	//                  nb_left = 5
	//
	// Dans cet exemple l'indice du premier bit à lire est nb_left - 1 = 4

	uint8_t bits_left;

	// Dernier octet extrait du fichier mais non complétement lu
	uint8_t	buffer;

	// True si le dernier octet lu = 0xFF (utile pour le byte stuffing)
	bool last_FF;
};


//─────────────────────────────────────────────────────────────


// Constructeur d'un bitstream (crée un bitstream et ouvre le fichier)
struct bitstream * create_bitstream(const char * fileName)
{
 	struct bitstream * stream =  malloc(sizeof(struct bitstream));

	stream->file = fopen(fileName, "r");

	if (stream->file ==  NULL) {
		fprintf(stderr, "%s", cat("create_bitstream :","Impossible d'ouvrir le fichier. Bitstream non créé"));
		exit(0);
	}

	stream->bits_left = 0;

	return stream;
}

// Destruction du bitstream (ferme le fichier)
void close_bitstream(struct bitstream * stream)
{
	if (stream != NULL) {
		fclose(stream->file);
		free(stream);
	}
}


//─────────────────────────────────────────────────────────────


// Extraction d'un octet du bitstream
uint8_t load_one_byte(struct bitstream * stream)
{
	if (stream->file ==  NULL) {
		fprintf(stderr, "%s", cat("load_one_byte:","bitstream non valide"));
		exit(1);
	}
	if (end_of_bitstream(stream)) {
		fprintf(stderr, "%s", cat("load_one_byte:","Fin du bitstream atteint"));
		exit(1);
	}

	fread(&(stream->buffer), sizeof(uint8_t), 1, stream->file);

	// A chaque lecture d'un octet on réinitialise le nombre
	// de bits restants à lire à 8
	stream->bits_left = 8;

	return stream->buffer;
}


//─────────────────────────────────────────────────────────────


// Lecture par blocs de nb_bits depuis un bitstream
uint8_t read_bitstream(struct bitstream * stream, uint8_t nb_bits, uint32_t * dest,
                        				bool discard_byte_stuffing)
{
	if (stream == NULL) {
		fprintf(stderr, "%s", cat("read_bitstream :"," Stream non valide"));
		exit(1);
	}

	if (nb_bits > 32) {
		fprintf(stderr, "%s", cat("read_bitstream :", " Le nombre de bits à lire doit être <= 32"));
		exit(1);
	}

	if (nb_bits == 0)
		return 0;

	uint8_t nb_of_bits_read = 0;
	*dest = 0;

	// Premier bit non lu dans buffer
	uint8_t bit_read;

	for (uint32_t i = 0; i < nb_bits; i++) {
	    // S'il ne reste rien dans le buffer on lit
	    // un nouvel octet à partir du fichier
		if (stream->bits_left == 0) {
			load_one_byte(stream);

			// Si l'octet lu est 0x00 alors que le précédent était 0xFF
			// (et si discard_byte_stuffing = true) on saute 0x00
			// et on lit un nouvel octet
			if (discard_byte_stuffing && stream->buffer == (uint8_t) 0x00 && stream->last_FF)
				load_one_byte(stream);

			if(stream->buffer == (uint8_t) 0xFF)
				stream->last_FF = true;
			else
				stream->last_FF = false;

		}

		// *dest contient les bits déjà lus
		// On le décale de 1 bit vers la gauche
		*dest = 2 * (*dest);

		// Puis on récupere le premier bit non lu dans buffer (à la position stream->bits_left - 1)
		bit_read = stream->buffer & (1 << (stream->bits_left - 1)) ? 1 : 0;

		// et on l'ajoute "à droite "de *dest
		*dest += bit_read;

		//On décrémente le nombre de bits restants dans le buffer
		stream->bits_left--;
		// Et on incrémente le nombre de bits lus
		nb_of_bits_read++;
	}

	return nb_of_bits_read;
}


//─────────────────────────────────────────────────────────────


// Cette fonction teste l'arrivée à la fin du bitstream
bool end_of_bitstream(struct bitstream * stream)
{
	if (stream ==  NULL || stream->file ==  NULL) {
		fprintf(stderr, "%s", cat("end_of_bitstream:", " bitstream non valide"));
		exit(1);
	}

	// On est en fin de bitstream lorsque le dernier octet du fichier
	// a été extrait (feof != 0)
	// et s'il ne reste plus de bits à lire dans le buffer
	if (feof(stream->file) != 0 && stream->bits_left == 0)
		return true;
	else
		return false;
}
