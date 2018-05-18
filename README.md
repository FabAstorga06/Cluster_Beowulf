# Cluster Beowulf

Cluster Beowulf de tres nodos para agilizar algoritmos implementados en C++ para procesamiento de imágenes mediante una interfaz de paso de mensajes (MPI). Primeramente, se requiere configurar el clúster Beowulf. Para más información, puede consultar la documentación en la carpeta [doc](https://github.com/FabAstorga06/Cluster_Beowulf/tree/master/doc). 

### Compilación
Para compilar el código, dirígase a la carpeta donde se encuentra el archivo Makefile y ejecute el siguiente comando: 
```
$ make
```

### Ejecución
Para ejecutar el código, corra el ejecutable que se generó en la carpeta donde se encuentran los archivos, por medio de mpich, mediante el siguiente comando:
```
$ mpiexec -f hosts -n nproc ./cluster k s goku.png salida.png f
```

donde "hosts" corresponde al archivo con los identificadores de cada nodo perteneciente al cluster. La cantidad de núcleos de procesamiento se especifican en "nproc". Las dimensiones del kernel para el filtro de Gauss para la imagen corresponde a "k" y "s" es la constante sigma. Por último, la bandera "f" corresponde al manejo de los bordes de la imagen, la cual puede ser "-c" para el efecto circular, o bien, "-r" para un efecto reflectivo.

Asegúrese de configurar correctamente el clúster e introducir todos los parámetros necesarios. 

### Ejemplos 
```
$ mpiexec -f hosts -n 8 ./cluster 23 10 goku.png salida.png -r
$ mpiexec -f hosts -n 4 ./cluster 51 15 tapion.png salida.png -c
$ mpiexec -f hosts -n 3 ./cluster 101 20 buu.png salida.png -r
```


