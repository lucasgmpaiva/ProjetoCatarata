# PROJETO ITP 2017.2
## Programa Para Diagnóstico de Catarata

#### Feito por: Lucas Gabriel Matias Paiva


### Funcionalidades:
	1. Leitura de uma imagem em formato PPM com método de desconsiderar comentários;
	2. Capacidade da criação/escrita de uma imagem/arquivo PPM;
	3. Filtro para aplicar na imagem tons de cinza;
	4. Função para aplicar o filtro gaussiano (Aplica blur na imagem);
	5. Função sobel para aplicar realce de arestas;
	6. Função para binarizar a imagem com base em um limiar;
	7. Função para aplicar a Transformada de Hough, segmentar pupila e apagar o resto da imagem;
	8. Função diagnóstico para criar arquivo txt contendo o diagnóstico: se a doença existe no olho estudado ou não, e a porcentagem;
	9. Função "retornoDePixel" com a finalidade de capturar um pixel específico da imagem (utilizado para o filtro gaussiano);
	10. Função para criar uma struct do tipo imagem, passando como parâmetro a altura e a largura desejada.

### Estruturas:
	1. Pixel: Estrutura para armazenar dados sobre pixels, contém valores RBG separados em variáveis do tipo inteiro;
	2. Imagem: Estrutura para armazenar dados de imagem (Largura, Altura, Matriz de Pixels);
	3. Centro: Estrutura para armazenar dados sobre o centro da circunferência (Coordenada X, Y e Raio).

### O que seria feito diferente:
	1. Função para calcular o limiar para a binarização da imagem, ao perceber que cada imagem teria um limiar diferente para binarização, foi notado que isso seria possível;
	2. Otimizar Transformada de Hough, diminuindo seu tempo de execução e aperfeiçoando seus resultados;
	3. Função para listar todos os círculos encontrados por Hough e não apenas apresentar um.
	
### Para compilar:
	No terminal, no diretório no qual este .txt se encontra;
	Digite 'make' no terminal;
	Execute no formato: ./Catarata -i <input-imagem-com-formato> -o <input-formato-da-imagem> -f <nome-do-arquivo-diagnóstico.ppm>.

### Para limpar executáveis:
	No terminal, no mesmo diretório de compilação digitar 'make clean'
	

