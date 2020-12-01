CC ?= gcc
CFLAGS = -Wall -Wextra -g

COBJS ?= src/client.o src/bmp.o src/couleur.o src/shared.o src/validateur.o
SOBJS ?= src/serveur.o src/shared.o src/validateur.o

.SUFFIXES: .c .o

SERVER = serveur
CLIENT = client

all: $(SERVER) $(CLIENT)

$(SERVER): $(SOBJS)
	$(CC) -o $(SERVER) $(SOBJS)

$(CLIENT): $(COBJS)
	$(CC) -o $(CLIENT) $(COBJS)

.c.o: 
	$(CC) -o $*.o -c $*.c $(CFLAGS)

