#ifndef __EASYMODE_H__
#define __EASYMODE_H__

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

class EasyMode : public Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(EasyMode);
    int health;
    int money;
    int selector;
    int scoring;
    int countenemiesdeath;
    std::string texto;
    std::string texto2;
    std::string texto3;
    std::string texto4;
    std::string texto5;

    Sprite* FondoDePantalla;
    Sprite* LevelMap;
    Sprite* UI;
    Sprite* NormalTowerOption;
    Sprite* AdvancedTowerOption;
    cocos2d::Sprite* mainBase;

    std::vector<cocos2d::Sprite*> enemyWave;
    std::vector<cocos2d::Sprite*> turrets;
    std::vector<cocos2d::Sprite*> advancedTurrets;
    std::vector<Sprite*> zonas;

    Label* vida;
    Label* currency;
    Label* score;
    Label* cost1;
    Label* cost2;


    void spawnEnemyLeft(float dt);
    void spawnEnemyRight(float dt);
    void createSpecialTitles();
    void Fire(float dt);
    void SumCoins(float dt);

    void physics(Sprite* sprite);
    void physicsBullet(Sprite* sprite);
    bool onContactBegin(PhysicsContact& contact);

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    bool onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void update(float dt);


};
#endif  __EASYMODE_H__