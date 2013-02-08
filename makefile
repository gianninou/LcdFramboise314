#definition du compilateur
CC = g++

#specification des options du compilateur
CFLAGS = 

#nom de l executable
PROG = Lcd

#definition de la cible a reconstruire
all : $(PROG)

#edition de liens et production de l executable
$(PROG) : Lcd.o main.o
	$(CC) Lcd.o main.o -o $(PROG) -lwiringPi

#compilation du fichier Lcd
Lcd.o : Lcd.cpp Lcd.hpp 
	$(CC) $(CFLAGS) -c Lcd.cpp

#compilation du fichier main
main.o : main.cpp Lcd.hpp
	$(CC) $(CFLAGS) -c main.cpp

#destruction de tous les fichiers intermediaires crees
clean : 
	-rm -f *.o

#suppression de tout ce qui peut etre regenere
mrproper : clean
	-rm -f $(PROG)
