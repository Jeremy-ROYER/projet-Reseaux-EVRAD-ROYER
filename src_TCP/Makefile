# options de compilation
CC = gcc
CCFLAGS = -Wall
LIBS = 
LIBSDIR =
EXEC = client serveur

# règle initiale
all: $(EXEC)

# règles de compilation
%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $<

# règles d'édition de liens
client: client.o
	$(CC) -o $@ $^ $(LIBS) $(LIBSDIR)

serveur: serveur.o
	$(CC) -o $@ $^ $(LIBS) $(LIBSDIR)

# règles supplémentaires
clean:
	rm -f *.o ~* $(EXEC)
