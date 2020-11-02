CC ?= gcc

COBJS ?= src/client.o src/bmp.o src/couleur.o
SOBJS ?= src/serveur.o 

.SUFFIXES: .c .o

SERVER = serveur
CLIENT = client

all: $(SERVER) $(CLIENT)

$(SERVER): $(SOBJS)
	$(CC) -o $(SERVER) $(SOBJS)

$(CLIENT): $(COBJS)
	$(CC) -o $(CLIENT) $(COBJS)

.c.o: 
	$(CC) -c $*.c

