CC ?= gcc 
CFLAGS = -Wall -Wextra -g -lm

COBJS ?= src/client.o src/bmp.o src/couleur.o src/shared.o
SOBJS ?= src/serveur.o src/shared.o

.SUFFIXES: .c .o

SERVER = serveur
CLIENT = client

all: $(SERVER) $(CLIENT)

$(SERVER): $(SOBJS)
	$(CC) -o $(SERVER) $(SOBJS) -lm

$(CLIENT): $(COBJS)
	$(CC) -o $(CLIENT) $(COBJS) -lm

.c.o: 
	$(CC) -o $*.o -c $*.c $(CFLAGS)

