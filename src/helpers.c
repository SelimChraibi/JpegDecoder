#include "helpers.h"

// valeur(m, i) comme définie à la table 2.1
int16_t value(uint8_t magnitude, uint32_t index)
{
    // Si lindex est inferieur à la moitier
    // du nombre de valeurs possibles,
    // la valeur cherchée est 1 + i - 2^m
    if (index < pow(2, magnitude - 1)) {
        return 1 + index - pow(2, magnitude);
    } else {
        return index;
    }
}


//─────────────────────────────────────────────────────────────


// Récupère le nom du fichier s'il est valide, exit sinon
const char * valid_filename(int argc, char const * argv[])
{
    // On vérififie qu'on a bien 2 arguements
    if (argc != 2) {
        fprintf(stderr, cat("Aucun fichier n'a été donné en argument", "Utilisation: %s fichier.jp[e]g"), argv[0]);
        exit(1);
    }

    // On récupère l'extension
    const char * filename = argv[1];
	char * extension = strrchr(filename, '.');

    // On vérifie qu'elle n'est pas vide...
    if (extension == NULL) {
        fprintf(stderr, cat("\"%s\" n'est pas un fichier JPEG", "Utilisation: %s fichier.jp[e]g"), filename, argv[0]);
        exit(1);
    // ..et qu'elle correspond bien à un fichier jp[e]g
	} else {
		if (strcmp(extension, ".jpg") != 0) {
			if (strcmp(extension, ".jpeg") != 0) {
                fprintf(stderr, cat("\"%s\" n'est pas un fichier JPEG", "Utilisation: %s fichier.jp[e]g"), filename, argv[0]);
                exit(EXIT_FAILURE);
			}
		}
	}

    // On récupère le fichier
    FILE * file;
    // S'il existe on le ferme
    if ((file = fopen(filename, "r"))){
        fclose(file);
    // Sinon on exit avec une erreur
    } else {
        fprintf(stderr, cat("\"%s\" n'existe pas", "Utilisation: %s fichier.jp[e]g"), filename, argv[0]);
        exit(EXIT_FAILURE);
    }
    return filename;
}


//─────────────────────────────────────────────────────────────


// Imprime le temps d'execution
// void print_execution_time(image * my_image, const char * filename, clock_t start)
void print_execution_time(const char * filename, clock_t start)
{
    // On récupère le temps courant (fin d'execution) en seconde
    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // On le convertit en minutes + secondes
    int minutes = cpu_time_used / 60;
    double seconds = cpu_time_used - minutes * 60;

    printf("\n%s \n\n\
    ┌───────────────────────────────────────────┐\n\
    │                                           │\n\
    │                                           │\n\
    │       Temps d'execution : %dm %.3fs         \n\
    │                                           │\n\
    │                                           │\n\
    └───────────────────────────────────────────┘ \n\n", basename((char *)filename), minutes, seconds);
}


//─────────────────────────────────────────────────────────────


// Formatage de l'impression de 2 lignes de texte (pour les erreurs)
char * cat(char * line1, char * line2)
{
    char * cat1 = "\n         ╱╲___╱╲\n        ╱ x   x ╲          ";
    char * cat2 = "\n        ╲   ◠   ╱  ──────  ";

    char * cat3 = "    \n        ╱      ╱\n   ▁▁▁▁╱      ▕▁▁▁▁▁\n  ╱▏  ╱       ▕    ╱▏\n ╱ ▏ ▕    ▕ ▕ ▕   ╱ ▏\n▕▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▏╱\n▕▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁╱\n \n";


    // On cré une nouvelle string sufisement grande pour
    // qu'on y copie cat1, 2, 3, line1, 2
    char * output_string = malloc(strlen(cat1) + strlen(cat2) + strlen(cat3)
                            + strlen(line1) + strlen(line2) + 1);
    // On y copie cat1
    strcpy(output_string, cat1);
    // à laquelle on concatenne line1, cat2 etc.
    strcat(output_string, line1);
    strcat(output_string, cat2);
    strcat(output_string, line2);
    strcat(output_string, cat3);

    return output_string;
}
