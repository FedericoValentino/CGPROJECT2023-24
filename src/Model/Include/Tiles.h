#ifndef CGPRJ2023_24_TILES_H
#define CGPRJ2023_24_TILES_H
#include "Position3D.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

enum HEIGHT{
    FLOOR,
    HOUSE,
    SKYSCRAPER
};


struct Tiles {
    int height;
    Position3D pos;
    Tiles(int row, int col);
    bool checkCollision(float x,float z,float radius) const;
};


#endif //CGPRJ2023_24_TILES_H
