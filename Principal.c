#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "imagem.h"


int main(int argument_count, char *argv[]){
	
	char *arg2 = argv[2];
	
	char *imagem = "Imagens/";
	char Nome[20];
	Nome[0] = '\0';	

	strcat(Nome, imagem);
	strcat(Nome, arg2);

	Imagem *img;
	Imagem *img2;
	img = lerImagem(Nome);
	img2 = lerImagem(Nome);
	if(!img){
		printf("Imagem não lida\n");

	} else{

		printf("/ Aplicando Escala de Cinza\n");
		aplicaEscalaCinza(img);
		criarPPM("Cinza.ppm", img);
		printf("/ Aplicando Filtro de Gauss\n");
		img = filtroGaussiano(img);
		criarPPM("Gauss.ppm", img);
		printf("/ Aplicando Sobel\n");
		img = filtroSobel(img);
		criarPPM("Sobel.ppm", img);
		printf("/ Aplicando Binarização\n");
		img = binarizacaoImg(img);
		criarPPM("Binarizada.ppm", img);
		printf("/ Aplicando Transformada de Hough\n");
		transformadaHough(img, img2);
		criarPPM("Pupila.ppm", img2);
		printf("/ Realizando Diagnóstico\n");
 		diagnosticoCatarata(img2, argv[6]);
		printf("Finalizado!\n");

	}

	return 0;
}
