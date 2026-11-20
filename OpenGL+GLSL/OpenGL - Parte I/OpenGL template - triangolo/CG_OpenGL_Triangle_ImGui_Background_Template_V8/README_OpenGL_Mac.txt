OPENGL DEL CORSO SU MAC M2 - PROMEMORIA

PRIMA
Il template della prof era configurato per Visual Studio su Windows:
progetto .vcxproj, template .vstemplate, GLFW compilata come glfw3.lib,
alcuni include con percorsi Windows. Non era direttamente compilabile su macOS.

DOPO
Gli stessi sorgenti dell'esercitazione sono compilati su macOS con Clang,
CMake e VS Code. Restano GLAD, ImGui, shader, geometria e render loop della
prof. La finestra con triangolo e GUI e' stata avviata sul Mac.

MODIFICHE E MOTIVI
1. main.cpp e Gui.cpp: sostituiti gli include ../ImGui/... (anche con backslash)
   con "imgui.h", "imgui_impl_glfw.h", "imgui_impl_opengl3.h".
   CMake specifica la cartella dependencies/ImGui in cui cercarli.
   I vecchi include vanno eliminati, non affiancati ai nuovi.

2. ShaderMaker.cpp: eliminato #include <GL/gl.h>.
   Quel percorso non e' quello degli header OpenGL di macOS; GLAD fornisce
   gia' le dichiarazioni necessarie attraverso <glad/glad.h>.
   Aggiunto <cstdio> per dichiarare fopen, fread, fseek e le altre funzioni file.
   Eliminato #pragma warning(disable:4996), specifico del compilatore Microsoft.

3. main.cpp, DENTRO main(), prima di glfwCreateWindow:
   dopo GLFW_OPENGL_PROFILE sono state aggiunte queste righe:

   #ifdef __APPLE__
       glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
   #endif

   Specificano un contesto forward-compatible per macOS. Il controllo
   __APPLE__ limita questa impostazione alle piattaforme Apple.
   Con GLFW 3.4 l'hint e' ignorato perche' il comportamento e' gia' automatico;
   resta utile con versioni precedenti. OpenGL richiesto resta 3.3 Core.
   Queste chiamate NON vanno in ShaderMaker.cpp o fuori da una funzione:
   sono istruzioni da eseguire durante l'inizializzazione della finestra.

4. Aggiunto CMakeLists.txt al posto della configurazione di build Visual Studio.
   Elenca tutti i sorgenti, compila glad.c come C e gli altri come C++,
   aggiunge gli include, collega GLFW per macOS e OpenGL, e copia gli shader
   nella cartella build. GLFW_INCLUDE_NONE impedisce a GLFW di includere
   automaticamente header OpenGL che potrebbero interferire con GLAD.

5. Installata GLFW per macOS tramite Homebrew: glfw3.lib e' una libreria
   Windows e non si puo' collegare a un eseguibile macOS.
   Si usano glad.c, header GLAD e ImGui dello stesso ZIP della prof;
   non serve mischiarli con le altre copie delle dipendenze.

STRUMENTI
- Command Line Tools Apple: xcode-select --install
- Homebrew: brew install cmake glfw
- VS Code: estensioni Microsoft C/C++ e CMake Tools.
  CMake Tools non installa da sola il programma CMake.
  Se non lo trova: which cmake e inserire quel percorso in cmake.cmakePath.

CONTROLLO CMAKE (nel Terminale del Mac o nel terminale di VS Code)
cmake --version
Mostra la versione installata, ad esempio "cmake version ...".

which cmake
Mostra il percorso dell'eseguibile; su Mac M2 con Homebrew normalmente e':
/opt/homebrew/bin/cmake
Se VS Code non lo trova, inserire il percorso restituito in cmake.cmakePath
(Impostazioni: Cmd + ,), poi Developer: Reload Window (Cmd + Shift + P).
Se cmake --version dice command not found, installare CMake con Homebrew
oppure verificare il PATH.

PRIMA CONFIGURAZIONE (dalla cartella con CMakeLists.txt)
cmake -S . -B build -DCMAKE_PREFIX_PATH="$(brew --prefix)"
cmake --build build
cd build
./OpenGLCorso

BUILD E AVVIO DALLA CARTELLA PRINCIPALE DEL PROGETTO
cmake --build build
Compila e collega i sorgenti, creando/aggiornando l'eseguibile.
Se termina senza errori:
cd build
./OpenGLCorso
Apre la finestra OpenGL. Per tornare alla cartella principale: cd ..

DOPO OGNI MODIFICA (se il terminale e' gia' in build)
1. Chiudere il programma e salvare i sorgenti.
2. cmake --build .
3. Solo se la compilazione riesce: ./OpenGLCorso

Avviare da build: il codice cerca gli shader nella cartella corrente.
Non usare Run Code sul solo main.cpp: servono tutti i sorgenti e le librerie.
Se si aggiungono nuovi .cpp, aggiungerli anche a CMakeLists.txt.
Warning non significa errore: verificare che la build termini con successo.
