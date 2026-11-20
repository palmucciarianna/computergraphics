Posizionarsi in cartella che contiene CMakeLists.txt:

cmake -S . -B build_finestra -DCMAKE_PREFIX_PATH="$(brew --prefix)"
cmake --build build_finestra

Se compare [100% built]:
./build_finestra/OpenGLCorso