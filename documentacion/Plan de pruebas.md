## Estimaciones iniciales:

Duración total 30-40min
Duración por puzzle 5-10min
Duración del tutorial 3-6 min
Duración sala 5-10 min

## Cosas a testear

-Comprensión de los objetos interactuables en escena por parte del jugador. (Objetivo: El jugador e capaz de reconocer los objetos con los que se puede interactuar.)
-Duración de los puzzles y el juego en general. (Objetivo: El jugador tarda lo estimado en cada etapa del juego.)
-Comprensión de que hay que hacer a partir de los diálogos de pista. (Objetivo: la información dada en los diálogos e suficiente para la resolución de los puzzles en el tiempo estimado.)
-Que el jugador sea capaz de retener la información proporcionada en el tutorial y utilizarla durante el juego. Véase la comprensión de que y no es interactuable así como el uso de los diferentes botones de la UI. (Objetivo: El jugador no se traba durante el juego principal y todo lo esencial ha sido mostrado en el tutorial.)

## Datos inGame
### Descripción
Estos datos son los que serán recogidos de manera automática por el ejecutable del programa e introduciéndolos en un csv/excel.
#### Datos comunes a todas las escenas.
-Tiempo que el jugador pasa en la escena.
-Contador de clicks dentro de la escena.
-Contador de clicks en los componentes concretos de la escena (interactuables).
-Contador de cuantas veces se entra en la escena(solo en las de puzzle).
-Contador de cuantas veces se activa el log en escenas concretas.

#### ¿Qué queremos recoger con estos datos?

Con estos datos queremos saber cuanto tiempo pasa un jugador en la sala y cuantas veces interactúa con el entorno vs cuantas con los objetos interactuables, con datos cuantitativos. El contador del log nos da pistas de cuantas veces el jugador necesita acceder a la información que le proporcionamos en los diálogos.
#### Datos a recoger en sala
-Cuanto tiempo pasa en cada parte de la sala además del tiempo global.

#### Datos a recoger en los distintos puzzles
##### -Vita flumen

- Cuantas veces se pulsa cada pipe.

##### -Memento Mori

- Cuantas veces se consulta cada libro VS cuantas veces se pulsa su botón en el candado numérico.

##### -Tempus Fugit

- Cuantas veces se cambia cada manecilla.
- Cuantas veces se entra en la escena antes de conseguir las dos manecillas.

##### -Taza

- Cuantas veces se entra en escena sin la cuchara.

## Datos a observar
### Descripción
Esto son  los datos que recogeremos a partir de la observación de los testers. Estos datos serán los mismos para todos los observadores (obviamente siempre hay lugar a poner algo de información extra si fuera necesario). 
#### Observaciones de cada escena

##### RoomScene
- El jugador comprende que tiene que el cadáver es un punto de interés. (El jugador pulsa al cadáver.)
- El jugador es capaz de navegar entre las dos habitaciones. (El juador entiende que la puerta cambia de sala.)
- El jugador es capaz de distinguir los objetos interactuadles. (El jugador entiende qué objetos son interactuables.)
- El jugador es capaz de descubrir la cuchara del entorno. (Esto hay que cambiarlo igualmente.)

##### Vita Flumen
- El jugador comprende el funcionamiento de la puerta Y.
- El jugador comprende el funcionamiento de la puerta XOR.
- El jugador comprende que debe guiar el agua al cubo.
- El jugador reconoce la recompensa del puzzle como algo interactuable.

##### Memento Mori
- El jugador es capaz de descubrir que hay información en los libros.
- El jugador es capaz de ver como funciona el candado de código numérico.
- El jugador es capaz de descubrir el primer número.
- El jugador es capaz de descubrir el  segundo número.
- El jugador es capaz de descubrir el tercero número.
- El jugador reconoce la recompensa del puzzle como algo interactuable.

##### Tempus Fugit
- El jugador es capaz de ver que le faltan objetos para resolver el puzzle.
- El jugador es capaz de sacar los objetos del inventario para usarlos en el puzzle.
- El jugador es capaz de descubrir la hora del reloj a través de las pistas dadas.
- El jugador reconoce la recompensa del puzzle como algo interactuable.

##### Taza
- El jugador es capaz de ver que le faltan objetos para resolver el puzzle.
- El jugador es capaz de sacar los objetos del inventario para usarlos en el puzzle.
- El jugador es capaz de darse cuenta de que este puzzle carece de recompensa material.


## Preguntas a los testers

Estas preguntas serán realizadas al terminar las pruebas donde obtendremos información mas concreta. Estas pueden variar un poco y los observadores. Es importante hacerles hablar, si las respuestas del jugador son monosílabos esa información sirve de entre poco y nada. Preguntad sobre cosas concretas como "El puzzle de las tuberías lo has resuelto muy rápido ¿Te ha resultado muy fácil?" así los forzamos a soltarse un poco mas y no parece un interrogatorio policial.

- Preguntar por todo aquello que se haya resuelto fuera del margen de tiempo dado, ya sea más o menos stiempo que el del intervalo.

Puntos a tener en cuenta para las preguntas:

-> Dificultades con cualquier paso del tutorial.
-> Dificultades o facilidad con cualquiera de los puzzles.
-> Dificultades a la hora de navegar la UI.

(Apuntad cualquier tipo de cometario que haga en voz alta durante el testeo que sea del estilo: "Ahh esto no lo entendi por...").

## Información que se puede dar

La única cosa que se puede decir inmediatamente si se nota confusión es la posición de la cuchara (ya que esto está mal y no lo podemos testear igualmente)
A parte de esto, NO deis nada de información hasta que se haya pasado el intervalo de tiempo esperado. Si el intervalo de tiempo se ha superado por 5 minutos se puedem dar las siguientes pistas por cada puzzle siempre que se apunten en comentarios:

##### General

Si se han saltado diálogos (apuntarlo en comentarios, esto es importante), indicarles que pueden leerlo de nuevo en el log si no lo hacen por iniciativa propia.

##### RoomScene

Indicar cosas en las escena que les falten por pulsar

##### Vita Flumen

Explicar el funcionamiento de las puertas de paso

##### Memento Mori

Señalar que los libros tienen números 
Señalar que las estanterías tienen un orden

(Si hay que dar ambas dejar un espacio de 3 min entre ellas)

##### Tempus Fugit

Señalar al calendario si no se ha pulsado aún
Señalar el log si no se han leido / acuerdan del diálogo cuando se recoge la etiqueta.
Señalar que se pueden arrastrar objetos del inventario a la escena.

(Si hay que dar varias dejar un espacio de 3 min entre ellas)

##### Tempus Fugit

Señalar que se pueden arrastrar objetos del inventario a la escena.
