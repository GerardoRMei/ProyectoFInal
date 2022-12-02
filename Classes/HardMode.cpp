#include "HardMode.h"
#include "HelloWorldScene.h"
#include "GameOverScene.h"
#include "GameWonScene.h"
#include "Enemy.h"
#include "Turret.h"
#include "Turret2.h"
#include "ui/CocosGUI.h"
#include <cocostudio/SimpleAudioEngine.h>
#include <iostream>
#include <string>

USING_NS_CC;
USING_NS_CC_EXT;

using namespace CocosDenshion;
using namespace cocos2d;

Scene* HardMode::createScene()
{
    auto b = Scene::createWithPhysics();
    b->getPhysicsWorld()->setGravity(Vect(0, 0));

    auto layer = HardMode::create();
    b->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);

    b->addChild(layer);
    return b;

}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}


// on "init" you need to initialize your instance
bool HardMode::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }
    h = 1;
    chooseTurret = 0;
    countenemiesdeath2 = 0;
    m = 10;
    scoring2 = 0;
    txt = std::to_string(h);
    txt2 = std::to_string(m);
    txt3 = std::to_string(10);
    txt4 = std::to_string(30);
    txt5 = "Score:    " + std::to_string(scoring2);

    schedule(CC_SCHEDULE_SELECTOR(HardMode::spawnEnemyLeft2), 3, 4, 5);
    schedule(CC_SCHEDULE_SELECTOR(HardMode::spawnEnemyRight2), 3, 9, 25);
    schedule(CC_SCHEDULE_SELECTOR(HardMode::spawnEnemyWave3), 3, 11, 45);
    schedule(CC_SCHEDULE_SELECTOR(HardMode::spawnEnemyWave4), 3, 10, 55);
    schedule(CC_SCHEDULE_SELECTOR(HardMode::spawnLASTWAVE), 0.4, 6, 80);
    schedule(CC_SCHEDULE_SELECTOR(HardMode::SumCoins2), 1, CC_REPEAT_FOREVER, 0);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto musica = SimpleAudioEngine::getInstance();
    musica->stopBackgroundMusic(true);
    musica->playBackgroundMusic("audio/MainTheme.mp3", false);
    

    FondoDePantalla2 = Sprite::create("background.png");
    FondoDePantalla2->setAnchorPoint(Vec2::ZERO);
    FondoDePantalla2->setPosition(0, 0);
    this->addChild(FondoDePantalla2, 0);

    createSpecialTitles2();

    //UI For the player
    UI2 = Sprite::create("UIPLAYER.png");
    UI2->setPosition(350, 75);
    this->addChild(UI2);

    score2 = Label::createWithSystemFont(txt5, "Comic Sans", 14);
    score2->setAnchorPoint(Vec2(0, 0));
    score2->setPosition(50, visibleSize.height - 12);
    this->addChild(score2, 1);

    vida2 = Label::createWithSystemFont(txt, "Arial", 10);
    vida2->setPosition(380, 20);
    vida2->setTextColor(Color4B::RED);
    this->addChild(vida2);

    currency2 = Label::createWithSystemFont(txt2, "Arial", 10);
    currency2->setPosition(360, 20);
    currency2->setTextColor(Color4B::YELLOW);
    this->addChild(currency2);

    cost12 = Label::createWithSystemFont(txt3, "Arial", 7);
    cost12->setTextColor(Color4B::YELLOW);
    cost12->setPosition(400, 20);
    this->addChild(cost12);

    cost22 = Label::createWithSystemFont(txt4, "Arial", 7);
    cost22->setTextColor(Color4B::YELLOW);
    cost22->setPosition(420, 20);
    this->addChild(cost22);

    NormalTowerOption2 = Sprite::create("TorretaNormalIcon.png"); NormalTowerOption2->setPosition(400, 45);
    AdvancedTowerOption2 = Sprite::create("Torreta2Icon.png"); AdvancedTowerOption2->setPosition(420, 45);
    this->addChild(NormalTowerOption2, 1);
    this->addChild(AdvancedTowerOption2, 1);

    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(HardMode::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HardMode::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(HardMode::onTouchEnded, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    //Generate Map
    Map2 = Sprite::create("NivelOficial.png");
    Map2->setPosition(Point((visibleSize.width / 2) + origin.x, 150));
    this->addChild(Map2);

    mainBase2 = Sprite::create("LiderArrestado.png");
    mainBase2->setPosition(Point((visibleSize.width / 2) + origin.x + 140, (visibleSize.height) - 43));
    mainBase2->setTag(25);
    auto fisica = PhysicsBody::createCircle(mainBase2->getContentSize().width / 2);
    fisica->setCategoryBitmask(0x02);
    fisica->setCollisionBitmask(0x01);
    fisica->setContactTestBitmask(0xFFFFFFFF);
    fisica->setDynamic(false);
    fisica->setGravityEnable(false);
    mainBase2->addComponent(fisica);
    this->addChild(mainBase2);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(HardMode::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    schedule(CC_SCHEDULE_SELECTOR(HardMode::Fire2), 1.5, CC_REPEAT_FOREVER, 1);

    this->scheduleUpdate();

    return true;
}

void HardMode::update(float dt) {
    for (auto posicionamiento : enemyWave2) { //simular que mueren
        if (posicionamiento->getChildByName<Enemy*>("enemySoul")->getHealth() <= 0) {
            posicionamiento->setPosition(4500, 4500);
        }
    }

    if (countenemiesdeath2 == 49) {//check if he defeated all the enemies
        auto esena = GameWonScene::createScene();
        Director::getInstance()->replaceScene(esena);
    }

    if (scoring2 < 0) {
        scoring2 = 0;
        txt5 = "Score:    " + std::to_string(scoring2);
        score2->setString(txt5);
    }

}


void HardMode::Fire2(float dt) {
    for (auto m : turrets2) {
        for (auto n : enemyWave2) {
            log("paso primer cheque");
            if (m->getChildByName<Turret*>("turretData")->EnemyinRango(m, n) == true && m->getChildByName<Turret*>("turretData")->EnemyFueraDeRango(m, n) == false
                && n->getChildByName<Enemy*>("enemySoul")->getHealth() > 0) {
                log("paso segundo cheque");
                auto bullet = Sprite::create("bala.png");
                physicsBullet2(bullet);
                bullet->setPosition(m->getPosition());
                bullet->setTag(20);
                auto place = MoveTo::create(0.1, n->getPosition());
                auto remove = Sequence::create(place, RemoveSelf::create(), nullptr);
                this->addChild(bullet, 1);
                bullet->runAction(remove);
                log("paso el ultimo cheque");
               auto fireSFX = SimpleAudioEngine::getInstance();
                fireSFX->SimpleAudioEngine::playEffect("audio/fire.mp3", false);
            }
            else {

            }

        }
    }

    for (auto m : advancedTurrets2) {
        for (auto n : enemyWave2) {
            log("paso primer cheque");
            if (m->getChildByName<Turret2*>("turretData")->EnemyinRango2(m, n) == true && m->getChildByName<Turret2*>("turretData")->EnemyFueraDeRango2(m, n) == false
                && n->getChildByName<Enemy*>("enemySoul")->getHealth() > 0) {
                log("paso segundo cheque");
                auto bullet = Sprite::create("bala.png");
                physicsBullet2(bullet);
                bullet->setPosition(m->getPosition());
                bullet->setTag(30);
                auto place = MoveTo::create(0.1, n->getPosition());
                auto remove = Sequence::create(place, RemoveSelf::create(), nullptr);
                this->addChild(bullet, 1);
                bullet->runAction(remove);
                log("paso el ultimo cheque");
               auto fireSFX = SimpleAudioEngine::getInstance();
               fireSFX->SimpleAudioEngine::playEffect("audio/fire.mp3", false);
            }
            else {

            }

        }
    }
}


void HardMode::createSpecialTitles2() {

    auto sprite = Sprite::create("4piece.png");
    sprite->setPosition(363, 85);
    this->addChild(sprite, 0);
    zonas2.push_back(sprite);

    auto sprite2 = Sprite::create("4piece.png");
    sprite2->setPosition(192, 85);
    this->addChild(sprite2, 0);
    zonas2.push_back(sprite2);

    auto sprite3 = Sprite::create("2Vertical.png");
    sprite3->setPosition(198, 2);
    this->addChild(sprite3, 0);
    zonas2.push_back(sprite3);

    auto sprite4 = Sprite::create("2Vertical.png");
    sprite4->setPosition(276, 2);
    this->addChild(sprite4, 0);
    zonas2.push_back(sprite4);

    auto sprite5 = Sprite::create("2Vertical.png");
    sprite5->setPosition(198, 180);
    this->addChild(sprite5, 0);
    zonas2.push_back(sprite5);

    auto sprite6 = Sprite::create("2Vertical.png");
    sprite6->setPosition(276, 180);
    this->addChild(sprite6, 0);
    zonas2.push_back(sprite6);

    auto sprite7 = Sprite::create("2Horizontal.png");
    sprite7->setPosition(270, 125);
    this->addChild(sprite7, 0);
    zonas2.push_back(sprite7);

    auto sprite8 = Sprite::create("2Horizontal.png");
    sprite8->setPosition(281, 205);
    this->addChild(sprite8, 0);
    zonas2.push_back(sprite8);

    auto sprite9 = Sprite::create("2Horizontal.png");
    sprite9->setPosition(330, 205);
    this->addChild(sprite9, 0);
    zonas2.push_back(sprite9);

}

void HardMode::spawnEnemyLeft2(float dt) {
    auto aux = Director::getInstance();
    auto size = aux->getWinSize();

    auto enemy = Sprite::create("Enemy.png");
    enemy->retain();
    auto Soul = new Enemy();
    Soul->setName("enemySoul");

    enemy->setPosition(Point((size.width / 2) - 55, 0));
    enemy->addChild(Soul);
    enemy->getChildByName<Enemy*>("enemySoul")->setHealth(2);

    physics2(enemy);
    auto move = MoveTo::create(2.5, Point((size.width / 2) - 55, 93));
    auto horizontal = MoveTo::create(2.5, Point(130, 93));
    auto arriba = MoveTo::create(1, Point(130, 200));
    auto derecha = MoveTo::create(1, Point((size.width / 2) - 30, 200));
    auto arribaDeNuevo = MoveTo::create(1, Point((size.width / 2) - 30, 275));
    auto derechaDeNuevo = MoveTo::create(3.5, Point((size.width / 2) + 119, 275));
    auto ruta = Sequence::create(move, horizontal, arriba, derecha, arribaDeNuevo, derechaDeNuevo, nullptr);

    enemy->runAction(ruta);


    enemyWave2.push_back(enemy);
    this->addChild(enemy);

}

void HardMode::spawnEnemyWave3(float dt) {
    auto aux = Director::getInstance();
    auto size = aux->getWinSize();

    auto enemy = Sprite::create("Enemy.png");
    enemy->retain();
    auto Soul = new Enemy();
    Soul->setName("enemySoul");

    enemy->setPosition(Point((size.width / 2) - 55, 0));
    enemy->addChild(Soul);
    enemy->getChildByName<Enemy*>("enemySoul")->setHealth(4);

    physics2(enemy);
    auto move = MoveTo::create(1, Point((size.width / 2) - 55, 93));
    auto horizontal = MoveTo::create(1, Point(130, 93));
    auto arriba = MoveTo::create(3, Point(130, 200));
    auto derecha = MoveTo::create(2.3, Point((size.width / 2) - 30, 200));
    auto arribaDeNuevo = MoveTo::create(2, Point((size.width / 2) - 30, 275));
    auto derechaDeNuevo = MoveTo::create(3.5, Point((size.width / 2) + 119, 275));
    auto ruta = Sequence::create(move, horizontal, arriba, derecha, arribaDeNuevo, derechaDeNuevo, nullptr);

    enemy->runAction(ruta);


    enemyWave2.push_back(enemy);
    this->addChild(enemy);

}
void HardMode::spawnEnemyRight2(float dt) {
    auto aux = Director::getInstance();
    auto size = aux->getWinSize();

    auto malo = Sprite::create("Enemy.png");
    auto Soul = Enemy::create();
    Soul->setName("enemySoul");

    malo->setPosition(Point((size.width / 2) - 25, 0));
    malo->addChild(Soul);
    malo->getChildByName<Enemy*>("enemySoul")->setHealth(3);

    physics2(malo);
    auto move = MoveTo::create(2, Point((size.width / 2) - 25, 93));
    auto horizontal = MoveTo::create(2, Point(350, 93));
    auto arriba = MoveTo::create(2, Point(350, 200));
    auto izquierda = MoveTo::create(2, Point((size.width / 2) - 30, 200));
    auto arribaDeNuevo = MoveTo::create(2, Point((size.width / 2) - 30, 275));
    auto derechaDeNuevo = MoveTo::create(2, Point((size.width / 2) + 119, 275));
    auto ruta = Sequence::create(move, horizontal, arriba, izquierda, arribaDeNuevo, derechaDeNuevo, nullptr);

    malo->runAction(ruta);

    enemyWave2.push_back(malo);
    this->addChild(malo);
}
void HardMode::spawnEnemyWave4(float dt) {
    auto aux = Director::getInstance();
    auto size = aux->getWinSize();

    auto malo = Sprite::create("Enemy.png");
    auto Soul = Enemy::create();
    Soul->setName("enemySoul");

    malo->setPosition(Point((size.width / 2) - 25, 0));
    malo->addChild(Soul);
    malo->getChildByName<Enemy*>("enemySoul")->setHealth(5);

    physics2(malo);
    auto move = MoveTo::create(1, Point((size.width / 2) - 25, 93));
    auto horizontal = MoveTo::create(1.7, Point(350, 93));
    auto arriba = MoveTo::create(2.3, Point(350, 200));
    auto izquierda = MoveTo::create(2, Point((size.width / 2) - 30, 200));
    auto arribaDeNuevo = MoveTo::create(1, Point((size.width / 2) - 30, 275));
    auto derechaDeNuevo = MoveTo::create(3, Point((size.width / 2) + 119, 275));
    auto ruta = Sequence::create(move, horizontal, arriba, izquierda, arribaDeNuevo, derechaDeNuevo, nullptr);

    malo->runAction(ruta);

    enemyWave2.push_back(malo);
    this->addChild(malo);
}

void HardMode::spawnLASTWAVE(float dt) {
    auto aux = Director::getInstance();
    auto size = aux->getWinSize();

    auto malo = Sprite::create("Enemy.png");
    auto Soul = Enemy::create();
    Soul->setName("enemySoul");

    malo->setPosition(Point((size.width / 2) - 25, 0));
    malo->addChild(Soul);
    malo->getChildByName<Enemy*>("enemySoul")->setHealth(5);

    physics2(malo);
    auto move = MoveTo::create(1, Point((size.width / 2) - 25, 93));
    auto horizontal = MoveTo::create(1.7, Point(350, 93));
    auto arriba = MoveTo::create(1.9, Point(350, 200));
    auto izquierda = MoveTo::create(1.2, Point((size.width / 2) - 30, 200));
    auto arribaDeNuevo = MoveTo::create(1, Point((size.width / 2) - 30, 275));
    auto derechaDeNuevo = MoveTo::create(2.5, Point((size.width / 2) + 119, 275));
    auto ruta = Sequence::create(move, horizontal, arriba, izquierda, arribaDeNuevo, derechaDeNuevo, nullptr);

    malo->runAction(ruta);

    enemyWave2.push_back(malo);
    this->addChild(malo);
}

void HardMode::physics2(Sprite* sprite) {
    auto hurtbox = PhysicsBody::createCircle(sprite->getContentSize().width / 2);
    hurtbox->setGravityEnable(false);
    hurtbox->setDynamic(true);
    hurtbox->setCategoryBitmask(0x01);
    hurtbox->setCollisionBitmask(0x02);
    hurtbox->setContactTestBitmask(0xFFFFFFFF);
    sprite->addComponent(hurtbox);
}

void HardMode::physicsBullet2(Sprite* sprite) {
    auto hurtbox = PhysicsBody::createCircle(sprite->getContentSize().width / 2);
    hurtbox->setGravityEnable(false);
    hurtbox->setDynamic(true);
    hurtbox->setCategoryBitmask(0x02);
    hurtbox->setCollisionBitmask(0x01);
    hurtbox->setContactTestBitmask(0xFFFFFFFF);
    sprite->addComponent(hurtbox);
}


bool HardMode::onContactBegin(PhysicsContact& contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();;

    if (nodeA && nodeB)
    {
        if (nodeA->getTag() == 25)
        {
            nodeB->removeFromParentAndCleanup(true);
        }
        else if (nodeB->getTag() == 25)
        {
            nodeA->getPhysicsBody()->getOwner()->getChildByName<Enemy*>("enemySoul")->setHealth(0);
           auto BaseHurtSFX = SimpleAudioEngine::getInstance();
          BaseHurtSFX->SimpleAudioEngine::playEffect("audio/BaseWasHurt.mp3", false);
            log("choco al jefe");
            scoring2 -= 10000;
            txt5 = "Score:    " + std::to_string(scoring2);
            score2->setString(txt5);
            h--;
            txt = std::to_string(h);
            vida2->setString(txt);

            countenemiesdeath2++;


            if (h == 0) {
                auto esena = GameOverScene::createScene();
                Director::getInstance()->replaceScene(esena);
            }
        }

        else if (nodeA->getTag() == 20)
        {
            if (nodeB->getPhysicsBody()->getOwner()->getChildByName<Enemy*>("enemySoul") != nullptr) {
                nodeA->removeFromParentAndCleanup(true);
                log("le chocaron A");

                nodeB->getPhysicsBody()->getOwner()->getChildByName<Enemy*>("enemySoul")->gotHit(1);
                log("perdiovida");


                if (nodeB->getPhysicsBody()->getOwner()->getChildByName<Enemy*>("enemySoul")->getHealth() <= 0) {
                   auto deathSFX = SimpleAudioEngine::getInstance();
                   deathSFX->SimpleAudioEngine::playEffect("audio/death.mp3", false);
                    scoring2 += 1000;
                    txt5 = "Score:    " + std::to_string(scoring2);
                    score2->setString(txt5);

                    log("murio");
                    m += 5;
                    txt2 = std::to_string(m);
                    currency2->setString(txt2);
                    countenemiesdeath2++;
                    //nodeB->removeFromParentAndCleanup(true);
                    //CC_SAFE_RELEASE_NULL(nodeB);
                }
            }

        }
        else if (nodeB->getTag() == 20) //Check the state of the bullets
        {
            if (nodeA->getPhysicsBody()->getOwner()->getChildByName<Enemy*>("enemySoul") != nullptr) {
                nodeB->removeFromParentAndCleanup(true);
                log("le chocaron B");

                nodeA->getPhysicsBody()->getOwner()->getChildByName<Enemy*>("enemySoul")->gotHit(1);
                log("perdiovida");


                if (nodeA->getPhysicsBody()->getOwner()->getChildByName<Enemy*>("enemySoul")->getHealth() <= 0) {
                  auto BaseHurtSFX = SimpleAudioEngine::getInstance();
                   BaseHurtSFX->SimpleAudioEngine::playEffect("audio/death.mp3", false);
                    scoring2 += 1000;
                    txt5 = std::to_string(scoring2);
                    score2->setString(txt5);

                    log("murio");
                    m += 5;
                    txt2 = std::to_string(m);
                    currency2->setString(txt2);
                    countenemiesdeath2++;
                }
            }
            else {

            }


        }
        else if (nodeA->getTag() == 30)
        {
            if (nodeB->getPhysicsBody()->getOwner()->getChildByName<Enemy*>("enemySoul") != nullptr) {

                log("le chocaron A");

                nodeB->getPhysicsBody()->getOwner()->getChildByName<Enemy*>("enemySoul")->gotHit(5);
                log("perdiovida");


                if (nodeB->getPhysicsBody()->getOwner()->getChildByName<Enemy*>("enemySoul")->getHealth() <= 0) {
                  auto deathSFX = SimpleAudioEngine::getInstance();
                  deathSFX->SimpleAudioEngine::playEffect("audio/death.mp3", false);
                    scoring2 += 1000;
                    txt5 = "Score:    " + std::to_string(scoring2);
                    score2->setString(txt5);

                    log("murio");
                    m += 5;
                    txt2 = std::to_string(m);
                    currency2->setString(txt2);
                    countenemiesdeath2++;
                    //nodeB->removeFromParentAndCleanup(true);
                    //CC_SAFE_RELEASE_NULL(nodeB);
                }
            }
            nodeA->removeFromParentAndCleanup(true);

        }
        else if (nodeB->getTag() == 30) //Check the state of the bullets
        {
            if (nodeA->getPhysicsBody()->getOwner()->getChildByName<Enemy*>("enemySoul") != nullptr) {


                log("le chocaron B");

                nodeA->getPhysicsBody()->getOwner()->getChildByName<Enemy*>("enemySoul")->gotHit(5);
                log("perdiovida");


                if (nodeA->getPhysicsBody()->getOwner()->getChildByName<Enemy*>("enemySoul")->getHealth() <= 0) {
                   auto deathSFX = SimpleAudioEngine::getInstance();
                  deathSFX->SimpleAudioEngine::playEffect("audio/death.mp3", false);
                    scoring2 += 1000;
                    txt5 = std::to_string(scoring2);
                    score2->setString(txt5);

                    log("murio");
                    m += 5;
                    txt2 = std::to_string(m);
                    currency2->setString(txt2);
                    //nodeA->removeFromParentAndCleanup(true);
                    //CC_SAFE_RELEASE_NULL(nodeA);
                    countenemiesdeath2++;
                }
                nodeB->removeFromParentAndCleanup(true);
            }
            else {

            }


        }
    }
    //bodies can collide
    return true;
}

bool HardMode::onTouchBegan(Touch* touch, Event* event)
{
    Point touchLoc = touch->getLocation();

    if (NormalTowerOption2->getBoundingBox().containsPoint(touchLoc))
    {
        chooseTurret = 1;
        return true;
    }
    if (AdvancedTowerOption2->getBoundingBox().containsPoint(touchLoc))
    {
        chooseTurret = 2;
        return true;
    }

    return false;
}

void HardMode::onTouchMoved(Touch* touch, Event* event)
{
    if (chooseTurret == 1) NormalTowerOption2->setPosition(touch->getLocation());

    if (chooseTurret == 2) AdvancedTowerOption2->setPosition(touch->getLocation());
}

bool HardMode::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {

    if (chooseTurret == 1 && m >= 10) {
        m -= 10;
        txt2 = std::to_string(m);
        currency2->setString(txt2);
        Turret* data = new Turret();
        data->setRange(50);
        auto newSprite = Sprite::create("TorretaNormalIcon.png");
        data->setName("dataTorreta");
        newSprite->addChild(data);
        newSprite->setPosition(touch->getLocation());
        auto radio = DrawNode::create();
        radio->DrawNode::drawCircle(newSprite->getPosition(), data->getRange(), 360, 30, false, Color4F::GRAY);

        this->addChild(newSprite);
        this->addChild(radio);
        turrets2.push_back(newSprite);

        NormalTowerOption2->setPosition(400, 45);
    }
    else {
        NormalTowerOption2->setPosition(400, 45);
    }
    if (chooseTurret == 2 && m >= 30) {
        m -= 30;
        txt2 = std::to_string(m);
        currency2->setString(txt2);
        Turret2* data = new Turret2();
        data->setRange2(50);
        auto newSprite = Sprite::create("Torreta2Icon.png");

        data->setName("dataTorreta");
        newSprite->addChild(data);

        auto radio = DrawNode::create();
        newSprite->setPosition(touch->getLocation());
        radio->DrawNode::drawCircle(newSprite->getPosition(), 50, 360, 30, false, Color4F::GRAY);
        this->addChild(newSprite);
        advancedTurrets2.push_back(newSprite);
        this->addChild(radio);
        AdvancedTowerOption2->setPosition(420, 45);
    }
    else {
        AdvancedTowerOption2->setPosition(420, 45);
    }
    chooseTurret = 0;

    return true;
}

void HardMode::SumCoins2(float dt) {
    scoring2++;
    txt5 = "Score:    " + std::to_string(scoring2);
    score2->setString(txt5);

    int m = rand() % 2;

    switch (m) {
    case 0:
        score2->setTextColor(Color4B::RED);
        break;
    case 1:
        score2->setTextColor(Color4B::BLUE);
        break;
    case 2:
        score2->setTextColor(Color4B::WHITE);
        break;

    }

}