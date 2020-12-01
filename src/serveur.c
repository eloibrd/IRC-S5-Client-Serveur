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
#include "shared.h"

void tri_a_bulle(int *tableau, int const size) 
{
	int ordered = 1;

	for (int i = 0; i < size && ordered; ++i)
	{
		ordered = 0;
		for (int j = 1; j < size - i; ++j)
		{
			if (tableau[j-1] > tableau[j])
			{
				int swap = tableau[j-1];
				tableau[j-1] = tableau[j];
				tableau[j] = swap;
				ordered = 1;
 			}
		}
	}
}

float moyenne(int *tableau, int const size) 
{
    
}

int maximum(int *tableau, int const size) 
{
    
}

int minimum(int *tableau, int const size) 
{
	
}

float ecart_type(int *tableau, int const size) 
{
	
}

void complexe_operation(char * data){
    //parsing datas
    char operateurs[1024];
    // la réinitialisation de l'ensemble des données
    memset(operateurs, 0, sizeof(operateurs));
    strcpy(operateurs,data);

    // analyse chaine couleur
    // parse la string pour savoir combien de couleurs ont étés recues
    char separator = ',';
    int operation = -1;
    char * sep_at = strchr(operateurs, separator);
    if(sep_at != NULL){
        *sep_at = '\0'; 
        operation = atoi(operateurs);
        strcpy(operateurs, sep_at + 1);
    }
    printf("opération : %d, nombres : %s\n", operation, operateurs);
    // recopier les balises
    char operateurs_array[100];
    sep_at = strchr(operateurs, separator);
    int cpt = 0;
    while(sep_at != NULL){
        *sep_at = '\0';
        operateurs_array[cpt]= atoi(operateurs);
        strcpy(operateurs, sep_at + 1);
        sep_at = strchr(operateurs, separator);
        cpt++;
    }
    operateurs_array[cpt]= atoi(operateurs);

}

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
    printf("Le client connecté a envoyé : %s\n",data);
    // réponse saisie par l'utilisateur
    char response[1024];
    // la réinitialisation de l'ensemble des données
    memset(response, 0, sizeof(response));
    // Demandez à l'utilisateur d'entrer un message
    char message[100];
    printf("Votre réponse au client (max 1000 caracteres): ");
    fgets(message, 1024, stdin);
    removeChar(message,'\n');
    Json_object MSG;
    strcpy(MSG.code,"message");
    strcpy(MSG.valeur,message);    
    JSONToString(response,&MSG);
    // snprintf(response,sizeof(response),"{ \"code\" : \"message\" , \"valeurs\" : [\"%s\"] }",message);
    printf("Envoi de la reponse au client ...\n");

    int write_status = write(client_socket_fd, response, strlen(response));
    if ( write_status < 0 ) {
        perror("erreur ecriture");
        exit(EXIT_FAILURE);
    }
}

int renvoie_nom(int client_socket_fd, char *data) {
    printf("Client connecté : %s\n", data);
    char message[100]="";
    strcpy(message,data);
    char response[1024];
    memset(response, 0, sizeof(response));
    // snprintf(response,sizeof(response),"{ \"code\" : \"nom\" , \"valeurs\" : [\"%s\"] }",data);
    removeChar(message,'\n');
    Json_object MSG;
    strcpy(MSG.code,"nom");
    strcpy(MSG.valeur,message);    
    JSONToString(response,&MSG);
    int write_data = write (client_socket_fd, response, strlen(response));

    if (write_data < 0) {
        perror("erreur ecriture");
        return(EXIT_FAILURE);
    }
}

int  recois_numeros_calcule(int client_socket_fd, char *data){
    /*
    *Récéption d'un message de type calcule
    */
    char response[1024];
    int init_size = strlen(data);
    char delim[] = ",";

    char *ptr = strtok(data, delim);
    
    printf("Calcul '%s'\n", ptr);

    char *op1 = ptr;
    char *operands = strtok(NULL, delim);
    int op2 = atoi(operands);
    operands = strtok(NULL, delim);
    int op3 = atoi(operands);

    printf("operation: %d %s %d\n",op2,op1,op3);

    int result;
    if(strcmp("+",op1)==0){
        result= op2 + op3;
    }else if(strcmp("-",op1)==0){
        result= op2 - op3;
    }else if(strcmp("*",op1)==0){
        result= op2 * op3;
    }else if(strcmp("/",op1)==0){
        result= op2 / op3;
    }else{
        // TODO Tache 4 here
      //  complexe_operation(data);
    }
    printf("resultat: %d\n",result);
    char value[100];
    sprintf(value, "%d", result);

    Json_object MSG;
    strcpy(MSG.code,"calcule");
    strcpy(MSG.valeur,value);    
    JSONToString(response,&MSG);
    // snprintf(response,sizeof(response),"{ \"code\" : \"calcule\" , \"valeurs\" : [\"%s\"] }",value);
    printf("Envoi du resultat au client ...\n");
    int data_size_write = write (client_socket_fd, (void *) response, strlen(response));

    if (data_size_write< 0) {
        perror("erreur lecture");
        return(EXIT_FAILURE);
    }
}

int recois_chaine_couleurs(int client_socket_fd, char *data) {    
    char couleurs[1024];
    // la réinitialisation de l'ensemble des données
    memset(couleurs, 0, sizeof(couleurs));
    strcpy(couleurs, data);

    // analyse chaine couleur
    // parse la string pour savoir combien de couleurs ont étés recues
    char separator = ',';
    int nbcouleurs = -1;
    char * sep_at = strchr(couleurs, separator);
    if(sep_at != NULL){
        *sep_at = '\0'; 
        nbcouleurs = atoi(couleurs);
        strcpy(couleurs, sep_at + 1);
    }
    printf("%d couleurs : %s\n", nbcouleurs, couleurs);
    // recopier les couleurs
    char couleurs_array[nbcouleurs][8];
    sep_at = strchr(couleurs, separator);
    int cpt = 0;
    while(sep_at != NULL && cpt < nbcouleurs){
        *sep_at = '\0';
        strcpy(couleurs_array[cpt], couleurs);
        strcpy(couleurs, sep_at + 1);
        sep_at = strchr(couleurs, separator);
        cpt++;
    }
    strcpy(couleurs_array[nbcouleurs-1], couleurs);
    
    system("echo '-- Nouvelle entrée --' >> couleurs.txt");
    char echo1[] = "echo '";
    char echo2[] = "' >> couleurs.txt";
    char echo_msg[100];
    for(int i = 0;i<nbcouleurs;i++){
        strcpy(echo_msg, echo1);
        strcat(echo_msg, couleurs_array[i]);
        strcat(echo_msg, echo2);
        system(echo_msg);  
    }    
    printf("Couleurs sauvegardées dans le fichier couleurs.txt\n");

    // réponse au client
    char response[1024];
    char value[] = "couleurs reçues et sauvegardées";
    Json_object MSG;
    strcpy(MSG.code,"couleurs");
    strcpy(MSG.valeur,value);    
    JSONToString(response,&MSG);
    // snprintf(response,sizeof(response),"{ \"code\" : \"couleurs\" , \"valeurs\" : [\"%s\"] }",value);
    int write_status = write(client_socket_fd, response, strlen(response));
    if ( write_status < 0 ) {
        perror("erreur ecriture");
        exit(EXIT_FAILURE);
    }
}

int recois_balises(int client_socket_fd, char *data) {    
    char balises[1024];
    // la réinitialisation de l'ensemble des données
    memset(balises, 0, sizeof(balises));
    strcpy(balises,data);

    // analyse chaine couleur
    // parse la string pour savoir combien de couleurs ont étés recues
    char separator = ',';
    int nbbalises = -1;
    char * sep_at = strchr(balises, separator);
    if(sep_at != NULL){
        *sep_at = '\0'; 
        nbbalises = atoi(balises);
        strcpy(balises, sep_at + 1);
    }
    printf("%d balises : %s\n", nbbalises, balises);
    // recopier les balises
    char balises_array[nbbalises][100];
    sep_at = strchr(balises, separator);
    int cpt = 0;
    while(sep_at != NULL && cpt < nbbalises){
        *sep_at = '\0';
        strcpy(balises_array[cpt], balises);
        strcpy(balises, sep_at + 1);
        sep_at = strchr(balises, separator);
        cpt++;
    }
    strcpy(balises_array[nbbalises-1], balises);
    
    system("echo '-- Nouvelle entrée --' >> balises.txt");
    char echo1[] = "echo '";
    char echo2[] = "' >> balises.txt";
    char echo_msg[100];
    for(int i = 0;i<nbbalises;i++){
        strcpy(echo_msg, echo1);
        strcat(echo_msg, balises_array[i]);
        strcat(echo_msg, echo2);
        system(echo_msg);  
    }    
    printf("Balises sauvegardées dans le fichier balises.txt\n");

    // réponse au client
    char response[1024];
    char value[100] = "balises reçues et sauvegardées";
    Json_object MSG;
    strcpy(MSG.code,"balises");
    strcpy(MSG.valeur,value);    
    JSONToString(response,&MSG);
    // snprintf(response,sizeof(response),"{ \"code\" : \"balises\" , \"valeurs\" : [\"%s\"] }",value);
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
    // validation du JSON
    int notValid = JSONValidator(data);
    if(notValid){
        printf("Le message reçu n'a pas une syntaxe valide... \n");
    }else{
        printf("Le message reçu a une syntaxe valide... \n");
    }
    Json_object JSON_message;
    StringToJSON(data,&JSON_message);

    printf ("Message recu : %s \nanalyse ...\n",data);
    // sscanf(data, "{ \"code\" : %s , \"valeurs\" : [\" %s \"] }",code_value,value);
    printf("Code recu:%s \n",JSON_message.code);
    printf("Valeurs:%s \n",JSON_message.valeur);
    printf("Traitement ... \n\n");

    if (strstr(JSON_message.code, "message") != NULL) {
        renvoie_message(client_socket_fd, JSON_message.valeur);
    }
    else if (strstr(JSON_message.code, "nom") != NULL) {
        renvoie_nom(client_socket_fd, JSON_message.valeur);
    }
    else if (strstr(JSON_message.code, "calcule") != NULL) {
        recois_numeros_calcule(client_socket_fd, JSON_message.valeur);
    }
    else if (strstr(JSON_message.code, "couleurs") != NULL) {
        recois_chaine_couleurs(client_socket_fd, JSON_message.valeur);
    }
    else if (strstr(JSON_message.code, "balises") != NULL) {
        recois_balises(client_socket_fd, JSON_message.valeur);
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
