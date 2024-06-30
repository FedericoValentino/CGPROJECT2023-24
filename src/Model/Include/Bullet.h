//
// Created by feder34 on 21/05/24.
//

#ifndef CGPRJ2023_24_BULLET_H
#define CGPRJ2023_24_BULLET_H

#include "glm/glm.hpp"
#include "Position3D.h"
#include "PlaneType.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>


class Bullet {

private:
    unsigned int static count_;
    Position3D position3D_;
    float size_;
    float speed_;
    unsigned int id_;
    bool debug_;
    PLANETYPE owner;
public:
    bool toClear;
    Bullet(const Position3D& position3D, PLANETYPE type, bool debug=false);

    [[nodiscard]] const Position3D& getPosition3D() const ;
    [[nodiscard]] float getSize() const;
    [[nodiscard]] float getSpeed() const;
    [[nodiscard]] unsigned int getId() const;
    void move(const float deltaT); // move to direction_
    [[nodiscard]]bool isPositionInsideScreen(const float SCREEN_WIDTH,
                                const float SCREEN_HEIGHT) const;
    PLANETYPE getType();
};


#endif //CGPRJ2023_24_BULLET_H
