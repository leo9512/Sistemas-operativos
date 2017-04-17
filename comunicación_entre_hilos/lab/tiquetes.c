#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#define NUMTICKETS 40
#define NUMVENDEDORES 4

double counter =0; //contador para verificar que no se vendan mas de las boletas disponibles, esta compartida por los hilos
sem_t sem; //semaforo
struct param{
	int id;
	int ventas;
};

void * venta (void *);

int main (){
	pthread_t tid[NUMVENDEDORES];
	sem_init(&sem,0,1);//inicialización del semaforo con un contador que empieza desde 1.
	int i;
	struct param param[NUMVENDEDORES];
	int *ventas[NUMVENDEDORES];
	int total=0;

	for(i=0;i<NUMVENDEDORES;i++){
		param[i].id=i;
		pthread_create(&tid[i],NULL,venta,&param[i]);
	}

	for(i=0;i<NUMVENDEDORES;i++){
		pthread_join(tid[i],(void*)&ventas[i]);
		total = total + *ventas[i]; // se suma la cantidad de boletas vendidas por cada vendedor
	}

	printf("Numero de Tiquetes: %d\n",NUMTICKETS);
	printf("Número de tiquetes vendidos: %d \n", total);
	

	return 0;
}

void * venta(void * param){
	int res=0;
	int *total;
	struct param* m= (struct param*)param;
	while(1){
		sem_wait(&sem);	
		if(counter <NUMTICKETS){
			counter ++;
			res++;
			sem_post(&sem); //Se incrementa el contador del semaforo
			printf("Vendedor: %d  esta vendiendo 1 ...\n",(m->id)+1);
			sleep(1);
		}
		else{
			sem_post(&sem);//Se incrementa el contador del semaforo
			break;
		}
	}
	total = malloc(sizeof(int*));
	*total=res; // contiene el número de boletas que vendio cada vendedor
	return (void*)total;
}