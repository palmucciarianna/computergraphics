// ============================================================================
// CANNONE 2D - PROIETTILI, FARFALLE E CUORI IN MOVIMENTO
// Basato sul progetto dell'esercitazione 6. OpenGL 3.3 + GLFW + GLAD + GLM.
// Tutti gli oggetti sono Geometry nella stessa Scene.
// Animazioni: aggiornamento della Model Matrix, senza riscrivere i VBO.
// ============================================================================
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <random>
#include <vector>

#include "Shader.h"
#include "ShapeGenerator.h"
#include "Scene.h"
#include "Renderer.h"
#include "Input.h"
#include "Gui.h"
#include <../ImGui/imgui.h>

constexpr float WORLD_HALF = 100.0f;
constexpr float BARREL_LENGTH = 24.0f;
constexpr float BARREL_THICKNESS = 10.0f;
constexpr float PROJECTILE_RADIUS = 2.4f;
constexpr float BUTTERFLY_RADIUS = 7.0f; // approssimazione della sagoma
constexpr float HEART_RADIUS = 7.0f; // approssimazione della sagoma
constexpr int MAX_BUTTERFLIES = 15;
constexpr int MAX_HEARTS = 15;
constexpr int MAX_PROJECTILES = 30;
constexpr float FIRE_COOLDOWN = 0.18f;  //Tempo che deve trascorrere tra uno sparo e l'altro
const glm::vec2 BASE_POSITION(0.0f, -78.0f);

    struct Projectile
    {
        std::size_t instanceIndex = 0;
        glm::vec2 position{0.0f};
        glm::vec2 velocity{0.0f};
        bool active = false;
    };

    enum class TargetType { Butterfly, Heart };

    struct Target
    {
        std::size_t instanceIndex = 0;
        TargetType type = TargetType::Butterfly;
        glm::vec2 position{0.0f};
        glm::vec2 direction{1.0f, 0.0f};
        float directionTimer = 0.0f;
        bool alive = true; // false dopo un colpo: non ricompare cambiando gli slider
    };

    std::mt19937 randomEngine(2026); // seed fisso: esperimenti riproducibili

    float randomBetween(float lo, float hi)
    {
        std::uniform_real_distribution<float> dist(lo, hi);
        return dist(randomEngine);
    }

    glm::mat4 translate2D(const glm::vec2& position)
    {
        return glm::translate(glm::mat4(1.0f),
                              glm::vec3(position, 0.0f));
    }

    glm::mat4 rotateZ(float degrees)
    {
        return glm::rotate(glm::mat4(1.0f), glm::radians(degrees),
                           glm::vec3(0.0f, 0.0f, 1.0f));
    }

    glm::mat4 scale2D(float sx, float sy)
    {
        return glm::scale(glm::mat4(1.0f), glm::vec3(sx, sy, 1.0f));
    }

    MeshData makeLink(float length, float thickness, const glm::vec4& color)
    {
        const float h = thickness * 0.5f;
        MeshData mesh;
        mesh.primitiveType = GL_TRIANGLES;
        mesh.positions = {
            {0.0f,-h,0.0f}, {length,-h,0.0f}, {length,h,0.0f},
            {0.0f,-h,0.0f}, {length,h,0.0f}, {0.0f,h,0.0f}
        };
        mesh.colors.assign(mesh.positions.size(), color);
        return mesh;
    }

    // Restituisce anche i limiti ortografici, necessari per convertire il mouse.
    void updateOrthoProjection(GLFWwindow* window, Renderer& renderer,
                               float& halfWidth, float& halfHeight)
    {
        int w = 0, h = 0;
        glfwGetFramebufferSize(window, &w, &h);
        if (w <= 0 || h <= 0) return;
        const float aspect = static_cast<float>(w) / static_cast<float>(h);
        halfWidth = WORLD_HALF;
        halfHeight = WORLD_HALF;
        if (aspect >= 1.0f) halfWidth *= aspect;
        else halfHeight /= aspect;
        renderer.setProjectionMatrix(glm::ortho(
            -halfWidth, halfWidth, -halfHeight, halfHeight, -1.0f, 1.0f));
        glViewport(0, 0, w, h);
    }

    glm::vec2 cursorToWorld(GLFWwindow* window, float halfWidth, float halfHeight)
    {
        // Il cursore GLFW e' espresso in coordinate logiche di finestra.
        // Si usa glfwGetWindowSize (non la dimensione del framebuffer).
        double mouseX = 0.0, mouseY = 0.0;
        int w = 0, h = 0;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        glfwGetWindowSize(window, &w, &h);
        if (w <= 0 || h <= 0) return glm::vec2(0.0f);
        const float nx = 2.0f * static_cast<float>(mouseX) / w - 1.0f;
        const float ny = 1.0f - 2.0f * static_cast<float>(mouseY) / h;
        return glm::vec2(nx * halfWidth, ny * halfHeight);
    }

    // Il raggio e' un'approssimazione conservativa della sagoma visibile.
    float targetRadius(const Target& t)
    {
        return t.type == TargetType::Butterfly ? BUTTERFLY_RADIUS : HEART_RADIUS;
    }

    // Test tra due cerchi in coordinate mondo, con piccolo margine aggiuntivo.
    bool overlapping(
        const Target& a,
        const Target& b,
        float margin = 1.5f)
    {
        // Recupera la posizione dei due bersagli.
        const glm::vec2 positionA = a.position;
        const glm::vec2 positionB = b.position;

        // Calcola la distanza tra i centri.
        const glm::vec2 difference =
            positionA - positionB;

        const float distance =
            glm::length(difference);

        // Determina i raggi dei due bersagli.
        const float radiusA = targetRadius(a);
        const float radiusB = targetRadius(b);

        // Calcola la distanza minima richiesta,
        // aggiungendo un margine di sicurezza.
        const float minDistance =
            radiusA + radiusB + margin;

        // Se la distanza effettiva è minore
        // della distanza minima, i bersagli
        // sono troppo vicini.
        if (distance < minDistance)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    // Cerca una posizione che non si sovrapponga agli altri bersagli attivi.
    // La funzione viene usata sia all'avvio sia dopo un colpo.
    // In caso di alta densita' effettua piu' tentativi, poi usa una
    // ricerca su una griglia di posizioni candidate.
    void respawnTarget(Target& b, const std::vector<Target>& targets,
                       const Scene& scene)
    {
        const float radius = targetRadius(b);
        const float xmin = -88.0f + radius;
        const float xmax =  88.0f - radius;
        const float ymin =   0.0f + radius;
        const float ymax =  88.0f - radius;

        auto freePosition = [&](const glm::vec2& candidate) {
            Target probe = b;
            probe.position = candidate;
            for (const Target& other : targets) {
                if (&other == &b || other.instanceIndex == b.instanceIndex) continue;
                if (!other.alive || !scene.instance(other.instanceIndex).visible) continue;
                if (overlapping(probe, other)) return false;
            }
            return true;
        };

        bool found = false;
        for (int attempt = 0; attempt < 500; ++attempt) {
            glm::vec2 candidate(randomBetween(xmin, xmax), randomBetween(ymin, ymax));
            if (freePosition(candidate)) {
                b.position = candidate;
                found = true;
                break;
            }
        }
        if (!found) {
            // Ricerca deterministica di riserva, utile con molti bersagli.
            for (float y = ymin; y <= ymax && !found; y += 3.0f) {
                for (float x = xmin; x <= xmax; x += 3.0f) {
                    if (freePosition(glm::vec2(x, y))) {
                        b.position = glm::vec2(x, y);
                        found = true;
                        break;
                    }
                }
            }
        }
        if (!found) {
            // Caso limite: spazio insufficiente per inserire un altro cerchio.
            // Evitiamo di introdurre una sovrapposizione: segnaliamo il problema.
            std::cerr << "Impossibile trovare una posizione libera per un bersaglio.\n";
        }
        const float angle = randomBetween(0.0f, 2.0f * glm::pi<float>());
        b.direction = glm::vec2(std::cos(angle), std::sin(angle));
        b.directionTimer = randomBetween(0.7f, 2.0f);
    }

    // Collisioni fra tutti i bersagli visibili (farfalle e cuori).
    // Risolve la compenetrazione e riflette le componenti normali delle
    // direzioni: urto elastico semplificato fra oggetti di massa uguale.
    void resolveTargetCollisions(Scene& scene, std::vector<Target>& targets)
    {
        // TODO 5B - COLLISIONI TRA BERSAGLI (approfondimento)
        // Confrontare tutte le coppie i<j di bersagli alive e visibili.
        // Se la distanza e' minore della somma dei raggi, separarli
        // lungo la congiungente e aggiornare le direzioni di movimento.
        // Aggiornare poi le Model Matrix delle istanze spostate.
        (void)scene; (void)targets;
    }

    void updateTargets(Scene& scene, std::vector<Target>& targets,
                       int activeButterflies, int activeHearts, float speed, float dt)
    {
        // TODO 4 - MOVIMENTO DEI BERSAGLI
        // 1. Determinare quanti cuori e farfalle sono abilitati dalla GUI.
        // 2. Impostare instance.visible = enabled && target.alive.
        // 3. Per i soli bersagli visibili: aggiornare directionTimer,
        //    cambiare direzione a intervalli casuali, muovere con speed*dt,
        //    rimbalzare sui bordi e ricostruire la Model Matrix.
        // Per ora: al massimo 1 farfalla e 1 cuore fermi;
        // nel TODO 4 eliminare il limite std::min(..., 1).
        // Non riattivare i bersagli gia' colpiti.
        int butterfliesSeen = 0, heartsSeen = 0;
        for (Target& b : targets) {
            const bool enabled = b.type == TargetType::Butterfly
                ? butterfliesSeen++ < std::min(activeButterflies, 1)
                : heartsSeen++ < std::min(activeHearts, 1);
            scene.instance(b.instanceIndex).visible = enabled && b.alive;
        }
        (void)speed; (void)dt;
    }

    void fireProjectile(std::vector<Projectile>& projectiles, const glm::vec2& origin,
                        const glm::vec2& direction, float speed)
    {
        // TODO 2 - SPARO
        // Cercare il primo Projectile con active == false.
        // Impostare active=true, position=origin e velocity=direction*speed.
        // Interrompere la ricerca dopo l'attivazione di un proiettile.
        (void)projectiles; (void)origin; (void)direction; (void)speed;
    }

    void updateProjectiles(Scene& scene, std::vector<Projectile>& projectiles,
                           std::vector<Target>& targets,
                           float halfWidth, float halfHeight, float dt, int& score)
    {
        // TODO 3 - MOVIMENTO DEI PROIETTILI
        // Per ogni proiettile attivo: position += velocity * dt.
        // Disattivarlo quando esce dalla regione visibile.
        //
        // TODO 5A - COLLISIONE PROIETTILE/BERSAGLIO
        // Per ogni proiettile attivo confrontare il centro con ciascun
        // bersaglio vivo e visibile usando:
        // dot(p.position-b.position, p.position-b.position)
        // <= (PROJECTILE_RADIUS + targetRadius(b))^2.
        // Al colpo: p.active=false; b.alive=false;
        // scene.instance(b.instanceIndex).visible=false; ++score;
        // NON richiamare respawnTarget(): il bersaglio resta eliminato.
        //
        // Questa parte gia' funzionante mantiene nascosti gli slot inattivi.
        for (const Projectile& p : projectiles) {
            GeometryInstance& instance = scene.instance(p.instanceIndex);
            instance.visible = p.active;
            if (p.active) instance.modelMatrix = translate2D(p.position);
        }
        (void)targets; (void)halfWidth; (void)halfHeight;
        (void)dt; (void)score;
    }
 

// Crea un mirino RGBA di 32x32 pixel. L'hotspot e' il centro (16,16).
// Il disegno viene realizzato in memoria: non servono file immagine esterni.
GLFWcursor* createCrosshairCursor()
{
    constexpr int size = 32;
    unsigned char pixels[size * size * 4] = {};

    auto setPixel = [&](int x, int y, unsigned char r,
                        unsigned char g, unsigned char b) {
        if (x < 0 || x >= size || y < 0 || y >= size) return;
        const int i = (y * size + x) * 4;
        pixels[i] = r;
        pixels[i + 1] = g;
        pixels[i + 2] = b;
        pixels[i + 3] = 255;
    };

    // Quattro bracci con contorno scuro per il contrasto su ogni sfondo.
    for (int v = 3; v <= 29; ++v) {
        if (v >= 13 && v <= 19) continue; // piccolo spazio al centro
        for (int offset = -1; offset <= 1; ++offset) {
            const unsigned char c = offset == 0 ? 255 : 20;
            setPixel(16 + offset, v, c, c, c);
            setPixel(v, 16 + offset, c, c, c);
        }
    }
    // Punto centrale.
    setPixel(16, 16, 255, 210, 50);

    GLFWimage image{};
    image.width = size;
    image.height = size;
    image.pixels = pixels;
    return glfwCreateCursor(&image, 16, 16);
}

int main()
{
    if (!glfwInit()) {
        std::cerr << "Errore inizializzazione GLFW\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(1100, 850,
        "Cannone 2D - Farfalle e cuori", nullptr, nullptr);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Errore inizializzazione GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, Input::keyCallback);
    glfwSetCursorPosCallback(window, Input::cursorPositionCallback);
    glfwSetFramebufferSizeCallback(window, Input::framebufferSizeCallback);

    Gui gui;
    gui.initialize(window);
    // ImGui non cambia la forma del cursore: il mirino resta visibile.
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    GLFWcursor* crosshairCursor = createCrosshairCursor();
    if (crosshairCursor != nullptr)
        glfwSetCursor(window, crosshairCursor);
    Shader shader;
    if (!shader.load("shader/vertexshaderC.glsl", "shader/fragmentshaderC.glsl")) {
        std::cerr << "Errore caricamento shader: controllare working directory.\n";
        gui.shutdown();
        if (crosshairCursor != nullptr) {
            glfwSetCursor(window, nullptr);
            glfwDestroyCursor(crosshairCursor);
        }
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    Scene scene;
    Renderer renderer(shader);
    float halfWidth = 100.0f, halfHeight = 100.0f;
    updateOrthoProjection(window, renderer, halfWidth, halfHeight);

    // Cannone: canna locale lungo +X e base circolare fissa.
    const std::size_t barrelGeometry = scene.addGeometry(makeLink(
        BARREL_LENGTH, BARREL_THICKNESS,
        glm::vec4(0.23f, 0.67f, 0.95f, 1.0f)));
    const std::size_t BARREL = scene.addInstance(barrelGeometry);
    const MeshData baseMesh = ShapeGenerator::createCircle(0, 0, 11, 11, 48,
        glm::vec4(0.75f, 0.86f, 0.96f, 1),
        glm::vec4(0.25f, 0.42f, 0.67f, 1));
    const std::size_t baseGeometry = scene.addGeometry(baseMesh);
    const std::size_t BASE = scene.addInstance(baseGeometry);

    scene.instance(BASE).modelMatrix = glm::mat4(1.0);

    // Il cannone e' gia' nella Scene: BARREL e BASE sono due istanze.
    // Creiamo UNA mesh GPU per tutte le farfalle e UNA per tutti i cuori.
    const MeshData butterflyMesh = ShapeGenerator::createButterfly(0, 0, 0.1f, 0.1f, 120,
        glm::vec4(1, 1, 1, 1), glm::vec4(0.93f, 0.42f, 0.77f, 1));
    const MeshData heartMesh = ShapeGenerator::createHeart(0, 0, 0.05f, 0.05f, 80,
        glm::vec4(1.0f, 0.85f, 0.85f, 1.0f), glm::vec4(0.95f, 0.12f, 0.32f, 1.0f));
    const std::size_t butterflyGeometry = scene.addGeometry(butterflyMesh);
    const std::size_t heartGeometry = scene.addGeometry(heartMesh);

    std::vector<Target> targets;
    targets.reserve(MAX_BUTTERFLIES + MAX_HEARTS);
    for (int i = 0; i < MAX_BUTTERFLIES; ++i) {
        Target b;
        b.type = TargetType::Butterfly;
        b.instanceIndex = scene.addInstance(butterflyGeometry);
        targets.push_back(b);
        scene.instance(b.instanceIndex).visible = false;
    }
    for (int i = 0; i < MAX_HEARTS; ++i) {
        Target h;
        h.type = TargetType::Heart;
        h.instanceIndex = scene.addInstance(heartGeometry);
        targets.push_back(h);
        scene.instance(h.instanceIndex).visible = false;
    }

    // Posizionamento iniziale senza sovrapposizioni.
    // Solo gli oggetti gia' posizionati sono considerati dal test.
    for (Target& t : targets) {
        respawnTarget(t, targets, scene);
        scene.instance(t.instanceIndex).visible = true;
    }

    // Configurazione iniziale per la lezione: una farfalla e un cuore fermi.
    // Tutte le altre istanze restano disponibili per il TODO 4.
    int initializedButterflies = 0, initializedHearts = 0;
    for (Target& b : targets) {
        if (b.type == TargetType::Butterfly && initializedButterflies++ == 0)
            b.position = glm::vec2(-38.0f, 45.0f);
        if (b.type == TargetType::Heart && initializedHearts++ == 0)
            b.position = glm::vec2(38.0f, 45.0f);
        const float sc = b.type == TargetType::Butterfly ? 20.0f : 8.0f;
        scene.instance(b.instanceIndex).modelMatrix =
            translate2D(b.position) * scale2D(sc, sc);
    }

    const MeshData bulletMesh = ShapeGenerator::createCircle(0, 0,
        PROJECTILE_RADIUS, PROJECTILE_RADIUS, 24,
        glm::vec4(1, 1, 0.65f, 1), glm::vec4(1, 0.65f, 0.10f, 1));
    const std::size_t projectileGeometry = scene.addGeometry(bulletMesh);
    std::vector<Projectile> projectiles(MAX_PROJECTILES);
    for (Projectile& p : projectiles) {
        p.instanceIndex = scene.addInstance(projectileGeometry);
        scene.instance(p.instanceIndex).visible = false;
    }

    // Si lascia il controllo dell'input alla finestra, senza sparare dalla GUI.
    bool previousFire = false;
    float fireCooldown = 0.0f;
    double lastTime = glfwGetTime();
    int score = 0;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        const double now = glfwGetTime();
        const float dt = std::min(0.05f, std::max(0.0f, static_cast<float>(now - lastTime)));
        lastTime = now;
        updateOrthoProjection(window, renderer, halfWidth, halfHeight);

        gui.beginFrame();
        gui.draw();
        if (gui.resetRequested()) {
            score = 0;
            for (Target& b : targets) scene.instance(b.instanceIndex).visible = false;
            for (Target& b : targets) {
                b.alive = true; // il reset avvia una nuova partita
                respawnTarget(b, targets, scene);
                // updateTargets applichera' i limiti degli slider ImGui.
                scene.instance(b.instanceIndex).visible = true;
            }
            for (Projectile& p : projectiles) p.active = false;
            fireCooldown = 0.0f;
        }

        // TODO 1 - PUNTAMENTO DEL CANNONE
        // Convertire il mouse in coordinate del mondo con cursorToWorld().
        // Calcolare aim = target - BASE_POSITION e normalizzarlo.
        // Calcolare angleDeg = degrees(atan2(direction.y, direction.x)).
        // Impostare Model Matrix della canna: T(BASE_POSITION)*Rz(angleDeg).
        // Calcolare muzzle = BASE_POSITION + direction*BARREL_LENGTH.
        // Per ora la canna resta verticale e il proiettile partirebbe da +Y.
        
        scene.instance(BARREL).modelMatrix = glm::mat4(1.0);
        
         
        //Gestione sparo

        const ImGuiIO& io = ImGui::GetIO();
        // Controlla se il pulsante sinistro del mouse è premuto.
        const bool mousePressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

        // Controlla se la barra spaziatrice è premuta.
        const bool spacePressed = glfwGetKey(window, GLFW_KEY_SPACE)== GLFW_PRESS;

        // Verifica se il mouse può essere utilizzato dal gioco.
        bool fireWithMouse = false;

        if (mousePressed && !io.WantCaptureMouse)
        {
            fireWithMouse = true;
        }

        // Verifica se la tastiera può essere utilizzata dal gioco.
        bool fireWithKeyboard = false;

        if (spacePressed && !io.WantCaptureKeyboard)
        {
            fireWithKeyboard = true;
        }

        // Il giocatore può sparare con il mouse oppure con la tastiera.
        const bool firePressed =fireWithMouse || fireWithKeyboard;
        // Riduce il tempo di attesa in base al tempo trascorso.
        fireCooldown = fireCooldown - dt;

        // Impedisce che il tempo di attesa diventi negativo.
        if (fireCooldown < 0.0f)
        {
            fireCooldown = 0.0f;
        }

        //if (firePressed && !previousFire && fireCooldown <= 0.0f) {
        //    fireProjectile(projectiles, muzzle, direction, gui.bulletSpeed());
        //    fireCooldown = FIRE_COOLDOWN;
        //}
        previousFire = firePressed;

        updateTargets(scene, targets, gui.butterflyCount(), gui.heartCount(),
                      gui.butterflySpeed(), dt);
        resolveTargetCollisions(scene, targets);
        updateProjectiles(scene, projectiles, targets,
                          halfWidth, halfHeight, dt, score);
        int active = 0;
        for (const Projectile& p : projectiles) if (p.active) ++active;
        gui.setScore(score);
        gui.setActiveProjectiles(active);

        renderer.beginFrame(gui.clearColor());
        renderer.setWireframe(gui.wireframe());
        renderer.drawScene(scene);
        renderer.setWireframe(false);
        gui.render();
        glfwSwapBuffers(window);
    }

    scene.clear();
    shader.destroy();
    gui.shutdown();
    if (crosshairCursor != nullptr) {
        glfwSetCursor(window, nullptr);
        glfwDestroyCursor(crosshairCursor);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
