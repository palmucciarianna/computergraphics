#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>

// La camera iniziale e' fissa: view() e projection() sono gia' funzionanti.
glm::mat4 Camera::view() const {
    return glm::lookAt(position, target, up);
}

glm::mat4 Camera::projection(float aspect) const {
    return glm::perspective(glm::radians(fovDegrees), aspect, 0.1f, 200.0f);
}

void Camera::orbit(float horizontal, float vertical) {
    // TODO 3: implementare l'orbita della camera attorno a target.
    // Suggerimento:
    // 1. offset = position - target
    // 2. ricavare radius, theta e phi
    // 3. aggiornare theta e phi con horizontal e vertical
    // 4. limitare phi per evitare i poli
    // 5. ricostruire position usando le coordinate sferiche
    (void)horizontal;
    (void)vertical;
}

void Camera::zoom(float amount) {
    // TODO 4 (facoltativo): modificare la distanza tra position e target.
    (void)amount;
}

void Camera::moveLocal(float forward, float right, float vertical) {
    // TODO 5: spostare la camera lungo i suoi assi locali.
    // Calcolare front, side e movement.
    // Aggiornare sia position sia target con lo stesso movement.
    (void)forward;
    (void)right;
    (void)vertical;
}
