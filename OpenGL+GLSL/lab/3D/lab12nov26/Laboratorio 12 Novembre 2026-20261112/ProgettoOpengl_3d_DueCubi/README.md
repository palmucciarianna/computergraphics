# Scena 3D - Due Cubi (Automatico e Manuale)

Questo progetto espande l'esercitazione 3D aggiungendo una scena interattiva con **due cubi distinti**, un piano di appoggio, una piramide decorativa, una telecamera completamente navigabile e un'interfaccia grafica Dear ImGui per il controllo dei parametri in tempo reale.

---

## 1. Come Avviare l'Eseguibile

Dal terminale, spostati nella cartella ed esegui il programma:

```bash
cd "ProgettoOpengl_3d_DueCubi"
./ProgettoOpengl_3d_DueCubi
```

oppure dalla cartella `build/`:
```bash
./ProgettoOpengl_3d_DueCubi/build/ProgettoOpengl_3d_DueCubi
```

---

## 2. I Due Cubi nella Scena

Entrambi i cubi condividono la stessa geometria (`cubeGeometry`), ma hanno **due istanze e matrici di modellazione (`ModelMatrix`) distinte e indipendenti**:

### 🟧 Cubo 1: Automatico (In movimento continuo)
- **Comportamento**: Ruota e orbita continuamente sopra il piano descrivendo una traiettoria circolare con oscillazione verticale fluida ($dt$ delta time).
- **Controlli da ImGui**:
  - Checkbox per mettere in pausa o riprendere l'animazione (*Play / Pause*).
  - Slider per la velocità di movimento.
  - Slider per il raggio dell'orbita.
  - Slider per l'altezza dal suolo.
  - Selettore di colore dedicato.

### 🟦 Cubo 2: Manuale (Controllabile a mano)
Si può muovere a mano in **due modi**:
1. **Da Tastiera**:
   - `I` / `K` : Sposta avanti / indietro lungo l'asse $Z$.
   - `J` / `L` : Sposta a sinistra / destra lungo l'asse $X$.
   - `U` / `O` : Sposta in alto / in basso lungo l'asse $Y$.
   - `N` / `M` : Ruota il cubo attorno all'asse $Y$.
2. **Dal Pannello Grafico ImGui**:
   - Slider / DragFloat per la **Posizione** $(X, Y, Z)$.
   - Slider per la **Rotazione** in gradi su $(X, Y, Z)$.
   - Slider per la **Scala** dell'oggetto.
   - Color picker per cambiare colore al cubo.
   - Pulsante *"Reset Posizione Manuale"*.

---

## 3. Controlli Telecamera 3D

La telecamera è completamente funzionante con coordinate sferiche (orbita) e assi locali:

- **Spostamento vista**:
  - `W` / `S` : Avanti / Indietro
  - `A` / `D` : Sinistra / Destra
  - `Q` / `E` : Salita / Discesa
- **Orbita**:
  - **Tasti Freccia** (Su, Giù, Sinistra, Destra) oppure
  - **Trascina con il Mouse** (tasto sinistro premuto sulla scena).
- **Zoom**:
  - **Rotellina del Mouse** oppure tasti `+` / `-`.
- **Reset**:
  - Pulsante *"Reset Visuale Telecamera"* nel pannello ImGui.

---

## 4. Come Ricompilare

```bash
cd "ProgettoOpengl_3d_DueCubi"
cmake -B build -S .
cmake --build build
```
oppure dalla radice del laboratorio:
```bash
cmake --build build
```
