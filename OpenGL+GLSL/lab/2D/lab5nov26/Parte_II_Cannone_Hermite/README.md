# Cannone 2D - Player con Geometrie Hermite (macOS)

Applicazione di gioco 2D basata su OpenGL 3.3 Core Profile, Dear ImGui, GLFW e GLM (Laboratorio 5 Novembre 2026 - Parte II).
Dimostra la separazione tra la logica di gioco e la rappresentazione grafica:
- Il Player e i Target (farfalle, funghi, cuori) utilizzano modelli geometrici esportati in formato **OBJ / MTL** (creati con il modellatore Hermite 2D) oppure primitive procedurali.
- Animazioni gestite interamente via **Model Matrix**, senza riscrivere i buffer dei vertici (VBO).

---

## Prerequisiti su macOS

Installare i pacchetti necessari tramite **Homebrew**:
```bash
brew install cmake glfw glm
```

---

## Come Avviare l'Applicazione

### Metodo 1: Script Rapido (Consigliato)
Dalla cartella del progetto:
```bash
chmod +x run.sh
./run.sh
```

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
export PATH="/opt/homebrew/bin:$PATH"
cmake -B build
cmake --build build
./Cannone_Hermite_2D
```

---

## Controlli di Gioco

- **Frecce Direzionali / WASD**: Movimento del player (cannone).
- **Cursore del Mouse**: Mira e orientamento del cannone.
- **Click Sinistro**: Spara proiettile.
- **F1**: Mostra / nasconde il pannello di controllo Dear ImGui.
- **Pannello ImGui**:
  - Selezione mesh del player (primitive vs OBJ/MTL importato da Hermite).
  - Parametri di gioco (velocità, frequenza bersagli, scala).
  - Statistiche e punteggio.
- **ESC**: Esci dall'applicazione.
