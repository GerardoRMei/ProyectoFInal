#ifndef __REGMODE_H__
#define __REGMODE_H__

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

class RegMode : public Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(RegMode);
    int health3;
    int money3;
    int selector3;
    int scoring3;
    int countenemiesdeath3;
    std::string texto3;
    std::string texto23;
    std::string texto33;
    std::string texto43;
    std::string texto53;

    Sprite* FondoDePantalla3;
    Sprite* LevelMap3;
    Sprite* UI3;
    Sprite* NormalTowerOption3;
    Sprite* AdvancedTowerOption3;
    cocos2d::Sprite* mainBase3;

    std::vector<cocos2d::Sprite*> enemyWave3;
    std::vector<cocos2d::Sprite*> turrets3;
    std::vector<cocos2d::Sprite*> advancedTurrets3;
    std::vector<Sprite*> zonas3;

    Label* vida3;
    Label* currency3;
    Label* score3;
    Label* cost13;
    Label* cost23;


    void spawnEnemyLeft3(float dt);
    void spawnEnemyRight3(float dt);
    void spawnFinalWave(float dt);
    void createSpecialTitles3();
    void Fire3(float dt);
    void SumCoins3(float dt);

    void physics3(Sprite* sprite);
    void physicsBullet3(Sprite* sprite);
    bool onContactBegin(PhysicsContact& contact);

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    bool onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void update(float dt);


};
#endif  __REGMODE_H__
