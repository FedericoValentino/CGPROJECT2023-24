//
// Created by Niccolò Feresini on 16/07/24.
//
#include "../Include/Constants.h"
#include "glm/gtc/matrix_transform.hpp"
namespace constant{
    const int MAX_FRAMES_IN_FLIGHT = 2;
    const float MOVE_SPEED = 5.0f;
    const float ROT_SPEED = glm::radians(120.0f);
    const float FOVy = glm::radians(105.0f);
    const float nearPlane = 0.1f;
    const float farPlane = 100.f;
    const float camHeight = 20.0f;
    const float camDistZ = -10.0f;
    const float Ar = 5.0f / 3.0f;
    const glm::mat4 Proj = glm::scale(glm::mat4(1),glm::vec3(1,-1,1)) *
            glm::frustum(-Ar*nearPlane*std::tan(FOVy/2),Ar*nearPlane*std::tan(FOVy/2),-constant::nearPlane*std::tan(FOVy/2),nearPlane*std::tan(FOVy/2),nearPlane,farPlane);

}
