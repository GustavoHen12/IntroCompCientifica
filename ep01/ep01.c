#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <matheval.h>
#include <math.h>

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
    int iteracao = 1;

    // Temporarias
    double newton_x, newton_crit;
    double secante_x_ant, secante_x, secante_crit;

    newton_x = x_inicial;

    newton_crit = 0;
    secante_crit = 0;

    double erro_abs, erro_relat;
    printf("iteracao, newton_x, newton_crit, secante_x, secante_crit, erro_abs, erro_relat\n");
    do {

        // Método de Newton-Raphson
        double numerador_newton = evaluator_evaluate_x(f, newton_x);
        double denominado_newton = evaluator_evaluate_x(f_der, newton_x);
        // printf("%1.16e / %1.16e \n", numerador_newton, denominado_newton);
        newton_x = newton_x - (numerador_newton / denominado_newton);

        // Método da secante
        if(iteracao == 1){
            secante_x = newton_x;
            secante_x_ant = x_inicial;
        } else {
            double numerador_secante = evaluator_evaluate_x(f, secante_x) * (secante_x - secante_x_ant);
            double denominador_secante = evaluator_evaluate_x(f, secante_x) - evaluator_evaluate_x(f, secante_x_ant);
            double secante_parcial = numerador_secante / denominador_secante ;
            secante_x_ant = secante_x;
            secante_x = secante_x - secante_parcial;
        }

        // printf("anterior: %1.16e, anterior anterior: %1.16e \n", secante_x, secante_x_ant);
        // Calcula erro
        erro_abs = newton_x - secante_x;
        erro_relat = fabs(erro_abs / newton_x);
        
        // Imprime resultado parcial
        printf("%d,%1.16e,%1.16e,%1.16e,%1.16e,%1.16e,%1.16e,\n", iteracao, newton_x, newton_crit, secante_x, secante_crit, erro_abs, erro_relat);

        iteracao++;
    } while(iteracao <= 5);

    //while(!criterioParada(/*x_old, x_new*/));

    evaluator_destroy(f);
    evaluator_destroy(f_der);

    exit(EXIT_SUCCESS);
}