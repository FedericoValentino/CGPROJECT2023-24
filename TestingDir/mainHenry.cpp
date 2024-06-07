//
// Created by feder34 on 01/06/24.
//

#include "../src/Model/Include/Enemy.h"
#include "../src/Model/Include/Plane.h"
#include "../src/Model/Include/Position3D.h"


void testEnemyOrientation(){
    auto enemy = new Enemy({glm::vec4(2.0f, 0.0f, 2.0f, 1.0f), glm::vec4(0.0f, 45.0f, 0.0f, 1.0f)});
    Position3D playerPosition {glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec4(0.0f, 90.0f, 0.0f, 1.0f)};

    enemy->changeDirection(playerPosition, 1.0f);
    assert(enemy->getPosition().orientation.y < 45.0f);
}

int main (){

    testEnemyOrientation();
}