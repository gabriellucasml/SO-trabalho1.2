#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>

/*a=limite inferior, b=limite superior, n = numero de trapézios, t = numero de threads*/

int n, t;
float a, b;
double h;
double* solucoes;

double f1(double x){
    return 5;
}

double f2(double x){
    return sin(2.0*x) + cos(5.0*x);
}
//N número de trapézios
//t número de threads
//N/t = número de trapézios por thread

/*tid = id da thread;*/
void* threadFunction(void* tid){
    if(n%t == 0){
        double local_a = a + (int)(size_t)tid*h*(n/t);
        double local_b = b - ((t-1)-(int)(size_t)tid)*h*(n/t);
        double area_total = (f2(local_a)+f2(local_b))/2;
        double x_i;
        for(int i=1; i<n/t; i++){
            x_i = local_a+i*h;
            area_total += f2(x_i);
        }
        area_total *= h;
        solucoes[(int)(size_t)tid] = area_total;
        printf("solução da thread #%d = %.2e\n", (int)(size_t)tid, area_total);
    }else{
        if((int)(size_t)tid == 0){
            double local_a = a + (int)(size_t)tid*h*(floor(n/t)+n%t);
            double local_b = b - ((t-1)-(int)(size_t)tid)*h*(floor(n/t)+n%t);
            double area_total = (f2(local_a)+f2(local_b))/2;
            double x_i;
            for(int i=1; i<(floor(n/t)+n%t); i++){
                x_i = local_a+i*h;
                area_total += f2(x_i);
            }
            area_total *= h;
            solucoes[(int)(size_t)tid] = area_total;
            printf("solução da thread #%d = %.2e\n", (int)(size_t)tid, area_total);
        }else{
            double local_a = a + (int)(size_t)tid*h*(floor(n/t));
            double local_b = b - ((t-1)-(int)(size_t)tid)*h*(floor(n/t));
            double area_total = (f2(local_a)+f2(local_b))/2;
            double x_i;
            for(int i=1; i<floor(n/t); i++){
                x_i = local_a+i*h;
                area_total += f2(x_i);
            }
            area_total *= h;
            solucoes[(int)(size_t)tid] = area_total;
            printf("solução da thread #%d = %.2e\n", (int)(size_t)tid, area_total);
        }
    }
}

//argv[1] = a, argv[2] = b, argv[3] = n e argv[4] = t
int main(int argc, char* argv[]){
    a = atof(argv[1]); b = atof(argv[2]); n = atoi(argv[3]); t = atoi(argv[4]);
    if(n < t){
        printf("Erro: número de threads deve ser menor que o número de trapézios\n");
        return -1;
    }
    h = (b-a)/n;
    solucoes = malloc(atoi(argv[4])*sizeof(double));
    pthread_t threads[atoi(argv[4])];
    int status, i;
    void* thread_return;

    for(i=0; i<atoi(argv[4]); i++){
        printf("processo principal criando thread #%d\n", i);
        status = pthread_create(&threads[i], NULL, threadFunction, (void*)(size_t)i);
    
        if(status!=0){
            printf("Erro na criação da thread, código de erro: %d\n", status);
            return 1;
        }
    }
    for(i = 0; i < t; i++)
        pthread_join(threads[i], NULL);
    double solucaoFinal = 0;
    for(i = 0; i< t; i++)
        solucaoFinal += solucoes[i];
    printf("Integral definida de f no intervalo de %.2e, %.2e = %.2e\n", a, b, solucaoFinal);

    return 0;
}
