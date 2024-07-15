//
// Created by Pudduh on 07/06/2024.
//

#include "../Include/Plane.h"
#include <glm/gtx/intersect.hpp>


/**
 * Plane constructor handles only PLANETYPE_UTILITY agnostic attributes of planes
 */
Plane::Plane() {
    this->bullets = new std::set<Bullet*>;
    this->avoidBuilding = false;
}

/**
 * Checks if a bullet has hit a plane, and decreases the plane HP by the bullet size
 * @param bullet the considered bullet
 */
void Plane::planeHit(Bullet bullet)
{
    if (hp>=1)
        hp -= bullet.getSize();
    if (hp <= 0)
        dead = true;
}

/**
 * Empties the bullet set of a plane from a bullet that needs to be despawned
 * @param own the bullet to despawn
 */
void Plane::clearBullet(Bullet* own)
{
    own->toClear = true;
    bullets->erase(own);
}

/**
 * Checks whether a point lies within a sphere of a certain radius. The radius is decided based on the PLANETYPE_UTILITY
 * of the desiredRadius that calls the function
 * @param center the center of the sphere
 * @param point the point we want to check
 * @param desiredRadius the PLANETYPE_UTILITY of the caller
 * @return true if the point lies within the sphere (or on its surface)
 */
bool Plane::checkDistance3D(glm::vec3 center, glm::vec3 point, PLANETYPE_UTILITY desiredRadius) {
    float radius = 0.0f;
    switch (desiredRadius) {
        case(ENEMY):
            radius = 20.0f;
            break;
        case(BOSS):
            radius = 30.0f;
            break;
        case PLAYER:
            radius = 2.0f;
            break;
        case COLLISION_SKY:
            radius = 10.0f;
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
    if(!avoidBuilding)
        changeDirection(point, deltaT);
    else
    {
        evasive(deltaT);
        evasionTimer += deltaT;
    }
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
    float y =  glm::sin(position.rotation.x) * translationSpeed * deltaT;
    float z =  glm::cos(position.rotation.y) * translationSpeed * deltaT;
    glm::mat4 T = glm::translate(glm::mat4(1), glm::vec3(x, -y, z));
    position.origin= T * glm::vec4(position.origin,1.0f);
}

void Plane::roll(int direction, float deltaT) {
    if(position.rotation.z + rotationSpeed/2.0f *  deltaT <= M_PI/4 && position.rotation.z + rotationSpeed/2.0f *  deltaT >= -M_PI/4)
        position.rotation.z += rotationSpeed/2.0f * direction * deltaT; // roll
    if (position.rotation.z < -M_PI/4)
    {
        position.rotation.z = -M_PI/4;
    }
    else if (position.rotation.z > M_PI/4)
    {
        position.rotation.z = M_PI/4;
    }
}

void Plane::climb(int direction, float climbRate, float deltaT)
{
    if(position.rotation.x + (rotationSpeed/climbRate) * direction * deltaT <= M_PI/4 && position.rotation.x + (rotationSpeed/climbRate) * direction * deltaT >= -M_PI/4)
        position.rotation.x -= (rotationSpeed/climbRate) * direction * deltaT; //pitch
    if (position.rotation.x < -M_PI/4)
    {
        position.rotation.x = -M_PI/4;
    }
    else if (position.rotation.x > M_PI/4)
    {
        position.rotation.x = M_PI/4;
    }
}

void Plane::evasive(float deltaT)
{
    //neuter roll
    if(position.rotation.z < 0.0f)
        position.rotation.z += rotationSpeed/2.0f * deltaT;
    else if(position.rotation.z > 0.0f)
        position.rotation.z -= rotationSpeed/2.0f * deltaT;

    //pitch up or down
    if(position.origin.y < 12.0f)
        climb(1, 2.0f, deltaT);

    if(position.origin.y > 12.0f )
        climb(-1, 2.0f, deltaT);

    if(evasionTimer >= 2.0f)
    {
        avoidBuilding = false;
        evasionTimer = 0.0f;
    }


    //printf("Height while avoiding %f\n", position.origin.y);
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
    float dir_y = glm::sin(position.rotation.x);

    glm::vec3 direction = glm::normalize(glm::vec3(dir_x, dir_y, dir_z));
    float distanceToIntersect;

    bool intersecting = false;

    if(glm::intersectRaySphere(position.origin, direction, inputPosition.origin, 10.0f, distanceToIntersect))
    {
        intersecting = true;
        if(position.rotation.z < 0.0f)
            position.rotation.z += rotationSpeed/2.0f * deltaT;
        else if(position.rotation.z > 0.0f)
            position.rotation.z -= rotationSpeed/2.0f * deltaT;
    }
    else if ((pos_x - target_x) * dir_z > (pos_z - target_z) * dir_x)
        roll(1, deltaT);
    else if((pos_x - target_x) * dir_z < (pos_z - target_z) * dir_x)
        roll(-1, deltaT);


    if(!intersecting) {
        if (position.rotation.z < 0.0f)
            position.rotation.y += rotationSpeed * deltaT;
        else
            position.rotation.y -= rotationSpeed * deltaT;

        position.rotation.y = fmod(position.rotation.y, 2.0f * M_PI);
    }

    float distanceFromPlayerHeight = position.origin.y - 8.40f;

    if(distanceFromPlayerHeight > 3.0f && position.rotation.x > -M_PI/6)
        climb(-1, 1.0f, deltaT);
    else if(distanceFromPlayerHeight < -3.0f && position.rotation.x < M_PI/6)
        climb(1, 1.0f, deltaT);
    else if(abs(distanceFromPlayerHeight) <= 0.05f)
        position.rotation.x = 0.0f;

    //printf("Height not avoiding %f\n", position.origin.y);
}

void Plane::timePasses(const float deltaT) {
    elapsedTime += deltaT;
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


PLANETYPE_UTILITY Plane::getType() {
    return type;
}

float Plane::getTranslationSpeed() const {
    return translationSpeed;
}

float Plane::getRotationSpeed() const {
    return rotationSpeed;
}

void Plane::setAvoidBuilding(bool avoiding)
{
    avoidBuilding = avoiding;
}














