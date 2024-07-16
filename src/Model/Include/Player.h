//
// Created by Pudduh on 21/05/24.
//

#ifndef CGPRJ2023_24_PLAYER_H
#define CGPRJ2023_24_PLAYER_H

#include "glm/glm.hpp"
#include "vector"
#include "Bullet.h"
#include "Plane.h"


class Player : public Plane{

public:

    Player();

    void setPosition(const glm::vec3& newPosition);

    void setOrientation(const glm::vec3& newOrientation);

    std::shared_ptr<Bullet> shoot(const Position3D& inputPosition, float deltaT) override;

};


#endif //CGPRJ2023_24_PLAYER_H
