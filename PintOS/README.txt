--------------------------------------
-     Jerarquía carpetas PintOS		 -
-									 -
- Estudiantes: Nicolás Jiménez       -
-			   Kevin Umaña		     -
--------------------------------------

"threads/": Se encuentra el código fuente para el núcleo base. Los archivos en este directorio involucran asignaciones de memoria, paginación, código de inicialización y carga del núcleo en la memoria física, así como la gestión de los procesos, hilos y su respectiva comunicación y sincronización.

"userprog/": Contiene el código fuente del cargador de programa de usuario.

"vm/": Prácticamente es un directorio vacío. Sin embargo, es creado con el propósito de implementar una memoria virtual en él.

"filesys/": Contiene el código fuente para un sistema de archivos básico.

"devices/": Código fuente para la interfaz de dispositivos de entrada/salida: teclado, cronómetro, entre otros.

"lib/": Consiste en la implementación de un subconjunto de la biblioteca estándar de C.

"lib/kernel": Posee partes de la biblioteca de C que únicamente son incluidas en el núcleo de PintOS. Se incluye además implementaciones de algunos tipos de datos que pueden ser utilizados en el código del núcleo.

"lib/user": Contiene partes de la biblioteca de C que únicamente están incluidas en los programas de usuario de PintOS.

"misc/ y utils/": Archivos de utilidad para trabajar con PintOS en una máquina propia.