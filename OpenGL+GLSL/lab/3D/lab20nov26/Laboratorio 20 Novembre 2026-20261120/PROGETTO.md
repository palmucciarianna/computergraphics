# Documentazione del Progetto: Scena 3D Interattiva

Questo documento descrive:
1. **Com'è il progetto all'inizio** (architettura, componenti e funzionamento nativo).
2. **Cosa è stato fatto per l'adattamento su macOS** (senza alcuna modifica ai file `.cpp` e `.h`).
3. **Quali saranno le prossime modifiche (TODO 1 - 7)** da implementare per l'esercitazione.
4. **Registro delle modifiche svolte** (da aggiornare man mano prima di ogni commit).

---

## 1. Com'è il Progetto all'Inizio

L'applicazione realizza una scena tridimensionale interattiva in OpenGL 3.3 Core Profile con un pannello GUI gestito da Dear ImGui.

### Concetto Chiave: Geometry vs Instance
Il codice fa una distinzione architetturale netta tra forma e oggetto:
- **Geometry (`Geometry.h`, `Geometry.cpp`)**: descrive la mesh (vertici, indici, colori per vertice, VAO/VBO/EBO) nel suo sistema di coordinate locale. È stateless e immutabile una volta caricata in GPU. Più oggetti possono condividere la stessa geometria (es. più cubi).
- **Instance (`Scene.h`)**: rappresenta un'entità specifica presente nel mondo 3D. Contiene le proprietà dinamiche:
  - Posizione (`glm::vec3 position`)
  - Rotazione (`glm::vec3 rotation` in gradi)
  - Scala (`glm::vec3 scale`)
  - Matrice Model (`glm::mat4 modelMatrix`) calcolata con TRS (`updateModelMatrix()`)
  - Colore uniforme (`glm::vec3 color`) e flag `useUniformColor`
  - Visibilità (`bool visible`) e selezionabilità (`bool selectable`)
  - Nome identificativo (`std::string name`) e indice della geometria associata (`std::size_t geometryId`)

### Moduli del Codice
| File / Modulo | Responsabilità Principale |
| :--- | :--- |
| **`ShapeGenerator`** | Genera i dati geometrici per le 5 primitive: Cubo, Piano, Piramide, Sfera, Toro. |
| **`Geometry`** | Incapsula le risorse OpenGL (VAO, VBO, EBO) e il disegno (`glDrawElements`). |
| **`Scene`** | Contenitore centrale di tutte le geometrie caricate e delle istanze create. |
| **`Camera`** | Gestisce la matrice di vista (`View`) e proiezione (`Projection`), movimento locale e orbita sferica. |
| **`Renderer`** | Riceve le istanze da `Scene`, imposta uniform e shader e renderizza la scena. Disegna anche l'ancora/assi dell'oggetto selezionato. |
| **`GuiLayer`** | Inizializza Dear ImGui, disegna la finestra dei controlli e gestisce gli input della GUI. |
| **`ShaderMaker` & `Shader`** | Compilazione, linking e gestione degli shader GLSL (`shaders/objects.vert` e `shaders/objects.frag`). |
| **`main.cpp`** | Inizializzazione GLFW/GLAD, setup iniziale della scena, ciclo di rendering principale e smistamento degli eventi con `handleInput()`. |

### Cosa funziona già di default (senza modifiche)
- Creazione della finestra GLFW e contesto OpenGL 3.3 Core Profile con GLAD.
- Caricamento delle 5 geometrie base e posizionamento iniziale di 5 istanze (Cubo, Piano, Piramide, Sfera, Toro).
- Navigazione della camera da tastiera:
  - `W` / `S`: avanti / indietro lungo la direzione della vista
  - `A` / `D`: sinistra / destra (strafe)
  - `Q` / `E`: alto / basso
  - `Frecce`: orbita attorno al punto focale
- Rendering con shader e depth test attivo.

---

## 2. Cosa è stato fatto per l'Adattamento su macOS

> **Nota fondamentale:** Nessun file sorgente `.cpp` o `.h` del progetto didattico è stato toccato o alterato.

Per permettere la compilazione e l'esecuzione nativa su macOS sono stati introdotti:

1. **Integrazione `dependencies_GL_GLFW/`**:
   - Header `glad/glad.h` e `KHR/khrplatform.h` necessari per la compilazione di `glad.c` e degli include nei sorgenti.
   - Libreria completa Dear ImGui con i backend ufficiali `imgui_impl_glfw.cpp` e `imgui_impl_opengl3.cpp`.
2. **Build System `CMakeLists.txt` per macOS**:
   - Standard C++17 e ricerca pacchetti Homebrew (`/opt/homebrew`: `glfw3`, `glm`, `OpenGL`).
   - Linking dei framework nativi di macOS necessari a GLFW: `-framework Cocoa`, `-framework IOKit`, `-framework CoreVideo`.
   - Post-build step: copia automatica della cartella `shaders/` e dell'eseguibile nella directory radice per garantire che i path relativi `shaders/objects.vert` e `shaders/objects.frag` vengano sempre trovati.
3. **Script di avvio e comodità**:
   - `run.sh`: script bash eseguibile che imposta l'ambiente Homebrew nel PATH, compila con CMake e lancia subito l'applicazione.
   - `Makefile`: comandi `make build` e `make run` per chi preferisce l'uso diretto di Make.
   - `.gitignore`: esclude cartelle di build, eseguibili binari e file di sistema macOS (`.DS_Store`).

---

## 3. Quali saranno le Prossime Modifiche (Roadmap dei 7 TODO)

L'esercitazione richiede il completamento di 7 funzionalità specifiche:

### 📌 TODO 1 — Impostazioni Generali della Scena
- **File**: `GuiLayer.cpp` → Metodo: `drawSettings()`
- **Obiettivo**: Aggiungere widget ImGui per modificare i membri privati:
  - Colore di sfondo: `ImGui::ColorEdit4("Sfondo", &background_.x)`
  - Wireframe: `ImGui::Checkbox("Wireframe", &wireframe_)`
  - Mostra ancore: `ImGui::Checkbox("Mostra ancore", &showAnchor_)`
  - Modalità navigazione: `ImGui::Checkbox("Navigazione (mouse)", &navigationMode_)`

### 📌 TODO 2 — Selezione della Primitiva e Aggiunta
- **File**: `GuiLayer.cpp` → Metodo: `drawAddObject()`
- **Obiettivo**:
  - Menu a tendina `ImGui::Combo` per scegliere tra: Cubo, Piano, Piramide, Sfera, Toro.
  - Pulsante `ImGui::Button("Aggiungi oggetto")` che invoca `addObject(type_)` solo al click.

### 📌 TODO 3 — Creazione di una Nuova Istanza
- **File**: `GuiLayer.cpp` → Metodo: `addObject(int type)`
- **Obiettivo**:
  - Validare il tipo di primitiva (0..4).
  - Generare un nome progressivo (es. "Cubo 1", "Cubo 2", ecc.).
  - Chiamare `scene_.addInstance(geometryIds_[type], name)`.
  - Assegnare posizione iniziale centrata/visibile, richiamare `updateModelMatrix()`.
  - Impostare `selectable = true` e selezionarla immediatamente (`selected_ = newIndex`).

### 📌 TODO 4 — Elenco e Selezione degli Oggetti
- **File**: `GuiLayer.cpp` → Metodo: `drawObjectList()`
- **Obiettivo**:
  - Iterare sulle istanze della scena (`scene_.instanceCount()`).
  - Mostrare solo le istanze con `selectable == true`.
  - Usare `ImGui::Selectable()` con `PushID`/`PopID`.
  - Evidenziare l'oggetto selezionato e aggiornare `selected_`.

### 📌 TODO 5 — Modifica dei Parametri dell'Istanza Selezionata
- **File**: `GuiLayer.cpp` → Metodo: `drawSelectedObject()`
- **Obiettivo**:
  - Se nessun oggetto è selezionato (`selected_ == -1`), mostrare messaggio di avviso.
  - Altrimenti mostrare controlli per:
    - Traslazione: `DragFloat3("Posizione", ...)`
    - Rotazione: `DragFloat3("Rotazione", ...)`
    - Scala: `DragFloat3("Scala", ...)`
    - Colore: scelta tra colore vertici o colore uniforme (`ColorEdit3`)
    - Visibilità: checkbox `visible`
    - Eliminazione: pulsante "Elimina" che invoca `scene_.removeInstance(selected_)` e resetta `selected_ = -1`.
  - Ricalcolare la matrice con `updateModelMatrix()` se TRS cambia.

### 📌 TODO 6 — Manipolazione Istanza da Tastiera
- **File**: `main.cpp` → Funzione: `handleInput()`
- **Obiettivo**:
  - Tasti `1`, `2`, `3`: impostare asse attivo X, Y o Z su `gui.setAxis()`.
  - Verificare se c'è un'istanza valida selezionata (`gui.selected() >= 0`).
  - `T`: trasla lungo l'asse selezionato (`position += axis * (3.0f * dt)`).
  - `R`: ruota attorno all'asse selezionato (`rotation += axis * glm::degrees(dt)`).
  - `G`: scala uniforme (`scale *= (1.0f + 0.5f * dt)`).
  - `Canc` / `Backspace`: elimina l'istanza una sola volta per pressione.
  - Aggiornare `gui.setOperation(...)` e chiamare `updateModelMatrix()`.

### 📌 TODO 7 — Orbita della Camera con Trascinamento Mouse
- **File**: `main.cpp` → Funzione: `handleInput()`
- **Obiettivo**:
  - Leggere coordinate cursore con `glfwGetCursorPos`.
  - Riconoscere il click sinistro e controllare se il drag parte sulla scena 3D (`!mouseOnGui`).
  - Se `navigationMode()` è attiva e il drag è valido:
    - Calcolare `dx = x - lastX` e `dy = lastY - y`.
    - Chiamare `camera.orbit(dx * 0.007f, dy * 0.007f)`.
  - Gestire correttamente il rilascio del tasto del mouse e aggiornare `lastX`/`lastY`.

---

## 4. Registro delle Modifiche Svolte (Changelog Studente)

*Aggiornare questa sezione man mano che si completano le attività, prima di richiedere un commit git.*

- [x] **Setup & Adattamento Mac**: Configurazione CMakeLists.txt, dipendenze Dear ImGui / GLAD / GLFW / GLM, script di lancio `run.sh` e Makefile.
- [x] **TODO 1 - Impostazioni scena**: Completati in `GuiLayer.cpp` i widget per sfondo (`ColorEdit4`), wireframe (`Checkbox`), mostra ancore (`Checkbox`) e modalità navigazione mouse (`Checkbox`).
- [x] **TODO 2 - Scelta primitiva**: Implementati in `GuiLayer.cpp::drawAddObject()` la combo box con l'array delle 5 primitive e il pulsante per aggiungere l'oggetto invocando `addObject(type_)`.
- [x] **TODO 3 - Creazione istanza**: Completato `GuiLayer.cpp::addObject()` con validazione tipo, conteggio progressivo per nome ("Cubo 1", ecc.), aggiunta istanza alla scena, setup TRS iniziale, `updateModelMatrix()` e auto-selezione.
- [x] **TODO 4 - Elenco e selezione**: Completato `GuiLayer.cpp::drawObjectList()` con iterazione sulle istanze, filtro per `selectable == true`, widget `ImGui::Selectable()` con ID univoco e gestione indice `selected_`. Nascosti i testi descrittivi temporanei dei TODO nella GUI.
- [x] **TODO 5 - Dettaglio oggetto e TRS**: Completato `GuiLayer.cpp::drawSelectedObject()` con controlli per visibilità (`visible`), colore (`ColorEdit3`), uso colore uniforme, slider interattivi TRS (`DragFloat3`) per posizione, rotazione e scala (con limite minimo a 0.01), aggiornamento `updateModelMatrix()` in tempo reale e pulsante "Elimina oggetto" con reset dell'indice `selected_ = -1`.
- [x] **TODO 6 - Trasformazioni tastiera (1/2/3, T/R/G, Canc)**: Implementato in `main.cpp::handleInput()` con selezione asse X/Y/Z (tasti 1/2/3), traslazione (tasto T), rotazione (tasto R) e scala uniforme (tasto G) con ricalcolo di `updateModelMatrix()` e cancellazione singola dell'oggetto (CANC / Backspace).
- [x] **TODO 7 - Drag mouse per orbita camera**: Completato in `main.cpp::handleInput()` con rilevamento posizione cursore GLFW, distinzione inizio drag fuori da ImGui (`!mouseOnGui`), orbita sferica fluida con sensibilità 0.007f quando `navigationMode()` è attiva, e reset al rilascio del tasto sinistro.
