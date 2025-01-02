# MI-MINISHELL

Proyecto Minishell de 42. 

### Introducción

blblabla

Cosas que tiene que hacer el shell:

- Mostrar una entrada mientras espera un comando nuevo (readline con prompt??)
- Tener un historial funcional (add_history()??)

- Buscar y ejecutar el ejecutable correcto (basado en la variable PATH ✅ o mediante el uso de rutas relativas o absolutas ❌) --> con PATH lo tenemos, lo otro hay que mirar

- Evitar usar más de una variable global para indicar la recepción de una señal (para lo de la recepción de señales)

PARSEO / GESTON INPUT

- No interpretar comillas sin cerrar o caracteres especiales no especificados en el enunciado como \ o ;
- Gestionar que la ' evite que el shell interprete los metacaracteres en la secuencia entrecomillada
- Gestionar que la " evite que el shell interprete los metacaracteres en la secuencia entrecomillada exceptuando el $.

----------------------------------
_LO QUE TENEMOS POR AHORA DE GESITÓN DE INPUT (process_input() en prueba_process_input.c)_

- Recibimos la línea del input con readline() en el main.
- split_commands() -> separa la línea del input en comandos con el pipe ('|') como separador. Cada uno de estos comandos tendrá un comando propiamente dicho, y puede tener un infile y un outfile. Si no los tiene, significa que funciona con el STDIN o el STDOUT o las redirecciones que le toque con el pipe.
- itera a través de estos comandos con process_commands()
- cada comando es procesado en process_single_command()
- process_single_comand() ->
  1. splitea el comando (compuesto por comando propiamente dicho, sus argumentos y, si hay, infile y/o outfile) en tokens con split_tokens() con espacio (' ') como separador.
  2. Da memoria y guarda los argumentos con allocate_args() (!! -> aquí habrá que liberar o realocar memoria, porque da memoria tanto a comandos propiamente dichos como a infiles y outfiles, y posteriormente los files los vamos a guardar en otro sitio, así que nos va a sobrar memoria en args).
  3. Cuenta la cantidad de redirecciones de output con count_outfile_redirections() (porque podemos tener varias y eso significa que hay que crear todos los archivos aunque al final solo utilicemos el último). count_outfile_redirections() guarda los nombres de outfile en el array de char node->outfile_name y los tipos de redirecciones (si son APPEND o TRUNC) en el array de ints node->outfile_type.
  4. Procesamiento de tokens con process_tokens(), que identifica si el token es una redireccion u otra cosa -> si es una redirección, lo gestiona con handel_redirection(), que identifica el tipo de redirección y guarda el file asociado donde le toca (si es infile u outfile y de qué tipo) (!! esto no se repite con lo de contar el número de otufiles???); si no es una redirección, guarda los argumentos (comandos propiamente dichos) en node->full_cmd (!! aquí es donde nos sobra memoria (??) basta con un free(args) para solucionarlo?).
  
_LO QUE FALTA DE GESITÓN DE INPUT_

Interpretación de comillas:

- '
- "
- caracteres especiales
- comillas sin cerrar
- lo de $

----------------------------------------

REDIRECCIONES Y EJECUCION

- Implementar redirecciones: 
  - "<" redirige el input ✅
  - ">" redirige el output ✅ 
   
     - Si haces cat infile.txt > outfile1.txt > outfile2.txt > outfile3.txt > outfile4.txt lo que hace bash es generar todos estos outfiles pero no guardar el resultado en ellos, solo lo guarda en el último generado. ~~Nuestra minishell solo crea el último Y DEBERÍA CREAR TODOS! --> POSIBLE SOLUCIÓN: ALTERAR LA ESTRUCTURA DE NODO Y PONER CHAR *OUTFILE_NAMES PARA QUE SEA UN ARRAY DE OUTFILE_NAMES; tb hacer int *outfile_type, necesitamos saber si cada uno de estos outfiles tiene que ser abierto con > o con >> (esto es necesario porque si tenemos una redirección tipo APPEND en el outfile intermedio, no borrará lo que hay en este outfile; si es tipo TRUNC, sí lo hará). EN SET_OUT_FD ITERAR A TRAVÉS DE TODOS ESOS NOMBRES, ABRIRLOS Y CREARLOS COMO TOCA Y EN LOS PROCESOS REDIRIGIR SOLO EL ÚLTIMO OUTFILE_NAME (ITERANDO SOBRE ELLOS?).~~ Ya funciona.
       
  - "<<" recibe un delimitador. Lee del input de la fuente actual hasta que aparezca una línea que contenga solo el delimitador. No necesita utilizar el historial. (ESTO SERÁ SUSTITUIR EL READLINE POR UN GET_NEXT_LINE CON X CARACTER/DELIMITADOR EN VEZ DE CON \N?? --> MIRAR SI OTROS MINISHELLS GUARDAN EL DELIMITADOR EN LA ESTRUCTURA DEL NODO) ❌
  - ">>" redirige el output en modo append ✅
- Implementar pipes (|). El output de cada comando en la pipeline se conecta a través de un pipe al input del siguiente comando ✅
- Gestionar $?: debe expandirse al estado de salida del comando más reciente ejecutado en la pipeline --> RELACIONADO CON VARIABLE GLOBAL (?) -> extern int	g_status --> relacionado con errno y etc, codigos de error!!!

  SIGNAL... (?)
- Gestionar ctr-C, ctrl-D y ctrl-\, que deben funcionar como en bash
- Cuando sea interactivo: (--> esto va con las señales, EFRAIN - .20), funcion signal())
   - ctrl-C: imprime una nueva entrada en una línea nueva
   - ctrl-D: termina el shell
   - ctrl-\: no hace nada
  
BUILINTS
- Implementar los built-ins (esto son funciones aparte que hay que diseñar, supongo que tendrán que guardarse en la estructura de nodo en la parte de full_cmd, pero al ejecutar (execute()) no hay que buscar su PATH, si no que habrá que ejecutar la función (?)).
   - echo con la opción -n
   - cd solo con una ruta relativa o absoluta
   - pwd sin opciones
   - export sin opciones
   - unset sin opciones
   - env sin opciones (hay que guardar nuestra propia env para poder modificarla...)
   - exit sin opciones

 ### Funciones autorizadas

| ***readline()***   |          |
|------------------|-----------------|
| Librería| readline/readline.h |
| ¿Qué hace?| Lee una línea de la terminal y la devuelve. |
| Prototipo| char *readline(const char *prompt) |
| Argumentos|prompt: mensaje que se pone delante de la línea leída que devuelve. |
| Return   |  |

| ***rl_clear_history()***   |          |
|------------------|-----------------|
| Librería| #include <readline/readline.h> |
| ¿Qué hace?| Limpiar el historial de líneas escritas en el terminal// Preparar el espacio para escribir nuevas líneas sin que aparezcan las antiguas |
| Prototipo| void rl_clear_history(void) |
| Argumentos| |
| Return   | |

| ***rl_on_new_line()***   |          |
|------------------|-----------------|
| Librería| #include <readline/readline.h> |
| ¿Qué hace?| Se utiliza para configurar el comportamiento del terminal cuando una nueva línea se inserta. En particular, esta función permite especificar qué acción debe tomar el terminal cuando se presiona la tecla de retorno en un campo de texto |
| Prototipo| void rl_on_new_line(int new_line) |
| Argumentos|???|
| Return   | |

| ***rl_replace_line()***   |          |
|------------------|-----------------|
| Librería| #include <readline/readline.h> |
| ¿Qué hace?| se utiliza para reemplazar una parte del texto de una línea con un nuevo texto. Esta función es útil cuando se quiere personalizar el comportamiento del terminal en relación con las líneas.|
| Prototipo| int rl_replace_line(const char *buf, const char *, const char *)|
| Argumentos|`const char *buf`: Una puntero a un carácter (`char`) que contiene el texto que se va a reemplazar en la línea actual.`const char *`: Un puntero a un carácter (`char`) que indica el carácter de finalización de la línea. En este caso, no está claro qué valor exacto se espera aquí, pero es común utilizar `\n` para indicar el final de una línea.`const char *`: Un puntero a un carácter (`char`) que contiene el texto que se va a reemplazar en la línea actual.
| Return   | 0, cuando hay una línea en el buffer y se reemplaza todo el contenido, -1, si  no hay una línea en el buffer o cuando la línea es vacía y se intenta reemplazarla con un carácter de finalización|


| ***rl_redisplay()***   |          |
|------------------|-----------------|
| Librería| #include <readline/readline.h> |
| ¿Qué hace?| Se utiliza para actualizar y mostrar el contenido del buffer de la terminal. Esto significa que se actualizan los caracteres en el buffer que han cambiado desde la última vez que se mostró el buffer|
| Prototipo| void rl_redisplay(void) |
| Argumentos| |
| Return   | |

| ***add_history()***   |          |
|------------------|-----------------|
| Librería| #include <readline/readline.h> |
| ¿Qué hace?| 1.  Verifica si ya se ha llamado a esta función para agregar contenido <br> al historial de líneas. <br> 2.  Si no es así, agrega el contenido del buffer de texto al historial <br> de líneas. <br> 3.  Actualiza la posición del cursor en la terminal.|
| Prototipo| char *add_history(const char *const _buf)|
| Argumentos|_buf`: Es un constante pointer a una cadena de caracteres (`const char *const`).|
| Return   | puntero a una cadena de caracteres (`char *`) que apunta al contenido agregado al historial de líneas.|


| ***access()***   |          |
|------------------|-----------------|
| Librería| unistd.h |
| ¿Qué hace?     |Comprueba la accesibilidad de un archivo o directorio. |
| Prototipo      |int access(const char *pathname, int mode)|
| Argumentos     |pahtname: cadena que representa la ruta. <br> mode: entero que especifica el tipo de acceso que queremos verificar. <br> - F_OK: verifica la existencia del archivo. <br> - F_OK: verifica la existencia del archivo. <br> - R_OK: accesible para lectura. <br> - W_OK: accesible para escritura. <br>  - X_OK: accesible para ejecución.|
| Return   | Devuelve 0 si el acceso especificado está permitido,<br> en caso contrario devuelve -1 y esatblece errno.|


| ***open()***   |          |
|------------------|-----------------|
| Librería| unistd.h  |
| ¿Qué hace?      | Abre un archivo y obtiene su file descriptor (fd)|
| Prototipo         | int open(const char *pathname, int flags, mode_t mode) |
| Argumentos       |   pahtname: ruta del archivo. <br> flags: entero que especifica el modo en el que se abre el archivo. <br> Algunos modos comunes: <br>   - O_RDONLY: solo lectura. <br> - O_WRONLY: solo escritura. <br> - O_RDWR: lectura y escritura. <br> - O_CREAT: crea el archivo si no existe. <br> - O_TRUNC: si el archivo existe,  elimina su contenido. <br> - O_APPEND: escribe al final del archivo sin truncarlo. <br> mode: solo se utiliza con O_CREAT, <br> establece los permisos del archivo, expresados en octal.|
| Return   | Devuelve un fd en caso de éxito y -1 en caso de error.|

| ***close()***   |                 |
|------------------|-----------------|
| Librería| unistd.h  |
| ¿Qué hace?      | Cierra un archivo que ha sido abierto mediante open(). <br> Al cerrarlo, el sistema libera el fd y cualquier recurso asociado, <br> permitiendo que el SO reutilice el descriptor para otras operaciones.|
| Prototipo         | Prototipo: int close(int fd)|
| Return   | Devuelve 0 en caso de éxito y en caso de error, -1 y establece errno .|

| ***read()***   |          |
|------------------|-----------------|
| Librería| unistd.h |
| ¿Qué hace?      | Lee datos desde un archivo (a partir de su fd) o entrada (como stdin).|
| Prototipo       |ssize_t read (int fd, void *buffer, size_t count) |
| Argumentos       |fd: file descriptor desde el que se quiere leer. <br> Se obtiene mediante open() o dup() o un descritor estándar como 0 (stdin).<br> buffer: puntero a un espacio de memoria donde se almacenarán los datos leídos. <br> count: número máximo de bytes que se intentarán leer.|
| Return   | Devuelve el número de bytes leídos. Si se alcanza el final del archivo (EOF), <br> devuelve 0. Devuelve -1 y establece errno en caso de error.|

También hemos visto fork8), wait(), waitpid()... Pero y wait3, wait4??


| ***dup()***   |          |
|------------------|-----------------|
| Librería| unistd.h |
| ¿Qué hace?      |Duplica fds. Útil para redirigir entradas y salidas, especialmente en procesos <br> que necesitan manipular la stdin, la stdout o la stderr.|
| Prototipo         |int dup(int oldfd)|
| Argumentos       |oldfd: file descriptor que se desea duplicar.|
| Return   | Devuelve el nuevo fd duplicado o 1 en caso de error, estableciendo errno. <br> El nuevo fd apunta al mismo archivo que oldfd, comparten el mismo puntero <br> de archivo y hereda los mismos permisos de acceso.|

| ***dup2()***   |          |
|------------------|-----------------|
| Librería| unistd.h |
| ¿Qué hace?      |Duplica un fd en un nuevo fd.|
| Prototipo         |int dup(int oldfd, int newfd)|
| Argumentos       |oldfd: fd que se desea duplicar. <br> newfd: nuevo fd. <br> Si newfd está en uso, dup2 lo cierra antes de duplicar oldfd en él.|
| Return   | Devuelve newfd o -1 en caso de error.|


| ***execve()***   |          |
|------------------|-----------------|
| Librería| unistd.h |
| ¿Qué hace?      |Reemplaza el proceso actual con un nuevo programa. <br> Se utiliza para ejecutar un programa especificado en un archivo ejecutable, <br> con el entorno y los argumentos que se le proporcionan.  |
| Prototipo         |int execve(const char *filename, char *const argv[], char *const envp[])|
| Argumentos       |filename: ruta del archivo ejecutable. <br> argv: array de cadenas que representan los argumentos del programa.  <br> envp: array de cadenas que contiene las variables de entorno.|
| Return   | En caso de éxito, no devuelve nada; el proceso actual se reemplaza <br> por el nuevo programa y la ejecución continúa desde el punto de entrada <br> del nuevo programa. En caso de error devuelve -1 y establece errno.|

| ***exit()***   |          |
|------------------|-----------------|
| ¿Qué hace?      |Termina un programa de manera controlada. Permite salir de un programa <br> con un código de estado que indica si la salida fue exitosa o si ocurrió un error.|
| Prototipo         | void exit(int status) |
| Argumentos       |status: entero que se devuelve al SO cuando el programa termina. <br> Este valor puede indicar éxito (0) o error (valor distinto de 0).|


| ***fork()***   |          |
|------------------|-----------------|
| Librería| unistd.h |
| ¿Qué hace?      |Genera un nuevo proceso hijo a partir de un proceso padre. <br> El proceso hijo es una copia del proceso padre, pero tiene su propio PID <br> (indentificador de proceso). El proceso padre y el proceso hijo <br> se ejecutan simultáneamente,  aunque pueden comportarse de manera diferente.|
| Prototipo         | fd_t fork(void) |
| Return   | En el proceso padre, fork() devuelve el PID del proceso hijo. <br> El el proceso hijo, fork() devuelve 0. <br> Si fork() falla, devuelve -1 en el proceso padre y no crea ningún proceso hijo.|

| ***pipe()***   |          |
|------------------|-----------------|
| Librería| unistd.h |
| ¿Qué hace?      |Crea un canal de comunicación unidireccional entre procesos. <br> Permite que un proceso (el escritor) envíe datos a otro proceso (el lector) <br> a través de un buffer de memoria. <br> pipe() crea un array de dos fds que representan los extremos del pipe. <br> - pipefd[0]: extremo de lectura. <br> - pipefd[1]: extremo de escritura. <br> pipefd[1] escribirá en su fd y pipefd[0] leerá el pipefd[1] y escribirá en su propio fd. |
| Prototipo         |int pipe(int pipefd[2])|
| Return   | Devuelve 0 en caso de éxito y en caso de error devuelve -1 y establece errno.|


    //Ejemplo: fork() y pipe()

    pid_t    do_pipe(int pipefd[2])
    {
        pid_t    pid;

        if (pipe(pipefd) == -1)
        {
            perror("Pipe");
            exit(EXIT_FAILURE);
        }
        pid = fork();
        if (pid < 0)
        {
            perror("Fork");
            exit(EXIT_FAILURE);
        }
        return (pid);
    }

    int    main(void)
    {
        pid_t    pid;
        int      pipefd[2];

        pid = do_pipe(pipefd);
        if (pid == 0)
            printf("Estamos en el proceso hijo.\n");
        else
            printf("Estamos en el proceso padre.\n");
        return (0);
    }

    | ***wait()***   |          |
|------------------|-----------------|
| Librería|  |
| ¿Qué hace?      |Permite que un proceso padre suspenda su ejecución hasta que <br> uno de sus procesos hijo termine. Cuando un proceso hijo finaliza, <br> wait() recupera su estado de salida, lo que permite al proceso padre <br> realizar tareas específicas dependiendo del resultado de la ejecución del hijo. |
| Prototipo         |pid_t wait(int *status)|
| Argumentos       |status: puntero a un entero donde wait() almacena el estado de salida del hijo. <br> Este valor sirve para obtener información sobre cómo terminó el proceso hijo,<br> normalmente mediante macros como WIFEXITED o WEXITSTATUS...|
| Return   | Devuelve el PID del hijo que terminó en caso de éxito. <br> Si no hay hijos a los que esperar, devuelve -1 y establece errno.|

**ERRNO
->ECHILD: Indica que no hay procesos hijos. Esto puede ocurrir si todos los procesos hijos ya han terminado y sus estados ya han sido recogidos.
->EINTR: La llamada a wait() fue interrumpida por una señal antes de que cualquier proceso hijo terminara.
->EINVAL: Se pasó un argumento inválido a la llamada de sistema. Aunque esto es menos común con wait(), puede ocurrir con funciones relacionadas como waitpid().

| ***waitpid()***   |          |
|------------------|-----------------|
| ¿Qué hace?      |Permite que un proceso padre espere a que un proceso hijo termine su ejecución. <br> Es más flexible que wait porque permite especificar a qué proceso hijo se debe esperar. <br> Útil cuando hay múltiples procesos hijo. |
| Prototipo         |pid_t waitpid(pid_t pid, int *status, int options)|
| Argumentos       |pid: puede ser el PID del proceso hijo al que se quiere esperar. <br> Si es -1 indica que se espera a cualquier proceso hijo (comportamiento similar a wait()). <br> Si es 0 espera a cualquier proceso hijo del mismo grupo de procesos. <br> Si es otro valor negativo, espera a cualquier hijo cuyo grupo de procesos sea el mismo <br> que el del proceso padre. <br> status: puntero a un entero donde wait() almacena el estado de salida del hijo. <br> Este valor sirve para obtener información sobre cómo terminó el proceso hijo, <br> normalmente mediante macros como WIFEXITED o WEXITSTATUS... <br> options: 0 indica el comportamiento estándar, espera a que el hijo termine. <br> WNOHANG no bloquea, si no hay hijos que no hayan terminado, retorna inmediatamente. <br> WUNTRACED también retorna si un hijo está detenido. |
| Return   | Devuelve el PID del hijo que terminó en caso de éxito. En caso de error, devuelve -1 y establece errno.|

| ***perror()***   |          |
|------------------|-----------------|
| ¿Qué hace?      | Imprime un mensaje de error en stderr. <br> Muestra un mensaje personalizado junto con la descripción del error <br> correspondiente a errno.|
| Prototipo         | void perror(const char *s)|
| Argumentos       |s: Mensaje personalizado a imprimir antes de la descripción del error.|

| ***strerror()***   |          |
|------------------|-----------------|
| ¿Qué hace?      | Convierte el valor de errno en un mensaje de error. |
| Prototipo         | char *strerror(int errnum) |
| Argumentos       |errnum: código de error a convertir en un mensaje de error. <br> Típicamente es el valor de errno después de una operación fallida.|
| Return   | Devuelve un puntero a una cadena con el mensaje de error <br> correspondiente al valor de errnum.|

| ***signal()***   |          |
|------------------|-----------------|
| Librería| #include <signal.h> |
| ¿Qué hace?| permite manejar señales en un programa. Una señal es una interruptión no esperada por el sistema operativo que causa un programa a cambiar su comportamiento o a terminar |
| Prototipo| int signal(int sig, void (*func)(int)) |
| Argumentos| **`sig`**: Es el número de señal. Puedes encontrar este valor en una tabla de señales (señales numéricas) o mediante funciones como `sigaction()`, `kill()` y `SIGALRM`|
| Return   | ????|

SIGINT -> Ctrl + C -> interrupción del usuario
SIGQUIT -> termina el programa inmediatamente -> Ctrl + \  --> EN LA HOJA DE PROYECTO PONE QUE ESTO NO TIENE QUE HACER NADA
SIGPIPE -> Ctrl + D -> tiene que terminar el shell

| ***signaction()***   |          |
|------------------|-----------------|
| Librería| <signal.h> |
| ¿Qué hace?| Se utiliza para cambiar la acción que el proceso debe realizar al recibir una señal específica. Es una alternativa más flexible y potente a la función signal()|
| Prototipo| int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact) |
| Argumentos| signum: El número de la señal que se desea manejar (por ejemplo, SIGINT para una interrupción del teclado). act: Un puntero a una estructura struct sigaction que especifica la nueva acción para la señal. oldact: Un puntero a una estructura struct sigaction donde se almacenará la acción anterior para la señal. Este parámetro puede ser NULL si no se desea recuperar la acción anterior|
| Return   | Si sigaction() retorna 0, sabes que la operación fue exitosa. Si retorna -1, puedes mirar el valor de errno para averiguar qué salió mal. "Errno" es una variable global que indica el error específico|

| ***kill()***   |          |
|------------------|-----------------|
| Librería| <signal.h> |
| ¿Qué hace?| Se utiliza para enviar señales a procesos. A pesar de su nombre, no necesariamente se utiliza para "matar" un proceso; puede enviar cualquier señal especificada al proceso indicado. |
| Prototipo| int kill(pid_t pid, int sig) |
| Argumentos| pid: El identificador del proceso al que se desea enviar la señal. <br> pid > 0: Envía la señal al proceso cuyo ID es pid. <br> pid == 0: Envía la señal a todos los procesos en el mismo grupo de procesos que el proceso que llama. <br> pid == -1: Envía la señal a todos los procesos para los cuales el proceso que llama tenga permisos para enviar señales, excepto al proceso 1 (init). <br> pid < -1: Envía la señal a todos los procesos en el grupo de procesos cuyo ID es -pid. <br> sig: El número de la señal que se desea enviar. Algunas señales comunes incluyen: <br> SIGTERM: Solicita la terminación del proceso.<br> SIGKILL: Fuerza la terminación del proceso. <br> SIGINT: Interrumpe el proceso (equivalente a Ctrl+C en la terminal). <br> SIGHUP: Señal de desconexión. |
| Return   | "0" si la señal se ha enviado con éxito, "-1" si se ha producido un error. En este caso, la variable global errno se establece para proporcionar información adicional sobre el tipo de error|

exit()???

| ***getcwd()***   |          |
|------------------|-----------------|
| Librería| <unistd.h> |
| ¿Qué hace?| Llena la matriz de caracteres apuntada por buf con la ruta del directorio de trabajo actual. La ruta se almacena como una cadena de caracteres terminada en nulo (\0). Si buf es NULL, getcwd() asigna una matriz de tamaño size usando malloc() y devuelve un puntero a esta matriz; el llamador es responsable de liberar esta memoria.|
| Prototipo| char *getcwd(char *buf, size_t size) |
| Argumentos| buf: Un puntero a una matriz de caracteres donde se almacenará el directorio de trabajo actual. size: El tamaño de la matriz buf. Debe ser lo suficientemente grande como para contener la ruta completa del directorio de trabajo actual, incluyendo el terminador nulo. |
| Return   | Puntero a la matriz de caracteres buf que contiene la ruta del directorio de trabajo actual. Retorna NULL y establece la variable global errno para indicar el error específico |


*ERRNO
-> ERANGE: El tamaño del buffer size es insuficiente.
-> EINVAL: size es 0 o negativo.
-> ENOMEM: No hay suficiente memoria disponible para asignar el buffer.


| ***chdir()***   |          |
|------------------|-----------------|
| Librería| <unistd.h> |
| ¿Qué hace?| Cambia el directorio de trabajo actual del proceso al directorio especificado por path. Este cambio afecta a todas las operaciones que se realicen después de la llamada a chdir() y que dependan del directorio de trabajo actual |
| Prototipo| int chdir(const char *path) |
| Argumentos| path: Un puntero a una cadena de caracteres que especifica la ruta del nuevo directorio de trabajo. Puede ser una ruta absoluta o relativa. |
| Return   | En caso de éxito: Retorna 0.  En caso de error: Retorna -1 y establece la variable global errno para indicar el error específico. |

*ERRNO
->EACCES: Permiso denegado para el directorio especificado.
->EFAULT: path apunta fuera del espacio de direcciones accesible.
->EINVAL: path no es válido.
->ENOENT: El directorio especificado no existe.
->ENOTDIR: Una parte de path no es un directorio.



### Parsing

### Ejecución/pipex

COSAS QUE SE TIENEN QUE HACER:

- Buscar y ejecutar el ejecutable correcto (basado en PATH o mediante el uso de rutas relativas o absolutas)
- Implementar redirecciones
- Implementar pipes

- 

Cómo va la cosa (20.12.24):

COSAS QUE FUNCIONAN:
- Si se ejecuta ./minishell empieza el loop con "minishell: "
- Si los comandos que se le pasan funcionan, vuelve a mostrar "minishell: "
- Nodos: guarda el nombre en formato char* del infile y el outfile, el tipo de outfile (si > es tipo APPEND y si >> es tipo TRUNC; se hace open() con diferentes parámetros), guarda el comando y sus argumentos en un char**, guarda el puntero al nodo previo y al next
- Sin pipes: se genera un solo nodo y acepta bien las redirecciones < , > y >>
  
      Ejemplo: < infile.txt cat > outfile.txt
              - Sin infile: lee del stdin hasta que se presiona Ctrl-D -> pero creo que Ctrl-D debe terminar el shell. Si eso es así, no se hasta cuándo debería leer del stdin (!!!)
              - Sin outfile: escribe en el stdout: esto funciona bien
- Con pipes: crea el número de nodos correcto



COSAS QUE NO FUNCIONAN:
- Si se introduce un comando inválido, el programa no termina pero no vuelve a mostrar la salida "minishell: "


### Estado comando previo ... $?

***Códigos de error***

<img src="https://github.com/AnaMac3/42-minishell/blob/main/img/Captura%20desde%202024-12-20%2017-43-59.png" alt="Números_error" width="800"/> 

***Ejemplo de códigos error***

<img src="https://github.com/AnaMac3/42-minishell/blob/main/img/Captura%20desde%202024-12-20%2017-47-25.png" alt="Ejemplo" width="800"/> 

***getenv()***

<img src="https://github.com/AnaMac3/42-minishell/blob/main/img/Captura%20desde%202024-12-20%2017-56-17.png" alt="getenv()" width="800"/> 


### Builints ??

### Recursos
https://www.youtube.com/watch?v=yTR00r8vBH8 (video de Oceano, guia simplificada)
#### Resumen video
1) 
Crea main.c y una funcion de get_line, basicamente, para sacar cosas por pantalla.
Hace referencia a REPL; ReadEvaluatePrintLoop, protocolo de bash.
---Stands for Read-Eval-Print Loop. It's an interactive programming environment commonly used in scripting languages like Python, Ruby, and JavaScript. Here's a quick breakdown of how it works:
A)Read: The REPL reads the user's input or command.
B)Eval: It evaluates (executes) the command or code entered.
C)Print: It prints out the result of the evaluation.
D)Loop: It loops back to allow the user to input another command.

3)
Crea utils.c y una funcion que es getcwd, Se usa para que cuando nos movamos por la terminal aparezca la ruta del archivo en que estemos.
Ademas de esto tambien a que aparezca un logo al iniciar la minihell.
4)
Digamos que la segunada parte, usa funcion gettokens.

La función `strtok()` tiene los siguientes pasos:

1.  **Obtención de la cadena de texto**: Recibe una cadena de texto como 
entrada y devuelve una cadena de texto actualizada que contiene el token 
más reciente encontrado.
2.  **Delimitador**: La función utiliza un carácter de delimitador (por 
ejemplo, espacio en blanco, tabulador, coma, etc.) para dividir la cadena 
de texto en tokens individuales.
3.  **Tokenización**: Comienza a leer la cadena de texto y busca el primer 
delimitador que encuentre. Al encontrarlo, devuelve la palabra 
correspondiente al token más reciente encontrado hasta ahora.
4.  **Nueva posición**: En cada llamada a la función, se mantiene la 
posición actual en la cadena de texto. Esto significa que si se llama a 
`strtok()` varias veces, siempre se devolverá el siguiente token 
encontrado.
 
Funciones usadas:
*get_line
*read_line
*getwdc
*gettoken -> https://craftinginterpreters.com/scanning.html (** La guia es de Java, pero se puede entender lo que son los tokens**)



https://github.com/EfrainVisconti/Minishell/tree/main/src

https://github.com/madebypixel02/minishell?tab=readme-ov-file

https://github.com/cclaude42/minishell





### Flujo de pipex

Apuntes varios:
- pipe() -> crea un canal de comunicación entre procesos.
- fork() -> genera un nuevo proceso hijo a partir de un proceso padre

```mermaid
graph LR;
    A["***main.c (main.c)***"] --> B["1 - Checkeo de los argumentos."];
    B --> C["Si argc != 5"];
    C --> D["Mensaje error y exit (EXIT_FAILURE)"];
    A --> E["2 - Pipe: crea un canal de comunicación entre procesos."];
    E --> F["Si pipe() == -1"];
    F --> D;
    A --> G["3 - Fork: genera un nuevo proceso hijo a partir de un proceso padre."];
    G --> H["Si fork() < 0"];
    H --> D;
    H --> I["Si fork() == 1: estamos en el proceso hijo"];
    I --> J["***child_process*** (pipex.c): configura y ejecuta el primer comando en el pipeline"];
    H --> K["Si fork() > 1: estamos en el proceso padre"];
    K --> L["***parent_process*** (pipex.c): configura y ejecuta el último comando en el pipeline"];
    J --> M["1 - Comprueba los accesos del archivo de entrada"];
    M --> N["Si el archivo no existe o no tiene permisos de lectura"];
    N --> Q["Mensaje error a ***handle_error*** (pipex_utils.c), ***close pipefd*** (pipex_utils.c) y exit (EXIT_FAILURE)"];
    J --> O["2 - Obtiene el fd del archivo de entrada con open()"];
    O --> P["Si fd == -1"];
    P --> Q;
    J --> R["3 - Cierra el extremo de lectura (pipefd[0]), porque aquí no se usa"];
    J --> S["4 - Redirige la stdin al fd del archivo de entrada y el stdout al extremo de escritura del pipe (pipefd[1])"];
    J --> T["5 - Cierra el input original y el pipefd[1] después de la redirección"];
    J --> U["6 - Llama a ***execute*** para runear el cmd1. El comando lee del stdin redirigido, es decir, del archivo de entrada, y escribe su output en el stdout redirigido, en pipefd[1]"];
    L --> W["1 - waipid(): espera a que termine el proceos hijo"];
    L --> X["2 - Abre o crea el archivo de salida con open(), con permisos de escritura y lectura. Si ya existe, lo trunca"];
    L --> Y["3 - Comprueba los accesos del archivo de salida"];
    Y --> Z["Si el archivo no tiene permisos de lectura"];
    Z --> AF["Mensaje error a ***handle_error*** (pipex_utils.c), ***close pipefd*** (pipex_utils.c) y exit (EXIT_FAILURE)"];
    X --> AA["Si fd == -1"];
    AA --> AF;
    L --> AB["4 - Cierra el extremo de escritura pipefd[1] porque aquí no se usa"];
    L --> AC["5 - Redirige la stdin al extremo de lectura del pipe (pipefd[0]) y el stdout al fd del archivo de salida"];
    L --> AD["6 - Cierra el pipefd[0] y el output original después de la redirección"];
    L --> AE["7 - Llama a ***execute*** para runear el cmd2. El comando lee del extremo de lectura (pipefd[0]) y escribe su output en el archivo de salida"];

style D fill:#ffcccb,stroke:#ff0000,stroke-width:1px
style Q fill:#ffcccb,stroke:#ff0000,stroke-width:1px
style AF fill:#ffcccb,stroke:#ff0000,stroke-width:1px

```

```mermaid
graph LR;
  A["***execute*** (pipex.c): "] --> B["Splitea el comando"];
  B --> C["Si el split da NULL o el primer elemento del array es nulo"];
  C --> D["***handle_cmd_error*** (pipex_utils.c): mensaje error y libera el array de argumentos. Si era el primer comando, cierra el stdout y exit (EXIT_FAILURE)"];
  A --> E["***get_path*** (get_path.c): obtiene el ejecutable para un comando."];
  E --> F["***get_path_from_envp*** (get_path.c): obtiene la ruta del array de la variable de entorno"];
  F --> G["Busca el string de envp que empieza por PATH= y devuelve el resto de esa string o NULL si no la encuentra"];
  E --> H["Splitea la ruta obtenida del ***get_path_from_envp*** para obtener los directorios, splitea el comando en sus argumentos y libera la ruta"];
  H --> I["Si no hay directorios o no hay argumentos de comando, libera los arrays y devuelve NULL"];
  E --> K["***build_cmd_path*** (get_path.c): construye la ruta del comando"];
  K --> L["Hace strjoin de cada directorio con / y con el primer argumento de comando. Si la ruta existe y tiene permisos de ejecución, libera los arrays y la devuelve. Si no, lo intenta con el siguiente directorio. Si no lo encuentra, devuelve NULL."];
  E --> M["Si existe, devuelve el array resultante de ***build_cmd_path***, si no, libera los arrays y devuelve un duplicado del cmd"];
  A --> N["Si la ruta obtenida de ***get_path*** es NULL o no tiene acceso de ejecución"];
  N --> O["Libera la ruta"];
  O --> D;
  A --> P["Llama a execve() con la ruta, los argumentos del comando y la variable de entorno"];
  P --> Q["Si execve() == -1"];
  Q --> R["***handle_cmd_error*** (pipex_utils.c): mensaje error y libera el array de argumentos. Libera la ruta, los argumentos de comando y exit (EXIT_FAILURE)"];

style D fill:#ffcccb,stroke:#ff0000,stroke-width:1px
style R fill:#ffcccb,stroke:#ff0000,stroke-width:1px

```

### Flujo de bonus

EL BONUS NO LO TUVE 100% BIEN!! ME FALLA EL HERE_DOC

```mermaid
graph LR;
    A["***main (main_bonus.c)***"] --> B["***check_args*** (main.c): checkeo de los argumentos."];
    B --> C["Si argc < 5"];
    C --> D["Mensaje error y exit (EXIT_FAILURE)"];
    B --> E["Si argv[1] es here_doc y argc != 6"];
    E --> D;

    A --> F["Si argv[1] es here_doc"];
    F --> G["***here_doc*** (pipex_bonus.c): configura y ejecuta e here_doc en un proceso hijo"];
    G --> H["Crea un pipe con ***do_pipe*** ()"];
    H --> I["Si pipe() o fork() < 1"];
    I --> Ñ["Mensaje error y exit (EXIT_FAILURE)"];
    G --> J["En el proceso hijo cierra el prev_pipefd[0] (extremo de lectura) y utiliza ***get_next_line*** (./my_libft) para leer las líneas del stdin hasta el delimitador (argv[2]) y escribirlas en prev_pipefd[1] (el extremo de escritura)"];
    G --> K["En el proceso padre cierra prev_pipefd[1] y redirecciona el stdin a a prev_pipefd[0] (extremo de lectura). Luego waitpid() para esperar a que termine el proceso hijo."];
    A --> L["En el resto de los casos llamamos a ***first_process_infile*** (pipex_bonus)"];
    L --> M["Hace exactamente lo mismo que ***child_process** (pipex.c) de la parte no bonus"];
    A --> N["Después, ***process_cmds*** (main_bonus.c)"]
    N --> O["Empieza por argv[2] o argv[3] dependiendo de si es here_doc o no. Entra en un bucle que hace pipes para cada argumento hasta el último."]
    N --> P["Los argumentos intermedios los pasa a ***inter_process*** (con el prev_pipefd y el pipefd) y el último argumento a ***last_process*** con el último comando y el prev_pipefd"];
    P --> Q["***inter_process*** (pipex_bonus.c)"];
    Q --> R["Espera a que termine cualquier proceso hijo con waitpid(). Cierra los pipefd que no utiliza (el de lectura del pipe actual (pipefd[0]) y el de escritura de pipe previo (prev_pipefd[1])). Redirige stdin al extremo de lectura del pipe previo (prev_pipefd[0]), y redirige el stdout al extremo de escritura del pipe actual (pipefd[1]). Cierra los fds originales depsués de la redirección y llama a ***execute*** (pipex_bonus.c), que es igual al del pipex sin bonus."];
  N --> S["***last_process*** (pipex_bonus.c): funciona exactamente igual que el ***parent_process*** del pipex sin bonus"];
  N --> T["Al final del bucle, cierra el extremo de lectura del pipe previo (prev_pipefd[0]) y el extremo de escritura del pipe actual (pipefd[1]). Sustituye el pipe previo por el pipe actual."];

   
style D fill:#ffcccb,stroke:#ff0000,stroke-width:1px
style Ñ fill:#ffcccb,stroke:#ff0000,stroke-width:1px

```

