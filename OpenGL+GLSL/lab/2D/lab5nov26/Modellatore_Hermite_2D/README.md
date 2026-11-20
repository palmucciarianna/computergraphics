# Modellatore 2D con Curve Cubiche di Hermite (macOS)

Applicazione interattiva per la creazione, manipolazione ed esportazione di forme 2D mediante curve cubiche di Hermite interpolanti (Laboratorio del 5 Novembre 2026).  
Adattato ed eseguibile su macOS con CMake, GLFW, GLM, Dear ImGui ed OpenGL 3.3 Core Profile.

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
./Modellatore_Hermite_2D
```

---

## Guida Rapida all'Uso del Modellatore

### 1. Comandi Base e Scorciatoie
- **F1**: Mostra / nasconde il pannello ImGui (quando nascosto puoi disegnare anche nelle aree precedentemente coperte dal menu).
- **ESC**: Chiude l'applicazione.
- **T / C / B** (con un vertice selezionato in modalità *Modifica*):
  - **T** / **Shift + T**: Diminuisce / aumenta la **Tensione** ($\pm 0.1$)
  - **C** / **Shift + C**: Diminuisce / aumenta la **Continuità** ($\pm 0.1$)
  - **B** / **Shift + B**: Diminuisce / aumenta il **Bias** ($\pm 0.1$)

### 2. Le 3 Modalità di Gestione dei Punti
Nel pannello **Gestione dei punti**:
1. **Inserimento**: Clic con il tasto sinistro nell'area grafica per posizionare i punti di controllo in sequenza.
2. **Modifica**: Clicca e trascina con il tasto sinistro un punto di controllo esistente per riposizionarlo.
3. **Elimina**: Clicca su un vertice esistente per cancellarlo (la curva si ricalcola automaticamente).

### 3. Gestione delle Tangenti
1. Attiva la casella **Visualizza Tangenti** per vedere i vettori tangenti su ciascun punto di controllo.
2. Attiva **Modifica Tangenti**: seleziona il vertice desiderato e trascina l'estremo del vettore tangente per cambiarne direzione ed intensità.
> **Nota**: mentre *Modifica Tangenti* è attiva, il vertice rimane bloccato per evitare spostamenti involontari. Per spostare il vertice disattiva *Modifica Tangenti*.

### 4. Curve Multiple e Chiusura
- **Numero di curve**: imposta quante curve compongono la figura (fino a 20) e premi **Crea curve mancanti**.
- Seleziona **Curva 1**, **Curva 2**, ecc. per lavorarci singolarmente. Ciascuna curva può avere il proprio colore (**Colore curva selezionata**).
- **Chiudi Forma (C1)**: connette l'ultimo punto al primo garantendo continuità $C^1$ delle derivate (fondamentale per permettere la triangolazione e il riempimento).
- **Parametrizzazione**: scegli tra *Uniforme* e *Secondo le corde*.

### 5. Visualizzazione e Riempimento
- **Visualizza Poligono di Controllo**: mostra la spezzata che unisce i punti di controllo.
- **Renderizza**: calcola la triangolazione (con libreria *earcut*) e mostra la forma piena colorata.
- **Wireframe**: mostra il reticolo di triangolazione generato.

### 6. Salvataggio ed Esportazione
- **Salva / Apri Forma (`.txt`)**: salva la definizione matematica (punti e curve) per poterla riaprire e continuare a modificare nel modellatore.
- **Esporta Mesh OBJ/MTL (`.obj` + `.mtl`)**: genera il modello geometrico 2D triangolato con materiali, pronto per essere importato negli altri progetti del corso (es. sprite del player, nemici, ostacoli).
