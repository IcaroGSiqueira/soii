#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#define NUM_THREADS 12

int vg;
pthread_mutex_t trava;

void *Gerador(void *threadid){
	long i, NUM_PONTOS;
	float resultado;
	long tid;
	tid = (long)threadid;

	printf("\nIniciou, thread #%ld!\n", tid);
	
	NUM_PONTOS=1000000000000/NUM_THREADS;

	for (i=0; i<NUM_PONTOS; i++){
		resultado = sin(1.77)*cos(.99);
		//printf("teste\n");
		//printf("teste2\n");
	}

/* Testa o mutex. Se liberado ativa o mutex e avanca */
	pthread_mutex_lock(&trava);

	printf("Valor vg = %d - Thread #%ld\n", vg,tid);
	vg = vg +1;

	printf("Terminou, thread #%ld!\n", tid);

/* libera o mutex */
	pthread_mutex_unlock(&trava);

	pthread_exit(NULL);
}

int main (int argc, char *argv[]){
	pthread_t threads[NUM_THREADS];
	int retorno;
	long j;
	//long ti, tempog;
	void *status;
	pthread_attr_t attr;
   	
   	time_t inicio, fim, reghora;

	//time_t reghora;
	//struct timeval inicio,fim,timediff;

	struct tm *tmptr;
	
	printf("\n\nPrograma principal iniciou\n\n");
	
	reghora = time(NULL);
	tmptr = localtime(&reghora);
	printf("Horario de Inicio: %s\n", asctime(tmptr));
	
	inicio= time(NULL);

	//gettimeofday(&inicio,0x0);

	if (pthread_mutex_init(&trava, NULL) != 0){
		printf("\n mutex init failed\n");
		return 1;
	}


/* Bloqueia o inicio das threads ateh que todas sejam criadas */
	pthread_mutex_lock(&trava);

/* cria as threads */
	vg = 0;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for(j=0; j<NUM_THREADS; j++){
		printf("In main: creating thread %ld\n", j);
		retorno = pthread_create(&threads[j], &attr, Gerador, (void *)j);
		if (retorno){
			printf("ERROR; return code from pthread_create() is %d\n", retorno);
			exit(-1);
		}
	}

/* liberas as threads para iniciarem */
	pthread_mutex_unlock(&trava);

/* Rotina para esperar que todas threads terminem */
	pthread_attr_destroy(&attr);

	for(j=0; j<NUM_THREADS; j++){
		pthread_join(threads[j], &status);
	}

	fim=time(NULL);

	//gettimeofday(&fim,0x0);
	
	reghora = time(NULL);
	tmptr = localtime(&reghora);
	printf("\nHorario do Fim: %s", asctime(tmptr));
	
	fprintf(stdout, "Tempo Gasto = %f\n\n", difftime(fim, inicio));

	//fprintf(stdout, "Tempo Gasto = %d\n\n", timeval_subtracao(&timediff,&fim,&inicio));

	pthread_exit(NULL);
	pthread_mutex_destroy(&trava);
}
