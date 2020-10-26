/*
	sprint.c
	@author JiveOff (Antoine Banha - antoine@jiveoff.fr)
	@author ShinProg (Logan Tann - logon313@hotmail.fr)
*/

#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef enum { FAUX = 0, VRAI = 1 } Booleen;
Booleen EchoActif = FAUX;

// Messages emis par les instructions

#define MSG_DEVELOPPE "## nouvelle specialite \"%s\" ; cout horaire \"%d\"\n"
#define MSG_SPECIALITES "specialites traitees : "
#define MSG_INTERRUPTION "## fin de programme\n"
#define MSG_TRAVAILLEURS "la specialite %s peut etre prise en charge par : "
#define MSG_CLIENT "le client %s a commande : "
#define MSG_COMMANDE "## nouvelle commande \"%s\", par client \"%s\"\n"
#define MSG_SUPERVISION "## consultation de l’avancement des commandes\n"
#define MSG_TACHE "## la commande \"%s\" requiere la specialite \"%s\" (nombre d’heures \"%d\")\n"
#define MSG_CHARGE   "charge de travail pour %s : "
#define MSG_PROGRESSION "## pour la commande \"%s\", pour la specialite \"%s\" : \"%d\" heures de plus ont ete realisees\n"
#define MSG_PASSE "## une reallocation est requise\n"

// Lexemes

#define LGMOT 35
#define NBCHIFFREMAX 5
#define MAX_COMMANDES 500
#define MAX_SPECIALITES 10
typedef char Mot[LGMOT + 1]; // Définition du type Mot
typedef unsigned long long Gros_entier;


void get_id(Mot* id) {
	scanf("%s", id);
	if (EchoActif) printf(">>echo %s\n", id);
}

int get_int() {
	char buffer[NBCHIFFREMAX + 1];
	scanf("%s", buffer);
	if (EchoActif) printf(">>echo %s\n", buffer);
	return atoi(buffer);
}

// Déclarations des constantes

const enum {
	SPECIALITE_SIZE = 10,
	TRAVAILLEURS_SIZE = 50,
	CLIENTS_SIZE = 100,
	COMMANDES_SIZE = 500
};

// Specialité

typedef struct {
	Mot nom;
	int cout_horaire;
} Specialite;

typedef struct {
	Specialite table[SPECIALITE_SIZE];
	int inserted;
} Specialites;

// Travailleurs

typedef struct {
	Mot nom;
	Booleen tag_specialite[SPECIALITE_SIZE];
	int heuresRealises;
} Travailleur;

typedef struct {
	Travailleur table[TRAVAILLEURS_SIZE];
	int inserted;
} Travailleurs;

// Client

typedef struct {
	Mot nom;
} Client;

typedef struct {
	Client table[CLIENTS_SIZE];
	int inserted;
} Clients;

// Commande et taches

typedef  struct {
	unsigned int nb_heures_requises;
	unsigned int nb_heures_effectuees;
} Tache;

typedef struct {
	Mot produit;
	Mot nom_client;
	Tache liste_taches[SPECIALITE_SIZE];
	Booleen complete;
	// L'identifiant du travailleur en charge de la tache
	unsigned int en_charge_tache[SPECIALITE_SIZE];
} Commande;

typedef struct {
	Commande table[COMMANDES_SIZE];
	int inserted;
} Commandes;


// Stockage global

typedef struct {
	Specialites specialites;
	Travailleurs travailleurs;
	Clients clients;
	Commandes commandes;

	// pour la commande progression passe
	int lastCommande;
	int lastSpecialite;
	int lastProgression;
} Stockage;

// Declaration des prototypes de fonction ---------------------------------------------------//


// fonctions helpers

/**
* [brief] Permet de récupérer une entrée écrite par l'utilisateur.
* id [out] Pointeur de type Mot qui permettra la modification de la variable originale.
* [pre] (l'entrée de l'utilisateur doit être valide)
**/
void get_id(Mot* id);

/**
* [brief] A le même effet que get_id, mais retourne l'entrée en tant que int plutôt que affecter dans l'argument via pointeur.
* [pre] (l'entrée de l'utilisateur doit être un entier naturel)
* return: l'entier qui a été entré par l'utilisateur.
**/
int get_int();

/**
* [brief] Donné une structure Commandes (in 1) et le nom d'un produit (in 2), retourne l'indice du produit concerné dans le tableau de la structure Commandes.
*	ie. trouve et retourne l'entier n tel que Commandes->table[n].produit est égale nom_produit
* commandes [in] : pointeur constant d'une struct de type Commandes. Spécifie la struct dans laquelle on va chercher.
* nom_produit [in] : de type Mot. Spécifie le nom du produit à rechercher dans la struct donnée en tant que premier argument.
* return: l'indice du produit concerné dans le tableau de la structure Commandes.
**/
int getIndex_cmd(const Commandes* commandes, Mot nom_produit);

/**
* [brief] Donné une structure Travailleurs (in 1) et le nom d'un travailleur (in 2), retourne l'indice du travailleur concerné dans le tableau de la structure Travailleurs.
*	ie. trouve et retourne l'entier n tel que Travailleurs->table[n].nom est égale nom
* travailleurs [in] : pointeur constant d'une struct de type Travailleurs. Spécifie la struct dans laquelle on va chercher.
* nom [in] : de type Mot. Spécifie le nom du travailleur à rechercher dans la struct donnée en tant que premier argument.
* return: l'indice du travailleur concerné dans le tableau de la structure Travailleurs.
**/
int getIndex_trv(const Travailleurs* travailleurs, Mot nom);

/**
* [brief] Donné une structure Specialites (in 1) et le nom d'une spécialité (in 2), retourne l'indice de la spécialité concerné dans le tableau de la structure Specialites.
*   ie. trouve et retourne l'entier n tel que Specialites->table[n].nom est égale nom
* specialites [in] : pointeur constant d'une struct de type Specialites. Spécifie la struct dans laquelle on va chercher.
* nom [in] : de type Mot. Spécifie le nom de la spécialité à rechercher dans la struct donnée en tant que premier argument.
* return: l'indice de la spécialité concerné dans le tableau de la structure Specialites.
**/
int getIndex_spe(const Specialites* specialites, Mot nom);

/**
* [brief] Une spécialité sera créée à l'aide de l'entrée utilisateur demandée.
* store [in-out] : pointeur constant d'une struct de type Stockage. Spécifie la struct dans laquelle on va chercher et éditer.
**/
void traite_developpe(Stockage* store);

/**
* [brief] La liste des spécialités sera affichée à l'utilisateur.
* store [in] : pointeur constant d'une struct de type Stockage. Spécifie la struct dans laquelle on va chercher.
**/
void traite_specialites(const Stockage* store);

/**
* [brief] Un nouveau travailleur sera crée à l'aide de l'entrée utilisateur.
* store [in-out] : pointeur constant d'une struct de type Stockage. Spécifie la struct dans laquelle on va chercher et éditer.
**/
void traite_embauche(Stockage* store);

/**
* [brief] La liste des travailleurs sera affichée à l'utilisateur selon la spécialité mentionnée ou "tous".
* store [in] : pointeur constant d'une struct de type Stockage. Spécifie la struct dans laquelle on va chercher.
**/
void traite_travailleurs(const Stockage* store);

/**
* [brief] Un nouveau client sera crée à l'aide de l'entrée utilisateur.
* store [in-out] : pointeur constant d'une struct de type Stockage. Spécifie la struct dans laquelle on va chercher et éditer.
**/
void traite_demarche(Stockage* store);

/**
* [brief] La liste des commandes d'un client donnée par l'utilisateur sera affichée à l'utilisateur.
*		  L'utilisateur peut aussi mentionner "tous" si il veut voir toutes les commandes de chaque client.
* store [in] : pointeur constant d'une struct de type Stockage. Spécifie la struct dans laquelle on va chercher.
**/
void traite_client(const Stockage* store);

/**
* [brief] Une nouvelle commande sera créée à l'aide de l'entrée utilisateur.
* store [in-out] : pointeur constant d'une struct de type Stockage. Spécifie la struct dans laquelle on va chercher et éditer.
**/
void traite_commande(Stockage* store);

/**
* [brief] Affiche l'avancement actuel des commandes.
* store [in] : pointeur constant d'une struct de type Stockage. Spécifie la struct dans laquelle on va chercher.
**/
void traite_supervision(const Stockage* store);

/**
* [brief] Determine le travailleur le plus adéquat pour une spécialité et le renvoie sous forme de char car le MAX_TRAVAILLEURS < CHAR_MAX.
* store [in] : pointeur constant d'une struct de type Stockage. Spécifie la struct dans laquelle on va chercher.
* id_spe [in] : indice de la spécialité.
* return: indice du travailleur determiné.
**/
char determiner_travailleur_pour(const Stockage* store, int id_spe);

/**
* [brief] Ajoute une specialité ainsi que le nombre d'heures requises à une commande.
* store [in-out] : pointeur constant d'une struct de type Stockage. Spécifie la struct dans laquelle on va chercher et éditer.
**/
void traite_tache(Stockage* store);

/**
* [brief] Affiche la charge de travail d'un travailleur.
* store [in] : pointeur constant d'une struct de type Stockage. Spécifie la struct dans laquelle on va chercher.
**/
void traite_charge(const Stockage* store);

/**
* [brief] Vérifie si toutes les tâches de la commande spécifiée dans l'argument cmd sont complétées.
* store [in] : pointeur constant d'une struct de type Stockage. Spécifie la struct dans laquelle on va chercher.
* cmd [in] : indice de la commande
* return: booleen VRAI ou FAUX indiquant si les tâches de la commande spécifiée sont bien completées.
**/
Booleen verif_facturation(const Stockage* store, unsigned int cmd);

/**
* [brief] Vérifie si absolutement toutes les commandes sont complétées.
* store [in] : pointeur constant d'une struct de type Stockage. Spécifie la struct dans laquelle on va chercher.
* return: booleen VRAI ou FAUX indiquant si toutes les tâches de toutes les commande sont bien completées.
**/
Booleen verif_facturationsGlobales(const Stockage* store);

/**
* [brief] Indique une progression pour une tâche donnée par l'entrée utilisateur.
* store [in] : pointeur constant d'une struct de type Stockage. Spécifie la struct dans laquelle on va chercher et éditer.
* return: booleen VRAI ou FAUX indiquant si toutes les tâches de toutes les commande sont bien completées en utilisant les fonctions de vérifications au dessus.
**/
Booleen traite_progression(Stockage* store);

/**
* [brief] Réaffecte un nouveau travailleur pour la dernière tâche passée dans l'instruction progression.
* store [in] : pointeur constant d'une struct de type Stockage. Spécifie la struct dans laquelle on va chercher et éditer.
**/
void traite_passe(Stockage* store);

// Porte d'entrée du programme

int main(int argc, char* argv[]) {
	if (argc >= 2 && strcmp("echo", argv[1]) == 0) {
		EchoActif = VRAI;
	}
	setlocale(LC_ALL, "fr-FR");

	Stockage globalStore;
	globalStore.specialites.inserted = 0;
	globalStore.travailleurs.inserted = 0;
	globalStore.clients.inserted = 0;
	globalStore.commandes.inserted = 0;

	globalStore.lastCommande = -1;
	globalStore.lastSpecialite = -1;
	globalStore.lastProgression = -1;

	Mot buffer;
	while (VRAI) {
		get_id(&buffer);
		if (strcmp(buffer, "developpe") == 0) {
			traite_developpe(&globalStore);
			continue;
		}
		if (strcmp(buffer, "specialites") == 0) {
			traite_specialites(&globalStore);
			continue;
		}
		if (strcmp(buffer, "embauche") == 0) {
			traite_embauche(&globalStore);
			continue;
		}
		if (strcmp(buffer, "travailleurs") == 0) {
			traite_travailleurs(&globalStore);
			continue;
		}
		if (strcmp(buffer, "demarche") == 0) {
			traite_demarche(&globalStore);
			continue;
		}
		if (strcmp(buffer, "client") == 0) {
			traite_client(&globalStore);
			continue;
		}
		if (strcmp(buffer, "commande") == 0) {
			traite_commande(&globalStore);
			continue;
		}
		if (strcmp(buffer, "supervision") == 0) {
			traite_supervision(&globalStore);
			continue;
		}
		if (strcmp(buffer, "tache") == 0) {
			traite_tache(&globalStore);
			continue;
		}
		if (strcmp(buffer, "charge") == 0) {
			traite_charge(&globalStore);
			continue;
		}
		if (strcmp(buffer, "progression") == 0) {
			if (traite_progression(&globalStore)) {
				continue;
			}
			else {
				break;
			}
		}
		if (strcmp(buffer, "passe") == 0) {
			traite_passe(&globalStore);
			continue;
		}
		if (strcmp(buffer, "interruption") == 0) {
			break;
		}

		printf("!!! instruction inconnue >%s< !!!\n", buffer);
	}
	return 0;
}

// Helpers

int getIndex_cmd(const Commandes* commandes, Mot nom_produit) {
	// dans le cas où commandes->inserted vaut 0, la boucle n'est pas executée.
	// Il ne s'agit donc pas d'une précondition
	for (unsigned int i = 0; i < commandes->inserted; i++) {
		if (strcmp(commandes->table[i].produit, nom_produit) == 0) {
			return i;
		}
	}
	return -1; //fallback
}

int getIndex_trv(const Travailleurs* travailleurs, Mot nom) {
	for (unsigned int i = 0; i < travailleurs->inserted; i++) {
		if (strcmp(travailleurs->table[i].nom, nom) == 0) {
			return i;
		}
	}
	return -1; //fallback
}

int getIndex_spe(const Specialites* specialites, Mot nom) {
	for (unsigned int i = 0; i < specialites->inserted; i++) {
		if (strcmp(specialites->table[i].nom, nom) == 0) {
			return i;
		}
	}
	return -1; //fallback
}

// Commandes

/*
* traite_developpe() || CF. prototype pour la doc de cette fonction
*
* Entrée utilisateur attendue : developpe <Mot nom_specialite> <int cout_horaire>
* où nom_specialite est le nom de la spécialité à créer
* et cout_horaire le prix de facturation d'une heure de travail pour cette spé
*/
void traite_developpe(Stockage* store) {
	Mot nom_specialite;
	get_id(&nom_specialite);
	int cout_horaire = get_int();

	if (store->specialites.inserted < SPECIALITE_SIZE) {
		strcpy(store->specialites.table[store->specialites.inserted].nom, nom_specialite);
		store->specialites.table[store->specialites.inserted].cout_horaire = cout_horaire;
		store->specialites.inserted++;
	}
}

/*
* traite_specialites()|| CF. prototype pour la doc de cette fonction
*
* Entrée utilisateur attendue : specialites
* Cette instruction affiche les specialités enregistrées dans le programme dans l’ordre de leur déclaration, ainsi que le coût associé
*/
void traite_specialites(Stockage* store) {
	printf(MSG_SPECIALITES);

	for (int i = 0; i < store->specialites.inserted; ++i) {
		if (i == 0)
			printf("%s/%d", store->specialites.table[i].nom, store->specialites.table[i].cout_horaire);
		else
			printf(", %s/%d", store->specialites.table[i].nom, store->specialites.table[i].cout_horaire);
	}

	printf("\n");
}

/*
* traite_embauche()|| CF. prototype pour la doc de cette fonction
*
* Entrée utilisateur attendue : embauche <Mot travailleur> <Mot specialite>
* Recherche le travailleur qui a pour nom le premier argument (ci celui-ci n'existe pas, alors il sera créé),
* et lui assigne la spécialité donnée en tant que second argument
*/
void traite_embauche(Stockage* store) {
	Mot travailleur, specialite;
	get_id(&travailleur);
	get_id(&specialite);

	if (store->travailleurs.inserted < TRAVAILLEURS_SIZE) {

		int travailleurExistant = getIndex_trv(&store->travailleurs, travailleur);

		if (travailleurExistant >= 0) {
			int indexSpe = getIndex_spe(&store->specialites, specialite);
			store->travailleurs.table[travailleurExistant].tag_specialite[indexSpe] = VRAI;
		}
		else {
			strcpy(store->travailleurs.table[store->travailleurs.inserted].nom, travailleur);
			int indexSpe = getIndex_spe(&store->specialites, specialite);
			for (unsigned int i = 0; i < SPECIALITE_SIZE; ++i) {
				store->travailleurs.table[store->travailleurs.inserted]
					.tag_specialite[i] = (i == indexSpe) ? VRAI : FAUX;
				// Necessité d'initiliaser chaque cases pour éviter des bugs.
			}
			store->travailleurs.table[store->travailleurs.inserted].heuresRealises = 0;
			store->travailleurs.inserted++;
		}
	}
}

/*
* traite_travailleurs() || CF. prototype pour la doc de cette fonction
*
* Entrée utilisateur attendue : travailleurs <Mot specialite || "tous">
* Affiche la liste des travailleurs compétents pour la spécialité donnée en argument,par ordre de déclaration des travailleurs.
* Si le nom de la spécialité vaut "tous", affiche la liste des travailleurs pour toutes les spécialités.
*/
void traite_travailleurs(const Stockage* store) {
	Mot specialite;
	get_id(&specialite);
	if (strcmp(specialite, "tous") == 0) {
		for (int specialitesI = 0; specialitesI < store->specialites.inserted; ++specialitesI) {
			printf(MSG_TRAVAILLEURS, store->specialites.table[specialitesI].nom);
			int passedCheck = 0;
			for (int travailleursI = 0; travailleursI < store->travailleurs.inserted; ++travailleursI) {
				if (store->travailleurs.table[travailleursI].tag_specialite[specialitesI] == VRAI) {
					if (passedCheck == 0)
						printf("%s", store->travailleurs.table[travailleursI].nom);
					else
						printf(", %s", store->travailleurs.table[travailleursI].nom);
					passedCheck++;
				}
			}
			printf("\n");
		}
	}
	else {
		printf(MSG_TRAVAILLEURS, specialite);
		int indexSpe = getIndex_spe(&store->specialites, specialite);
		int passedCheck = 0;
		for (int travailleursI = 0; travailleursI < store->travailleurs.inserted; ++travailleursI) {
			if (store->travailleurs.table[travailleursI].tag_specialite[indexSpe] == VRAI) {
				if (passedCheck == 0)
					printf("%s", store->travailleurs.table[travailleursI].nom);
				else
					printf(", %s", store->travailleurs.table[travailleursI].nom);
				passedCheck++;
			}
		}
		printf("\n");
	}
}

/*
* traite_demarche() || CF. prototype pour la doc de cette fonction
*
* demarche <Mot nom_client>
* Déclare un nouveau client qui aura pour nom le premier argument
*/
void traite_demarche(Stockage* store) {
	Mot nom_client;
	get_id(&nom_client);

	if (store->clients.inserted < CLIENTS_SIZE) {
		strcpy(store->clients.table[store->clients.inserted].nom, nom_client);
		store->clients.inserted++;
	}
}

/*
* traite_client() || CF. prototype pour la doc de cette fonction
*
* client <Mot nom_client>
* Affiche les commandes du client ayant pour nom le premier argument
*/
void traite_client(const Stockage* store) {
	Mot nom_client;
	get_id(&nom_client);
	if (strcmp(nom_client, "tous") == 0) {
		for (int clientsI = 0; clientsI < store->clients.inserted; ++clientsI) {
			printf(MSG_CLIENT, store->clients.table[clientsI].nom);
			int passedCheck = 0;
			for (int commandesI = 0; commandesI < store->commandes.inserted; ++commandesI) {
				if (strcmp(store->commandes.table[commandesI].nom_client, store->clients.table[clientsI].nom) == 0) {
					if (passedCheck == 0)
						printf("%s", store->commandes.table[commandesI].produit);
					else
						printf(", %s", store->commandes.table[commandesI].produit);
					passedCheck++;
				}
			}
			printf("\n");
		}
	}
	else {
		printf(MSG_CLIENT, nom_client);
		int passedCheck = 0;
		for (int i = 0; i < store->commandes.inserted; ++i) {
			if (strcmp(store->commandes.table[i].nom_client, nom_client) == 0) {
				if (passedCheck == 0)
					printf("%s", store->commandes.table[i].produit);
				else
					printf(", %s", store->commandes.table[i].produit);
				passedCheck++;
			}
		}
		printf("\n");
	}
}

/*
* traite_commande() || CF. prototype pour la doc de cette fonction
*
* commande <Mot produit> <Mot nom_client>
* Ajoute une commande au programme, qui aura pour nom le premier argument.
* Le second spécifie le nom du client qui l'a commandé
*/
void traite_commande(Stockage* store) {
	Mot produit, nom_client;
	get_id(&produit);
	get_id(&nom_client);

	const unsigned int i = store->commandes.inserted++; // stockage puis incrémentation
	strcpy(store->commandes.table[i].nom_client, nom_client);
	strcpy(store->commandes.table[i].produit, produit);
	store->commandes.table[i].complete = FAUX;

	//initialisation de la liste de tâches
	for (unsigned int speNbr = 0; speNbr < MAX_SPECIALITES; ++speNbr)
	{
		store->commandes.table[i].liste_taches[speNbr].nb_heures_requises = 0;
		store->commandes.table[i].liste_taches[speNbr].nb_heures_effectuees = 0;
	}

}

/*
* traite_supervision() || CF. prototype pour la doc de cette fonction
*
* Affiche l'avancement actuel des commandes sous le format suivant:
*	"etat des taches pour <nom_produit> : [<liste>, ..., <liste>]"
*		avec <liste> valant "<specialite>:<heures effectuées>/<heures nécessaires>"
*/
void traite_supervision(const Stockage* store) {
	for (int i_cmd = 0; i_cmd < store->commandes.inserted; ++i_cmd)
	{
		printf("etat des taches pour %s : ", store->commandes.table[i_cmd].produit);
		Booleen isFirstTime = VRAI;
		for (int i_spe = 0; i_spe < store->specialites.inserted; ++i_spe)
		{
			const Tache tacheCourante = store->commandes.table[i_cmd].liste_taches[i_spe];
			if (tacheCourante.nb_heures_requises) { // valeur si vide : 0 = faux
				if (isFirstTime) {
					isFirstTime = FAUX;
				}
				else {
					printf(", ");
				}

				printf("%s:", store->specialites.table[i_spe].nom);
				printf("%d/%d", tacheCourante.nb_heures_effectuees, tacheCourante.nb_heures_requises);
			}
		}
		printf("\n");
	}
}


/**
 * Détermine le travailleur le plus adapté pour la réalisation d'une tâche.
 * Une tâche étant déterminée par une spécialité, il attends donc en ENTREE
 * l'identifiant d'une spécialitée attendue, et afin de faire ses opérations
 * le pointeur du tableau de travailleurs
 * En SORTIE, il retourne l'index du travailleur à prendre en charge.
**/
char determiner_travailleur_pour(const Stockage* store, int id_spe) {

	unsigned int retval = TRAVAILLEURS_SIZE;

	int totalWorker[TRAVAILLEURS_SIZE];
	for (int i = 0; i < TRAVAILLEURS_SIZE; ++i) totalWorker[i] = 0;

	for (int id_worker = 0; id_worker < store->travailleurs.inserted; ++id_worker) {
		for (int i_cmd = 0; i_cmd < store->commandes.inserted; ++i_cmd) {
			for (int i_spe = 0; i_spe < store->specialites.inserted; ++i_spe) {
				if (store->commandes.table[i_cmd].en_charge_tache[i_spe] == id_worker) {
					totalWorker[id_worker] += store->commandes.table[i_cmd].liste_taches[i_spe].nb_heures_requises - store->commandes.table[i_cmd].liste_taches[i_spe].nb_heures_effectuees;
				}
			}
		}
	}

	int lowestHours = -1;
	for (int id_worker = 0; id_worker < store->travailleurs.inserted; ++id_worker) {
		if (store->travailleurs.table[id_worker].tag_specialite[id_spe] == VRAI) {
			if (lowestHours < 0) {
				lowestHours = totalWorker[id_worker];
				retval = id_worker;
			}

			if (EchoActif)
				printf(">>> >>> %s %d: %d (%d)\n", store->travailleurs.table[id_worker].nom, id_worker, totalWorker[id_worker], lowestHours);
			if (lowestHours > totalWorker[id_worker]) {
				retval = id_worker;
				lowestHours = totalWorker[id_worker];
			}
		}
	}

	return retval;
}

/*
* traite_tache() || CF. prototype pour la doc de cette fonction
*
* tache <Mot commande> <Mot specialite> <int heures>
* Ajoute une specialité ainsi que le nombre d'heures requises à une commande
*/
void traite_tache(Stockage* store) {
	Mot commande, specialite;
	get_id(&commande);
	get_id(&specialite);
	int heures = get_int();

	// recherche de la commande concernée puis de la spécialité concernée
	const unsigned int cmd_i = getIndex_cmd(&store->commandes, commande);
	const unsigned int id_spe = getIndex_spe(&store->specialites, specialite);
	// initialisation de la tâche pour la commande en question
	store->commandes.table[cmd_i].liste_taches[id_spe].nb_heures_requises = heures;
	// Assignation du travailleur à la tache
	const unsigned int id_worker = determiner_travailleur_pour(store, id_spe);
	if (id_worker < TRAVAILLEURS_SIZE) {
		store->commandes.table[cmd_i].en_charge_tache[id_spe] = id_worker;
		// store->travailleurs.table[id_worker].heuresRealises += heures;
	}
	else if (EchoActif) {
		printf("$ Erreur : aucun travailleur trouvé pour traiter la spécialité demandée.\n");
	}
	// Il sera nécessaire de stoker le nombre de [tâches||d'heures] au bout d'une
	// affectation de tâche afin de trouver par la suite le meilleur travailleur
	// pour la prochaine tâche
	// pour la beta : set_total_tasks_for(Travailleur* travailleur);
	// pour la release : set_total_hours_for(Travailleur* travailleur);
}

/*
* traite_charge() || CF. prototype pour la doc de cette fonction
*
* charge <Mot nom_travailleur>
*/
void traite_charge(const Stockage* store) {
	Mot nom_travailleur;
	get_id(&nom_travailleur);
	// printf(MSG_CHARGE, nom_travailleur);
	// superProduit/reseau/45heure(s)

	unsigned int travailleursId = getIndex_trv(&store->travailleurs, nom_travailleur);

	printf("charge de travail pour %s : ", nom_travailleur);

	Booleen isFirstTime = VRAI;
	for (int commandesI = 0; commandesI < store->commandes.inserted; commandesI++) {
		for (int specialitesI = 0; specialitesI < SPECIALITE_SIZE; specialitesI++) {

			const Tache tache = store->commandes.table[commandesI].liste_taches[specialitesI];

			if (store->commandes.table[commandesI].en_charge_tache[specialitesI] == travailleursId
				&& tache.nb_heures_requises != tache.nb_heures_effectuees) {

				if (isFirstTime) {
					isFirstTime = FAUX;
				}
				else {
					printf(", ");
				}

				printf("%s/%s/%dheure(s)", store->commandes.table[commandesI].produit, store->specialites.table[specialitesI].nom, tache.nb_heures_requises - tache.nb_heures_effectuees);
			}
		}
	}

	printf("\n");

}

/*
* verif_facturation() || CF. prototype pour la doc de cette fonction
*/
Booleen verif_facturation(Stockage* store, const unsigned int cmd) {
	int compteur = 0;
	for (int spec = 0; spec < SPECIALITE_SIZE; ++spec) {
		Tache tacheCourante = store->commandes.table[cmd].liste_taches[spec];
		if (tacheCourante.nb_heures_effectuees >= tacheCourante.nb_heures_requises) {
			compteur++;
		}
	}
	if (compteur >= SPECIALITE_SIZE) {
		return VRAI;
	}
	else {
		return FAUX;
	}
}

/*
* verif_facturationsGlobales() || CF. prototype pour la doc de cette fonction
*/
Booleen verif_facturationsGlobales(const Stockage* store) {
	int compteurGlobal = 0;
	for (int cmd = 0; cmd < store->commandes.inserted; ++cmd) {
		if (store->commandes.table[cmd].complete == VRAI) {
			compteurGlobal++;
		}
	}
	if (compteurGlobal == store->commandes.inserted) {
		return VRAI;
	}
	else {
		return FAUX;
	}
}

/*
* traite_progression() || CF. prototype pour la doc de cette fonction
*
* entrée utilisateur attendue tache : <Mot produit> <Mot specialite> <int heures_travaillees>
* Donné le nom d'une commande, et le nom de la spécialité à mettre à jour, modifie son nombre d'heures travaillées
*/
Booleen traite_progression(Stockage* store) {
	Mot commande, specialite;
	get_id(&commande);
	get_id(&specialite);
	int heures_travaillees = get_int();

	const unsigned int cmd_i = getIndex_cmd(&store->commandes, commande);
	const unsigned int id_spe = getIndex_spe(&store->specialites, specialite);
	store->commandes.table[cmd_i].liste_taches[id_spe].nb_heures_effectuees += heures_travaillees;

	store->lastCommande = cmd_i;
	store->lastSpecialite = id_spe;
	store->lastProgression = heures_travaillees;

	Booleen checkFacturation = verif_facturation(store, cmd_i);
	if (checkFacturation && store->commandes.table[cmd_i].complete == FAUX) {
		printf("facturation %s : ", commande);
		Booleen firstDone = VRAI;
		for (int specCommande = 0; specCommande < SPECIALITE_SIZE; ++specCommande) {
			if (store->commandes.table[cmd_i].liste_taches[specCommande].nb_heures_requises != 0) {
				if (firstDone) {
					firstDone = FAUX;
				}
				else {
					printf(", ");
				}
				Gros_entier cout_commande = store->commandes.table[cmd_i].liste_taches[specCommande].nb_heures_effectuees * store->specialites.table[specCommande].cout_horaire;
				printf("%s:%llu", store->specialites.table[specCommande].nom, cout_commande);
				store->commandes.table[cmd_i].complete = VRAI;
			}
		}
		printf("\n");
	}

	Booleen checkFacturationsGlobales = verif_facturationsGlobales(store);
	if (checkFacturationsGlobales) {
		printf("facturations : ");
		Booleen firstDone = VRAI;
		for (int client = 0; client < store->clients.inserted; ++client) {
			Gros_entier cout_total_client = 0;
			for (int commande = 0; commande < store->commandes.inserted; ++commande) {
				if (strcmp(store->commandes.table[commande].nom_client, store->clients.table[client].nom) == 0) {
					for (int spec = 0; spec < SPECIALITE_SIZE; ++spec) {
						Tache tacheCourante = store->commandes.table[commande].liste_taches[spec];
						cout_total_client += tacheCourante.nb_heures_effectuees * store->specialites.table[spec].cout_horaire;
					}
				}
			}
			if (firstDone) {
				firstDone = FAUX;
			}
			else {
				printf(", ");
			}
			printf("%s:%llu", store->clients.table[client].nom, cout_total_client);
		}
		printf("\n");
		return FAUX;
	}

	return VRAI;
}

/*
* traite_passe() || CF. prototype pour la doc de cette fonction
*
* entrée utilisateur attendue : passe
* réaffecte un nouveau travailleur pour la dernière tâche passée en argument de l'instruction progression
*/
void traite_passe(Stockage* store) {
	if (store->lastCommande && store->lastSpecialite) {
		if (EchoActif)
			printf(">>> commande: %d / specialite: %d / worker actuel: %d \n", store->lastCommande, store->lastSpecialite, store->commandes.table[store->lastCommande].en_charge_tache[store->lastSpecialite]);
		const unsigned int id_worker = determiner_travailleur_pour(store, store->lastSpecialite);

		store->commandes.table[store->lastCommande].en_charge_tache[store->lastSpecialite] = id_worker;
		if (EchoActif)
			printf(">>> nouveau worker: %d - %d\n", id_worker, store->commandes.table[store->lastCommande].en_charge_tache[store->lastSpecialite]);
		store->lastCommande = 0;
		store->lastSpecialite = 0;
	}
}