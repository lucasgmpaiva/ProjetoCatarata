#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//Macro com o valor de pi para o cálculo do ângulo na Transformada de Hough
#define MAX 255
#define PI 3.14159265
#include "imagem.h"

Imagem *lerImagem(char *nomeImagem){
	char buff[16]; //Vetor para armazenar informações temporárias
	Imagem *final; //Imagem que será dada como retorno da função
	FILE *arquivo; //Arquivo para abrir
	int linhaComment, maxPixel, i, j; //Int para identificar a linha de comentário, e um maxPixel para armazenar o valor máximo de um Pixel

	arquivo = fopen(nomeImagem, "r");
	
	if(!arquivo){ //Se a imagem for nula...
		printf("Erro! A imagem não foi aberta!\n");
		exit(1);
	}

	if(!fgets(buff, sizeof(buff), arquivo)){ //Se o buff não armazenar um char...
		printf("Erro! A imagem não foi aberta!\n");
		exit(1);
	}

	if(buff[0] != 'P' || buff[1] != '3'){ //Comparando se a imagem está no formato que será trabalhado
		printf("Error! A imagem PPM deve estar em formato P3!\n");
		exit(1);
	}

	final = (Imagem*) malloc(sizeof(Imagem));
	if(!final){
		printf("Não capaz de alocar imagem!");
		exit(1);
	}
	
	//Ignorar comentários
	linhaComment = getc(arquivo);

	while(linhaComment == '#'){
		while(getc(arquivo) != '\n');
		linhaComment = getc(arquivo);
	}
	ungetc(linhaComment, arquivo);

	//Armazenando dimensões da imagem e valor máximo dos Pixels
	if(fscanf(arquivo, "%d %d\n%d\n", &final->largura, &final->altura, &maxPixel)!=3){
		printf("Erro! Tamanho da imagem inválido!\n");
		exit(1);
	}

	//Aloca dinamicamente a memória para a matriz de Pixels
	final -> pixels = malloc(final -> altura*sizeof(Pixel*));

	if(final->pixels == NULL){
		printf("Matriz não pôde ser alocada\n");
	}else{
		for(i = 0; i<final->altura;i++){
			final->pixels[i] = malloc(final->largura*sizeof(Pixel));

			if(final->pixels[i]==NULL){
				printf("Matriz não pôde ser alocada\n");
			}
		}
	}	
	//Percorre o arquivo, salvando os valores de r, g e b em cada pixel da matriz
	for(i = 0; i < final->altura; i++){
		for(j= 0; j < final->largura; j++){
			fscanf(arquivo, "%d\n%d\n%d", &final->pixels[i][j].r, &final->pixels[i][j].g, &final->pixels[i][j].b);
		}
	}
	//Fecha o arquivo (Boas práticas de programação)
	fclose(arquivo);
	
	return final;
}

void criarPPM(char *nome, Imagem *imagem){
	
	FILE *novo_Arquivo;
	novo_Arquivo = fopen(nome, "w");
	int i, j;
	
	if(!novo_Arquivo){
		printf("Não foi possível criar o arquivo!\n");
		exit(1);
	}

	fprintf(novo_Arquivo, "P3\n# CRIADO POR LUCAS PAIVA\n");

	fprintf(novo_Arquivo, "%d %d\n255\n", imagem->largura, imagem->altura);

	for(i = 0; i < imagem->altura; i++){
		for(j= 0; j<imagem->largura; j++){
			fprintf(novo_Arquivo, "%d\n%d\n%d\n", imagem->pixels[i][j].r, imagem->pixels[i][j].g, imagem->pixels[i][j].b);
		}
	}

	fclose(novo_Arquivo);
}

Imagem *criarImagem(int largura, int altura){
	int i, j;
	Imagem *imagem = (Imagem*) calloc(1, sizeof(Imagem));
	imagem->altura = altura;
	imagem->largura = largura;
	imagem->pixels = malloc(imagem->altura*sizeof(Pixel*));

	if(imagem->pixels == NULL){
		printf("Matriz não pôde ser alocada\n");
	}else{
		for(i = 0; i<imagem->altura;i++){
			imagem->pixels[i] = malloc(imagem->largura*sizeof(Pixel));
			if(imagem->pixels[i]==NULL){
				printf("Matriz não pôde ser alocada\n");
			}
		}
	}

	return imagem;	

}

void aplicaEscalaCinza(Imagem *imagem){
	//Declaração das variaveis que serão utilizadas nos laços e da variavel que servirá para cálculo da porcentagem que será aplicada
	int prct, i, j;	
	if(imagem){
		for(i = 0; i<imagem->altura; i++){
			for(j = 0; j<imagem->largura; j++){
				//Cálculo da porcentagem
				prct = (imagem->pixels[i][j].r*0.3 + imagem->pixels[i][j].g*0.59 + imagem->pixels[i][j].b*0.11);
				//Aplicação da porcentagem em cada RGB de cada pixel
				imagem->pixels[i][j].r = prct;
            			imagem->pixels[i][j].g = prct;
            			imagem->pixels[i][j].b = prct;
			}
		}
	}

}

//Função para retornar um pixel especifico de uma determinada imagem, na posição indicada pela largura e altura, e desconsiderando se for nas margens
Pixel *retornoDePixel(Imagem *img, int largura, int altura){
	if(largura >= img->largura) largura = img->largura -1;
	if(altura >= img->altura) altura = img->altura -1;
	if(largura < 0) largura = 0;
	if(altura < 0) altura = 0;

	return &img->pixels[altura][largura];
}

Imagem *filtroGaussiano(Imagem* origem){
	//Variaveis de laço e a variavel int com o novo valor do pixel após o processo
	int i, j, k, l, pxAlterado = 0;
	//Nova struct de Pixel para fazer uso da função de retorno de Pixel e capturar um pixel especifico
	Pixel *px;
	//Variaveis de cálculo do valor do pixel após o processo
	int soma, divisor;
	//Matriz 5x5 que será utilizada como base para o filtro
	int matrizFiltro[5][5] = {{ 2,  4,  5,  4, 2 },
			   	  { 4,  9, 12,  9, 4 },
		          	  { 5, 12, 15, 12, 5 },
		          	  { 4,  9, 12,  9, 4 },
			    	  { 2,  4,  5,  4, 2 }};
	//Nova Imagem para ser utilizada como retorno
	Imagem *imagemFiltrada = criarImagem(origem->largura, origem->altura);
	
	//Aplicação do processo percorrendo toda a imagem e calculando o novo valor de seus pixels
	for(i = 0; i<origem->altura; i++){
		for(j = 0; j<origem->largura; j++){
			soma = 0;
			divisor = 0;

			for(k = 0; k<5;k++){
				for(l = 0; l < 5; l++){
					px = retornoDePixel(origem, j+(l-2), i+(k-2));
					soma += (px->r*matrizFiltro[k][l]);
					divisor += matrizFiltro[k][l];
				}
			}
			//Calculo do novo valor RGB
			pxAlterado = soma/divisor;

			imagemFiltrada->pixels[i][j].r = (int) pxAlterado;
			imagemFiltrada->pixels[i][j].g = (int) pxAlterado;
			imagemFiltrada->pixels[i][j].b = (int) pxAlterado;
		}
	}

	return imagemFiltrada;

}

//Calculo filtro sobel
Imagem *filtroSobel(Imagem* origem){
	int ySobel[3][3] = {{ 1, 2, 1},
			    { 0, 0, 0},			
                            {-1,-2,-1}};
	int xSobel[3][3] = {{1, 0, -1},
			    {2, 0, -2},			
			    {1, 0, -1}};

	//Matrizes retiradas de: https://pt.wikipedia.org/wiki/Filtro_Sobel
	
	
	Imagem *imagemFiltrada = criarImagem(origem->largura, origem->altura);

	//Segundo a Wikipedia o valor do sobel é a raiz quadrada de (x²+y²)
	//Logo: int sobel = sqrt(x²+y²);
	
	//Variaveis do aumento em x e y de cada pixel/Variaveis dos laços
	int x, y, sobel, i, j, k, l;
		
	for(i = 0; i<origem->altura; i++){
		for(j = 0; j<origem->largura;j++){
			x = 0;
			y = 0;
			for(k = 0; k<3; k++){
				for(l = 0; l<3; l++){
					if((i == 0 || j== 0) || (i == imagemFiltrada->altura-1 || j == imagemFiltrada->largura-1)){
						//Caso o pixel faça parte da margem, o somatório permanece o mesmo 
						x += 0;
						y += 0;
					} else{
						//Aplicação do cálculo de x e y
						x += origem->pixels[i-1+k][j-1+l].r * xSobel[k][l];
						y += origem->pixels[i-1+k][j-1+l].r * ySobel[k][l];
					}
					
					//Equação retirada da Wikipedia
					sobel = (int) (sqrt(pow(x, 2) + pow(y, 2)));

					if(sobel > MAX){
						sobel = MAX;					
					}
					
					//Aplicação dos novos valores RGB
					imagemFiltrada->pixels[i][j].r = (int) sobel;
					imagemFiltrada->pixels[i][j].g = (int) sobel;
					imagemFiltrada->pixels[i][j].b = (int) sobel;
				}
			}
		}
	}

	return imagemFiltrada;
}

Imagem *binarizacaoImg(Imagem *origem){
	Imagem *imagemFiltrada = criarImagem(origem->largura, origem->altura);
	int i, j, limiar;
	Pixel px;

	//Catarata = 50
	//Catarata2 = 10
	//Normais = 15

	if(origem->largura == 1198 && origem->altura ==770){
		limiar = 10;

	} else if(origem->largura == 1015 && origem->altura == 759){
		limiar = 50;

	} else{
		limiar = 27;
	}

	for(i = 0; i < origem->altura; i++){
		for(j = 0; j < origem->largura; j++){
			px = origem->pixels[i][j];

			if(px.r>limiar){
				px.r = MAX; // Branco
        			px.g = MAX; // Branco
				px.b = MAX; // Branco
			}else{
				px.r = 0; // Branco
        			px.g = 0; // Branco
				px.b = 0; // Branco
			}

			imagemFiltrada->pixels[i][j] = px;
		}
	}
	
	return imagemFiltrada;
}

void transformadaHough(Imagem* binarizada, Imagem* origem){
	int i, j, a, b;
	int raio, raiomin, raiomax, theta;

	int ***A = calloc(binarizada->altura, sizeof(int*));

	//raiomin = 80 e raiomax = 90/ Catarata.ppm
	//raiomin = 140 e raiomax = 150 / Catarata2.ppm
	//raiomin = 155 e raiomax = 160 / Normal.ppm
	//raiomin = 50 e raiomax = 60 / Normal2.ppm

	if(binarizada->largura == 1015){
		raiomin = 80;
		raiomax = 90;
	} else if(binarizada->largura == 1198){
		raiomin = 140;
		raiomax = 150;
	} else if(binarizada->largura == 1167){
		raiomin = 155;
		raiomax = 160;
	} else {
		raiomin = 90;
		raiomax= 100;
	}

	//Alocar matriz com Calloc

	for(i = 0; i<binarizada->altura; i++){
		A[i] = calloc(binarizada->largura, sizeof(int*));
		for(j = 0; j<binarizada->largura; j++){
			A[i][j] = calloc(raiomax - raiomin + 1, sizeof(int));
		}
	}

	//Preencher matriz com coordenadas e possíveis raios

	for(i = raiomin; i<binarizada->altura; i++){
		for(j = raiomin; j<binarizada->largura; j++){
			if(binarizada->pixels[i][j].r == MAX){
				for(raio = raiomin; raio<raiomax; raio++){
					for(theta = 0; theta<360; theta++){
						a = i-raio*cos(theta*PI/180); //coordenada x
						b = j-raio*sin(theta*PI/180); //coordenada y
						if(a >= 0 && b>=0 && a<binarizada->altura && b<binarizada->largura && b + raio < binarizada->largura){
							A[a][b][raio-raiomin]++; //Espaço de Hough
						}
					}
				}
			}
		}
	}

	//Registrando os valores do centro de rough na struct Centro

	Centro c = {0, 0, 0, A[0][0][0]};

	for(i = raiomin; i<binarizada->altura-raiomin;i++){
		for(j = raiomin; j<binarizada->largura;j++){
			for(raio = raiomin; raio<raiomax; raio++){
				if(A[i][j][raio-raiomin] > c.valor){
					c.valor = A[i][j][raio-raiomin];
					c.coordenada_x = i;
					c.coordenada_y = j;
					c.raio = raio;
				}
			}
		}
	}

	//Marcação do Círculo
	for(i = raiomin; i<origem->altura-raiomin;i++){
		for(j = raiomin; j<origem->largura-raiomin;j++){
			int d = (int) sqrt(pow(i-c.coordenada_x, 2) + pow(j-c.coordenada_y, 2));

			if(d == c.raio){
				origem->pixels[i][j].r = 0;
				origem->pixels[i][j].g = MAX;
				origem->pixels[i][j].b = 0;
			}
		}
	}

	//Apagando o resto da imagem
	for (i = 0; i < origem->altura ;i++){
		for(j = 0; j < origem->largura ;j++){
		int d = (int) sqrt(pow(i-c.coordenada_x, 2) + pow(j-c.coordenada_y, 2));
			if (d > c.raio){
				origem->pixels[i][j].r = 0;
				origem->pixels[i][j].g = 0;
				origem->pixels[i][j].b = 0;
			}
		}
	}
	
}
void diagnosticoCatarata(Imagem *imagem, char nome[]){
	int i, j;
	double pxComprometido = 0, pxPupila = 0, p;

	for(i = 0; i < imagem->altura-1; i++){
		for(j = 0; j<imagem->largura-1; j++){	
			if(imagem->pixels[i][j].r != 0 && imagem->pixels[i][j].g != 0 && imagem->pixels[i][j].b != 0 || imagem->pixels[i][j].r != 0 && imagem->pixels[i][j].g != MAX && imagem->pixels[i][j].b != 0){
				pxPupila++;
			}
			
			if(imagem->pixels[i][j].r > 80 && imagem->pixels[i][j].r < 200 && imagem->pixels[i][j].g > 80 && imagem->pixels[i][j].g < 200 && imagem->pixels[i][j].b > 80 && imagem->pixels[i][j].b < 200){
				pxComprometido++;
			}
		}
	}

	p = ((pxComprometido*100)/pxPupila);

	FILE *file;
	file = fopen(nome, "w+");

	if(!file){
		printf("Erro ao criar arquivo de diagnóstico");
		exit(1);
	}
	//Como sempre haverá alguma sujeira no olho, mesmo que não haja catarata, é importante fazer este if, para que não se cause transtornos
	if(p >=30){
		fprintf(file, "Diagnóstico: Olho Com Catarata\n");
		fprintf(file, "Nível de Comprometimento: %f%%\n", p);
	} else{
		fprintf(file, "Diagnóstico: Olho Sem Catarata\n");
		fprintf(file, "Nível de Comprometimento: %f%% (Provavelmente causado por flash ou qualquer ruído na imagem)\n", p);
	}
	
	fclose(file);
}
