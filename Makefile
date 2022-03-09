
exo2_run: jeu
	gcc jeu.c && ./a.out
exo1_run: exo.c
	gcc $< -o exo1
	valgrind ./exo1
clean:
	rm -f Makefile~ *.sh~ *.c~ *.o *.h~ a.out exo1_run a.out exo1 jeu
