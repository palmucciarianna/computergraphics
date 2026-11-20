# Scena 3D Interattiva con OpenGL e Dear ImGui (macOS)

Progetto per il Laboratorio di Computer Graphics A.A. 2026/2027 (Esercitazione del 20 Novembre 2026).  
Adattato ed eseguibile su macOS (Apple Silicon / Intel) con CMake, Dear ImGui, GLFW, GLM e OpenGL.

---

## Prerequisiti su Mac

I pacchetti necessari sono gestiti via **Homebrew** (già installati e rilevati):
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
./Progetto3D_Interattivo
```

---

## Controlli da Tastiera e Mouse

| Comando / Tasto | Funzione |
| :--- | :--- |
| **W / S** | Movimento camera avanti / indietro |
| **A / D** | Movimento camera sinistra / destra |
| **Q / E** | Movimento camera verticale (su / giù) |
| **Frecce direzionali** | Orbita della camera attorno al centro |
| **TAB** | Attiva / disattiva modalità navigazione mouse |
| **Spazio** | Attiva / disattiva visualizzazione Wireframe |
| **1 / 2 / 3** | Seleziona asse di manipolazione (X / Y / Z) |
| **T / R / G** | Trasla / Ruota / Scala l'oggetto selezionato |
| **Canc / Backspace** | Elimina l'istanza selezionata |
| **Mouse Drag** | Orbita camera con trascinamento sulla scena (se navigazione attiva) |
| **ESC** | Chiusura immediata dell'applicazione |

---

## Documentazione Dettagliata del Progetto

Per comprendere l'architettura iniziale, cosa è stato fatto per Mac e i 7 TODO da svolgere, consulta:
👉 **[PROGETTO.md](file:///Users/ariannapalmucci/Desktop/AScelta/Computer%20Graphics/OpenGL%2BGLSL/lab/3D/lab20nov26/Laboratorio%2020%20Novembre%202026-20261120/PROGETTO.md)**
