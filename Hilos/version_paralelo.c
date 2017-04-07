#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

int* crearVector(char const *FILE_NAME, int*);
void* producto_punto( void* param);
struct param{
	int *vec1; //vector 1
	int *vec2; //vector 2
	int n;
	int operados;
	int inicio;
	int limite;
};

int main(int argc, char const *argv[])
{	
	printf("VERSIÓN EN PARALELO\n");
	struct timeval begin, end; //para calcular los tiempos

	int* size = (int*)malloc(sizeof(int*));//se reserva espacio en memoria para el tamaño de los vectores.
	int total_hilos = atoi(argv[3]); //Se obtiene la cantidad de hilos totales.
	struct param parametros[total_hilos];//arreglo de estructuras.
	int * respuestas[total_hilos]; // acá se guardan las respuestas que arroja cada hilo.
	pthread_t hilos_ids[total_hilos]; // se crean tantos hilos sean ingresados por parametro.
	int resultado=0;//Este sera el valor a mostrar en pantalla.

	int* vec_1 = crearVector(argv[1],size); //se crea el vector 1	
	int* vec_2 = crearVector(argv[2],size); // se crea el vector 2
	int division= *size/total_hilos; //se calcula a cada hilo cuantas posiciones del vector le deberia de corresponder hacer
	int modulo = *size % total_hilos; // se calcula si sobran posiciones por operar en los vectores
	int j;

	for(j=0;j<total_hilos;j++){ // lo que se pretende hacer acá es a cada hilo asignarle una estructura en donde solo varia inicio
		parametros[j].vec1=vec_1;
		parametros[j].vec2=vec_2;
		parametros[j].n=*size;
		parametros[j].inicio = j*(division);// inicio es en donde tiene que empezar a ejecutar la operación el hilo dentro del vector
		parametros[j].limite = (j*(division))+division;
		parametros[j].operados = *size - modulo;
	}

	if(total_hilos > *size){ // en cado de que el número de hilos supere el total de posiciones de los vectores
		printf("La cantidad de hilos no puede superar el tamaño de los vectores\n");
		return 0;
	}
	//---------------------------------------------Desde acá se analiza el total del tiempo de ejecución para todos los hilos------------------------------------
	gettimeofday(&begin, NULL);
	int i;
	for(i=0;i<total_hilos;i++) {//a cada hilo se le asigna un intervalo del vector a operar
		pthread_create(&hilos_ids[i],NULL,producto_punto,&parametros[i]);
	}
	for(i=0;i<total_hilos;i++) {// se espera a que todos los hilos terminen sui ejecución.
		pthread_join(hilos_ids[i],(void*)&respuestas[i]);
		resultado =(resultado + *respuestas[i]);//cada valor retornado por cada hilo se suma
	}

	

	gettimeofday(&end, NULL);
	//--------------------------------------------Acá finaliza el tiempo a analizar---------------------------------------------------------------------------------
	printf("resultado = %d\n",resultado);// se muestra el resultado
	printf("tiempo en milisegundos: %f\n", ((end.tv_sec - begin.tv_sec)*1000000.0 +(end.tv_usec- begin.tv_usec))/1000.0); //se muestra el tiempo en milisegundos
	printf("tiempo en segundos: %f\n", ((end.tv_sec - begin.tv_sec)*1000000.0 +(end.tv_usec- begin.tv_usec))/1000000.0); //se muestra el tiempo en segundos
	
	//se libera la memoria
	free(size);
	free(vec_1);
	free(vec_2);
	return 0;
}


/*
	Este metodo permite crear un vector a partir de un Archivo de texto
*/
int* crearVector(char const *FILE_NAME, int * size){
	FILE *archivo;
	int num;
	archivo= fopen(FILE_NAME,"r"); // se abre el archivo
	if(archivo==NULL){
		printf("No se puede abrir el archivo: %s\n", FILE_NAME);
		exit(1);
	}
	int tam=10;
	int* vector = (int*)malloc(tam*sizeof(int*)); // se reserva memoria, inicialmente para 10 enteros
	int i=0;
	while(fscanf(archivo,"%d",&num)>0){// se recorre el contenido del archivo
		i++;
		if(i==11 || i== 1001 || i==1000001){//en caso de necesitar mas espacio en memoria dinamica
			switch(i){
				case 11:
					vector = (int*)realloc(vector ,pow(tam,3) * sizeof(int*)); // en caso de que los 10 enteros no sean suficientes se reserva espacio para 1000 enteros y asi sucesivamente
				break;
				case 1001:
					vector = (int*)realloc(vector ,pow(tam,6) * sizeof(int*));
				break;
				case 1000001:
					vector = (int*)realloc(vector ,pow(tam,8) * sizeof(int*));
				break;
			}
		}
		vector[i-1]=num; // se guarda el valor i del archivo en la posición que le corresponde en el vector.
	}
	*size = i;
	fclose(archivo);//se cierra el archivo
	return vector; //se retorna el vector
} 

/*
	Este metodo calcula el producto punto entre dos vectores
*/
void* producto_punto( void* param){

	struct param* m= (struct param*)param;
	int n_1 = m -> n; //total de posiciones del vector
	int inicio_1 = m ->inicio;//posicion de los vectores en la cual inicia el recorrido el hilo 
	int limite_1 = m -> limite;//hasta donde llega a rrecorrer el hilo
	int operados_1= m -> operados; //Cantidad de posiciones a recorrer por los hilos
	int res=0;//resultado
	int *total ;//Resultado a retornar
	int i;
	
	for( i= inicio_1 ;i< limite_1; i++){ // se hace el producto punto
		res = res + (m->vec1[i] * m->vec2[i]);
	}
	if(operados_1 == limite_1){ // si es el ultimo hilo y ademas faltan operar mas posiciones de vector, entonces estas se hacen por el ultimo hilo.
		for (i = limite_1; i < n_1; ++i)
		{
			res = res + (m->vec1[i] * m->vec2[i]);
		}
	}
	total = malloc(sizeof(int*));
	*total= res; 
	return (void *)total;//se retorna el resultado obtenido para ese hilo
}