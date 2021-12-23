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
#include <pthread.h>
#include <limits.h>
/* ================= */

/* ==== DEFINE ==== */
#define PORT 20011
#define ADDRESS "127.0.0.1"
/* ================ */

/* Procédure pour l'initialisation de la connexion 
	Renvoie le socket client 
		SI erreur renvoie -1 */
int init_connection()
{
	int flagVerif = -1;

	/* ETAPE SOCKET */
	int socketClient = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addrClient;
	addrClient.sin_addr.s_addr = inet_addr(ADDRESS); //IP privé 127.0.0.1
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(PORT);
	/* ============ */

	/* ETAPE CONNECTION */
	flagVerif = connect(socketClient, (const struct sockaddr *) &addrClient, sizeof(addrClient));
	if(flagVerif < 0)
	{
		printf("Erreur lors de la connection avec le serveur.\n");
		return INT_MAX;
	}
	printf("Connected\n");
	/* ============ */

	return socketClient;
}

/* Procédure de connexion d'un client 
	- 0 si OK
	- -1 sinon */
int connection_client(int socketClient)
{
	int flagVerif = -1;
	int id;
	char password[32];
	char connect[256];

	while(flagVerif == -1)
	{
		/* ETAPE RECEPTION 1 */
		flagVerif = recv(socketClient, &connect, sizeof(connect), 0);
		if(flagVerif < 0)
		{
			printf("Erreur lors de la réception du message pour l'indentifiant.\nVeuillez redémarrer l'appplication.\n");
			return -1;
		}
		printf("%s", connect);
		/* ================= */

		flagVerif = scanf("%6d", &id);
		/* On vide le tampon */
		scanf("%*[^\n]");
		getchar();
		/* On vérifie qu'un entier a bien été rentré */
		while(flagVerif != 1)
		{
			printf("	Veuillez rentrez un identifiant valide : ");
			flagVerif = scanf("%6d", &id);
			//On vide le tampon
			scanf("%*[^\n]");
			getchar();
		}
		/* ETAPE ENVOIE 1 */
		flagVerif = send(socketClient, &id, sizeof(id), 0);
		if(flagVerif < 0)
		{
			printf("Erreur lors de l'envoie de l'indentifiant au serveur.\n");
			return -1;
		}
		/* ============== */

		/* ETAPE RECEPTION 2 */
		flagVerif = recv(socketClient, &connect, sizeof(connect), 0);
		if(flagVerif < 0)
		{
			printf("Erreur lors de la réception du message pour l'indentifiant.\nVeuillez redémarrer l'appplication.\n");
			return -1;
		}
		printf("%s", connect);
		/* ================= */

		flagVerif = scanf("%9s", password);
		/* On vide le tampon */
		scanf("%*[^\n]");
		getchar();
		
		/* ETAPE ENVOIE 2 */
		flagVerif = send(socketClient, &password, sizeof(password), 0);
		if(flagVerif < 0)
		{
			printf("Erreur lors de l'envoie de l'indentifiant au serveur.\n");
			return -1;
		}
		/* ============== */

		/* ETAPE RECEPTION 3 */
		flagVerif = recv(socketClient, &connect, sizeof(connect), 0);
		if(flagVerif < 0)
		{
			printf("Erreur lors de la réception du message pour la vérification.\nVeuillez redémarrer l'appplication.\n");
			return -1;
		}

		/* ================= */
		if(strcmp(connect,"STOP") == 0)
		{
			printf("Nombre de tentatives de connexions dépassées.\n");
			close(socketClient);
			printf("Close\n");
			return -1;
		}
		else if(strcmp(connect,"OK") == 0)
		{
			printf("\nConnexion réussie.\n");
			printf(" 	Vous êtes maintenant connecté(e) sur votre compte client.\n\n");
			flagVerif = 0;
		}
		else
		{
			printf("%s\n",connect);
			flagVerif = -1;
		}
	}

	return 0;
}

/* Fonction pour la réception du message de bienvenue
	- 0 si OK
	- -1 sinon */
int bienvenue(int socketClient)
{
	int flagVerif = -1;

	/* ETAPE RECEPTION */
	char welcome[1024];
	flagVerif = recv(socketClient, &welcome, sizeof(welcome), 0);
	if(flagVerif < 0)
	{
		printf("Erreur lors de la réception du message de bienvenue.\nVeuillez redémarrer l'appplication.\n");
		return -1;
	}
	printf("%s\n", welcome);
	/* ============ */

	return 0;
}

/* Fonction permettant d'envoyer l'identifiant du compte 
	- 0 si OK
	- -1 sinon */
int send_idCompte(int socketClient)
{
	int flagVerif = -1;
	int id_compte;
	char getId[256];


	while(flagVerif == -1)
	{
		/* ETAPE RECEPTION 1 */
		flagVerif = recv(socketClient, &getId, sizeof(getId), 0);
		if(flagVerif < 0)
		{
			printf("Erreur lors de la réception du message pour l'indentifiant de compte.\nVeuillez redémarrer l'appplication.\n");
			return -1;
		}
		printf("%s", getId);
		/* ================= */

		flagVerif = scanf("%8d", &id_compte);
		/* On vide le tampon */
		scanf("%*[^\n]");
		getchar();
		/* On vérifie qu'un entier a bien été rentré */
		while(flagVerif != 1 || id_compte == 0)
		{
			printf("	Veuillez rentrez un identifiant valide : ");
			flagVerif = scanf("%8d", &id_compte);
			//On vide le tampon
			scanf("%*[^\n]");
			getchar();
		}
		/* ETAPE ENVOIE 1 */
		flagVerif = send(socketClient, &id_compte, sizeof(id_compte), 0);
		if(flagVerif < 0)
		{
			printf("Erreur lors de l'envoie de l'indentifiant de compte au serveur.\n");
			return -1;
		}
		/* ============== */

		/* ETAPE RECEPTION 2 */
		flagVerif = recv(socketClient, &getId, sizeof(getId), 0);
		if(flagVerif < 0)
		{
			printf("Erreur lors de la réception du message pour la vérification.\nVeuillez redémarrer l'appplication.\n");
			return -1;
		}
		/* ================= */

		if(strcmp(getId,"OK") == 0)
		{
			printf("\nIdentifiant du compte valide.\n");
			flagVerif = 0;
		}
		else
		{
			printf("%s\n",getId);
			flagVerif = -1;
		}
	}

	return 0;
}

/* Fonction permettant d'envoyer une somme d'argent 
	- 0 si OK
	- -1 sinon */
int send_Somme(int socketClient)
{
	int flagVerif = -1;
	float somme;
	char getSum[256];


	while(flagVerif == -1)
	{
		/* ETAPE RECEPTION 1 */
		flagVerif = recv(socketClient, &getSum, sizeof(getSum), 0);
		if(flagVerif < 0)
		{
			printf("Erreur lors de la réception du message pour la somme.\nVeuillez redémarrer l'appplication.\n");
			return -1;
		}
		printf("%s", getSum);
		/* ================= */

		flagVerif = scanf("%f", &somme);
		/* On vide le tampon */
		scanf("%*[^\n]");
		getchar();
		/* On vérifie qu'un entier a bien été rentré */
		while(flagVerif != 1 || somme == 0)
		{
			printf("	Veuillez rentrez une somme valide : ");
			flagVerif = scanf("%f", &somme);
			//On vide le tampon
			scanf("%*[^\n]");
			getchar();
		}
		/* ETAPE ENVOIE 1 */
		flagVerif = send(socketClient, &somme, sizeof(somme), 0);
		if(flagVerif < 0)
		{
			printf("Erreur lors de l'envoie de la somme au serveur.\n");
			return -1;
		}
		/* ============== */

		/* ETAPE RECEPTION 2 */
		flagVerif = recv(socketClient, &getSum, sizeof(getSum), 0);
		if(flagVerif < 0)
		{
			printf("Erreur lors de la réception du message pour la vérification.\nVeuillez redémarrer l'appplication.\n");
			return -1;
		}
		/* ================= */

		if(strcmp(getSum,"OK") == 0)
		{
			printf("\nSomme valide.\n");
			flagVerif = 0;
		}
		else
		{
			printf("%s\n",getSum);
			flagVerif = -1;
		}
	}

	return 0;
}
/* Fonction permettant de recevoir la liste des comptes possédés par le client
	- 0 si OK
	- -1 sinon */
int get_comptes(int socketClient)
{
	int flagVerif = -1;

	/* ETAPE RECEPTION */
	char comptes[1024];
	flagVerif = recv(socketClient, &comptes, sizeof(comptes), 0);
	if(flagVerif < 0)
	{
		printf("Erreur lors de la réception du solde.\nVeuillez redémarrer l'appplication.\n");
		return -1;
	}
	printf("%s\n", comptes);
	/* ============ */

	return 0;
}

/* Fonction permettant de recevoir le solde du compte demandé 
	- 0 si OK
	- -1 sinon */
int get_solde(int socketClient)
{
	int flagVerif = -1;

	/* ETAPE RECEPTION */
	char solde[128];
	flagVerif = recv(socketClient, &solde, sizeof(solde), 0);
	if(flagVerif < 0)
	{
		printf("Erreur lors de la réception du solde.\nVeuillez redémarrer l'appplication.\n");
		return -1;
	}
	printf("%s\n", solde);
	/* ============ */

	return 0;
}

/* Fonction permettant de recevoir les opérations du compte demandé 
	- 0 si OK
	- -1 sinon */
int get_operations(int socketClient)
{
	int flagVerif = -1;

	/* ETAPE RECEPTION */
	char operations[2048];
	flagVerif = recv(socketClient, &operations, sizeof(operations), 0);
	if(flagVerif < 0)
	{
		printf("Erreur lors de la réception des operations.\nVeuillez redémarrer l'appplication.\n");
		return -1;
	}
	printf("%s\n", operations);
	/* ============ */

	return 0;
}

/* vérifie que la chaîne de caractère est bien une opération valide
	(AJOUT ou RETRAIT ou SOLDE ou OPERATIONS)
		- 0 si OK
		- -1 sinon */
int verifOP(char* op)
{
	if(strcmp(op, "AJOUT") == 0 || strcmp(op, "RETRAIT") == 0 || 
		strcmp(op, "SOLDE") == 0 || strcmp(op, "OPERATIONS") == 0
		|| strcmp(op, "QUITTER") == 0 || strcmp(op, "DECONNEXION") == 0)
		return 0;

	printf("/!\\ Vous devez saisir une opération valide.");
	printf(" (AJOUT ou RETRAIT ou SOLDE ou OPERATIONS ou QUITTER)\n\n");
	return -1;
}


int main(void)
{
	int flagVerif = -1;
	/* Permet de savoir s'il faut faire l'étape de connection du client */
	int flagConnection = 0;
	int flagConnection_done = -1;

	int socketClient = init_connection();
	if(socketClient == INT_MAX){
		return EXIT_FAILURE;
	}


	int flag_verifOP = -1;
	char buffer[64];

	while(1){
		if(flagConnection == 0)
		{
			flagVerif = connection_client(socketClient);
			if(flagVerif == -1){
				return EXIT_FAILURE;
			}

			flagVerif = bienvenue(socketClient);
			if(flagVerif == -1){
				return EXIT_FAILURE;
			}

			flagConnection = -1;
			flagConnection_done = 0;
		}
		else{
			while(flag_verifOP){
				if(flagConnection_done == -1){
					printf("\nVous pouvez effectuer diverses opérations sur vos comptes : \n");
					printf("	AJOUT / RETRAIT / SOLDE / OPERATIONS\n");
					printf("	DECONNEXION / QUITTER\n\n");
				}
				printf("Veuillez rentrer l'opération voulue : ");
				scanf("%64s", buffer);
				flag_verifOP = verifOP(buffer);
			}
			flagConnection_done = -1;
			flag_verifOP = -1;


			/* ETAPE ENVOIE */
			flagVerif = send(socketClient, &buffer, sizeof(buffer), 0);
			if(flagVerif < 0)
			{
				printf("Erreur lors de l'envoie du message au serveur.\n");
				return EXIT_FAILURE;
			}
			/* ============ */

			/* REQUETE AJOUT */
			if(strcmp(buffer, "AJOUT") == 0)
			{
				flagVerif = get_comptes(socketClient);
				if(flagVerif == -1){
					return EXIT_FAILURE;
				}

				flagVerif = send_idCompte(socketClient);
				if(flagVerif == -1){
					return EXIT_FAILURE;
				}

				flagVerif = send_Somme(socketClient);
				if(flagVerif == -1){
					return EXIT_FAILURE;
				}
			}
			/* ============= */

			/* REQUETE AJOUT */
			else if(strcmp(buffer, "RETRAIT") == 0)
			{
				flagVerif = get_comptes(socketClient);
				if(flagVerif == -1){
					return EXIT_FAILURE;
				}

				flagVerif = send_idCompte(socketClient);
				if(flagVerif == -1){
					return EXIT_FAILURE;
				}

				flagVerif = send_Somme(socketClient);
				if(flagVerif == -1){
					return EXIT_FAILURE;
				}
			}
			/* ============= */

			/* REQUETE SOLDE */
			else if(strcmp(buffer, "SOLDE") == 0)
			{
				flagVerif = get_comptes(socketClient);
				if(flagVerif == -1){
					return EXIT_FAILURE;
				}

				flagVerif = send_idCompte(socketClient);
				if(flagVerif == -1){
					return EXIT_FAILURE;
				}

				flagVerif = get_operations(socketClient);
				if(flagVerif == -1){
					return EXIT_FAILURE;
				}
			}
			/* =============== */

			/* REQUETE SOLDE */
			else if(strcmp(buffer, "OPERATIONS") == 0)
			{
				flagVerif = get_comptes(socketClient);
				if(flagVerif == -1){
					return EXIT_FAILURE;
				}

				flagVerif = send_idCompte(socketClient);
				if(flagVerif == -1){
					return EXIT_FAILURE;
				}

				flagVerif = get_operations(socketClient);
				if(flagVerif == -1){
					return EXIT_FAILURE;
				}
			}
			/* =============== */

			/* REQUETE DECONNEXION */
			else if(strcmp(buffer, "DECONNEXION") == 0)
			{
				printf("\nVous vous êtes bien déconnectés de votre compte client !\n");
				printf("Vous pouvez dès à présent vous connecter à un autre compte client.\n\n");
				flagConnection = 0;
			}
			/* =============== */

			/* REQUETE QUITTER */
			else if(strcmp(buffer, "QUITTER") == 0)
			{
				printf("\nMerci de votre visite sur le site de votre banque !\nÀ bientôt !\n");
				close(socketClient);
				printf("Close\n");
				return EXIT_SUCCESS;
			}
			/* =============== */
		}

	}

	/* ETAPE FERMETURE SOCKET */
	close(socketClient);
	printf("Close\n");
	/* ============ */

	return EXIT_SUCCESS;
}
