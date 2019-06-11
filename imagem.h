#ifndef IMG //Verifica se esse arquivo já foi chamado

#define IMG

typedef struct{
	int r,g,b; //Variáveis inteiras para o armazenamento dos valores RGB de cada pixel

} Pixel;

typedef struct {
	int largura, altura; //Valores serão obtidos pela leitura da 2ª linha do arquivo
	Pixel **pixels; //Matriz onde serão armazenados os pixels(com seus valores de RGB)
} Imagem;

typedef struct{
	int coordenada_x, coordenada_y, raio, valor; //Coordenadas de onde o centro se encontra na imagem; raio do círculo; valor registrado na matriz tridimensional
} Centro;

//Função para realizar a leitura do arquivo: armazenará os dados fornecidos pelo arquivo em uma struct Imagem, que será dada como retorno
Imagem *lerImagem(char *nomeImagem); 
//Função para a criação de um arquivo de Imagem
void criarPPM(char *nome, Imagem *imagem);
//Função para a criação de uma struct de Imagem
Imagem *criarImagem(int largura, int altura);
//Função para aplicar escala de cinza
void aplicaEscalaCinza(Imagem *imagem);
//Função para definir um pixel
Pixel *retornoDePixel(Imagem *img, int largura, int altura);
//Função para filtro gaussiano
Imagem *filtroGaussiano(Imagem* origem);
//Função para aplicar sobel
Imagem *filtroSobel(Imagem* origem);
//Função para binarizar imagem
Imagem *binarizacaoImg(Imagem *origem);
//Função para a transformada de Hough e segmentar  a Pupila 
void transformadaHough(Imagem* binarizada, Imagem* origem);
//Função para realizar diagnóstico
void diagnosticoCatarata(Imagem *imagem, char nome[]);
#endif
