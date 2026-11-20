#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>
glm::mat4 Camera::view() const {
    return glm::lookAt(position,target,up);}
glm::mat4 Camera::projection(float aspect) const {
    return glm::perspective(glm::radians(fovDegrees),aspect,0.1f,200.0f);}
void Camera::orbit(float horizontal,float vertical) {
    glm::vec3 offset=position-target;
    float radius=glm::length(offset);
    float theta=std::atan2(offset.z,offset.x)+horizontal;
    float phi=std::asin(offset.y/radius)+vertical;
    phi=std::clamp(phi,-1.45f,1.45f);
    position=target+radius*glm::vec3(std::cos(phi)*std::cos(theta),std::sin(phi),std::cos(phi)*std::sin(theta));
}
void Camera::zoom(float amount) {
    glm::vec3 offset=position-target;
    float distance=std::clamp(glm::length(offset)+amount,3.0f,80.0f);
    position=target+glm::normalize(offset)*distance;
}
void Camera::moveLocal(float forward, float right, float vertical) {
    glm::vec3 front=glm::normalize(target-position);
    glm::vec3 side=glm::normalize(glm::cross(front,up));
    glm::vec3 movement=front*forward+side*right+up*vertical;
    position+=movement;
    target+=movement;
}
