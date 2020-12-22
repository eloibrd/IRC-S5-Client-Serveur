/*
 * SPDX-FileCopyrightText: 2020 John Samuel
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "client.h"
#include "bmp.h"
#include "shared.h"

/*
 * Fonction d'envoi et de réception de messages
 * Il faut un argument : l'identifiant de la socket
 */

int envoie_recois_message(int socketfd) {

    char data[1024];
    // la réinitialisation de l'ensemble des données
    memset(data, 0, sizeof(data));

    // Demandez à l'utilisateur d'entrer un message
    char message[100];
    printf("Votre message (max 1000 caracteres): ");
    fgets(message, 1024, stdin);

    removeChar(message,'\n');
    Json_object MSG;
    strcpy(MSG.code,"message");
    strcpy(MSG.valeur,message);    
    JSONToString(data,&MSG);
    //snprintf(data,sizeof(data),"{ \"code\" : \"message\" , \"valeurs\" : [\"%s\"] }",message);
	printf("Envoi du message ...\n");
    int write_status = write(socketfd, data, strlen(data));
    if ( write_status < 0 ) {
        perror("erreur ecriture");
        exit(EXIT_FAILURE);
    }

    // la réinitialisation de l'ensemble des données
    memset(data, 0, sizeof(data));


    // lire les données de la socket
    int read_status = read(socketfd, data, sizeof(data));
    if ( read_status < 0 ) {
        perror("erreur lecture");
        return -1;
    }

    Json_object JSON_message;
    StringToJSON(data,&JSON_message);
   
    printf("Message recu du serveur: %s\n",JSON_message.valeur);

    return 0;
}

void analyse(char *pathname, char *data) {
    //compte de couleurs
    couleur_compteur *cc = analyse_bmp_image(pathname);

    int count;
    strcpy(data, "couleurs: ");
    char temp_string[10] = "10,";
    if (cc->size < 10) {
        sprintf(temp_string, "%d,", cc->size);
    }
    strcat(data, temp_string);

    //choisir 10 couleurs
    for (count = 1; count < 11 && cc->size - count >0; count++) {
        if(cc->compte_bit ==  BITS32) {
        sprintf(temp_string, "#%02x%02x%02x,", cc->cc.cc24[cc->size-count].c.rouge,cc->cc.cc32[cc->size-count].c.vert,cc->cc.cc32[cc->size-count].c.bleu);
        }
        if(cc->compte_bit ==  BITS24) {
        sprintf(temp_string, "#%02x%02x%02x,", cc->cc.cc32[cc->size-count].c.rouge,cc->cc.cc32[cc->size-count].c.vert,cc->cc.cc32[cc->size-count].c.bleu);
        }
        strcat(data, temp_string);
    }

    //enlever le dernier virgule
    data[strlen(data)-1] = '\0';
}

int envoie_couleurs(int socketfd, char *pathname) {
    char data[1024];
    memset(data, 0, sizeof(data));
    analyse(pathname, data);

    int write_status = write(socketfd, data, strlen(data));
    if ( write_status < 0 ) {
        perror("erreur ecriture");
        exit(EXIT_FAILURE);
    }

    return 0;
}

int envoie_chaine_couleurs(int socketfd){
    char data[1024];
    // la réinitialisation de l'ensemble des données
    memset(data, 0, sizeof(data));
    // Demandez à l'utilisateur d'entrer les couleurs
    char message[100];
    printf("Merci d'envoyer vos couleurs RGB sous la forme 'nbcouleurs,#rrggbb,#rrggbb...' (ex: 2,#ffffff,#0066FF)\n");
    printf("Vos couleurs (format rgb) (max 100 caractères): ");
    fgets(message, 1024, stdin);

    removeChar(message,'\n');
    Json_object couleurs;
    strcpy(couleurs.code,"couleurs");
    strcpy(couleurs.valeur,message);    
    JSONToString(data,&couleurs);
    // snprintf(data,sizeof(data),"{ \"code\" : \"couleurs\" , \"valeurs\" : [\"%s\"] }",message);
	printf("Envoi des couleurs ...\n");
    // strcpy(data, "couleurs: ");
    // strcat(data, message);
    int write_status = write(socketfd, data, strlen(data));
    if ( write_status < 0 ) {
        perror("erreur ecriture");
        exit(EXIT_FAILURE);
    }

    // réponse du serveur
    char response[100];
    // la réinitialisation de l'ensemble des données
    memset(response, 0, sizeof(response));
    int read_status = read(socketfd, response, sizeof(response));
    if ( read_status < 0 ) {
        perror("erreur lecture");
        return -1;
    }
    Json_object JSON_message;
    StringToJSON(response,&JSON_message);
   
    printf("Message recu du serveur: %s\n",JSON_message.valeur);
    return 0;
}

int envoie_balises(int socketfd){
    char data[1024];
    memset(data, 0, sizeof(data));
    // Demandez à l'utilisateur d'entrer les balises
    char message[100];
    printf("Merci d'envoyer vos balises sous la forme 'nbbalises,#balise1,#balise2...' (ex: 2,#toto,#arbre)\n");
    printf("Vos balises (max 100 caractères): ");
    fgets(message, 1024, stdin);

    removeChar(message,'\n');
    Json_object balises;
    strcpy(balises.code,"balises");
    strcpy(balises.valeur,message);    
    JSONToString(data,&balises);
    //snprintf(data,sizeof(data),"{ \"code\" : \"balises\" , \"valeurs\" : [\"%s\"] }",message);
	printf("Envoi des balises ...\n");
    // strcpy(data, "balises: ");
    // strcat(data, message);
    int write_status = write(socketfd, data, strlen(data));
    if ( write_status < 0 ) {
        perror("erreur ecriture");
        exit(EXIT_FAILURE);
    }

    // réponse du serveur
    char response[100];
    // la réinitialisation de l'ensemble des données
    memset(response, 0, sizeof(response));
    int read_status = read(socketfd, response, sizeof(response));
    if ( read_status < 0 ) {
        perror("erreur lecture");
        return -1;
    }
    Json_object JSON_message;
    StringToJSON(response,&JSON_message);
   
    printf("Message recu du serveur: %s\n",JSON_message.valeur);
    return 0;
}

int envoie_nom_de_client(int socketfd){
    char data[1024];
    // la réinitialisation de l'ensemble des données
    memset(data, 0, sizeof(data));
    // Demandez à l'utilisateur d'entrer un message
    char message[100];
    printf("Votre nom (max 100 caracteres): ");
    fgets(message, 1024, stdin);

    removeChar(message,'\n');
    Json_object nom_client;
    strcpy(nom_client.code,"nom");
    strcpy(nom_client.valeur,message);    
    JSONToString(data,&nom_client);
    // snprintf(data,sizeof(data),"{ \"code\" : \"nom\" , \"valeurs\" : [\"%s\"] }",message);
	printf("Envoi du nom ...\n");

    int write_status = write(socketfd, data, strlen(data));
    if ( write_status < 0 ) {
        perror("erreur ecriture");
        exit(EXIT_FAILURE);
    }

    memset(data,0,sizeof(data));
    int read_status = read(socketfd, data, sizeof(data));
    if ( read_status < 0 ) {
        perror("erreur lecture");
        return -1;
    }
    Json_object JSON_message;
    StringToJSON(data,&JSON_message);
   
    printf("Message recu du serveur: %s\n",JSON_message.valeur);

    return 0;
}

int envoie_operateur_numero(int socketfd){
    char data[1024];
    // la réinitialisation de l'ensemble des données
    memset(data, 0, sizeof(data));
    // Demandez à l'utilisateur d'entrer un message
    char message[100];
    printf("Merci d'envoyer votre calcul sous la forme 'calc,op1,op2' (ex: +,4,5)\n");
    printf("Votre calcule (max 100 caracteres): ");
    fgets(message, 1024, stdin);

    removeChar(message,'\n');
    Json_object calcul;
    strcpy(calcul.code,"calcule");
    strcpy(calcul.valeur,message);    
    JSONToString(data,&calcul);
    // snprintf(data,sizeof(data),"{ \"code\" : \"calcule\" , \"valeurs\" : [\"%s\"] }",message);
    printf("Envoi de l'opération ...\n");
    int write_status = write(socketfd, data, strlen(data));
    if ( write_status < 0 ) {
        perror("erreur ecriture");
        exit(EXIT_FAILURE);
    }

    memset(data, 0, sizeof(data));

    int read_status = read(socketfd, data, sizeof(data));
    if ( read_status < 0 ) {
        perror("erreur lecture");
        return -1;
    }

    Json_object JSON_message;
    StringToJSON(data,&JSON_message);
   
    printf("Réponse du serveur: %s\n",JSON_message.valeur);

    // printf("Message recu: %s\n", data);

    return 0;
}

int main(int argc, char **argv) {
    int socketfd;
    int bind_status;

    struct sockaddr_in server_addr, client_addr;

    /*
    * Creation d'une socket
    */
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if ( socketfd < 0 ) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    //détails du serveur (adresse et port)
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    //demande de connection au serveur
    int connect_status = connect(socketfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if ( connect_status < 0 ) {
        perror("connection serveur");
        exit(EXIT_FAILURE);
    }

    // ----- DIFFERENTES REQUETES -----
    // TODO : décommenter un seul appel à utiliser pour l'exécution
    // --------------------------------

    // envoi du nom du client au serveur
    // envoie_nom_de_client(socketfd);

    // envoi d'un message au serveur
     envoie_recois_message(socketfd);

    // envoi d'un calcul
    //envoie_operateur_numero(socketfd);
    
    // envoi d'une liste de couleurs au format rgb (#rrggbb)
    // envoie_chaine_couleurs(socketfd);

    // envoi d'une liste de balises
    // envoie_balises(socketfd);

    // envoi d'une image /!\ NON TRAITE NE PAS UTILISER
    // envoie_couleurs(socketfd, argv[1]);

    //fin de la connexion
    close(socketfd);
}
