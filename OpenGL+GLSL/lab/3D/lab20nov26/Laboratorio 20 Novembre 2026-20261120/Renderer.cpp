#include "Renderer.h"
#include <glad/glad.h>

void Renderer::draw(
    const Scene& scene,
    const Camera& camera,
    float aspect,
    bool wireframe,
    bool showAnchor,
    int selected
) const {

    // ==================================================
    // 1. ATTIVAZIONE DELLO SHADER
    // ==================================================

    // Attiva il programma shader che utilizzeremo
    // per disegnare tutti gli oggetti della scena.
    shader_.use();


    // ==================================================
    // 2. IMPOSTAZIONE DELLA TELECAMERA
    // ==================================================

    // La View Matrix trasforma le coordinate
    // dal sistema del mondo al sistema della camera.
    shader_.setMatrix("uView", camera.view());

    // La Projection Matrix applica la prospettiva.
    shader_.setMatrix("uProjection",camera.projection(aspect));


    // ==================================================
    // 3. MODALITA' DI VISUALIZZAZIONE
    // ==================================================

    // Di default disegniamo triangoli pieni.
    GLenum mode = GL_FILL;

    // Se l'utente ha attivato il wireframe,
    // disegniamo soltanto i bordi dei triangoli.
    if (wireframe) {
        mode = GL_LINE;
    }

    glPolygonMode(GL_FRONT_AND_BACK, mode);


    // ==================================================
    // 4. DISEGNO DEGLI OGGETTI DELLA SCENA
    // ==================================================

    // Visitiamo tutte le istanze presenti nella scena.
    for (std::size_t i = 0;
        i < scene.instances().size();
        ++i) {

        // Recuperiamo l'istanza corrente.
        const Instance& item = scene.instance(i);

        // Se l'oggetto non e' visibile,
        // passiamo all'istanza successiva.
        if (!item.visible) {
            continue;
        }

        // Impostiamo la Model Matrix dell'istanza.
        // La matrice determina posizione,
        // orientamento e scala dell'oggetto.
        shader_.setMatrix(
            "uModel",
            item.modelMatrix
        );

        // Passiamo allo shader il colore dell'istanza.
        shader_.setVec3("uObjectColor",item.color);

        // Decidiamo se utilizzare il colore uniforme
        // oppure i colori originali dei vertici.
        int useUniformColor = 0;

        if (item.useUniformColor) {
            useUniformColor = 1;
        }

        shader_.setInt("uUseUniformColor", useUniformColor);

        // Recuperiamo la geometria associata
        // all'istanza e la disegniamo.
        scene.geometry(item.geometryIndex).draw();
    }


    // ==================================================
    // 5. RIPRISTINO DELLA MODALITA' DI DISEGNO
    // ==================================================

    // Ripristiniamo i triangoli pieni,
    // anche se era stato attivato il wireframe.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    // ==================================================
    // 6. DISEGNO DELL'ANCHOR
    // ==================================================
    // Mostra il punto soltanto se richiesto e se la selezione e' valida.
    if (!showAnchor || selected < 0 ||
        static_cast<std::size_t>(selected) >= scene.instances().size()) {
        return;
    }

    const Instance& item = scene.instance(static_cast<std::size_t>(selected));
    if (!item.visible) return;

    shader_.setMatrix("uModel", item.modelMatrix);
    shader_.setVec3("uObjectColor", glm::vec3(0.0f,0.0f,1.0));
    shader_.setInt("uUseUniformColor", 1);
    scene.geometry(item.geometryIndex).drawAnchor();
    
}
