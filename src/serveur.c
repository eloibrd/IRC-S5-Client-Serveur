/*
 * SPDX-FileCopyrightText: 2020 John Samuel
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "serveur.h"


void plot(char *data) {

    //Extraire le compteur et les couleurs RGB
    FILE *p = popen("gnuplot -persist", "w");
    printf("Plot");
    int count = 0;
    int n;
    char *saveptr = NULL;
    char *str = data;
    fprintf(p, "set xrange [-15:15]\n");
    fprintf(p, "set yrange [-15:15]\n");
    fprintf(p, "set style fill transparent solid 0.9 noborder\n");
    fprintf(p, "set title 'Top 10 colors'\n");
    fprintf(p, "plot '-' with circles lc rgbcolor variable\n");
    while(1) {
        char *token = strtok_r(str, ",", &saveptr);
        if (token == NULL) {
        break;
        }
        str=NULL;
        if (count == 0) {
        n = atoi(token);
        }
        else {
        // Le numéro 36, parceque 360° (cercle) / 10 couleurs = 36
        fprintf(p, "0 0 10 %d %d 0x%s\n", (count-1)*36, count*36, token+1);
        }
        count++;
    }
    fprintf(p, "e\n");
    printf("Plot: FIN\n");
    pclose(p);
}

/* renvoyer un message (*data) au client (client_socket_fd)
 */
int renvoie_message(int client_socket_fd, char *data) {
    // réponse saisie par l'utilisateur
    char response[1024];
    // la réinitialisation de l'ensemble des données
    memset(response, 0, sizeof(response));
    // Demandez à l'utilisateur d'entrer un message
    char message[100];
    printf("Votre réponse au client (max 1000 caracteres): ");
    fgets(message, 1024, stdin);
    //strcpy(response, "message: ");
    //strcat(response, message);
    snprintf(response,sizeof(response),"{ \"code\" : \"message\", \"valeurs\" : [\"%s\"] }",message);
    printf("JSON SEND :%s \n",response);

    int write_status = write(client_socket_fd, response, strlen(response));
    if ( write_status < 0 ) {
        perror("erreur ecriture");
        exit(EXIT_FAILURE);
    }
}

int renvoie_nom(int client_socket_fd, char *data) {
    int data_size = write (client_socket_fd, (void *) data, strlen(data));

    if (data_size < 0) {
        perror("erreur ecriture");
        return(EXIT_FAILURE);
    }
}

int  recois_numeros_calcule(int client_socket_fd, char *data){
    /*
    *Récéption d'un message de type calcule
    */
    printf ("Message Calcule recu: %s\n", data);
    // char str[100];
    // sscanf(data, "%s", str);

    int init_size = strlen(data);
    char delim[] = ":";

    char *ptr = strtok(data, delim);
    for (int i = 0; i < 1; i++)
    {
        ptr = strtok(NULL, delim);
    }
    printf("Calc'%s'\n", ptr);

    char *operands = strtok(ptr, " ");

    char *op1 = operands;
    operands = strtok(NULL, " ");
    int op2 = atoi(operands);
    operands = strtok(NULL, " ");
    int op3 = atoi(operands);

    printf("op1:%s\n",op1);
    printf("op2:%d\n",op2);
    printf("op3:%d\n",op3);

    int result;
    if(strcmp("+",op1)==0){
        result= op2 + op3;
    }else if(strcmp("-",op1)==0){
        result= op2 - op3;
    }else if(strcmp("*",op1)==0){
        result= op2 * op3;
    }else if(strcmp("/",op1)==0){
        result= op2 / op3;
    }
    printf("result:%d",result);
    char value[500];
    char* str2 = "Resultat: ";
    sprintf(value, "%d", result);
    char answer[500];
    strcat(answer,str2);
    strcat(answer,value);

    int data_size_write = write (client_socket_fd, (void *) answer, strlen(answer));

    if (data_size_write< 0) {
        perror("erreur lecture");
        return(EXIT_FAILURE);
    }
}

int recois_chaine_couleurs(int client_socket_fd, char *data) {    
    char couleurs[1024];
    // la réinitialisation de l'ensemble des données
    memset(couleurs, 0, sizeof(couleurs));
    // split la string pour enlever le code
    char separator = ':';
    char * sep_at = strchr(data, separator);
    if(sep_at != NULL)
    {
        *sep_at = '\0'; // cut la chaine au séparateur
        // prendre en compte l'espace après les ':' du code
        strcpy(couleurs, sep_at +2);
    }
    printf("chaine couleurs : %s",couleurs);

    // analyse chaine couleur
    // parse la string pour savoir combien de couleurs ont étés recues
    separator = ',';
    int nbcouleurs = -1;
    sep_at = strchr(couleurs, separator);
    if(sep_at != NULL){
        *sep_at = '\0'; 
        nbcouleurs = atoi(couleurs);
        strcpy(couleurs, sep_at + 1);
    }

    printf("%d couleurs : %s\n", nbcouleurs, couleurs);

    // réponse au client
    char response[] = "couleurs reçues";
    int write_status = write(client_socket_fd, response, strlen(response));
    if ( write_status < 0 ) {
        perror("erreur ecriture");
        exit(EXIT_FAILURE);
    }
}

/* accepter la nouvelle connection d'un client et lire les données
 * envoyées par le client. En suite, le serveur envoie un message
 * en retour
 */
int recois_envoie_message(int socketfd) {
    struct sockaddr_in client_addr;
    char data[1024];

    int client_addr_len = sizeof(client_addr);

    // nouvelle connection de client
    int client_socket_fd = accept(socketfd, (struct sockaddr *) &client_addr, &client_addr_len);
    if (client_socket_fd < 0 ) {
        perror("accept");
        return(EXIT_FAILURE);
    }

    printf("client connecté\n\n");
    // la réinitialisation de l'ensemble des données
    memset(data, 0, sizeof(data));

    //lecture de données envoyées par un client
    int data_size = read (client_socket_fd, (void *) data, sizeof(data));

    if (data_size < 0) {
        perror("erreur lecture");
        return(EXIT_FAILURE);
    }
    printf("data received\n\n");
    /*
    * extraire le code des données envoyées par le client.
    * Les données envoyées par le client peuvent commencer par le mot "message :" ou un autre mot.
    */

    char code_balise[10];
    char code_lim[10];
    char code_lim2[10];
    char code_value[10];
     printf ("Message recu: %s\n", data);
    sscanf(data, "%s %s %s %s", code_balise,code_lim,code_lim2,code_value);
    printf("Code value:%s \n",code_value);
    //Si le message commence par le mot: 'message:'
    if (strstr(code_value, "message") != NULL) {
        //printf ("Message recu aprés sscanf: %s\n", data);
        renvoie_message(client_socket_fd, data);
    }
    else if (strcmp(code_value, "\"nom\"") == 0) {
        printf ("Client connecté : %s\n", data);
        renvoie_nom(client_socket_fd, data);
    }
    else if (strcmp(code_value, "calcule:") == 0) {
        printf ("Client connecté : %s\n", data);
        recois_numeros_calcule(client_socket_fd, data);
    }
    else if (strcmp(code, "couleurs:") == 0) {
        recois_chaine_couleurs(client_socket_fd, data);
    }
    else {
        plot(data);
    }

    //fermer le socket
    close(socketfd);
}

int main() {

    int socketfd;
    int bind_status;
    int client_addr_len;

    struct sockaddr_in server_addr, client_addr;

    /*
    * Creation d'une socket
    */
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if ( socketfd < 0 ) {
        perror("Unable to open a socket");
        return -1;
    }

    int option = 1;
    setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    //détails du serveur (adresse et port)
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Relier l'adresse à la socket
    bind_status = bind(socketfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (bind_status < 0 ) {
        perror("bind");
        return(EXIT_FAILURE);
    }

    // Écouter les messages envoyés par le client
    listen(socketfd, 10);

    printf("----- Starting Server ----- \n\n");

    //Lire et répondre à une requête client client
    recois_envoie_message(socketfd);

    return 0;
}
