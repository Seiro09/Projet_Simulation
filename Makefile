all:main
	./main

main: main.cpp
	g++ -o main main.cpp -lm


clean:
	rm -f Station1
	rm -f Station10
	rm -f remplissageAnneau
	rm -f *.o
	rm -f *.pdf
	rm -f *.Rout
