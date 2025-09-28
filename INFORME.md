
# INFORME.md - Tarea 1: Llamadas al sistema en xv6

**Ramo:** Sistemas Operativos 2025

**Profesor:** Miguel Solís

**Integrantes:** Valentina Alessandra Rosello Serra y Felipe Alonso Madrid Jaramillo



## 1. Objetivo y contexto

El objetivo de esta tarea es modificar el sistema operativo educativo xv6 para agregar dos nuevas llamadas al sistema: `getppid()` y `getancestor(int)`. Esto permite profundizar en la comprensión de la estructura de procesos y el manejo de llamadas al sistema en un kernel real.


## 2. Funcionamiento de las llamadas al sistema

### getppid()
Esta llamada al sistema retorna el PID (identificador de proceso) del proceso padre del proceso que la invoca. Es útil para que un proceso pueda saber quién es su padre, lo que puede ser relevante para sincronización, comunicación o depuración. Si el proceso no tiene padre (por ejemplo, el proceso init), retorna 0 o -1 según la implementación.

**Ejemplo de uso:**
```c
int ppid = getppid();
printf("PID de mi padre: %d\n", ppid);
```

### getancestor(int n)
Esta llamada retorna el PID del ancestro n-ésimo del proceso que la invoca:
- `getancestor(0)`: retorna el PID del propio proceso.
- `getancestor(1)`: retorna el PID del padre.
- `getancestor(2)`: retorna el PID del abuelo, y así sucesivamente.
Si no existe tal ancestro (por ejemplo, se pide el ancestro 3 pero solo hay dos generaciones), retorna -1. Esto permite recorrer la cadena de ancestros de un proceso de manera controlada y es útil para depuración o para entender la genealogía de procesos.

**Ejemplo de uso:**
```c
for(int i = 0; i < 5; i++) {
	int anc = getancestor(i);
	printf("getancestor(%d) = %d\n", i, anc);
}
```



## 3. Explicación de las modificaciones realizadas

Para implementar ambas llamadas al sistema, se modificaron y agregaron los siguientes archivos:

- **kernel/sysproc.c:**
	- Se implementó la función `sys_getppid`, que retorna el PID del proceso padre accediendo al campo `parent` de la estructura `proc`.
	- Se implementó la función `sys_getancestor`, que recorre la cadena de ancestros usando el campo `parent` de la estructura `proc` tantas veces como indique el parámetro, retornando el PID correspondiente o -1 si no existe tal ancestro.

- **kernel/syscall.c:**
	- Se agregaron los prototipos de las nuevas funciones.
	- Se incluyeron en el arreglo de system calls para que el kernel pueda despachar correctamente las llamadas desde espacio de usuario.

- **kernel/syscall.h:**
	- Se definieron los números de sistema para `getppid` y `getancestor`.

- **user/user.h:**
	- Se agregaron los prototipos de las funciones de usuario para que los programas puedan invocar las nuevas llamadas.

- **user/usys.pl:**
	- Se incluyeron las nuevas llamadas para la generación automática de los stubs de llamada.

- **user/yosoytupadre.c:**
	- Se creó un programa de usuario que prueba y demuestra el funcionamiento de ambas llamadas, mostrando los PIDs y ancestros tanto en el proceso principal como en un hijo.

- **Makefile:**
	- Se incluyó `yosoytupadre` en la lista de programas de usuario a compilar y empaquetar en la imagen.



## 4. Dificultades encontradas y cómo se resolvieron

- **Comprensión de la estructura de procesos:**
	- Fue necesario revisar la estructura `struct proc` y entender cómo se enlazan los procesos mediante el campo `parent`, así como el ciclo de vida de los procesos en xv6 (creación, reparentización, terminación).

- **Validación de límites y punteros nulos:**
	- Para `getancestor`, se debió validar correctamente los límites del parámetro y el caso de no existencia de ancestros, asegurando que no se acceda a punteros nulos y retornando -1 en esos casos.

- **Errores de compilación y tipos:**
	- Se presentaron advertencias y errores relacionados con tipos (`uint` no definido), que se resolvieron usando `unsigned int` en los prototipos y funciones.

- **Conflictos de nombres con funciones estándar:**
	- Hubo que evitar conflictos de nombres con funciones estándar (`printf`, `fprintf`) al no incluir headers externos en los programas de usuario, usando solo las implementaciones propias de xv6.

- **Integración al Makefile:**
	- La integración fue directa siguiendo el patrón de otros programas de usuario, pero se verificó que el nuevo ejecutable estuviera correctamente empaquetado en la imagen.


## 5. Pruebas realizadas
Se creó el programa `yosoytupadre.c` que imprime el PID propio, el del padre y prueba la función `getancestor` en diferentes niveles, tanto en el proceso principal como en un hijo. El resultado esperado es que se muestren los PIDs correctos y que al llegar a un ancestro inexistente, se imprima -1.

### Ejemplo de salida esperada:

```
Mi PID: 3
PID de mi padre: -1

Probando getancestor(n):
getancestor(0) = 3
getancestor(1) = 2
getancestor(2) = 1
getancestor(3) = -1

[Hijo] Mi PID: 4, getppid(): -1
[Hijo] getancestor(0) = 4
[Hijo] getancestor(1) = 3
[Hijo] getancestor(2) = 2
[Hijo] getancestor(3) = 1
[Hijo] getancestor(4) = -1
```


## 6. Instrucciones de compilación y ejecución

1. Compilar y limpiar todo el sistema:
	```sh
	make clean && make qemu
	```
	Esto compila el kernel, los programas de usuario y lanza el emulador QEMU con xv6.

2. Una vez en la shell de xv6 (después de que aparezca el prompt `$`), ejecutar:
	```sh
	yosoytupadre
	```
	Esto mostrará en pantalla los resultados de las llamadas al sistema implementadas.

3. Para salir de QEMU, puedes usar `Ctrl+a` luego `x`.

---
