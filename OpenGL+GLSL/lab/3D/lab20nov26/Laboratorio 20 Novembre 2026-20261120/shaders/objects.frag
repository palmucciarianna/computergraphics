
#version 330 core

// Colore ricevuto dal vertex shader.
// Il valore viene interpolato tra i vertici
// durante la rasterizzazione.
in vec4 vertexColor;

// Colore finale prodotto dal fragment shader.
out vec4 FragColor;

// Colore uniforme associato all'istanza.
// Viene passato dal Renderer e può essere
// modificato attraverso la GUI.
uniform vec3 uObjectColor;

// Indica quale colore utilizzare:
// 0 = colore interpolato dei vertici
// 1 = colore uniforme dell'istanza
uniform int uUseUniformColor;

void main() {

    // Variabile che conterrà il colore RGB
    // da assegnare al frammento.
    vec3 rgb;

    // Sceglie la sorgente del colore.
    if (uUseUniformColor == 1) {

        // Utilizza il colore uniforme dell'istanza.
        rgb = uObjectColor;

    } else {

        // Utilizza il colore interpolato
        // dei vertici della geometria.
        rgb = vertexColor.rgb;
    }

    // Costruisce il colore finale RGBA.
    //
    // Le componenti RGB sono quelle appena scelte.
    // La componente alpha viene mantenuta
    // dal colore interpolato dei vertici.
    FragColor = vec4(rgb, vertexColor.a);
}

