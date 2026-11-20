# Griglia Animata 2D con OpenGL e Dear ImGui (macOS)

Progetto per il Laboratorio di Computer Graphics (Esercitazione del 15 Ottobre 2026).  
Adattato ed eseguibile su macOS (Apple Silicon / Intel) con CMake, Dear ImGui, GLFW, GLM e OpenGL.

---

## Prerequisiti su Mac

I pacchetti necessari sono gestiti via **Homebrew**:
```bash
brew install cmake glfw glm
```

---

## Come Avviare l'Applicazione

### Metodo 1: Script Rapido (Consigliato)
Dalla cartella del progetto:
```bash
./run.sh
```
Questo script compila automaticamente le modifiche (se presenti) e avvia l'eseguibile.

### Metodo 2: Makefile
```bash
make run
```
Oppure per compilare soltanto:
```bash
make build
```

### Metodo 3: CMake da riga di comando
```bash
cmake -B build
cmake --build build
./GrigliaAnimata2D
```

---

## Funzionalità e Controlli

L'applicazione realizza una griglia 2D configurabile di forme alternate (Farfalle nelle righe pari, Cuori nelle righe dispari) con proiezione ortografica e matrici di modellazione (traslazione, rotazione, scalatura):

- **Interfaccia ImGui**:
  - **Numero di righe (`numRows`)**: slider per variare dinamicamente le righe della griglia.
  - **Numero di colonne (`numCols`)**: slider per variare dinamicamente le colonne.
- **Rappresentazione e Matrici**:
  - Ogni elemento calcola la propria `Model Matrix`: $M_{r,c} = T(x_{r,c}, y_{r,c}) \cdot S(s_x, s_y)$.
  - Le farfalle e i cuori sono dimensionati per restare contenuti nelle rispettive celle senza sovrapposizioni.
- **Uscita**:
  - Premi **ESC** o chiudi la finestra per terminare l'applicazione.

---

> [!NOTE]
> Per il **Configuratore / Modellatore di Curve di Hermite 2D** (introdotto nel Laboratorio del 5 Novembre), consulta la cartella `../lab5nov26/Modellatore_Hermite_2D` e la guida completa riportata di seguito.
