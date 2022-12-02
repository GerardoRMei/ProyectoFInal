#pragma once
#include "cocos2d.h"
#include "Enemy.h"
#include "EasyMode.h"
#include <iostream>
#include <string>

USING_NS_CC;

class Turret : public Sprite
{
public:

    virtual bool init();

    CREATE_FUNC(Turret);

    int range;
    int cont=0;
    Point target;
    Point position;

    Turret();
    ~Turret();
    int getRange();
    void setRange(int newRange);

    bool EnemyinRango(Sprite* p1, Sprite* p2);
    bool EnemyFueraDeRango(Sprite* p1, Sprite* p2);

    void setPosition(Point* p);
    int getCont();
    void setCont(int c);

    bool Disparar(int con);
    void contar();

};