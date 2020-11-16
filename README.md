## Projet en C - CPE IRC S5 Module Algo

### [Github du projet](https://github.com/eloibrd/IRC-S5-Client-Serveur)

---

# Client Serveur pour traitement d'images

### A. _Objectifs_

L'objectif du projet est de créer un environnement client serveur dans lequel les clients traitent des images puis envoient leurs analyses au serveur. Le projet est découpé en 5 tâches qui sont détaillées ci-dessous.

#### Tâche 1 :

**_Missions :_**

1. [x] Le client envoie son nom puis le serveur renvoie son nom comme accusé de réception
2. [x] Le client envoie un message simple et le serveur renvoie une réponse
3. [x] Le client envoie une opération en 3 requêtes et le serveur renvoie le résultat de l'opération
4. [x] Le client envoie N couleurs et le serveur les enregistre dans un fichier
5. [x] Le client envoie N balises et le serveur les enregistre dans un fichier

**_Rendu 1 :_** Le début de la **tâche 2** (JSON) a déjà été effectué mais le corps du message n'est pas séparé dans le champs valeurs (ex: on a valeurs:["+,6,8"] au lieu d'avoir valeurs:["+","6","8"]).

#### Tâche 2 :

**_Missions :_**

1. [x] Messages au format JSON (Pour le moment le message est envoyé en une seule valeur) - **_TODO_** séparer les valeurs

    - [x] messages
    - [x] nom
    - [x] opérations
    - [x] couleurs
    - [x] balises

2. [ ] Images BMP

### B. _Fichiers_

### C. _Bibliothèques_

-   string
-   stdio
-   stdlib
-   unistd
-   sys/types
-   sys/socket
-   sys/epoll
-   netinet/in

### D. _Références_

-   [StackOverflow](https://stackoverflow.com/)
-   [Ecriture d'un Makefile](https://ensiwiki.ensimag.fr/images/e/eb/Makefile.pdf)

### E. _Difficulté_

### F. _Commentaires_
