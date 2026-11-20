#pragma once
#include <glm/glm.hpp>
class Camera {
public:
    glm::vec3 position{0,4,18};
    glm::vec3 target{0,0,0};
    glm::vec3 up{0,1,0};
    float fovDegrees=45.0f;
    glm::mat4 view() const;
    glm::mat4 projection(float aspect) const;
    void orbit(float horizontal,float vertical);
    void zoom(float amount);
    void moveLocal(float forward, float right, float vertical);
};
