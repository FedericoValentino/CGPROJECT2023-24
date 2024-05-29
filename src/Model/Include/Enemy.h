//
// Created by feder34 on 21/05/24.
//

#ifndef CGPROJECT2023_24_ENEMY_H
#define CGPROJECT2023_24_ENEMY_H
#include <glm/glm.hpp>
#include <unordered_map>
#include "Projectile.h"
#include "Player.h"
#include "Position3D.h"


class Enemy{
protected:
    Position3D position3D_;
    std::unordered_map<unsigned int,Projectile> projectiles_;
    const float speed_;
    bool debug_;

public:
    Enemy(const Position3D& position3D,const float speed, bool debug=false);
    [[nodiscard]] const Position3D& getPosition() const;
    int shoot(); // return 1 whether he shoots the main character, 0 otherwise.
    virtual void move(const Player& player,const float deltaT);
    void eraseProjectile(unsigned  int id);

};
#endif //CGPROJECT2023_24_ENEMY_H
