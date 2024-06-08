//
// Created by Pudduh on 07/06/2024.
//

#include "../Include/Plane.h"



Plane::Plane() {
    this->bullets = new std::set<Bullet*>;
}

void Plane::planeHit(Bullet bullet)
{
    if (bullet.getPosition3D().origin == position.origin && hp>1)
        hp -= bullet.getSize();
    else
        dead = true;
}

void Plane::clearBullet(Bullet* own)
{
    bullets->erase(own);
}


Position3D Plane::getPosition() {
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

void Plane::moveTowardsPoint(Position3D point, float deltaT)
{
    changeDirection(point, deltaT);
    changePosition(position, deltaT);
}

void Plane::changePosition(Position3D inputPosition, float deltaT)
{
    float x =  glm::sin(glm::radians(position.orientation.y)) * speed * deltaT;
    float y =  0.0f;
    float z =  glm::cos(glm::radians(position.orientation.y)) * speed * deltaT;
    glm::mat4 T = glm::translate(glm::mat4(1), glm::vec3(x, y, z));
    position.origin= position.origin * T;
}

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



