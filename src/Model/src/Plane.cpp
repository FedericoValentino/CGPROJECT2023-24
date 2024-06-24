//
// Created by Pudduh on 07/06/2024.
//

#include "../Include/Plane.h"


/**
 * Plane constructor handles only PLANETYPE agnostic attributes of planes
 */
Plane::Plane() {
    this->bullets = new std::set<Bullet*>;
}

/**
 * Checks if a bullet has hit a plane, and decreases the plane HP by the bullet size
 * @param bullet the considered bullet
 */
void Plane::planeHit(Bullet bullet)
{
    if (bullet.getPosition3D().origin == position.origin && hp>1)
        hp -= bullet.getSize();
    else
        dead = true;
}

/**
 * Empties the bullet set of a plane from a bullet that needs to be despawned
 * @param own the bullet to despawn
 */
void Plane::clearBullet(Bullet* own)
{
    bullets->erase(own);
}

/**
 * Checks whether a point lies within a sphere of a certain radius. The radius is decided based on the PLANETYPE
 * of the plane that calls the function
 * @param center the center of the sphere
 * @param point the point we want to check
 * @param plane the PLANETYPE of the caller
 * @return true if the point lies within the sphere (or on its surface)
 */
bool Plane::checkDistance3D(glm::vec3 center, glm::vec3 point, PLANETYPE plane) {
    float radius = 0.0f;
    switch (plane) {
        case(ENEMY):
            radius = 5.0f;
            break;
        case(BOSS):
            radius = 10.0f;
            break;
        default:
            break;
    }
    return (pow((point.x - center.x), 2) +
            pow((point.y - center.y), 2) +
            pow((point.z - center.z), 2) <=
            pow(radius,2));
}

/**
 * Moves a point (a plane) towards another point, while adjusting its direction
 * @param point the point we want to move towards
 * @param deltaT time
 */
void Plane::moveTowardsPoint(Position3D point, float deltaT)
{
    changeDirection(point, deltaT);
    changePosition(position, deltaT);
}

/**
 * Changes position of a point to match it's current direction. Used in moveTowardsPoint
 * @param inputPosition --
 * @param deltaT time
 */
void Plane::changePosition(Position3D inputPosition, float deltaT)
{
    float x =  glm::sin(position.rotation.y) * translationSpeed * deltaT;
    float y =  0.0f;
    float z =  glm::cos(position.rotation.y) * translationSpeed * deltaT;
    glm::mat4 T = glm::translate(glm::mat4(1), glm::vec3(x, y, z));
    position.origin= T * glm::vec4(position.origin,1.0f);
}

/**
 * Changes direction of a point to then move it alongside the new direction. It does this through checking the result of
 * a cross product between:
 * the vector describing the direction of the point we want to move
 * the vector describing the position of the point we want to reach
 * @param inputPosition the position of the point we want to reach
 * @param deltaT time
 */
void Plane::changeDirection(Position3D inputPosition, float deltaT)
{
    float target_x = inputPosition.origin.x;
    float target_z = inputPosition.origin.z;

    float pos_x = position.origin.x;
    float pos_z = position.origin.z;

    float dir_x = glm::sin(position.rotation.y);
    float dir_z = glm::cos(position.rotation.y);

    if ((pos_x - target_x) * dir_z > (pos_z - target_z) * dir_x) {
        position.rotation.y -= rotationSpeed * deltaT;
    } else {
        position.rotation.y += rotationSpeed * deltaT;
    }

    position.rotation.y = fmod(position.rotation.y, 2.0f * M_PI);

}


Position3D Plane::getPosition() const{
    return position;
}

std::set<Bullet*> Plane::getBullets() {
    return *bullets;
}

bool Plane::getDead() {
    return dead;
}

int Plane::getHP() {
    return hp;
}


PLANETYPE Plane::getType() {
    return type;
}

float Plane::getTranslationSpeed() const {
    return translationSpeed;
}

float Plane::getRotationSpeed() const {
    return rotationSpeed;
}





