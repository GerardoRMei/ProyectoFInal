#pragma once
#include "cocos2d.h"
#include "Enemy.h"
#include "EasyMode.h"
#include <iostream>
#include <string>

USING_NS_CC;

class Turret2 : public Sprite
{
public:

    virtual bool init();

    CREATE_FUNC(Turret2);

    int range2;
    int cont2 = 0;
    Point target2;
    Point position2;
    Turret2();
    ~Turret2();
    int getRange2();
    void setRange2(int newRange);

    bool EnemyinRango2(Sprite* p1, Sprite* p2);
    bool EnemyFueraDeRango2(Sprite* p1, Sprite* p2);

    void setPosition(Point* p);

    bool Disparar2(int con);

};