
# INFORME.md - Tarea 1: Llamadas al sistema en xv6

**Ramo:** Sistemas Operativos 2025
**Profesor:** Miguel Solís
**Integrantes:** (escribir nombres aquí)



## 1. Objetivo y contexto

El objetivo de esta tarea es modificar el sistema operativo educativo xv6 para agregar dos nuevas llamadas al sistema: `getppid()` y `getancestor(int)`. Esto permite profundizar en la comprensión de la estructura de procesos y el manejo de llamadas al sistema en un kernel real.

## 2. Funcionamiento de las llamadas al sistema


### getppid()
Retorna el PID del proceso padre del proceso que la invoca. Si el proceso no tiene padre, retorna 0 (o -1 si se desea distinguir el caso especial del proceso init).

### getancestor(int n)
Retorna el PID del ancestro n-ésimo del proceso que la invoca:
- `getancestor(0)`: retorna el PID del propio proceso.
- `getancestor(1)`: retorna el PID del padre.
- `getancestor(2)`: retorna el PID del abuelo, y así sucesivamente.
Si no existe tal ancestro (por ejemplo, se pide el ancestro 3 pero solo hay dos generaciones), retorna -1.


## 3. Modificaciones realizadas


### Archivos modificados y su propósito:
- `kernel/sysproc.c`: Implementación de las funciones del kernel `sys_getppid` y `sys_getancestor`.
- `kernel/syscall.c`: Registro de las nuevas llamadas en la tabla de system calls y declaración de prototipos.
- `kernel/syscall.h`: Definición de los números de sistema para las nuevas llamadas.
- `user/user.h`: Prototipos de las funciones de usuario para poder invocar las llamadas desde programas en user/.
- `user/usys.pl`: Inclusión de las nuevas llamadas para la generación automática de stubs de llamada.
- `user/yosoytupadre.c`: Programa de usuario para probar y demostrar el funcionamiento de ambas llamadas.
- `Makefile`: Inclusión de `yosoytupadre` en la lista de programas de usuario a compilar y empaquetar en la imagen.


## 4. Dificultades encontradas y soluciones
- Entender la relación entre procesos y cómo acceder a los ancestros requirió revisar la estructura `struct proc` y el campo `parent`, así como el ciclo de vida de los procesos en xv6.
- Para `getancestor`, fue necesario validar correctamente los límites y el caso de no existencia de ancestros, asegurando que no se acceda a punteros nulos.
- Se presentaron advertencias y errores de compilación relacionados con tipos (`uint` no definido), que se resolvieron usando `unsigned int`.
- Hubo que evitar conflictos de nombres con funciones estándar (`printf`, `fprintf`) al no incluir headers externos en los programas de usuario.
- La integración al Makefile fue directa siguiendo el patrón de otros programas de usuario.


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
