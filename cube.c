#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

//Variáveis globais para armazenar os ângulos de rotação.
float a, b, c;


//Função para calcular a coordenada X de acordo com os ângulos de rotação.
float calc_x(int i, int j, int k){
    return (j*sin(a)*sin(b)*cos(c)-
    k*cos(a)*sin(b)*cos(c)+
    j*cos(a)*sin(c)+
    k*sin(a)*sin(c)+
    i*cos(b)*cos(c));
}

//Função para calcular a coordenada Y de acordo com os ângulos de rotação.
float calc_y(int i, int j, int k){
    return (j*cos(a)*cos(c)+
    k*sin(a)*cos(c)-
    j*sin(a)*sin(b)*sin(c)+
    k*cos(a)*sin(b)*sin(c)-
    i*cos(b)*sin(c));
}

//Função para calcular a coordenada Z de acordo com os ângulos de rotação.
float calc_z(int i, int j, int k){
    return (k*cos(a)*cos(b)-
    j*sin(a)*cos(b)+
    i*sin(b));
}

//Função que calcula a projeção dos pontos na tela.
void calculator(float Cx, float Cy, float Cz, int ch, int dist, int largura, int K, int altura, float* zBuffer, char* buffer){
    float x, y, z, ooz, xp, yp;
    int idx;

    //Calcula a projeção dos pontos chamando as funções que fazem esses calculos.
    x=calc_x(Cx, Cy, Cz);
    y=calc_y(Cx, Cy, Cz);
    z=calc_z(Cx, Cy, Cz)+dist; //Ajusta a profundidade do objeto.

    //Calcula o inverso do z para controlar a profundidade do ponto (utilizado para saber se o ponto deve aparecer na tela ou não).
    ooz=1/z;

    //Cálcula a projeção das coordenadas 3D na tela 2D do monitor.
    xp=(int)(largura/2+K*ooz*x*2);
    yp=(int)(altura/2+K*ooz*y);

    //Calcula o indice que o ponto deve estar no buffer de acordo com a projeção na tela.
    idx=xp+yp*largura;

    //Verifica se o ponto esta dentro dos limites da tela. Verifica tbm se o ponto deve ser desenhado.
    if(idx>=0 && idx<largura*altura){
        if(ooz>zBuffer[idx]){ //Verifica se profundidade do ponto é maior que a guardada no vetor auxiliar 'zbuffer' para ver se ele deve ser visivel ou não. 
            zBuffer[idx]=ooz;
            buffer[idx]=ch;
        }
    }
}

int main(void){
    // Variáveis que definem os parâmetros do cubo e da tela.
    float tam_cubo=20, speed=0.6, K=40;
    int largura=160, altura=44, fundo='.', dist=100;
    float* zBuffer=(float*)malloc(sizeof(float)*(160*44)); //Buffer auxiliar para guardar a profundidade dos pontos.
    char* buffer=(char*)malloc(sizeof(char)*(160*44)); //Buffer para armazenar o cubo. 

    
    printf("\x1b[2J"); //Limpa o prompt

    while(1){ //Loop infinito para renderizar o cubo
        memset(buffer, fundo, altura*largura); //Preenche todo o buffer principal com '.' que será o fundo. 
        memset(zBuffer, 0, altura*largura*4); //Inicializa o zBuffer.

        //Loop que chama a função 'calculator()' para calcular os pontos de cada um dos pontos de cada face e guardar no 'buffer' os pontos que devem aparecer na tela.
        for(float Cx=-tam_cubo; Cx<tam_cubo; Cx += speed){
            for(float Cy=-tam_cubo; Cy<tam_cubo; Cy += speed){
                calculator(Cx, Cy, -tam_cubo, '@', dist, largura, K, altura, zBuffer, buffer);
                calculator(tam_cubo, Cy, Cx, '$', dist, largura, K, altura, zBuffer, buffer);
                calculator(-tam_cubo, Cy, -Cx, '~', dist, largura, K, altura, zBuffer, buffer);
                calculator(-Cx, Cy, tam_cubo, '#', dist, largura, K, altura, zBuffer, buffer);
                calculator(Cx, -tam_cubo, -Cy, ';', dist, largura, K, altura, zBuffer, buffer);
                calculator(Cx, tam_cubo, Cy, '+', dist, largura, K, altura, zBuffer, buffer);
            }
        }
        //Move o cursor para o topo da tela.
        printf("\x1b[H");

        //Imprime o buffer na tela
        for(int i=0; i<altura*largura; i++){
            if(i%largura==0){
                printf("\n");//Muda para a proxima linha.
            }else
                printf("%c", buffer[i]);
        }

        //Atualiza os ângulos de rotação.
        a+=0.05;
        b+=0.05;
        c+=0.01;

        //Faz uma pequena pausa para deixar a animação melhor.
        usleep(8000*2);
    }

return 0;
}