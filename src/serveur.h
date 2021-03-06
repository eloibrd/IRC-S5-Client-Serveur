/*
 * SPDX-FileCopyrightText: 2020 John Samuel
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */


#ifndef __SERVER_H__
#define __SERVER_H__

#define PORT 8089

/* accepter la nouvelle connection d'un client et lire les données
 * envoyées par le client. En suite, le serveur envoie un message
 * en retour
 */
int recois_envoie_message(int socketfdn,int pid);
float moyenne(int *tableau, int const size) ;
float maximum(int *tableau, int const size) ;
float minimum(int *tableau) ;
float ecart_type(int *tableau, int const size) ;

#endif
