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
bool Plane::checkDistance3D(glm::vec4 center, glm::vec4 point, PLANETYPE plane) {
    float radius;
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
    float x =  glm::sin(glm::radians(position.orientation.y)) * speed * deltaT;
    float y =  0.0f;
    float z =  glm::cos(glm::radians(position.orientation.y)) * speed * deltaT;
    glm::mat4 T = glm::translate(glm::mat4(1), glm::vec3(x, y, z));
    position.origin= position.origin * T;
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
    glm::vec3 pointingDirection = glm::vec4(glm::sin(glm::radians(position.orientation.y)), 0.0f, glm::cos(glm::radians(position.orientation.y)), 1.0f);
    glm::vec3 cross  = glm::cross(pointingDirection, glm::vec3(inputPosition.origin));
    if(cross.y > 0)
        position.orientation.y -= speed * deltaT;
    else if (cross.y < 0)
        position.orientation.y += speed * deltaT;
    //TODO If cross==0
}


Position3D Plane::getPosition() const{
    return position;
}

float Plane::getSpeed(){
    return speed;
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





