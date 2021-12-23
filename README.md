  ___  ____  ___  ____  ____  _____  _  _ 
 / __)( ___)/ __)(_  _)(_  _)(  _  )( \( )
( (_-. )__) \__ \  )(   _)(_  )(_)(  )  ( 
 \___/(____)(___/ (__) (____)(_____)(_)\_)
 
			  ____   ____ 
			(  _ \ ( ___)
			 )(_) ) )__) 
			(____/ (____)

  ___  _____  __  __  ____  ____  ____  ___ 
 / __)(  _  )(  \/  )(  _ \(_  _)( ___)/ __)
( (__  )(_)(  )    (  )___/  )(   )__) \__ \
 \___)(_____)(_/\/\_)(__)   (__) (____)(___/
 
		  ____    __    _  _   ___    __    ____  ____  ____  ___ 
		(  _ \  /__\  ( \( ) / __)  /__\  (_  _)(  _ \( ___)/ __)
		 ) _ < /(__)\  )  ( ( (__  /(__)\  _)(_  )   / )__) \__ \
		(____/(__)(__)(_)\_) \___)(__)(__)(____)(_)\_)(____)(___/


Descriptif : Application permettant de gérer des comptes bancaires en utilisant une architecture de type client-serveur.


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

