//
// Created by Niccolò Feresini on 25/05/24.
//

#ifndef CGPRJ2023_24_POSITION3D_H
#define CGPRJ2023_24_POSITION3D_H
#include <glm/glm.hpp>

struct Position3D
{
    //World Coordinates
    glm::vec3 origin;
    //Radians
    glm::vec3 rotation;
};
#endif //CGPRJ2023_24_POSITION3D_H
