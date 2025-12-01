## 🎮 SFML Mini Game

Juego arcade desarrollado en C++ con SFML, donde controlas a un jugador que debe esquivar enemigos que caen desde la parte superior de la pantalla. El objetivo es sobrevivir el mayor tiempo posible y aumentar tu puntuación.

⸻

### 🚀 Características

    •	Movimiento fluido del jugador.
    •	Enemigos con caída aleatoria.
    •	Detección de colisiones.
    •	Sistema de puntuación en tiempo real.
    •	Sonidos al colisionar.
    •	Soporte para:
    •	Imágenes (sprites)
    •	Audio (efectos de sonido)
    •	Tipografía personalizada
    •	Fondo dinámico.
    •	60 FPS estables.

⸻

### 🕹️ Controles

Acción Tecla
Mover arriba W o Flecha ↑
Mover abajo S o Flecha ↓
Mover izquierda A o Flecha ←
Mover derecha D o Flecha →
Salir del juego ESC

⸻

### 🧩 Requisitos

    •	Sistema operativo: macOS / Linux
    •	Compilador: g++ compatible con C++17
    •	Librería:
    •	SFML 2.x

### Instalación de SFML en macOS:

brew install sfml@2

⸻

### ⚙️ Compilación

Ubícate en la carpeta del proyecto y ejecuta:

g++ sfml_demo.cpp -o sfml_demo -std=c++17 \
-I/opt/homebrew/opt/sfml@2/include \
-L/opt/homebrew/opt/sfml@2/lib \
-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

⸻

### ▶️ Ejecución

./sfml_demo

⸻

### 🗂️ Recursos del Juego

Todos los archivos deben estar en la misma carpeta que el ejecutable.

Archivo Función
player.png Sprite del jugador
enemy.png Sprite del enemigo
bg.png Fondo del juego
beep.wav Sonido de colisión
PressStart2P-Regular.ttf Fuente retro

✅ Si algún archivo no está presente, el juego usa formas geométricas por defecto.

⸻

### 🧠 Lógica del Juego

• Los enemigos se generan cada cierto intervalo.
• La velocidad es aleatoria.
• Si un enemigo sale de pantalla: aumenta el score.
• Si el jugador colisiona:
• Se reproduce un sonido.
• La puntuación vuelve a 0.
• Se reinician los enemigos.

⸻

### 🛠️ Tecnologías Usadas

• Lenguaje: C++17
• Librería gráfica: SFML
• Audio: SFML Audio
• Tipografía personalizada

⸻

### 📦 Estructura del Proyecto

- game-sfml/
- ├── sfml_demo.cpp
- ├── player.png
- ├── enemy.png
- ├── bg.png
- ├── beep.wav
- ├── PressStart2P-Regular.ttf
- └── README.md

⸻

### 📌 Autor

- Jonás García Corniel
- Desarrollador Full Stack
- Email: gcjonas22@gmail.com

⸻

### 📄 Licencia

Este proyecto es de uso académico y educativo. Puede ser modificado libremente para aprendizaje.

⸻

### ⭐ Recomendación

No olvides dar una estrella al repositorio si te gustó el proyecto 💙
