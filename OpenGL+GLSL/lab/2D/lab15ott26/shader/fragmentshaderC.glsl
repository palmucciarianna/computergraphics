// ============================================================================
// shader/fragmentshaderC.glsl - GUIDA DIDATTICA
// Fragment shader: determina il colore finale del frammento.
// ============================================================================

#version 330 core

in vec4 vertexColor;
out vec4 FragColor;

void main()
{
    FragColor = vertexColor;
}
