#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#define NUM_THREADS 6

int timeval_subtracao (struct timeval *result, struct timeval *x,struct timeval  *y);

int vg;
pthread_mutex_t trava;

void *Gerador(void *threadid){
	long i, NUM_PONTOS;
	float resultado;
	long tid;
	tid = (long)threadid;

	printf("\nIniciou, thread #%ld!\n", tid);
	
	NUM_PONTOS = 100000000000/NUM_THREADS;

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
	void *status;
	pthread_attr_t attr;
   	
   	//time_t inicio, fim, reghora;

	time_t reghora;
	struct timeval inicio,fim,timedif;

	struct tm *tmptr;
	
	printf("\n\nPrograma principal iniciou\n\n");
	
	reghora = time(NULL);
	tmptr = localtime(&reghora);
	printf("Horario de Inicio: %s\n", asctime(tmptr));
	
	//inicio= time(NULL);

	gettimeofday(&inicio,0x0);

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
		printf("Main: criando thread %ld\n", j);
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

	//fim=time(NULL);

	gettimeofday(&fim,0x0);
	
	reghora = time(NULL);
	tmptr = localtime(&reghora);
	printf("\nHorario do Fim: %s", asctime(tmptr));
	
	//fprintf(stdout, "Tempo Gasto = %f\n\n", difftime(fim, inicio));

	timeval_subtracao(&timedif,&fim,&inicio);

	printf("Tempo Gasto = %ld,%ld\n", timedif.tv_sec, timedif.tv_usec);

	pthread_exit(NULL);
	pthread_mutex_destroy(&trava);
}


int timeval_subtracao (struct timeval *result, struct timeval *x,struct timeval  *y)  
{  
  /* Perform the carry for the later subtraction by updating y. */  
  if (x->tv_usec < y->tv_usec) {  
    int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;  
    y->tv_usec -= 1000000 * nsec;  
    y->tv_sec += nsec;  
  }  
  if (x->tv_usec - y->tv_usec > 1000000) {  
    int nsec = (y->tv_usec - x->tv_usec) / 1000000;  
    y->tv_usec += 1000000 * nsec;  
    y->tv_sec -= nsec;  
  }  
  
  /* Compute the time remaining to wait.
     tv_usec is certainly positive. */  
  result->tv_sec = x->tv_sec - y->tv_sec;  
  result->tv_usec = x->tv_usec - y->tv_usec;  
  
  /* Return 1 if result is negative. */  
  return x->tv_sec < y->tv_sec;  
}