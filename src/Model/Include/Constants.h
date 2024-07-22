//
// Created by Niccolò Feresini on 16/07/24.
//

#ifndef CGPRJ2023_24_CONSTANTS_H
#define CGPRJ2023_24_CONSTANTS_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace constant{
    inline constexpr float RADIUS_COLLISION = 5.5f;
    inline constexpr unsigned int MAPDIM = 25;
    inline constexpr size_t MAXBULLETS = 400;
    inline constexpr unsigned int MAXFLOORSPOTLIGHTS = 40;
    extern const int MAX_FRAMES_IN_FLIGHT;
    extern const float MOVE_SPEED;
    extern const float ROT_SPEED;
    extern const float FOVy;
    extern const float nearPlane;
    extern const float farPlane;
    extern const float camHeight;
    extern const float camDistZ;
    extern const glm::mat4 Proj;
    extern const float Ar;
}


#endif //CGPRJ2023_24_CONSTANTS_H
