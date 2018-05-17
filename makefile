# CONSEGNA:
# all​ : compila tutti i target
# clean​ : pulisce i file intermedi e l’eseguibile
# doc​ : genera la documentazione
# help:​ stampare l’elenco dei target possibili
# install​ : copiare l’eseguibile nella cartella ​ bin
# threads: ​ compilare la versione con le threads

# Versione dubbia, non capisco cosa voglia. non 'e chiaro cosa si aspetti da all ed install
# (fanno la stessa cosa?)

# E' necessario che i sorgente (file.c) siano nella sottocartella src e i file header nella sottocartella include
# Il make deve essere nella cartella principale

CC=gcc

OBJS = main.o padre.o

EX = key_finder

all: 
	
$(EX): $(OBJS)
	@$(CC) -o $@ $^
	@mkdir -p build
	@mv ./*.o ./build/

%.o: ./src/%.c
	@$(CC) -c -o $@ $<
		
clean:
	@echo "Via tutto"
	@-rm -f ./build/*.o
	@-rm -f ./bin/$(EX)
	
doc:

help:

install: $(EX)
	@mkdir -p bin
	@mv ./key_finder ./bin/

threads:
