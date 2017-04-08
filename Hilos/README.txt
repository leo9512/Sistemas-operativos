ejecutar así:
Para compilar paralelo --> gcc version_paralelo.c -o ejecutable -lpthread -lm
Para compilar secuencial --> gcc version_secuencial.c -o ejecutable 

Para ejecutar paralelo --> ./ejecutable nombre_archivo_1 nombre_archivo_2 num_hilos
Para ejecutar  secuencial --> ./ejecutable nombre_archivo_1 nombre_archivo_2 tamaño_vector
