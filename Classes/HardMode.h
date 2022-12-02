#ifndef __HARDMODE_H__
#define __HARDMODE_H__

#include "cocos2d.h"
#include "Turret.h"
#include "Enemy.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include <cocostudio/SimpleAudioEngine.h>
#include <iostream>
#include <string>

USING_NS_CC;
USING_NS_CC_EXT;

class HardMode : public Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(HardMode);
    int h;
    int m;
    int chooseTurret;
    int scoring2;
    int countenemiesdeath2;
    std::string txt;
    std::string txt2;
    std::string txt3;
    std::string txt4;
    std::string txt5;

    Sprite* FondoDePantalla2;
    Sprite* Map2;
    Sprite* UI2;
    Sprite* NormalTowerOption2;
    Sprite* AdvancedTowerOption2;
    cocos2d::Sprite* mainBase2;

    std::vector<cocos2d::Sprite*> enemyWave2;
    std::vector<cocos2d::Sprite*> turrets2;
    std::vector<cocos2d::Sprite*> advancedTurrets2;
    std::vector<Sprite*> zonas2;

    Label* vida2;
    Label* currency2;
    Label* score2;
    Label* cost12;
    Label* cost22;


    void spawnEnemyLeft2(float dt);
    void spawnEnemyRight2(float dt);
    void spawnEnemyWave3(float dt);
    void spawnEnemyWave4(float dt);
    void spawnLASTWAVE(float dt);
    void createSpecialTitles2();
    void Fire2(float dt);
    void SumCoins2(float dt);

    void physics2(Sprite* sprite);
    void physicsBullet2(Sprite* sprite);
    bool onContactBegin(PhysicsContact& contact);

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    bool onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void update(float dt);


};
#endif  __HARDMODE_H__
