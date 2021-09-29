#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <matheval.h>

/* Size of input buffer.  */
#define BUFFER_SIZE 256

// Função para iniciar uma função
// Recebe como parametro uma string e retorna um ponteiro para função
void* inicia_funcao(char* funcao){
    void* f = evaluator_create(funcao);
    assert(f);

    return f;
}

// Função para calcular a derivada de f
// Esta recebe como parametro a função e retorna o ponteiro para derivada desta
void* calcula_derivada(void* f){
    return evaluator_derivative_x(f);
}

int criterioParada(/*x_old, x_new*/){

    return 0;
}

void leParametros(void **f, double *x, double *epsilon, int *max_iter){
    char func[BUFFER_SIZE];
    scanf("%s", func);
    *f = inicia_funcao(func);

    scanf("%lf", x);
    scanf("%lf", epsilon);
    scanf("%d", max_iter);
}

int main(int argc, char **argv)
{
    // Parametros
    void *f, *f_der;
    double x_inicial;
    double epsilon;
    int max_iter;
    
    leParametros(&f, &x_inicial, &epsilon, &max_iter);
    
    f_der = calcula_derivada(f);

    printf ("f(x) = %s\n", evaluator_get_string (f));
    printf ("f'(x) = %s\n", evaluator_get_string (f_der));
    printf("x_0 = %lf\nepsilon = %lf \nmax_iter = %d \n\n", x_inicial, epsilon, max_iter);

    // Saídas
    int iteracao = 0;
    double newton_x, newton_crit;
    double secante_x, secante_crit;

    // Temporarias
    double x_old_old = x_inicial;
    double x_old, x_new;
    do {
        // Método de Newton-Raphson
        x_new = x_old - (evaluator_evaluate_x(f, x_old) / evaluator_evaluate_x(f_der, x_old));

        // Método da secante

        // Calcula erro

        // Imprime resultado parcial

        iteracao++;
    } while(!criterioParada(/*x_old, x_new*/));

    evaluator_destroy(f);
    evaluator_destroy(f_der);

    exit(EXIT_SUCCESS);
}