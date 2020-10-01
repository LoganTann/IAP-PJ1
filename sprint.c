/*
	sprint1.c
	
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
#define MSG_SPECIALITES "## consultation des specialites\n" 
#define MSG_INTERRUPTION "## fin de programme\n"
#define MSG_EMBAUCHE "## nouveau travailleur \"%s\" competent pour la specialite \"%s\"\n" 
#define MSG_TRAVAILLEURS "## consultation des travailleurs competents pour la specialite \"%s\"\n"
#define MSG_TRAVAILLEURS_TOUS "## consultation des travailleurs competents pour chaque specialite\n"
#define MSG_DEMARCHE "## nouveau client \"%s\"\n"
#define MSG_CLIENT "## consultation des commandes effectuees par \"%s\"\n"
#define MSG_CLIENT_TOUS "## consultation des commandes effectuees par chaque client\n"
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
void get_id(Mot id) {
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

typedef struct Specialite {
	Mot name;
	int cost;
} Specialite;

Specialite SpecListe[SPECIALITE_SIZE];

// Utils

int insertInSpec(Specialite pSpec) {
	printf("%s %d\n", pSpec.name, pSpec.cost);
	int i = 0;
	for (; i < SPECIALITE_SIZE; ++i) {
		if (!SpecListe[i].name) {
			SpecListe[i] = pSpec;
			printf("%s %d", SpecListe[i].name, SpecListe[i].cost);
			break;
		}
	}
	return i;
}

// Commandes

/*
* traite_developpe()
* developpe <Mot nom_specialite> <int cout_horaire>
* Ajoute une specialité au programme
*/
void traite_developpe() {
	Mot nom_specialite;
	get_id(nom_specialite);
	int cout_horaire = get_int();

	// printf(MSG_DEVELOPPE, nom_specialite, cout_horaire);

	Specialite spec;
	strcpy(spec.name, nom_specialite);
	spec.cost = cout_horaire;

	int insertedAt = insertInSpec(spec);
	printf("%d %s %d\n", insertedAt, spec.name, spec.cost);
}

/* 
* traite_specialites()
* specialites
* Affiche les specialités enregistrées dans le programme
*/ 
void traite_specialites() {
	printf(MSG_SPECIALITES);

	for (int i = 0; i < SPECIALITE_SIZE; ++i) {
		if (SpecListe[i].name != NULL) {
			printf("%s %d\n", SpecListe[i].name, SpecListe[i].cost);
		}
	}
}

/*
* traite_embauche() : traite les arguments de la commande suivante : 
* embauche <Mot travailleur> <Mot specialite>
*/
void traite_embauche() {
	Mot travailleur, specialite;
	get_id(travailleur);
	get_id(specialite);
	printf(MSG_EMBAUCHE, travailleur, specialite);
}

/* 
* traite_travailleurs()
* travailleurs <Mot specialite = "tous">
* Affiche les travailleurs selon la specialité choisie ou non
*/ 
void traite_travailleurs() {
	Mot specialite;
	get_id(specialite);
	if (strcmp(specialite, "tous") == 0)
		printf(MSG_TRAVAILLEURS_TOUS);
	else
		printf(MSG_TRAVAILLEURS, specialite);
}

/* 
* traite_demarche()
* demarche <Mot nom_client>
* Ajoute un client au programme
*/ 
void traite_demarche() {
	Mot nom_client;
	get_id(nom_client);
	printf(MSG_DEMARCHE, nom_client);
}

/* 
* traite_client()
* client <Mot nom_client>
* Affiche les commandes d'un client
*/ 
void traite_client() {
	Mot nom_client;
	get_id(nom_client);
	if (strcmp(nom_client, "tous") == 0)
		printf(MSG_CLIENT_TOUS);
	else
		printf(MSG_CLIENT, nom_client);
}

/* 
* traite_commande()
* commande <Mot produit> <Mot nom_client>
* Ajoute une commande au programme
*/ 
void traite_commande() {
	Mot produit, nom_client;
	get_id(produit);
	get_id(nom_client);
	printf(MSG_COMMANDE, produit, nom_client);
}

/*
* traite_supervision()
* supervision
* Affiche l'avancement actuel des commandes
*/
void traite_supervision() {
	printf(MSG_SUPERVISION);
}

/* 
* traite_tache()
* tache <Mot produit> <Mot specialite> <int heures>
* Ajoute une specialité ainsi que le nombre d'heures requises à une commande
*/ 
void traite_tache() {
	Mot produit, nom_client;
	get_id(produit);
	get_id(nom_client);
	int heures = get_int();
	printf(MSG_TACHE, produit, nom_client, heures);
}

/*
* traite_charge()
* charge <Mot nom_travailleur>
* A
*/
void traite_charge() {
	Mot nom_travailleur;
	get_id(nom_travailleur);
	printf(MSG_CHARGE, nom_travailleur);
}

/* 
* traite_progression()
* tache <Mot produit> <Mot specialite> <int heures_travaillees>
* Enregistre une progression aux seins d'une specialité d'une commande
*/ 
void traite_progression() {
	Mot produit, nom_client;
	get_id(produit);
	get_id(nom_client);
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
	Mot buffer;
	while (VRAI) {
		get_id(buffer);
		if (strcmp(buffer, "developpe") == 0) {
			traite_developpe();
			continue;
		}
		if (strcmp(buffer, "specialites") == 0) {
			traite_specialites();
			continue;
		}
		if (strcmp(buffer, "embauche") == 0) {
			traite_embauche();
			continue;
		}
		if (strcmp(buffer, "travailleurs") == 0) {
			traite_travailleurs();
			continue;
		}
		if (strcmp(buffer, "demarche") == 0) {
			traite_demarche();
			continue;
		}
		if (strcmp(buffer, "client") == 0) {
			traite_client();
			continue;
		}
		if (strcmp(buffer, "commande") == 0) {
			traite_commande();
			continue;
		}
		if (strcmp(buffer, "supervision") == 0) {
			traite_supervision();
			continue;
		}
		if (strcmp(buffer, "tache") == 0) {
			traite_tache();
			continue;
		}
		if (strcmp(buffer, "charge") == 0) {
			traite_charge();
			continue;
		}
		if (strcmp(buffer, "progression") == 0) {
			traite_progression();
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