GESTION DE COMPTES BANCAIRES


Descriptif : 
	
	Application permettant de gérer des comptes bancaires en utilisant une architecture de type client-serveur.


Contenue du projet :

	- projet-réseaux.pdf (sujet)
	- src_TCP/
		- client.c
		- serveur.c
		- Makefile
	- src_UDP/
		- client.c
		- serveur.c
		- Makefile
	- README.md


Installation et Utilisation :

	- Version TCP (lancé le serveur en premier)
		$ cd ./src_TCP/
		$ make
		$ ./serveur (dans un premier terminal)
		$ ./client (dans un second terminal)
	Suivre et utiliser le terminal client
	
	- Version UDP (lancé le client en premier)
		$ cd ./src_UDP/
		$ make
		$ ./client (dans un premier terminal)
		$ ./serveur (dans un second terminal)
	Suivre et utiliser le terminal client


Auteurs :

	- Jade EVRARD, élève de Polytech Sorbonne en Systèmes Embarqués
	- Jérémy ROYER, élève de Polytech Sorbonne en Systèmes Embarqués

