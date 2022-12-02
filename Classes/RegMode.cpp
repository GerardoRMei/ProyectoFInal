#include "RegMode.h"
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

Scene* RegMode::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vect(0, 0));

    auto layer = RegMode::create();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);

    scene->addChild(layer);
    return scene;

}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}


// on "init" you need to initialize your instance
bool RegMode::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }
    health3 = 3;
    selector3 = 0;
    countenemiesdeath3 = 0;
    money3 = 15;
    scoring3 = 0;
    texto3 = std::to_string(health3);
    texto23 = std::to_string(money3);
    texto33 = std::to_string(10);
    texto43 = std::to_string(30);
    texto53 = "Score:    " + std::to_string(scoring3);

    schedule(CC_SCHEDULE_SELECTOR(RegMode::spawnEnemyLeft3), 3, 9, 6);
    schedule(CC_SCHEDULE_SELECTOR(RegMode::spawnFinalWave), 1, 15, 45);
    schedule(CC_SCHEDULE_SELECTOR(RegMode::spawnEnemyRight3), 3, 5, 15);
    schedule(CC_SCHEDULE_SELECTOR(RegMode::SumCoins3), 1, CC_REPEAT_FOREVER, 0);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto musica = SimpleAudioEngine::getInstance();
    musica->stopBackgroundMusic(true);
    musica->playBackgroundMusic("audio/MainTheme.mp3", false);
    

    FondoDePantalla3 = Sprite::create("background.png");
    FondoDePantalla3->setAnchorPoint(Vec2::ZERO);
    FondoDePantalla3->setPosition(0, 0);
    this->addChild(FondoDePantalla3, 0);

    createSpecialTitles3();

    //UI For the player
    UI3 = Sprite::create("UIPLAYER.png");
    UI3->setPosition(350, 75);
    this->addChild(UI3);

    score3 = Label::createWithSystemFont(texto53, "Comic Sans", 14);
    score3->setAnchorPoint(Vec2(0, 0));
    score3->setPosition(50, visibleSize.height - 12);
    this->addChild(score3, 1);

    vida3 = Label::createWithSystemFont(texto3, "Arial", 10);
    vida3->setPosition(380, 20);
    vida3->setTextColor(Color4B::RED);
    this->addChild(vida3);

    currency3 = Label::createWithSystemFont(texto23, "Arial", 10);
    currency3->setPosition(360, 20);
    currency3->setTextColor(Color4B::YELLOW);
    this->addChild(currency3);

    cost13 = Label::createWithSystemFont(texto3, "Arial", 7);
    cost13->setTextColor(Color4B::YELLOW);
    cost13->setPosition(400, 20);
    this->addChild(cost13);

    cost23 = Label::createWithSystemFont(texto43, "Arial", 7);
    cost23->setTextColor(Color4B::YELLOW);
    cost23->setPosition(420, 20);
    this->addChild(cost23);

    NormalTowerOption3 = Sprite::create("TorretaNormalIcon.png"); NormalTowerOption3->setPosition(400, 45);
    AdvancedTowerOption3 = Sprite::create("Torreta2Icon.png"); AdvancedTowerOption3->setPosition(420, 45);
    this->addChild(NormalTowerOption3, 1);
    this->addChild(AdvancedTowerOption3, 1);

    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(RegMode::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(RegMode::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(RegMode::onTouchEnded, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    //Generate Map
    LevelMap3 = Sprite::create("NivelOficial.png");
    LevelMap3->setPosition(Point((visibleSize.width / 2) + origin.x, 150));
    this->addChild(LevelMap3);

    mainBase3 = Sprite::create("LiderArrestado.png");
    mainBase3->setPosition(Point((visibleSize.width / 2) + origin.x + 140, (visibleSize.height) - 43));
    mainBase3->setTag(25);
    auto fisica = PhysicsBody::createCircle(mainBase3->getContentSize().width / 2);
    fisica->setCategoryBitmask(0x02);
    fisica->setCollisionBitmask(0x01);
    fisica->setContactTestBitmask(0xFFFFFFFF);
    fisica->setDynamic(false);
    fisica->setGravityEnable(false);
    mainBase3->addComponent(fisica);
    this->addChild(mainBase3);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(RegMode::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    schedule(CC_SCHEDULE_SELECTOR(RegMode::Fire3), 1.5, CC_REPEAT_FOREVER, 1);

    this->scheduleUpdate();

    return true;
}

void RegMode::update(float dt) {
    for (auto posicionamiento : enemyWave3) { //simular que mueren
        if (posicionamiento->getChildByName<Enemy*>("enemySoul")->getHealth() <= 0) {
            posicionamiento->setPosition(4500, 4500);
        }
    }

    if (countenemiesdeath3 == 30) {//check if he defeated all the enemies
        auto esena = GameWonScene::createScene();
        Director::getInstance()->replaceScene(esena);
    }

    if (scoring3 < 0) {
        scoring3 = 0;
        texto53 = "Score:    " + std::to_string(scoring3);
        score3->setString(texto53);
    }

}


void RegMode::Fire3(float dt) {
    for (auto m : turrets3) {
        for (auto n : enemyWave3) {
            log("paso primer cheque");
            if (m->getChildByName<Turret*>("turretData")->EnemyinRango(m, n) == true && m->getChildByName<Turret*>("turretData")->EnemyFueraDeRango(m, n) == false
                && n->getChildByName<Enemy*>("enemySoul")->getHealth() > 0) {
                log("paso segundo cheque");
                auto bullet = Sprite::create("bala.png");
                physicsBullet3(bullet);
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

    for (auto m : advancedTurrets3) {
        for (auto n : enemyWave3) {
            log("paso primer cheque");
            if (m->getChildByName<Turret2*>("turretData")->EnemyinRango2(m, n) == true && m->getChildByName<Turret2*>("turretData")->EnemyFueraDeRango2(m, n) == false
                && n->getChildByName<Enemy*>("enemySoul")->getHealth() > 0) {
                log("paso segundo cheque");
                auto bullet = Sprite::create("bala.png");
                physicsBullet3(bullet);
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


void RegMode::createSpecialTitles3() {

    auto sprite = Sprite::create("4piece.png");
    sprite->setPosition(363, 85);
    this->addChild(sprite, 0);
    zonas3.push_back(sprite);

    auto sprite2 = Sprite::create("4piece.png");
    sprite2->setPosition(192, 85);
    this->addChild(sprite2, 0);
    zonas3.push_back(sprite2);

    auto sprite3 = Sprite::create("2Vertical.png");
    sprite3->setPosition(198, 2);
    this->addChild(sprite3, 0);
    zonas3.push_back(sprite3);

    auto sprite4 = Sprite::create("2Vertical.png");
    sprite4->setPosition(276, 2);
    this->addChild(sprite4, 0);
    zonas3.push_back(sprite4);

    auto sprite5 = Sprite::create("2Vertical.png");
    sprite5->setPosition(198, 180);
    this->addChild(sprite5, 0);
    zonas3.push_back(sprite5);

    auto sprite6 = Sprite::create("2Vertical.png");
    sprite6->setPosition(276, 180);
    this->addChild(sprite6, 0);
    zonas3.push_back(sprite6);

    auto sprite7 = Sprite::create("2Horizontal.png");
    sprite7->setPosition(270, 125);
    this->addChild(sprite7, 0);
    zonas3.push_back(sprite7);

    auto sprite8 = Sprite::create("2Horizontal.png");
    sprite8->setPosition(281, 205);
    this->addChild(sprite8, 0);
    zonas3.push_back(sprite8);

    auto sprite9 = Sprite::create("2Horizontal.png");
    sprite9->setPosition(330, 205);
    this->addChild(sprite9, 0);
    zonas3.push_back(sprite9);

}

void RegMode::spawnFinalWave(float dt) {
    auto aux = Director::getInstance();
    auto size = aux->getWinSize();

    auto enemy = Sprite::create("Enemy.png");
    enemy->retain();
    auto Soul = new Enemy();
    Soul->setName("enemySoul");

    enemy->setPosition(Point((size.width / 2) - 55, 0));
    enemy->addChild(Soul);
    enemy->getChildByName<Enemy*>("enemySoul")->setHealth(6);

    physics3(enemy);
    auto move = MoveTo::create(1.5, Point((size.width / 2) - 55, 93));
    auto horizontal = MoveTo::create(1.2, Point(130, 93));
    auto arriba = MoveTo::create(1.5, Point(130, 200));
    auto derecha = MoveTo::create(2, Point((size.width / 2) - 30, 200));
    auto arribaDeNuevo = MoveTo::create(2, Point((size.width / 2) - 30, 275));
    auto derechaDeNuevo = MoveTo::create(2, Point((size.width / 2) + 119, 275));
    auto ruta = Sequence::create(move, horizontal, arriba, derecha, arribaDeNuevo, derechaDeNuevo, nullptr);

    enemy->runAction(ruta);


    enemyWave3.push_back(enemy);
    this->addChild(enemy);

}

void RegMode::spawnEnemyLeft3(float dt) {
    auto aux = Director::getInstance();
    auto size = aux->getWinSize();

    auto enemy = Sprite::create("Enemy.png");
    enemy->retain();
    auto Soul = new Enemy();
    Soul->setName("enemySoul");

    enemy->setPosition(Point((size.width / 2) - 55, 0));
    enemy->addChild(Soul);
    enemy->getChildByName<Enemy*>("enemySoul")->setHealth(3);

    physics3(enemy);
    auto move = MoveTo::create(2.3, Point((size.width / 2) - 55, 93));
    auto horizontal = MoveTo::create(1.2, Point(130, 93));
    auto arriba = MoveTo::create(3, Point(130, 200));
    auto derecha = MoveTo::create(2, Point((size.width / 2) - 30, 200));
    auto arribaDeNuevo = MoveTo::create(2, Point((size.width / 2) - 30, 275));
    auto derechaDeNuevo = MoveTo::create(1.6f, Point((size.width / 2) + 119, 275));
    auto ruta = Sequence::create(move, horizontal, arriba, derecha, arribaDeNuevo, derechaDeNuevo, nullptr);

    enemy->runAction(ruta);


    enemyWave3.push_back(enemy);
    this->addChild(enemy);

}


void RegMode::spawnEnemyRight3(float dt) {
    auto aux = Director::getInstance();
    auto size = aux->getWinSize();

    auto malo = Sprite::create("Enemy.png");
    auto Soul = Enemy::create();
    Soul->setName("enemySoul");

    malo->setPosition(Point((size.width / 2) - 25, 0));
    malo->addChild(Soul);
    malo->getChildByName<Enemy*>("enemySoul")->setHealth(4);

    physics3(malo);
    auto move = MoveTo::create(1, Point((size.width / 2) - 25, 93));
    auto horizontal = MoveTo::create(1.6, Point(350, 93));
    auto arriba = MoveTo::create(2.5, Point(350, 200));
    auto izquierda = MoveTo::create(1.6, Point((size.width / 2) - 30, 200));
    auto arribaDeNuevo = MoveTo::create(2.7, Point((size.width / 2) - 30, 275));
    auto derechaDeNuevo = MoveTo::create(1, Point((size.width / 2) + 119, 275));
    auto ruta = Sequence::create(move, horizontal, arriba, izquierda, arribaDeNuevo, derechaDeNuevo, nullptr);

    malo->runAction(ruta);

    enemyWave3.push_back(malo);
    this->addChild(malo);
}

void RegMode::physics3(Sprite* sprite) {
    auto hurtbox = PhysicsBody::createCircle(sprite->getContentSize().width / 2);
    hurtbox->setGravityEnable(false);
    hurtbox->setDynamic(true);
    hurtbox->setCategoryBitmask(0x01);
    hurtbox->setCollisionBitmask(0x02);
    hurtbox->setContactTestBitmask(0xFFFFFFFF);
    sprite->addComponent(hurtbox);
}

void RegMode::physicsBullet3(Sprite* sprite) {
    auto hurtbox = PhysicsBody::createCircle(sprite->getContentSize().width / 2);
    hurtbox->setGravityEnable(false);
    hurtbox->setDynamic(true);
    hurtbox->setCategoryBitmask(0x02);
    hurtbox->setCollisionBitmask(0x01);
    hurtbox->setContactTestBitmask(0xFFFFFFFF);
    sprite->addComponent(hurtbox);
}


bool RegMode::onContactBegin(PhysicsContact& contact)
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
            scoring3 -= 10000;
            texto53 = "Score:    " + std::to_string(scoring3);
            score3->setString(texto53);
            health3--;
            texto3 = std::to_string(health3);
            vida3->setString(texto3);

            countenemiesdeath3++;


            if (health3 == 0) {
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
                    scoring3 += 1000;
                    texto53 = "Score:    " + std::to_string(scoring3);
                    score3->setString(texto53);

                    log("murio");
                    money3 += 5;
                    texto23 = std::to_string(money3);
                    currency3->setString(texto23);
                    countenemiesdeath3++;
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
                    scoring3 += 1000;
                    texto53 = std::to_string(scoring3);
                    score3->setString(texto53);

                    log("murio");
                    money3 += 5;
                    texto23 = std::to_string(money3);
                    currency3->setString(texto23);
                    countenemiesdeath3++;
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
                    scoring3 += 1000;
                    texto53 = "Score:    " + std::to_string(scoring3);
                    score3->setString(texto53);

                    log("murio");
                    money3 += 5;
                    texto23 = std::to_string(money3);
                    currency3->setString(texto23);
                    countenemiesdeath3++;
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
                    scoring3 += 1000;
                    texto53 = std::to_string(scoring3);
                    score3->setString(texto53);

                    log("murio");
                    money3 += 5;
                    texto23 = std::to_string(money3);
                    currency3->setString(texto23);
                    //nodeA->removeFromParentAndCleanup(true);
                    //CC_SAFE_RELEASE_NULL(nodeA);
                    countenemiesdeath3++;
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

bool RegMode::onTouchBegan(Touch* touch, Event* event)
{
    Point touchLoc = touch->getLocation();

    if (NormalTowerOption3->getBoundingBox().containsPoint(touchLoc))
    {
        selector3 = 1;
        return true;
    }
    if (AdvancedTowerOption3->getBoundingBox().containsPoint(touchLoc))
    {
        selector3 = 2;
        return true;
    }

    return false;
}

void RegMode::onTouchMoved(Touch* touch, Event* event)
{
    if (selector3 == 1) NormalTowerOption3->setPosition(touch->getLocation());

    if (selector3== 2) AdvancedTowerOption3->setPosition(touch->getLocation());
}

bool RegMode::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {

    if (selector3 == 1 && money3>= 10) {
        money3 -= 10;
        texto23 = std::to_string(money3);
        currency3->setString(texto23);
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
        turrets3.push_back(newSprite);

        NormalTowerOption3->setPosition(400, 45);
    }
    else {
        NormalTowerOption3->setPosition(400, 45);
    }
    if (selector3 == 2 && money3 >= 30) {
        money3 -= 30;
        texto23 = std::to_string(money3);
        currency3->setString(texto23);
        Turret2* data = new Turret2();
        data->setRange2(50);
        auto newSprite = Sprite::create("Torreta2Icon.png");

        data->setName("dataTorreta");
        newSprite->addChild(data);

        auto radio = DrawNode::create();
        newSprite->setPosition(touch->getLocation());
        radio->DrawNode::drawCircle(newSprite->getPosition(), 50, 360, 30, false, Color4F::GRAY);
        this->addChild(newSprite);
        advancedTurrets3.push_back(newSprite);
        this->addChild(radio);
        AdvancedTowerOption3->setPosition(420, 45);
    }
    else {
        AdvancedTowerOption3->setPosition(420, 45);
    }
    selector3 = 0;

    return true;
}

void RegMode::SumCoins3(float dt) {
    scoring3++;
    texto53 = "Score:    " + std::to_string(scoring3);
    score3->setString(texto53);

    int m = rand() % 2;

    switch (m) {
    case 0:
        score3->setTextColor(Color4B::RED);
        break;
    case 1:
        score3->setTextColor(Color4B::BLUE);
        break;
    case 2:
        score3->setTextColor(Color4B::WHITE);
        break;

    }

}