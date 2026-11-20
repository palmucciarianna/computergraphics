OPENGL CLASSI - MAC + VS CODE

Questa copia mantiene GLAD, ImGui, shader e classi del pacchetto originale.
GLFW viene collegato dalla versione macOS installata con Homebrew: glfw3.lib
nel pacchetto e' per Windows e non puo' essere collegata su Mac.
Aggiunto in main.cpp il flag forward-compatible per macOS.
CMake compila C/C++17, copia gli shader accanto all'eseguibile e usa Debug.
Non sono stati svolti gli esercizi del laboratorio.

1. Prerequisiti (Terminale del Mac):
   xcode-select --install
   brew install cmake glfw glm
   Se gli strumenti Xcode sono gia' installati, non reinstallarli.
   Homebrew deve essere gia' disponibile (brew --version).

2. Apri questa cartella in VS Code. Nel terminale integrato:
   cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
   cmake --build build -j 4
   cd build
   ./OpenGL_CLASSI
   Il lancio da build permette di trovare shader/.
   Dopo modifiche al codice, ricompila con cmake --build build dalla radice.

3. GLM da un pacchetto NuGet (alternativa a brew install glm):
   GLM e' una libreria utilizzabile tramite header. Il collegamento NuGet del
   progetto Visual Studio non viene importato da CMake.
   Scarica il pacchetto GLM indicato dalla docente, estrai il .nupkg come ZIP,
   e individua la directory che contiene glm/glm.hpp.
   Configura con il percorso reale, ad esempio:
   cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DGLM_INCLUDE_DIR="/percorso/include"
   cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DGLM_INCLUDE_DIR="$(brew --prefix glm)/include"

   Poi compila e lancia come sopra. Non e' un restore NuGet automatico:
   CMake usa direttamente gli header estratti. Non e' necessario installare
   anche GLM con Homebrew in questa modalita'.

4. DEBUG (promemoria richiesto):
   Installa CodeLLDB in VS Code. Prima compila in Debug come sopra.
   Metti un breakpoint sulla creazione del triangolo in main.cpp.
   In Esegui e debug scegli 'Debug OpenGL (CodeLLDB)' e avvia con F5.
   launch.json imposta build come directory corrente per trovare gli shader.
   Step Into entra nelle funzioni; Step Over esegue una riga; Continue riparte.
   Dopo ogni modifica ricompila prima di avviare il debug.

5. File aggiunti per funzionamento
   .vscode
      c_cpp_properties aggiunta: "compileCommands": "${workspaceFolder}/build/compile_commands.json"
   README_MAC.txt
   CMakeLists.txt
   spostati file .vstemplate e .vcxproj in cartella a parte

Validazione: controllati sorgenti, nomi dei file, configurazione e percorsi.
Questo ambiente non e' macOS e non dispone di CMake: compilazione e avvio
sul Mac non sono stati verificati qui.
