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




