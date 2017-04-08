#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int* leerVector(char const *FILE_NAME,int tam);
int productoPunto(int* vec1, int* vec2,int n);

int main(int argc, char const *argv[])
{	
	int n = atoi(argv[3]);
	int* vec1 = leerVector(argv[1],n);
	int* vec2 = leerVector(argv[2],n);
	struct timeval begin, end; //para calcular los tiempos
	//---------------------------------------------Desde acá se analiza el total del tiempo de ejecución para todos los hilos------------------------------------
	gettimeofday(&begin, NULL);
	int resultado = productoPunto(vec1,vec2,n);
	gettimeofday(&end, NULL);
	//--------------------------------------------Acá finaliza el tiempo a analizar---------------------------------------------------------------------------------
	printf("resultado = %d\n",resultado);
	printf("tiempo en milisegundos: %f\n", ((end.tv_sec - begin.tv_sec)*1000000.0 +(end.tv_usec- begin.tv_usec))/1000.0); //se muestra el tiempo en milisegundos
	printf("tiempo en segundos: %f\n", ((end.tv_sec - begin.tv_sec)*1000000.0 +(end.tv_usec- begin.tv_usec))/1000000.0); //se muestra el tiempo en segundos

	return 0;
}

int* leerVector(char const *FILE_NAME, int tam){
	FILE *archivo;
	int num;
	archivo= fopen(FILE_NAME,"r");
	if(archivo==NULL){
		printf("No se puede abrir el archivo: %s\n", FILE_NAME);
		exit(1);
	}
	int* vector = (int*)malloc(tam*sizeof(int*));
	int i=0;
	while(fscanf(archivo,"%d",&num)>0){
		vector[i]=num;
		i++;
	}
	return vector;
} 

int productoPunto(int* vec1, int* vec2, int n){
	int resultado=0;
	int i;
	for(i=0;i<n;i++){
		resultado= resultado + (vec1[i]*vec2[i]);
	}
	return resultado;
}