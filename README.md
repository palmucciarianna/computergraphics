# Computer Graphics Hub — University Portal 🎓💻

> Portale accademico e hub didattico per la materia **Computer Graphics** (A.A. 2026/2027), progettato per consultare slide e documenti, eseguire laboratori interattivi nel browser, esplorare i sorgenti originali C++/OpenGL e monitorare i progetti d'esame.

🌐 **Sito Live su GitHub Pages**: [https://palmucciarianna.github.io/computergraphics/](https://palmucciarianna.github.io/computergraphics/)

---

## 🏛️ Architettura & Struttura delle Cartelle

Il portale è stato progettato rispettando un vincolo fondamentale: **la struttura originale del repository è conservata intatta al 100%**. Nessun file o cartella preesistente è stato spostato o modificato.

L'intera applicazione web vive isolata nella cartella `portal/`:

```
computergraphics/
├── intro/                       # Slide introduttive e Geometria per CG (2026/2027)
├── modellazione geometrica/     # Curve di Hermite, Bézier e spline
├── pipeline rendering grafico/  # Sottosistemi geometrico, raster e antialiasing
├── progetti/                    # Specifiche dei progetti d'esame e domande parziale
├── OpenGL+GLSL/                 # Codice sorgente originale dei laboratori C++
│   ├── OpenGL - Parte I/        # Setup finestra GLFW, caricatore GLAD e CMake
│   └── lab/                     # Esercitazioni 2D e 3D svolte a lezione
├── computer graphics 25:26/     # Archivio storico A.A. precedente
├── old-github/                  # Note di Computer Vision e repository extra
│
├── index.html                   # Reindirizzamento automatico a portal/index.html
├── .github/workflows/deploy.yml # Workflow GitHub Actions per deploy su GitHub Pages
└── portal/                      # TUTTO IL PORTALE WEB È QUI DENTRO
    ├── index.html               # Single Page Application del portale
    ├── assets/
    │   ├── css/style.css        # Design System moderno (Dark/Light mode, Responsive)
    │   └── js/app.js            # Controller, router client-side, ricerca globale, PDF viewer
    ├── data/
    │   ├── materials.json       # Catalogo di 80+ documenti didattici in PDF
    │   ├── labs.json            # Schede dei laboratori desktop e web
    │   └── projects.json        # Catalogo configurabile dei progetti GitHub
    ├── labs-web/                # Versioni web interattive eseguibili nel browser
    │   ├── 00-setup/            # Setup finestra GLFW e ciclo di rendering
    │   ├── 01-triangle/         # Hello Triangle e editor interattivo shader GLSL
    │   ├── 02-transforms-2d/    # Sistemi di coordinate e griglia animata (cuori/farfalle)
    │   ├── 04-mountain-quest/   # Game engine 2D con fisica cinematica e collisioni AABB
    │   ├── 05-hermite-curve/    # Modellatore interattivo di curve di Hermite con tangenti
    │   ├── 06-cubes-3d/         # Due cubi 3D, orbita circolare, telecamera e Z-Buffer
    │   └── 07-interactive-3d/   # Scena 3D con illuminazione e shading di Phong
    └── playground/              # Laboratorio per esperimenti grafici (Bézier, de Casteljau, matrici)
```

---

## 🚀 Avvio Locale

Per avviare il portale sul tuo computer locale non servono bundler o dipendenze esterne:

```bash
# Apri il terminale nella cartella computergraphics ed esegui un server HTTP statico:
python3 -m http.server 8000
```

Poi apri nel browser:
👉 [http://localhost:8000](http://localhost:8000)

Il reindirizzamento ti porterà automaticamente all'hub principale `portal/index.html`.

---

## 🖥️ Modalità Laboratori: Desktop vs Web

### 1. Modalità Desktop (Originale C++/OpenGL)
Mantiene i laboratori nella loro forma nativa su macOS e Windows. Ciascuna scheda nel portale fornisce:
- Percorso della cartella originale del repository
- Elenco dei file sorgente (`main.cpp`, `ShaderMaker.cpp`, `CMakeLists.txt`, shader `.glsl`)
- Istruzioni di compilazione ed esecuzione per Mac tramite CMake
- Accesso diretto alla dispensa teorica in PDF

### 2. Modalità Web (Interattiva nel Browser)
Implementazioni native scritte in HTML5 Canvas, WebGL e GLSL per interagire con i concetti grafici senza necessità di compilazione locale:
- **Web Lab 00**: Simulazione del ciclo GLFW, SwapBuffers e clear color.
- **Web Lab 01**: Hello Triangle con editor real-time per vertex e fragment shader GLSL.
- **Web Lab 02**: Spazi di coordinate (World, NDC, Screen) e griglia animata di istanze con cuori e farfalle.
- **Web Lab 04**: Motore fisico di Mountain Quest con gravità, salto parabolico e collision detection AABB.
- **Web Lab 05**: Modellatore CAD di curve di Hermite 2D con punti di controllo e vettori tangenti trascinabili col mouse, e pulsante per esportare le coordinate in array C++.
- **Web Lab 06**: Cubi 3D in rotazione con telecamera orbitale (pitch/yaw/zoom), switch Prospettica/Ortografica e toggle del Depth Buffer (Z-Buffer).
- **Web Lab 07**: Scena 3D con sfera/toro ed equazione di illuminazione locale di Phong ($I = k_a I_a + k_d (N \cdot L) I_d + k_s (R \cdot V)^\alpha I_s$).

---

## 📚 Come Aggiungere Nuovi Materiali o Laboratori

I contenuti del portale sono gestiti in modo dinamico e dichiarativo attraverso i file JSON in `portal/data/`:

### Aggiungere una nuova Dispensa / Slide:
1. Posiziona il file PDF nella cartella desiderata (es. `intro/`, `modellazione geometrica/`, `pipeline rendering grafico/`).
2. Aggiungi un nuovo oggetto in `portal/data/materials.json`:
   ```json
   {
     "id": "intro_nuova_lezione_pdf",
     "title": "Titolo della Lezione",
     "filename": "Nuova_Lezione.pdf",
     "relPath": "intro/Nuova_Lezione.pdf",
     "category": "Introduzione & Sistemi Grafici",
     "tags": ["Argomento1", "Argomento2"],
     "size": "2.4 MB",
     "description": "Descrizione sintetica del contenuto della slide.",
     "academicYear": "2026/2027"
   }
   ```

### Aggiungere un nuovo Progetto GitHub Esterno:
Modifica `portal/data/projects.json` specificando il nome, la descrizione, il link al repository GitHub, l'eventuale live demo e i tag tecnologici.

---

## ⚙️ Configurazione Pubblicazione GitHub Pages

Il repository è configurato per il deploy automatico tramite GitHub Actions (`.github/workflows/deploy.yml`).

Quando sei pronta per pubblicare sul repository `palmucciarianna/computergraphics`:
```bash
# 1. Verifica che il remote utilizzi l'alias SSH dedicato:
git remote set-url origin git@github-palmucciarianna:palmucciarianna/computergraphics.git

# 2. Invia i commit a GitHub:
git push -u origin main
```

Nelle impostazioni del repository su GitHub (`Settings` → `Pages`), seleziona come **Source**: **GitHub Actions**. Il sito verrà pubblicato automaticamente all'indirizzo `https://palmucciarianna.github.io/computergraphics/`.
