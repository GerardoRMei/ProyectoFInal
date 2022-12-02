#include "Turret.h"
#include "EasyMode.h"
#include "Enemy.h"
#include "cocos2d.h"

using namespace cocos2d;

bool Turret::init()
{
    
    //this->scheduleUpdate();
    return true;
}

Turret:: Turret() {

}


Turret:: ~Turret() {

}

int Turret::getRange() {
    return this->range;
}

void Turret::setRange(int newRange) {
    range = newRange;
}

void Turret::setCont(int c) {
    cont = c;
}

void Turret::setPosition(Point* p) {
    position.set(p->x,p->y);
}
bool Turret::EnemyinRango(Sprite* p1, Sprite* p2){
    if (p2 != nullptr) {
        Point pos1 = Point(p1->getPosition());
        Point pos2 = Point(p2->getPosition());
        float dist = pos1.getDistance(pos2);

        if (dist <= 50) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
    
}

bool Turret::EnemyFueraDeRango(Sprite* p1, Sprite* p2) {
    if(p2 != nullptr) {
        Point pos1 = Point(p1->getPosition());
        Point pos2 = Point(p2->getPosition());
        float dist = pos1.getDistance(pos2);

        if (dist > 50) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
    return false;
    }
}

bool Turret::Disparar(int con) {
    if (con == 0) {
        return true;
    }
    else {
        return false;
    }
    return true;
}