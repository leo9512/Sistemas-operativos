#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

int* crearVector(char const *FILE_NAME, int*);
void* producto_punto( void* param);
struct param{
	int *vec1;
	int *vec2;
	int n;
	int operados;
	int inicio;
	int limite;
};

int main(int argc, char const *argv[])
{	
	printf("VERSIÓN EN PARALELO\n");
	struct timeval begin, end;

	int* size = (int*)malloc(sizeof(int*));
	int total_hilos = atoi(argv[3]);
	struct param parametros[total_hilos];//arreglo de estructuras
	int * respuestas[total_hilos];
	pthread_t hilos_ids[total_hilos];
	int resultado=0;

	int* vec_1 = crearVector(argv[1],size);
	int* vec_2 = crearVector(argv[2],size);
	int division= *size/total_hilos;
	int modulo = *size % total_hilos;
	int j;

	for(j=0;j<total_hilos;j++){
		parametros[j].vec1=vec_1;
		parametros[j].vec2=vec_2;
		parametros[j].n=*size;
		parametros[j].inicio = j*(division);
		parametros[j].limite = (j*(division))+division;
		parametros[j].operados = *size - modulo;
	}

	if(total_hilos > *size){
		printf("La cantidad de hilos no puede superar el tamaño de los vectores\n");
		return 0;
	}
	gettimeofday(&begin, NULL);
	int i;
	for(i=0;i<total_hilos;i++) {
		pthread_create(&hilos_ids[i],NULL,producto_punto,&parametros[i]);
		//pthread_join(hilos_ids[i],(void*)&respuestas[i]);
		//resultado =(resultado + *respuestas[i]);
	}
	for(i=0;i<total_hilos;i++) {
		pthread_join(hilos_ids[i],(void*)&respuestas[i]);
		resultado =(resultado + *respuestas[i]);
	}

	

	gettimeofday(&end, NULL);
	printf("resultado = %d\n",resultado);
	printf("tiempo en milisegundos: %f\n", ((end.tv_sec - begin.tv_sec)*1000000.0 +(end.tv_usec- begin.tv_usec))/1000.0);
	free(size);
	free(vec_1);
	free(vec_2);
	return 0;
}

int* crearVector(char const *FILE_NAME, int * size){
	FILE *archivo;
	int num;
	archivo= fopen(FILE_NAME,"r");
	if(archivo==NULL){
		printf("No se puede abrir el archivo: %s\n", FILE_NAME);
		exit(1);
	}
	int tam=10;
	int* vector = (int*)malloc(tam*sizeof(int*));
	int i=0;
	while(fscanf(archivo,"%d",&num)>0){
		i++;
		if(i==11 || i== 1001 || i==1000001){
			switch(i){
				case 11:
					vector = (int*)realloc(vector ,pow(tam,3) * sizeof(int*));
				break;
				case 1001:
					vector = (int*)realloc(vector ,pow(tam,6) * sizeof(int*));
				break;
				case 1000001:
					vector = (int*)realloc(vector ,pow(tam,8) * sizeof(int*));
				break;
			}
		}
		vector[i-1]=num;
	}
	*size = i;
	fclose(archivo);
	return vector;
} 

void* producto_punto( void* param){

	struct param* m= (struct param*)param;
	int n_1 = m -> n;
	int inicio_1 = m ->inicio;
	int limite_1 = m -> limite;
	int operados_1= m -> operados;
	int res=0;
	int *total ;
printf("Inicio =%d--------------Limite=%d\n", inicio_1,limite_1);

	int i;
	
	for( i= inicio_1 ;i< limite_1; i++){
		res = res + (m->vec1[i] * m->vec2[i]);
	}
	if(operados_1 == limite_1){
		for (i = limite_1; i < n_1; ++i)
		{
			res = res + (m->vec1[i] * m->vec2[i]);
		}
	}
	total = malloc(sizeof(int*));
	*total= res; 
	return (void *)total;
}