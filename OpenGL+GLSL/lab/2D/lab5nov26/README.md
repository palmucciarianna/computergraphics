# Laboratorio 5 Novembre 2026 - OpenGL 3.3 Core Profile (macOS)

Questo laboratorio contiene due applicazioni complete adattate ed ottimizzate per **macOS (Apple Silicon / Intel)** con **OpenGL 3.3 Core Profile**, **GLFW**, **GLM** e **Dear ImGui**:

1. [Modellatore_Hermite_2D](file:///Users/ariannapalmucci/Desktop/AScelta/Computer%20Graphics/computergraphics/OpenGL+GLSL/lab/2D/lab5nov26/Modellatore_Hermite_2D) (Parte I):  
   Modellatore interattivo 2D per creare e manipolare curve cubiche di Hermite interpolanti, modificare tensione/continuità/bias, gestire le tangenti, triangolare con *earcut* e salvare/esportare in formato `.txt` e mesh `.obj`/`.mtl`.

2. [Parte_II_Cannone_Hermite](file:///Users/ariannapalmucci/Desktop/AScelta/Computer%20Graphics/computergraphics/OpenGL+GLSL/lab/2D/lab5nov26/Parte_II_Cannone_Hermite) (Parte II):  
   Gioco 2D (*Cannone 2D*) che illustra la separazione architetturale tra logica di gioco e rappresentazione grafica. Il Player e i bersagli mobili (farfalle, funghi, cuori) possono essere renderizzati sia tramite primitive che importando le geometrie `.obj` generate con il modellatore Hermite, animate mediante trasformazioni con le **Model Matrix**.

---

## 1. Prerequisiti su Mac

Se non già installati, installa le librerie necessarie tramite **Homebrew**:
```bash
brew install cmake glfw glm
```

---

## 2. Come Compilare ed Eseguire

### Metodo 1: Script Rapido `run.sh` (Consigliato)

- **Per avviare il Modellatore Hermite 2D (Parte I)**:
  ```bash
  cd Modellatore_Hermite_2D
  ./run.sh
  ```

- **Per avviare il Gioco Cannone Hermite 2D (Parte II)**:
  ```bash
  cd Parte_II_Cannone_Hermite
  ./run.sh
  ```

---

### Metodo 2: Makefile dalla cartella principale

Dalla radice del laboratorio (`lab5nov26`):

- Compilare entrambi i progetti:
  ```bash
  make build
  ```
- Avviare il Modellatore 2D:
  ```bash
  make run-modeler
  ```
- Avviare il Gioco Cannone 2D:
  ```bash
  make run-game
  ```
- Pulizia dei file di build:
  ```bash
  make clean
  ```

---

### Metodo 3: CMake manuale

#### Modellatore 2D:
```bash
cd Modellatore_Hermite_2D
export PATH="/opt/homebrew/bin:$PATH"
cmake -B build
cmake --build build
./Modellatore_Hermite_2D
```

#### Gioco Cannone 2D:
```bash
cd Parte_II_Cannone_Hermite
export PATH="/opt/homebrew/bin:$PATH"
cmake -B build
cmake --build build
./Cannone_Hermite_2D
```

---

## 3. Guida Rapida ai Controlli

### Parte I: Modellatore Hermite 2D
- **F1**: Mostra / nasconde il pannello di controllo ImGui (quando nascosto è possibile cliccare liberamente nell'intera area di lavoro).
- **Tasti T / C / B** (con un punto selezionato in modalità *Modifica*):
  - `T` / `Shift+T`: Tensione $\pm 0.1$
  - `C` / `Shift+C`: Continuità $\pm 0.1$
  - `B` / `Shift+B`: Bias $\pm 0.1$
- **Modalità di lavoro**:
  - **Inserimento**: Click sinistro per piazzare nuovi punti di controllo.
  - **Modifica**: Trascina i punti di controllo con il mouse.
  - **Elimina**: Click su un vertice per rimuoverlo.
  - **Visualizza / Modifica Tangenti**: Seleziona il punto e sposta il vettore tangente.
  - **Chiudi Forma (C1)**: Connette l'ultimo al primo punto garantendo continuità $C^1$.
  - **Renderizza / Wireframe**: Triangolazione della curva chiusa con riempimento colorato.
  - **Salva / Esporta OBJ**: Salva la curva o esporta la mesh `.obj`/`.mtl` nella cartella `assets/`.
- **ESC**: Chiudi applicazione.

### Parte II: Gioco Cannone 2D
- **Frecce Direzionali / WASD**: Movimento del player (cannone).
- **Mouse**: Puntatore di mira del cannone.
- **Click Sinistro**: Spara proiettile.
- **F1**: Mostra / nasconde interfaccia ImGui (selezione mesh player OBJ vs Primitive, parametri di gioco).
- **ESC**: Chiudi applicazione.

---

## 4. Note sulle Modifiche macOS Effettuate

1. **Header OpenGL**: Rimossi gli include non portabili di Windows (`<GL/gl.h>`) e integrato **GLAD** per il caricamento dei puntatori OpenGL 3.3.
2. **Flag Forward Compatibility Apple**: Aggiunto `glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE)` necessario su macOS per abilitare il Core Profile di OpenGL 3.3+.
3. **Framework Apple**: Linkati `Cocoa`, `IOKit` e `CoreVideo` nel `CMakeLists.txt`.
4. **Post-build copy**: Gli shader GLSL e le cartelle `assets` vengono copiati automaticamente accanto all'eseguibile compilato sia in `build/` che nella cartella principale del progetto per garantire la corretta esecuzione con qualunque working directory.
