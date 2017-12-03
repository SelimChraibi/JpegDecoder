#include "huffman.h"



/*
    ┌───────────────────────────────────────────────────────────┐
    │                                                           │
    │              			 huffman_tree                       │
    │                                                           │
    └───────────────────────────────────────────────────────────┘
*/

struct huffman_tree {

	// Symbole quand il s'agit d'une feuille
	int8_t symbol;

	// VRAI s'il s'agit d'une feuille
	bool is_a_leaf;

	// Pointeurs sur les noeuds gauche et droit
	struct huffman_tree * left_node;
	struct huffman_tree * right_node;
};

// Constructeur de la structure d'arbre de huffman
struct huffman_tree	* new_huffman_tree(int8_t symbol, bool is_a_leaf)
{
	struct huffman_tree * my_huffman_tree = malloc(sizeof(struct huffman_tree));

	my_huffman_tree->symbol = symbol;

	my_huffman_tree->is_a_leaf = is_a_leaf;

	my_huffman_tree->left_node = NULL;
	my_huffman_tree->right_node = NULL;

	return my_huffman_tree;
}

// Destructeur de l'arbre de huffman
void free_huffman_tree(struct huffman_tree * my_huffman_tree)
{
   if (my_huffman_tree == NULL)
   	return;

   //Appels recursif pour destruction des arbres pointés par les noeuds
   free_huffman_tree(my_huffman_tree->left_node);
   free_huffman_tree(my_huffman_tree->right_node);

   //Liberation de la memoire de l'arbre courant
   free(my_huffman_tree);
}


/*
    ┌───────────────────────────────────────────────────────────┐
    │                                                           │
    │                	 Insertion de noeuds                    │
    │                                                           │
    └───────────────────────────────────────────────────────────┘
*/


// A l'insertion, la profondeur est imposée
// et on insert les symbol de gauche à droite
bool huffman_tree_insert_node(struct huffman_tree * my_huffman_tree, uint32_t depth, int8_t symbol)
{
  // Si my_huffman_tree est une feuille
  // on ne peut pas inserer le symbole
	if (my_huffman_tree->is_a_leaf)
		return false;

	// On profondeur 1 on insère en priorité à gauche
	// s'il n y a pas déjà un noeud, sinon on tente la droite
  if (depth == 1) {
  	if (my_huffman_tree->left_node == NULL) {
  		my_huffman_tree->left_node = new_huffman_tree(symbol, true);
  		return true;
  	}
  	else if (my_huffman_tree->right_node == NULL) {
  		my_huffman_tree->right_node = new_huffman_tree(symbol, true);
  		return true;
  	}
  	else
  	  // Il faut se décaler vers la droite
  	  return false;
  }
  else if (depth > 1) {
    // On insère à une profondeur de depth - 1.
    // On cree au besoin le noeud gauche (ou droit) s'il n'existe pas déjà
  	if (my_huffman_tree->left_node == NULL) {
  		my_huffman_tree->left_node = new_huffman_tree(0, false); // 0 est une valeur quelconque...
  		return huffman_tree_insert_node(my_huffman_tree->left_node, depth-1, symbol);
  	}
  	// Si le noeud gauche existait déjà, il est possible que toutes
	// les noeuds à la profondeur depth - 1 en partant du noeud de gauche
	// soient des feuilles :
  	else if (!huffman_tree_insert_node(my_huffman_tree->left_node, depth-1, symbol)){
  		if (my_huffman_tree->right_node == NULL) {
  			my_huffman_tree->right_node = new_huffman_tree(0, false);
  			return huffman_tree_insert_node(my_huffman_tree->right_node, depth-1, symbol);
  		}
  		else
  			return huffman_tree_insert_node(my_huffman_tree->right_node, depth-1, symbol);
  	}
  	else
  	  	// L'insertion a réussi à gauche sous un noeud existant
  		return true;
  }
  // Si on arrive jusqu'ici c'est que l'arbre de huffman à une profondeur nulle,
  // il n'a qu'un seul noeud
  my_huffman_tree->symbol = symbol;
  my_huffman_tree->is_a_leaf = true;

  return true;
}



/*
    ┌───────────────────────────────────────────────────────────┐
    │                                                           │
    │              		     huff_table                         │
    │                                                           │
    └───────────────────────────────────────────────────────────┘
*/

struct huff_table {

  	// nb_of_codes = nombres de codes à la profondeur 0, 1, 2... 16
  	// (162 valeurs possibles pour rle or 2^15 < 162 < 2^16 d'où profondeur = 16)
	uint32_t nb_of_codes[16];

	// Pointeur sur le vecteur des symboles
	int8_t * symbols;

	// Noeud sommet de l'arbre de huffman
	struct huffman_tree	* my_huffman_tree;

};

// Constructeur de la structure de la table huffman
// cf 2.4.1 table = < 0 3 2 d e a c >
struct huff_table *new_huff_table()
{
	struct huff_table *my_huff_table = malloc(sizeof(struct huff_table));

	my_huff_table->symbols = NULL;

	// On insère une structure vide d'arbre de huffman
	my_huff_table->my_huffman_tree = new_huffman_tree(0, false);

	return my_huff_table;
}

// Destructeur de huff_table
void free_huffman_table(struct huff_table * table)
{
  if (table == NULL)
  	return;

  free_huffman_tree(table->my_huffman_tree);

  free(table->symbols);

  free(table);
}


/*
    ┌───────────────────────────────────────────────────────────┐
    │                                                           │
    │              	     load_huffman_table                     │
    │                                                           │
    └───────────────────────────────────────────────────────────┘
*/


// Chargement d'une table de huffman à partir d'un bistream
// On évalue le nombre de byte lue et on le retourne dans nb_byte_read
void huff_table_load_symbols(struct huff_table * my_huff_table,
							 struct bitstream * stream,
							 uint16_t * nb_byte_read)
{
	uint32_t nb_of_symbols = 0;
	// Nombre total de symboles = Somme(nb_of_codes)
	for (uint8_t i = 0;  i < 16; i++){
		nb_of_symbols += my_huff_table->nb_of_codes[i];
	}

	// Allocation de la zone de mémoire pour les symboles
	my_huff_table->symbols = malloc(nb_of_symbols * sizeof(int8_t));

	uint32_t symbol_read;
	// On lit les symboles les un après les autres
	// On les stocke dans le vecteur des symboles
	// On incrémente les nombres de bytes lues
	for (uint32_t j =  0; j < nb_of_symbols; j++) {
		if (read_bitstream(stream, 8, &symbol_read, true) != 8){
			fprintf(stderr, cat("load_symbols :", "erreur lecture symbole n°: %d"), j);
			exit(1);
		}
		else {
			my_huff_table->symbols[j] = symbol_read;
		 	(*nb_byte_read)++;
		}
	}
}

// Remplissage de l'arbre de huffman associé à une table de huffman
void huffman_tree_setup(struct huff_table * my_huff_table)
{
  uint32_t	nb_inserted = 0;

  // On insère les tous les symboles
  // de la profondeur 1 jusqu'à 16
  for(uint32_t i = 0; i < 16; i++) {
  	for (uint32_t j = 0; j < my_huff_table->nb_of_codes[i]; j++) {
  		huffman_tree_insert_node(my_huff_table->my_huffman_tree, i + 1, my_huff_table->symbols[nb_inserted+j]);
  	}
  	nb_inserted += my_huff_table->nb_of_codes[i];
  }
}


// Chargement d'une table de huffman
// On évalue le nombre d'octets lus et on le retourne nb_byte_read
struct huff_table * load_huffman_table(struct bitstream	* stream,
                                       uint16_t * nb_byte_read)
{
	if (stream == NULL) {
		fprintf(stderr, "%s", cat("load_huffman_table :", "bitstream non valide"));
		exit(1);
	}

	// On cré une nouvelle table
	struct huff_table * my_huff_table = new_huff_table();

	// On commence par lire les nb_of_codes des profondeurs allant
	// de 1 à 15
	for (int i = 0; i < 16; i++){
		if (read_bitstream(stream, 8, &(my_huff_table->nb_of_codes[i]), true) != 8) {
			fprintf(stderr, cat("load_huffman_table :", "erreur lecture octet n°: %d"), i);
			exit(1);
		}
		// On incrémente le nombre d'octets lus
		(* nb_byte_read)++;
	}

	// Chargement des symboles
	// (Le nombre d'octets lus est mis à jour)
	huff_table_load_symbols(my_huff_table, stream, nb_byte_read);

	// On construit l'arbre de huffman à partir de la table
	huffman_tree_setup(my_huff_table);

	return my_huff_table;
}


/*
    ┌───────────────────────────────────────────────────────────┐
    │                                                           │
    │              	     next_huffman_value                     │
    │                                                           │
    └───────────────────────────────────────────────────────────┘
*/


// Parcours de l'arbre de huffman associé à une table de huffman.
// La table et son arbre sont supposés déjà construits
int8_t next_huffman_value(struct huff_table * table,
                          struct bitstream * stream)
{
	uint32_t bit;

  	if (table == NULL || stream == NULL) {
  		fprintf(stderr, "%s", cat("next_huffman_value :", "table ou bitstream non valide"));
    	exit(1);
  	}
  	struct huffman_tree * my_huffman_tree = table->my_huffman_tree;

  	if (my_huffman_tree == NULL) {
  		fprintf(stderr, "%s", cat("next_huffman_value :", "arbre non valide"));
    	exit(1);
  	}

	// Tant qu'on a pas atteind une feuille ou un noeud NULL
  	while(!my_huffman_tree->is_a_leaf && my_huffman_tree)
  	{
  		// Lecture d'un bit à partir du bitstream
  		if (read_bitstream(stream, 1, &bit, true) != 1) {
  			fprintf(stderr, "%s", cat("next_huffman_value :", "Echec de lecture par bits à partir du bitstream"));
  			exit(1);
  		}

  		if (bit == 0)
  	  	// si bit == 0 on descend à gauche
  			my_huffman_tree = my_huffman_tree->left_node;
  		else
  	  	// si bit == 1 on descend à droite
  			my_huffman_tree = my_huffman_tree->right_node;
  	}
  	// On fini par atteindre une feuille dont on retourne le symbol
  	return my_huffman_tree->symbol;
}
