#include "Turret2.h"
#include "EasyMode.h"
#include "Enemy.h"
#include "cocos2d.h"

using namespace cocos2d;

bool Turret2::init()
{

    //this->scheduleUpdate();
    return true;
}

Turret2::Turret2() {

}


Turret2:: ~Turret2() {

}

int Turret2::getRange2() {
    return this->range2;
}

void Turret2::setRange2(int newRange) {
    range2 = newRange;
}

void Turret2::setPosition(Point* p) {
    position2.set(p->x, p->y);
}
bool Turret2::EnemyinRango2(Sprite* p1, Sprite* p2) {
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

bool Turret2::EnemyFueraDeRango2(Sprite* p1, Sprite* p2) {
    if (p2 != nullptr) {
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

bool Turret2::Disparar2(int con) {
    //this->cont -= con;
    if (con == 0) {
        return true;
    }
    else {
        return false;
    }
    return true;
}