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
#define MSG_CHARGE   "## consultation de la charge de travail de \"%s\"\n"
#define MSG_PROGRESSION "## pour la commande \"%s\", pour la specialite \"%s\" : \"%d\" heures de plus ont ete realisees\n"
#define MSG_PASSE "## une reallocation est requise\n"

// Lexemes

#define LGMOT 35
#define NBCHIFFREMAX 5 

typedef char Mot[LGMOT + 1]; // Définition du type Mot

/**
	void get_id
	@param id Mot:
**/
void get_id(Mot* id) {
	scanf("%s", id);
	if (EchoActif) printf(">>echo %s\n", id);
}

/**
	int get_int()
	get the current word from the input, and returns it with the (int) type
**/
int get_int() {
	char buffer[NBCHIFFREMAX + 1];
	scanf("%s", buffer);
	if (EchoActif) printf(">>echo %s\n", buffer);
	return atoi(buffer);
}

// Stockage

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

// Commande

typedef struct {
	Mot produit;
	Mot client;
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
} Stockage;

// Santa's Little Helpers

int getIndex_trv(Travailleurs* travailleurs, Mot nom) {
	for (unsigned int i = 0; i < travailleurs->inserted; i++) {
		if (strcmp(travailleurs->table[i].nom, nom) == 0) {
			return i;
		}
	}
	return -1; //fallback
}

int getIndex_spe(Specialites* specialites, Mot nom) {
	for (unsigned int i = 0; i < specialites->inserted; i++) {
		if (strcmp(specialites->table[i].nom, nom) == 0) {
			return i;
		}
	}
	return 0; //fallback
}

// Commandes

/*
* traite_developpe()
* developpe <Mot nom_specialite> <int cout_horaire>
* Ajoute une specialité au programme
*/
void traite_developpe(Stockage* store) {
	Mot nom_specialite;
	get_id(&nom_specialite);
	int cout_horaire = get_int();

	if (store->specialites.inserted < SPECIALITE_SIZE) {
		strcpy(store->specialites.table[store->specialites.inserted].nom, &nom_specialite);
		store->specialites.table[store->specialites.inserted].cout_horaire = cout_horaire;
		store->specialites.inserted++;
	}
}

/*
* traite_specialites()
* specialites
* Affiche les specialités enregistrées dans le programme
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
* traite_embauche() : traite les arguments de la commande suivante :
* embauche <Mot travailleur> <Mot specialite>
*/
void traite_embauche(Stockage* store) {
	Mot travailleur, specialite;
	get_id(&travailleur);
	get_id(&specialite);

	if (store->travailleurs.inserted < TRAVAILLEURS_SIZE) {

		int travailleurExistant = getIndex_trv(&store->travailleurs, travailleur);

		if(travailleurExistant >= 0) {
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
			store->travailleurs.inserted++;
		}
	}
}

/*
* traite_travailleurs()
* travailleurs <Mot specialite = "tous">
* Affiche les travailleurs selon la specialité choisie ou non
*/
void traite_travailleurs(Stockage* store) {
	Mot specialite;
	get_id(&specialite);
	if (strcmp(specialite, "tous") == 0) {
		for (int specialitesI = 0; specialitesI < store->specialites.inserted; ++specialitesI) {
			printf(MSG_TRAVAILLEURS, store->specialites.table[specialitesI].nom);
			int passedCheck = 0;
			for (int travailleursI = store->travailleurs.inserted; travailleursI >= 0; --travailleursI) {
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
		for (int travailleursI = store->travailleurs.inserted; travailleursI >= 0; --travailleursI) {
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
* traite_demarche()
* demarche <Mot nom_client>
* Ajoute un client au programme
*/
void traite_demarche(Stockage* store) {
	Mot nom_client;
	get_id(&nom_client);

	if (store->clients.inserted < CLIENTS_SIZE) {
		strcpy(store->clients.table[store->clients.inserted].nom, &nom_client);
		store->clients.inserted++;
	}
}

/*
* traite_client()
* client <Mot nom_client>
* Affiche les commandes d'un client
*/
void traite_client(Stockage* store) {
	Mot nom_client;
	get_id(&nom_client);
	if (strcmp(nom_client, "tous") == 0) {
		for (int clientsI = 0; clientsI < store->clients.inserted; ++clientsI) {
			printf(MSG_CLIENT, store->clients.table[clientsI].nom);
			int passedCheck = 0;
			for (int commandesI = 0; commandesI < store->commandes.inserted; ++commandesI) {
				if (strcmp(store->commandes.table[commandesI].client, store->clients.table[clientsI].nom) == 0) {
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
			if (strcmp(store->commandes.table[i].client, nom_client) == 0) {
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
* traite_commande()
* commande <Mot produit> <Mot nom_client>
* Ajoute une commande au programme
*/
void traite_commande(Stockage* store) {
	Mot produit, nom_client;
	get_id(&produit);
	get_id(&nom_client);
	printf(MSG_COMMANDE, produit, nom_client);
}

/*
* traite_supervision()
* supervision
* Affiche l'avancement actuel des commandes
*/
void traite_supervision(Stockage* store) {
	printf(MSG_SUPERVISION);
}

/*
* traite_tache()
* tache <Mot produit> <Mot specialite> <int heures>
* Ajoute une specialité ainsi que le nombre d'heures requises à une commande
*/
void traite_tache(Stockage* store) {
	Mot produit, nom_client;
	get_id(&produit);
	get_id(&nom_client);
	int heures = get_int();
	printf(MSG_TACHE, produit, nom_client, heures);
}

/*
* traite_charge()
* charge <Mot nom_travailleur>
* A
*/
void traite_charge(Stockage* store) {
	Mot nom_travailleur;
	get_id(&nom_travailleur);
	printf(MSG_CHARGE, nom_travailleur);
}

/*
* traite_progression()
* tache <Mot produit> <Mot specialite> <int heures_travaillees>
* Enregistre une progression aux seins d'une specialité d'une commande
*/
void traite_progression(Stockage* store) {
	Mot produit, nom_client;
	get_id(&produit);
	get_id(&nom_client);
	int heures_travaillees = get_int();
	printf(MSG_PROGRESSION, produit, nom_client, heures_travaillees);
}

/*
* traite_passe()
* passe
* Traitement des réallocations des dernières progressions
*/
void traite_passe() {
	printf(MSG_PASSE);
}

/*
* traite_interruption()
* interruption
* Interromp le programme
*/
void traite_interruption() {
	printf(MSG_INTERRUPTION);
}

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
			traite_progression(&globalStore);
			continue;
		}
		if (strcmp(buffer, "passe") == 0) {
			traite_passe();
			continue;
		}
		if (strcmp(buffer, "interruption") == 0) {
			traite_interruption();
			break;
		}

		printf("!!! instruction inconnue >%s< !!!\n", buffer);
	}
	return 0;
}
