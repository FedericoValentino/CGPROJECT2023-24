//
// Created by feder34 on 21/05/24.
//

#ifndef CGPROJECT2023_24_ENEMY_H
#define CGPROJECT2023_24_ENEMY_H
#include <glm/glm.hpp>
#include <unordered_map>
#include "Bullet.h"
#include "Player.h"
#include "Position3D.h"
#include <mutex>
#include <thread>


class Enemy{
private:
    Position3D position3D_;
    std::unordered_map<unsigned int,Bullet> projectiles_;
    const float speed_;
    bool debug_;
    mutable std::mutex projectiles_mutex_;

    void moveProjectile(unsigned int projectileId, float distance,
                        float windowWidth, float windowHeight);

public:
    Enemy(const Position3D& position3D,const float speed, bool debug=false);
    [[nodiscard]] const Position3D& getPosition() const;
    void shoot(const float deltaT,float SCREEN_WIDTH, float SCREEN_HEIGHT); // return 1 whether he shoots the main character, 0 otherwise.
    virtual void move(const Player& player,const float deltaT);
    void eraseProjectile(unsigned  int id);

};
#endif //CGPROJECT2023_24_ENEMY_H
