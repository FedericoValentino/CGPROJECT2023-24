//
// Created by feder34 on 21/05/24.
//

#include "../Include/Tiles.h"

extern const unsigned int MAPDIM;

Tiles::Tiles(int row, int col) {
    this->height = 0;
    // compute the coordinates of the tiles
    glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(2.8072f * (static_cast<float>(constant::MAPDIM)-1), 0.0f, 2.8072f * (static_cast<float>(constant::MAPDIM)-1)));
    T = glm::translate(T, glm::vec3(-5.6144f * (float)row, 0.0f, -5.6144f * (float)col));
    this->pos.origin = T * glm::vec4(0.0f,0.0f,0.0f,1.0f);
    this->pos.rotation = glm::vec3(0,0,0);
}

bool Tiles::checkCollision(float x,float z,float radius) const
{
    float dx = pos.origin.x - x;
    float dz = pos.origin.z - z;
    return std::sqrt(dx * dx + dz * dz) < radius;
}

