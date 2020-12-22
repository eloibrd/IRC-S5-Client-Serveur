## Projet en C - CPE IRC S5 Module Algo

### [Github du projet](https://github.com/eloibrd/IRC-S5-Client-Serveur)

---

# Client Serveur pour traitement d'images

### A. _Objectifs_

L'objectif du projet est de créer un environnement client serveur dans lequel les clients traitent des images puis envoient leurs analyses au serveur. Le projet est découpé en 5 tâches qui sont détaillées ci-dessous.

#### Tâche 1 :

**_Missions :_**

-   [x] Le client envoie son nom puis le serveur renvoie son nom comme accusé de réception
-   [x] Le client envoie un message simple et le serveur renvoie une réponse
-   [x] Le client envoie une opération en 3 requêtes et le serveur renvoie le résultat de l'opération
-   [x] Le client envoie N couleurs et le serveur les enregistre dans un fichier
-   [x] Le client envoie N balises et le serveur les enregistre dans un fichier

**_Rendu 1 :_** Le début de la **tâche 2** (JSON) a déjà été effectué mais le corps du message n'est pas séparé dans le champs valeurs (ex: on a valeurs:["+,6,8"] au lieu d'avoir valeurs:["+","6","8"]).

#### Tâche 2 :

**_Missions :_**

-   [x] Messages au format JSON

    -   [x] messages
    -   [x] nom
    -   [x] opérations
    -   [x] couleurs
    -   [x] balises

-   [x] Images BMP

#### Tâche 3 :

**_Missions :_**

-   [x] Contrôle JSON valide
-   [x] Contrôle message du client correspondant au type de demande

#### Tâche 4 :

**_Missions :_**

-   [x] Operations complexes
    -   [x] Minimum
    -   [x] Maximum
    -   [x] Moyenne
    -   [X] Ecart-Type
    
#### Tâche 5 :

- Architecture multi clients [X]
- Test unitaire []

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


