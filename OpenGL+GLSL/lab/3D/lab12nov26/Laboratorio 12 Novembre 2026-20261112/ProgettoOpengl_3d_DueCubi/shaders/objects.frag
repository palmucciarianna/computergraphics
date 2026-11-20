#version 330 core

// Colore ricevuto dal vertex shader.
// E' interpolato tra i vertici del triangolo.
in vec4 vertexColor;

// Colore finale del pixel (fragment).
out vec4 FragColor;

// Colore uniforme scelto per l'oggetto.
uniform vec3 uObjectColor;

// Indica quale modalità di colore utilizzare:
// 0 = colori dei vertici
// 1 = colore uniforme dell'oggetto
uniform int uUseUniformColor;

void main()
{
    vec3 finalColor;

    // Scegliamo il colore da utilizzare.
    if (uUseUniformColor == 1)
    {
        // Tutti i frammenti dell'oggetto
        // utilizzano lo stesso colore.
        finalColor = uObjectColor;
    }
    else
    {
        // Utilizziamo il colore interpolato
        // ricevuto dal vertex shader.
        finalColor = vertexColor.rgb;
    }

    // Assegniamo il colore finale al frammento.
    // Manteniamo il valore alpha ricevuto dal vertex shader.
    FragColor = vec4(finalColor, vertexColor.a);
}
