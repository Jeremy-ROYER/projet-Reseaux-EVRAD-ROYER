/* ==== INCLUDE ==== */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
/* ================= */

/* ==== DEFINE ==== */
#define PORT 20011
#define ADDRESS "127.0.0.1"
/* ================ */


/* ==== STRUCT ==== */
typedef struct Operation
{
	char type[16];
	float montant;
	char date[64];
} Operation;

typedef struct Compte
{
	int id;
	float solde;
	Operation operations[10];
	int nb_op;
} Compte;

typedef struct Client
{
	int id;
	char password[32];
	Compte comptes[5];
} Client;
/* ================ */


/* ==== CREATION CLIENTS ==== */
void create_clients(Client *clients)
{
	/* "Faux" client */
	/* Client courant par défaut ou lors d'une déconnexion */
	Client no_connected;
	no_connected.id = INT_MAX;


	/* Premier client */
	Client eleonore;
	eleonore.id = 572986;
	strcpy(eleonore.password, "2Lt)$6Lt6");

	eleonore.comptes[0].id = 51894652;
	eleonore.comptes[0].solde = 1798.56;
	strcpy(eleonore.comptes[0].operations[0].type, "AJOUT");
	eleonore.comptes[0].operations[0].montant = 1798.56;
	strcpy(eleonore.comptes[0].operations[0].date, "Wed Dec  1 17:52:33 2021\n");
	eleonore.comptes[1].nb_op = 1;

	eleonore.comptes[1].id = 49875632;
	eleonore.comptes[1].solde = 526;
	strcpy(eleonore.comptes[1].operations[0].type, "AJOUT");
	eleonore.comptes[1].operations[0].montant = 626; 
	strcpy(eleonore.comptes[1].operations[0].date, "Thu Nov 11 16:43:13 2021\n");
	strcpy(eleonore.comptes[1].operations[1].type, "RETRAIT");
	eleonore.comptes[1].operations[1].montant = -100;
	strcpy(eleonore.comptes[1].operations[1].date, "Mon Dec  6 10:24:42 2021\n");
	eleonore.comptes[1].nb_op = 2;
	/* -------------- */

	/* Deuxième client */
	Client marius;
	marius.id = 399464;
	strcpy(marius.password, "rP*98r2_G");

	marius.comptes[0].id = 21546851;
	marius.comptes[0].solde = 91981;
	strcpy(marius.comptes[0].operations[0].type, "AJOUT");
	marius.comptes[0].operations[0].montant = 50000;
	strcpy(marius.comptes[0].operations[0].date, "Wed Sep 22 12:10:45 2021\n");
	strcpy(marius.comptes[0].operations[1].type, "AJOUT");
	marius.comptes[0].operations[1].montant = 50000;
	strcpy(marius.comptes[0].operations[1].date, "Fri Oct 22 12:10:45 2021\n");
	strcpy(marius.comptes[0].operations[2].type, "RETRAIT");
	marius.comptes[0].operations[2].montant = -8019;
	strcpy(marius.comptes[0].operations[2].date, "Tue Nov  9 14:56:15 2021\n");
	marius.comptes[0].nb_op = 3;

	marius.comptes[1].id = 21546925;
	marius.comptes[1].solde = 14230.85;
	strcpy(marius.comptes[1].operations[0].type, "AJOUT");
	marius.comptes[1].operations[0].montant = 14230.85; 
	strcpy(marius.comptes[1].operations[0].date, "Fri Jul 30 18:26:32 2021\n");
	marius.comptes[1].nb_op = 1;

	/* --------------- */

	/* Troisième client */
	Client veronica;
	veronica.id = 751947;
	strcpy(veronica.password, "DcW/36d%8");

	veronica.comptes[0].id = 59894614;
	veronica.comptes[0].solde = 2500;
	strcpy(veronica.comptes[0].operations[0].type, "AJOUT");
	veronica.comptes[0].operations[0].montant = 2500;
	strcpy(veronica.comptes[0].operations[0].date, "Tue Oct 26 15:45:24 2021\n");
	veronica.comptes[0].nb_op = 1;

	/* ---------------- */

	/* Tableau des clients */
	clients[0] = eleonore;
	clients[1] = marius;
	clients[2] = veronica;
	clients[3] = no_connected;
	/* ------------------- */
}
/* ========================== */


/* Fonction pour vérifier le password du client
	- indice du client si OK
	- -1 sinon */
int verifPW(Client *clients, int id_client, char* password)
{
	for (int i = 0; i < 3; i++)
	{
		if(clients[i].id == id_client){
			if(strcmp(clients[i].password, password) == 0){
				return i;
			}
			else{
				printf("Mot de passe incorrect pour l'identifiant saisi.\n");
				return -1;
			}
		}
	}
	return -1;
}

/* Fonction pour vérifier l'id du compte
	- indice du compte si OK
	- -1 sinon */
int verifIC(Client *clients, int indice_client, int id_compte)
{
	for (int i = 0; i < 5; i++)
	{
		if(clients[indice_client].comptes[i].id == id_compte){
			return i;
		}
	}
	printf("Identifiant de compte inexistant.\n");
	return -1;
}

/* Fonction pour vérifier si la somme à retirer est possible
	- 0 si OK
	- -1 sinon */
int verifRE(Client *clients, int indice_client, int indice_compte, int somme)
{
	if(clients[indice_client].comptes[indice_compte].solde < somme){
		printf("Somme trop importante, découvert impossible.\n");
		return -1;
	}

	return 0;
}

/* Procédure pour l'initialisation de la connexion 
	Remplis le socket client et le socket serveur 
		SI erreur remplis {INT_MAX, INT_MAX} */
void init_connection(int* sockets)
{
	int flagVerif = -1;

	/* ETAPE SOCKET */
	int socketServer = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addrServer;
	addrServer.sin_addr.s_addr = inet_addr(ADDRESS);
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(PORT);
	/* ============ */

	/*  ETAPE BIND */
	flagVerif = bind(socketServer, (const struct sockaddr *) &addrServer, sizeof(addrServer));
	if(flagVerif < 0)
	{
		printf("Erreur bind()\n");
		sockets[0] = INT_MAX;
		return;
	}
	printf("Bind done.\n");
	/* ============ */

	/* ETAPE LISTEN */
	flagVerif = listen(socketServer, 1);
	if(flagVerif < 0)
	{
		printf("Erreur listen()\n");
		sockets[0] = INT_MAX;
		return;
	}
	printf("Listen\n");
	/* ============ */

	/* ETAPE ACCEPT */
	struct sockaddr_in addrClient;
	socklen_t csize = sizeof(addrClient);
	int socketClient = accept(socketServer, (struct sockaddr *) &addrClient, &csize);
	printf("Accept done\n");

	printf("  client : %d\n", socketClient);
	/* ============ */

	sockets[0] = socketClient;
	sockets[1] = socketServer;
}

/* Procédure de connexion d'un client 
	- indice client si OK
	- -1 sinon */
int connection_client(Client *clients, int* sockets)
{
	int flagVerif = -1;
	int id_client = INT_MAX;
	int indice_client;
	/* Nombre d'essai max pour se connecter */
	int i = 3;
	char i_convert[2];

	char password[32];
	char connect[256];

	while(flagVerif == -1){
		/* ETAPE ENVOIE 1 */
		strcpy(connect, "Bienvenue sur le site de votre banque !\n");
		strcat(connect, "Veuillez vous connecter sur votre compte client : \n");
		strcat(connect, " 	Entrez votre identifiant client (6 chiffres) : ");

		flagVerif = send(sockets[0], &connect, sizeof(connect), 0);
		if(flagVerif < 0)
		{
			printf("Erreur lors de l'envoie du message pour l'identifiant.\nVeuillez redémarrer l'appplication.\n");
			return -1;
		}
		/* ============= */

		/* ETAPE RECEPTION 1 */
		/* Récupération de l'id client */
		flagVerif = recv(sockets[0], &id_client, sizeof(id_client), 0);
		if(flagVerif < 0)
		{
			printf("Erreur lors de la réception de l'identifiant du client.\n");
			return -1;
		}
		printf("%d\n", id_client);
		/* ================= */

		/* ETAPE ENVOIE 2 */
		strcpy(connect, "Veuillez maintenant rentrer votre mot de passe :\n");
		strcat(connect, " 	Entrez votre mot de passe client (9 caractères) : ");
		flagVerif = send(sockets[0], &connect, sizeof(connect), 0);
		if(flagVerif < 0)
		{
			printf("Erreur lors de l'envoie du message pour le mot de passe.\nVeuillez redémarrer l'appplication.\n");
			return -1;
		}
		/* ============= */

		/* ETAPE RECEPTION 2 */
		/* Récupération du mot de passe client */
		flagVerif = recv(sockets[0], &password, sizeof(password), 0);
		if(flagVerif < 0)
		{
			printf("Erreur lors de la réception du mot de passe du client.\n");
			return EXIT_FAILURE;
		}
		printf("%s\n", password);
		printf("*********\n");
		/* ================= */

		indice_client = verifPW(clients, id_client, password);

		if(indice_client == -1){
			i--;
			/* Nombre d'essais dépassé */
			if(i == 0)
			{
				/* ETAPE ENVOIE 3 */
				strcpy(connect, "STOP");
				flagVerif = send(sockets[0], &connect, sizeof(connect), 0);
				if(flagVerif < 0)
				{
					printf("Erreur lors de l'envoie du signal STOP.\nVeuillez redémarrer l'appplication.\n");
					return -1;
				}
				/* ============= */

				/* ETAPE FERMETURE SOCKET */
				close(sockets[0]);
				close(sockets[1]);
				printf("Close\n");
				return -1;
			}
			else
			{
				sprintf(i_convert, "%d", i);
				/* ETAPE ENVOIE 3 */
				strcpy(connect, "\nLa connection a échoué.\n");
				strcat(connect, " 	Identifiant et/ou mot de passe incorrect.\n");
				strcat(connect, "	Vous n'avez plus que ");
				strcat(connect, i_convert);
				strcat(connect," essai(s).\n");

				flagVerif = send(sockets[0], &connect, sizeof(connect), 0);
				if(flagVerif < 0)
				{
					printf("Erreur lors de l'envoie du message d'identifiant et/ou mot de passe incorrect.\nVeuillez redémarrer l'appplication.\n");
					return -1;
				}

				flagVerif = -1;
				/* ============= */
			}
		}
		else
		{
			/* ETAPE ENVOIE 3 */
			strcpy(connect, "OK");
			flagVerif = send(sockets[0], &connect, sizeof(connect), 0);
			if(flagVerif < 0)
			{
				printf("Erreur lors de l'envoie du signal OK.\nVeuillez redémarrer l'appplication.\n");
				return -1;
			}

			flagVerif = 0;
			/* ============= */
		}
	}

	return indice_client;
}


/* Fonction pour l'envoie du message de bienvenue
	- 0 si OK
	- -1 sinon */
int bienvenue(int socketClient)
{
	int flagVerif = -1;

	/* ETAPE ENVOIE */
	char welcome[1024];
	strcpy(welcome, "Bienvenue sur votre compte client !\n");
	strcat(welcome, "Vous pouvez effectuer diverses opérations sur vos comptes : \n");
	strcat(welcome, " 	- AJOUT d'une somme\n 	- RETRAIT d'une somme\n");
	strcat(welcome, "	- SOLDE du compte\n 	- OPERATIONS récentes\n\n");
	strcat(welcome, "	- DECONNEXION pour sortir du compte client\n");
	strcat(welcome, "	- QUITTER pour terminer l'application\n");

	flagVerif = send(socketClient, &welcome, sizeof(welcome), 0);
	if(flagVerif < 0)
	{
		printf("Erreur lors de l'envoie du message de bienvenue.\nVeuillez redémarrer l'appplication.\n");
		return -1;
	}
	/* ============ */

	return 0;
}

/* Fonction permettant de récupérer l'identifiant du compte 
	- indice compte si OK
	- INT_MAX sinon */
int get_idCompte(int* sockets, Client *clients, int indice_client)
{
	int flagVerif = -1;
	int id_compte;
	int indice_compte;
	char getId[256];

	while(flagVerif == -1)
	{
		/* ETAPE ENVOIE 1 */
		strcpy(getId, "\nVeuillez rentrer l'identifiant du compte sur lequel vous souhaitez effectuer l'opération : \n");
		strcat(getId, " 	Entrez l'identifiant du compte (8 chiffres) : ");

		flagVerif = send(sockets[0], &getId, sizeof(getId), 0);
		if(flagVerif < 0)
		{
			printf("Erreur lors de l'envoie du message pour l'identifiant du compte.\nVeuillez redémarrer l'appplication.\n");
			return INT_MAX;
		}
		/* ============= */

		/* ETAPE RECEPTION 1 */
		/* Récupération de l'id compte */
		flagVerif = recv(sockets[0], &id_compte, sizeof(id_compte), 0);
		if(flagVerif < 0)
		{
			printf("Erreur lors de la réception de l'identifiant du compte.\n");
			return INT_MAX;
		}
		printf("%d\n", id_compte);
		/* ================= */

		indice_compte = verifIC(clients, indice_client, id_compte);

		if(indice_compte == -1)
		{
			/* ETAPE ENVOIE 2 */
			strcpy(getId, "\nL'identifiant du compte est incorrect.\n");

			flagVerif = send(sockets[0], &getId, sizeof(getId), 0);
			if(flagVerif < 0)
			{
				printf("Erreur lors de l'envoie du message d'identifiant du compte incorrect.\nVeuillez redémarrer l'appplication.\n");
				return INT_MAX;
			}

			flagVerif = -1;
			/* ============= */
		}
		else
		{
			/* ETAPE ENVOIE 2 */
			strcpy(getId, "OK");
			flagVerif = send(sockets[0], &getId, sizeof(getId), 0);
			if(flagVerif < 0)
			{
				printf("Erreur lors de l'envoie du signal OK.\nVeuillez redémarrer l'appplication.\n");
				return INT_MAX;
			}

			flagVerif = 0;
			/* ============= */
		}
	}

	return indice_compte;
}

/* Fonction permettant de récupérer une somme d'argent 
	- somme si OK
	- INT_MAX sinon */
float get_Somme(int* sockets, Client *clients, int indice_client, int indice_compte, int flagOP)
{
	int flagVerif = -1;
	float somme;
	char getSum[256];

	char op[16];
	if(!flagOP)
		strcpy(op, "ajouter : ");
	else
		strcpy(op, "retirer : ");

	while(flagVerif == -1)
	{
		/* ETAPE ENVOIE 1 */
		strcpy(getSum, "\n");
		strcat(getSum, " 	Entrez la somme que vous voulez ");
		strcat(getSum, op);

		flagVerif = send(sockets[0], &getSum, sizeof(getSum), 0);
		if(flagVerif < 0)
		{
			printf("Erreur lors de l'envoie du message pour la somme.\nVeuillez redémarrer l'appplication.\n");
			return INT_MAX;
		}
		/* ============= */

		/* ETAPE RECEPTION 1 */
		/* Récupération de la somme */
		flagVerif = recv(sockets[0], &somme, sizeof(somme), 0);
		if(flagVerif < 0)
		{
			printf("Erreur lors de la réception de la somme.\n");
			return INT_MAX;
		}
		printf("%.2f\n", somme);
		/* ================= */

		if(somme < 0) somme = -somme;

		/* RETRAIT */
		if(flagOP)
		{
			flagVerif = verifRE(clients, indice_client, indice_compte, somme);

			if(flagVerif == -1)
			{
				/* ETAPE ENVOIE 2 */
				strcpy(getSum, "\nLa somme rentrée est trop importante face au solde du compte.\n");

				flagVerif = send(sockets[0], &getSum, sizeof(getSum), 0);
				if(flagVerif < 0)
				{
					printf("Erreur lors de l'envoie du message somme incorrect.\nVeuillez redémarrer l'appplication.\n");
					return INT_MAX;
				}

				flagVerif = -1;
				/* ============= */
			}
			else
			{
				/* ETAPE ENVOIE 2 */
				strcpy(getSum, "OK");
				flagVerif = send(sockets[0], &getSum, sizeof(getSum), 0);
				if(flagVerif < 0)
				{
					printf("Erreur lors de l'envoie du signal OK.\nVeuillez redémarrer l'appplication.\n");
					return INT_MAX;
				}

				flagVerif = 0;
				/* ============= */
			}
		}
		/* AJOUT */
		else{
			/* ETAPE ENVOIE 2 */
			strcpy(getSum, "OK");
			flagVerif = send(sockets[0], &getSum, sizeof(getSum), 0);
			if(flagVerif < 0)
			{
				printf("Erreur lors de l'envoie du signal OK.\nVeuillez redémarrer l'appplication.\n");
				return INT_MAX;
			}

			flagVerif = 0;
			/* ============= */
		}
	}

	return somme;
}

/* Fonction permettant d'envoyer la liste des comptes du client 
	- 0 si OK
	- -1 sinon */
int send_comptes(int socketClient, Client *clients, int indice_client)
{
	int flagVerif = -1;
	char id_convert[16];
	char num_convert[8];

	char comptes[1024];
	strcpy(comptes, "\nVous possédez le(s) compte(s) suivant(s) : \n");

	for(int i=0; i<5; i++)
	{
		if(clients[indice_client].comptes[i].id != 0)
		{
			sprintf(id_convert, "%d", clients[indice_client].comptes[i].id);
			sprintf(num_convert, "%d", i+1);

			strcat(comptes,"	Compte n°");
			strcat(comptes, num_convert);
			strcat(comptes, " dont l'identifiant est ");
			strcat(comptes, id_convert);
			strcat(comptes, "\n");
		}
	}
	/* ETAPE ENVOIE */
	flagVerif = send(socketClient, &comptes, sizeof(comptes), 0);
	if(flagVerif < 0)
	{
		printf("Erreur lors de l'envoie du message de la liste des comptes.\nVeuillez redémarrer l'appplication.\n");
		return -1;
	}
	/* ============ */

	return 0;
}

/* Fonction permettant de metre à jour le tableau des opérations d'un compte
	- 0 si OK
	- -1 sinon */
int update_operations(Client *clients, int indice_client, int indice_compte, Operation new_op)
{
	int nombre_operations = clients[indice_client].comptes[indice_compte].nb_op;
	/* Tableau d'opérations pas plein */
	if(nombre_operations < 10)
	{
		clients[indice_client].comptes[indice_compte].operations[nombre_operations] = new_op;
		clients[indice_client].comptes[indice_compte].nb_op++;
	}
	else if(nombre_operations == 10)
	{
		for (int i = 0; i < 9; i++)
		{
			/* op[i] = op[i+1] - jusqu'à op[8] = op[9] */
			clients[indice_client].comptes[indice_compte].operations[i] = clients[indice_client].comptes[indice_compte].operations[i+1];
		}
		/* Ajout dernière opération */
		clients[indice_client].comptes[indice_compte].operations[9] = new_op;
	}
	else
	{
		printf("Nombre d'opérations (variable) supérieure à 10.\n");
		return -1;
	}

	return 0;
}

/* Fonction pour ajouter la somme au compte client et mettre à jour les opérations 
	- 0 si OK
	- -1 sinon */
int ajout(Client *clients, int indice_client, int indice_compte, float somme, int flagOP)
{
	int flagVerif = -1;
	time_t now;
	now = time(0);
	/* Création de la nouvelle opération */
	Operation new_operation;
	if(!flagOP)
		strcpy(new_operation.type, "AJOUT");
	else
		strcpy(new_operation.type, "RETRAIT");
	new_operation.montant = somme;
	strcpy(new_operation.date, ctime(&now));

	/* MaJ du solde du compte */
	clients[indice_client].comptes[indice_compte].solde += somme;


	flagVerif = update_operations(clients, indice_client, indice_compte, new_operation);

	return flagVerif;
}

/* Fonction pour envoyer le solde du compte du client 
	- 0 si OK
	- -1 sinon*/
int send_solde(int socketClient, Client *clients, int indice_client, int indice_compte)
{
	int flagVerif = -1;
	char id_convert[16];
	char solde_convert[16];

	sprintf(id_convert, "%d", clients[indice_client].comptes[indice_compte].id);
	sprintf(solde_convert, "%.2f", clients[indice_client].comptes[indice_compte].solde);

	/* ETAPE ENVOIE */
	char solde[128];
	strcpy(solde, "\nLe solde de votre compte n°");
	strcat(solde, id_convert);
	strcat(solde, " est de €");
	strcat(solde, solde_convert);
	strcat(solde, " (euros)\n");

	flagVerif = send(socketClient, &solde, sizeof(solde), 0);
	if(flagVerif < 0)
	{
		printf("Erreur lors de l'envoie du message du solde.\nVeuillez redémarrer l'appplication.\n");
		return -1;
	}
	/* ============ */

	return 0;
}

/* Fonction pour envoyer les dix dernières opérations du client
	- 0 si OK
	- -1 sinon */
int send_operations(int socketClient, Client *clients, int indice_client, int indice_compte)
{
	int flagVerif = -1;
	char montant_convert[16];
	char num_convert[8];

	char operations[2048];
	strcpy(operations, "\nDernièrement, vous avez effectué les opérations suivantes :\n");

	for(int i=0; i<clients[indice_client].comptes[indice_compte].nb_op; i++)
	{			
		sprintf(montant_convert, "%.2f", clients[indice_client].comptes[indice_compte].operations[i].montant);
		sprintf(num_convert, "%d", clients[indice_client].comptes[indice_compte].nb_op-i);

		strcat(operations,"	Opération n°");
		strcat(operations, num_convert);
		strcat(operations, " : \n");
		strcat(operations, "		Type 	: ");
		strcat(operations, clients[indice_client].comptes[indice_compte].operations[i].type);
		strcat(operations, "\n");
		strcat(operations, "		Date 	: ");
		strcat(operations, clients[indice_client].comptes[indice_compte].operations[i].date);
		strcat(operations, "		Montant : ");
		strcat(operations, montant_convert);
		strcat(operations, "\n\n");
		}

	/* ETAPE ENVOIE */
	flagVerif = send(socketClient, &operations, sizeof(operations), 0);
	if(flagVerif < 0)
	{
		printf("Erreur lors de l'envoie du message de la liste des operations.\nVeuillez redémarrer l'appplication.\n");
		return -1;
	}
	/* ============ */

	return 0;
}

int main(void)
{
	int flagVerif = 0;
	/* Permet de savoir s'il faut faire l'étape de connection du client */
	int flagConnection = 0;


	Client clients[4];
	create_clients(clients); 
	int indice_client = 3; /* Client no_connected */;
	int indice_compte;

	/* sockets[0] = SocketClient | sockets[1] = SocketServer */
	int sockets[2];
	init_connection(sockets);
	if(sockets[0] == INT_MAX){
		return EXIT_FAILURE;
	}

	float somme = 0;
	char buffer[64];

	while(1){
		if(flagConnection == 0)
		{
			indice_client = connection_client(clients, sockets);
			if(indice_client == -1){
				return EXIT_FAILURE;
			}

			flagVerif = bienvenue(sockets[0]);
			if(flagVerif == -1){
				return EXIT_FAILURE;
			}

			flagConnection = -1;
		}
		else{
			/* 1ere ETAPE RECEPTION */
			/* Récupération du type d'opération */
			flagVerif = recv(sockets[0], &buffer, sizeof(buffer), 0);
			if(flagVerif < 0)
			{
				printf("Erreur lors de la réception du message du client.\n");
				return EXIT_FAILURE;
			}
			printf("%s\n", buffer);
			/* ==================== */

			if(strcmp(buffer, "AJOUT") == 0)
			{
				flagVerif = send_comptes(sockets[0], clients, indice_client);
				if(flagVerif == -1){
					return EXIT_FAILURE;
				}

				indice_compte = get_idCompte(sockets, clients, indice_client);
				if(indice_compte == INT_MAX){
					return EXIT_FAILURE;
				}

				somme = get_Somme(sockets, clients, indice_client, indice_compte, 0);
				if(somme == INT_MAX){
					return EXIT_FAILURE;
				}

				flagVerif = ajout(clients, indice_client, indice_compte, somme, 0);
				if(flagVerif == -1){
					return EXIT_FAILURE;
				}
			}

			else if(strcmp(buffer, "RETRAIT") == 0)
			{
				flagVerif = send_comptes(sockets[0], clients, indice_client);
				if(flagVerif == -1){
					return EXIT_FAILURE;
				}

				indice_compte = get_idCompte(sockets, clients, indice_client);
				if(indice_compte == INT_MAX){
					return EXIT_FAILURE;
				}

				somme = get_Somme(sockets, clients, indice_client, indice_compte, 1);
				if(somme == INT_MAX){
					return EXIT_FAILURE;
				}

				flagVerif = ajout(clients, indice_client, indice_compte, -somme, 1);
				if(flagVerif == -1){
					return EXIT_FAILURE;
				}
			}

			else if(strcmp(buffer, "SOLDE") == 0)
			{
				flagVerif = send_comptes(sockets[0], clients, indice_client);
				if(flagVerif == -1){
					return EXIT_FAILURE;
				}

				indice_compte = get_idCompte(sockets, clients, indice_client);
				if(indice_compte == INT_MAX){
					return EXIT_FAILURE;
				}

				flagVerif = send_solde(sockets[0], clients, indice_client, indice_compte);
				if(flagVerif == -1){
					return EXIT_FAILURE;
				}
			}

			else if(strcmp(buffer, "OPERATIONS") == 0)
			{
				flagVerif = send_comptes(sockets[0], clients, indice_client);
				if(flagVerif == -1){
					return EXIT_FAILURE;
				}

				indice_compte = get_idCompte(sockets, clients, indice_client);
				if(indice_compte == INT_MAX){
					return EXIT_FAILURE;
				}

				flagVerif = send_operations(sockets[0], clients, indice_client, indice_compte);
				if(flagVerif == -1){
					return EXIT_FAILURE;
				}
			}

			else if(strcmp(buffer, "DECONNEXION") == 0)
			{
				printf("Fermeture de la session. Initialisé par le client.\n");
				indice_client = 3; /* Client no_connected */
				flagConnection = 0;
			}

			else if(strcmp(buffer, "QUITTER") == 0)
			{
				printf("Fermeture de la connexion. Initialisé par le client.\n");
				close(sockets[0]);
				close(sockets[1]);
				printf("Close\n");
				return EXIT_SUCCESS;
			}

			else
			{
				printf("Erreur lors de la demande d'opération reçu par le serveur.");
				return EXIT_FAILURE;
			}

			/* ETAPE ENVOIE */
			/*Client client;
			flagVerif = send(sockets[0], &client, sizeof(Client), 0);
			if(flagVerif < 0)
			{
				printf("Erreur send()\n");
				return EXIT_FAILURE;
			}
			printf("Send done\n");*/
			/* ============ */
		}
	}

	/* ETAPE FERMETURE SOCKET */
	close(sockets[0]);
	close(sockets[1]);
	printf("Close\n");
	/* ============ */

	return EXIT_SUCCESS;
}
