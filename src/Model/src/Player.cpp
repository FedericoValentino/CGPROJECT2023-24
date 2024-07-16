//
// Created by Pudduh on 21/05/24.
//


#include <cstdio>
#include <algorithm>
#include "../Include/Player.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

Player::Player() {
    position.origin = glm::vec4(0.0f, 8.40f, 0.0f, 1.0f);
    position.rotation = glm::vec4 (0.0f, 0.0f, 0.0f, 1.0f);
    translationSpeed = 2;
    rotationSpeed = glm::radians(120.0f);
    hp = 100.0f;
    type = PLAYER;
    dead = false;
}

/**
 * Modifies the Player's position according to the new position
 * @param newPosition the new origin position
 */
void Player::setPosition(const glm::vec3& newPosition) {
    position.origin = newPosition;
}

/**
 * Modifies the Player's orientation according to the new direction
 * @param newDirection the new direction
 */
void Player::setOrientation(const glm::vec3& newOrientation) {
    position.rotation = newOrientation;
}

/**
 * Adds the projectile to be shot in the Player's bullet set
 * @param inputPosition not used
 * @param deltaT time
 */
std::shared_ptr<Bullet> Player::shoot(const Position3D& inputPosition, const float deltaT) {
    std::shared_ptr<Bullet> toShoot = nullptr;
    if(elapsedTime > 1 || bullets->empty())
    {
        toShoot = std::make_shared<Bullet>(position, PLAYER, false);
        bullets->insert(toShoot);
        elapsedTime = 0;
    }
    return toShoot;
}





