//
// Created by Niccolò Feresini on 25/05/24.
//

#ifndef CGPRJ2023_24_POSITION3D_H
#define CGPRJ2023_24_POSITION3D_H
#include <glm/glm.hpp>

struct Position3D
{
    glm::vec3 origin;
    glm::vec3 orientation;
    Position3D(const glm::vec3 _origin,glm::vec3 _orientation):origin(_origin),orientation(_orientation){};
    Position3D(const Position3D& position):Position3D(position.origin,position.orientation){};
};
#endif //CGPRJ2023_24_POSITION3D_H
