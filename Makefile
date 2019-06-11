all: Principal

Principal:	Principal.o	imagem.o
	gcc Principal.o imagem.o -o Catarata -lm

Principal.o:	Principal.c
	gcc -c Principal.c

imagem.o:	imagem.c imagem.h
	gcc -c imagem.c

clean: 
	rm Principal.o imagem.o Catarata

