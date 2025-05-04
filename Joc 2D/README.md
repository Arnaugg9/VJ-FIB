# VJ-FIB
VJ FIB (UPC) Curso 2024-25 Cuadrimestre Primavera

El ejecutable se encuentra en la carpeta de 'Binari'. 

En la carpeta 'Projecte' está todo el codigo del proyecto. 

Enunciado Juego 2D:
[Exercici2D-2024_25-Q2.pdf](https://github.com/user-attachments/files/19395688/Exercici2D-2024_25-Q2.pdf)

## DESCRIPCION DEL PROYECTO (info.txt)

### INTEGRANTES DEL GRUPO

Arnau García González
Oscar Cerezo Consuegra


### FUNCIONALIDADES IMPLEMENTADAS

A lo largo del desarrollo del proyecto, se han implementado múltiples funcionalidades para replicar de forma fiel la jugabilidad y estética del nivel original del juego. A continuación se detallan todas las características añadidas y su funcionamiento:

#### Jugador y animaciones
Se han implementado todas las animaciones del jugador, incluyendo caminar, saltar, agacharse y atacar entre otras, todas ellas con una gestión precisa de colisiones adaptadas a cada estado. La animación de ataque ha sido cuidadosamente replicada del juego original, permitiendo hacer daño a todos los tipos de enemigos existentes en el nivel.

#### Enemigos
El juego cuenta con tres tipos de enemigos, cada uno con un comportamiento diferenciado:
- Elefante volador: se desplaza por el aire dando botes y lanza proyectiles al jugador.
- Planta: dispara proyectiles, pero es invulnerable a los ataques del jugador.
- Caracol: se desplaza por paredes verticales y por el techo en todas direcciones. Puede esconderse en su caparazón como defensa.

#### Interfaz de usuario (UI)
La interfaz ha sido clonada respetando la estética del juego original. Esta muestra la vida del jugador, vidas extra (representadas como pociones) y otros elementos visuales necesarios para el seguimiento de la partida.

#### Mapa y entorno
Se ha recreado con detalle el nivel original "Magic Forest". Se han incorporado algunos elementos interactivos en el mapa como:
- Hojas que permiten al jugador realizar saltos más altos.
- Lava que resta vida al jugador al contacto, replicando su comportamiento original.

También se han mejorado las colisiones respecto al proyecto inicial, añadiendo detección de colisiones con el techo y simplificando las colisiones con las hojas (manteniendo únicamente la colisión por caída).

#### Power-ups e ítems
Se han implementado cinco objetos originales del juego que aparecen al matar enemigos con una cierta probabilidad:
- Corazón pequeño: restaura un corazón de vida.
- Corazón grande: restaura toda la vida.
- Calabaza: al recoger varias, aumenta la vida máxima.
- Casco: reduce el daño recibido durante cuatro impactos.
- Armadura: otorga invencibilidad temporal.

#### Armas
Se ha incorporado un arma especial: el lanzallamas, que sustituye la lanza normal por un ataque de fuego desde la punta del arma, capaz de dañar a enemigos sin necesidad de alargar la lanza.

#### Jefe final (Boss)
El jefe del nivel, una mariposa dentro de un capullo, ha sido recreado con sus tres fases originales:
1. Dentro del capullo, subiendo y bajando entre árboles.
2. Mariposa en vuelo, lanzando proyectiles.
3. Igual que la fase anterior, pero de color rojo, indicando que le queda poca vida.

#### Escenas y flujo del juego
El juego cuenta con varias escenas adicionales para estructurar la experiencia del jugador:
- Menú principal: acceso al juego pulsando cualquier tecla.
- Pantalla de créditos: aparece al recoger el tótem que deja el jefe tras su derrota.
- Pantalla de pausa: muestra los controles básicos y permite continuar o salir al menú principal.
- Pantalla de muerte: permite reintentar el nivel o volver al menú principal.

Con el fin de facilitar las pruebas de las funcionalidades a los creadores y la corrección al profesor, se ha implementado un God Mode donde el jugador es invencible (no le afectan los ataques) y una Heal para restaurar la vida a la máxima y poner el número de vidas extra a 2 cada vez que se pulse la tecla correspondiente.

#### Sonido y música
Para enriquecer la experiencia de juego y mejorar el feedback al jugador, se ha implementado:
- Música de fondo distinta para cada escena.
- Efectos de sonido asociados a acciones clave como saltar, atacar, lanzar proyectiles, recibir daño, morir, aparición de power-ups, explosiones y cambios de fase del jefe, entre otro.
- Otros aspectos de Game feeling como animaciones y transiciones.


### INSTRUCCIONES DEL JUEGO
Si se accede al menú de pausa (tecla p) se puede ver la descripción gráfica de los controles.
- Flecha derecha y flecha izquierda para moverse a la dirección correspondiente.
- Flecha arriba para cubrirse.
- Flecha abajo para agacharse.
- Z para saltar.
- X para atacar.
- X + Flecha arriba para atacar hacia arriba.
- X + Flecha abajo para atacar hacia debajo.
- C para cambiar de arma.
- P para pausar/despausar.
- H para Heal Cheat (curarse y poner las vidas extra a 2).
- G para God Mode (modo invencible).
- Esc para salir al menú principal en las pantallas pertinentes.
- E para salir del juego.
