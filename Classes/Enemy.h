#pragma once

#include "cocos2d.h"
#include "Turret.h"

USING_NS_CC;
class Enemy : public Sprite
{
public:

    virtual bool init();

    int vida;

    Enemy();
    ~Enemy();
    void gotHit(int i);
    
    int getHealth();
    void setHealth(int newHealth);

    bool isKilled();


    CREATE_FUNC(Enemy);
};

