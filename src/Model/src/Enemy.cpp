//
// Created by Pudduh && Niccolo on 21/05/24.
//

#include "../Include/Enemy.h"


Enemy::Enemy(const Position3D& position) {
    this->position = position;
    translationSpeed = 8;
    rotationSpeed = glm::radians(60.0f);
    type = ENEMY;
    hp = 1;
    dead = false;
    evasionTimer = 0.0;
}


/**
 * Enemy will shoot towards Player if he is sufficiently close and if his direction vector intersect a sphere centered
 * in the Player's position.
 * @param inputPosition the Player's position
 * @param deltaT time
 */
std::shared_ptr<Bullet> Enemy::shoot(const Position3D& inputPosition, const float deltaT)
{
    float distance;
    std::shared_ptr<Bullet> b = nullptr;
    glm::vec3 dir = glm::vec3(glm::sin(position.rotation.y), 0.0f, glm::cos(position.rotation.y));
    if(glm::intersectRaySphere(position.origin, dir, inputPosition.origin, pow(1.0f, 2.0f), distance)
        && checkDistance3D(inputPosition.origin, position.origin, ENEMY)
        && (elapsedTime > 2.0f || bullets->empty()))
    {
        b = std::make_shared<Bullet>(position,ENEMY,false);
        bullets->insert(b);
        elapsedTime = 0;
    }
    return b;
}

void Enemy::evasive(float deltaT)
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

void Enemy::climb(int direction, float climbRate, float deltaT)
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

void Enemy::moveTowardsPoint(const Position3D& point, float deltaT)
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
 * Changes direction of a point to then move it alongside the new direction. It does this through checking the result of
 * a cross product between:
 * the vector describing the direction of the point we want to move
 * the vector describing the position of the point we want to reach
 * @param inputPosition the position of the point we want to reach
 * @param deltaT time
 */
void Enemy::changeDirection(const Position3D& inputPosition, float deltaT)
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
