#include "EasyMode.h"
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

Scene* EasyMode::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vect(0, 0));

    auto layer = EasyMode::create();
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
bool EasyMode::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }
    health = 3;
    selector = 0;
    countenemiesdeath = 0;
    money = 20;
    scoring= 0;
    texto = std::to_string(health);
    texto2 = std::to_string(money);
    texto3 = std::to_string(10);
    texto4 = std::to_string(30);
    texto5 = "Score:    " + std::to_string(scoring);

    schedule(CC_SCHEDULE_SELECTOR(EasyMode::spawnEnemyLeft), 3, 9, 10);

    schedule(CC_SCHEDULE_SELECTOR(EasyMode::spawnEnemyRight), 3, 5, 20);
    schedule(CC_SCHEDULE_SELECTOR(EasyMode::SumCoins), 1, CC_REPEAT_FOREVER, 0);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    /*
    auto musica = SimpleAudioEngine::getInstance();
    musica->stopBackgroundMusic(true);
    musica->playBackgroundMusic("audio/MainTheme.mp3", false);
    */

    FondoDePantalla = Sprite::create("background.png");
    FondoDePantalla->setAnchorPoint(Vec2::ZERO);
    FondoDePantalla->setPosition(0, 0);
    this->addChild(FondoDePantalla, 0);

    createSpecialTitles();

    //UI For the player
    UI = Sprite::create("UIPLAYER.png");
    UI->setPosition(350, 75);
    this->addChild(UI);

    score = Label::createWithSystemFont(texto5,"Comic Sans", 14);
    score->setAnchorPoint(Vec2(0, 0));
    score->setPosition(50,visibleSize.height-12);
    this->addChild(score,1);

    vida = Label::createWithSystemFont(texto, "Arial", 10);
    vida->setPosition(380, 20);
    vida->setTextColor(Color4B::RED);
    this->addChild(vida);

    currency = Label::createWithSystemFont(texto2, "Arial", 10);
    currency->setPosition(360, 20);
    currency->setTextColor(Color4B::YELLOW);
    this->addChild(currency);

    cost1 = Label::createWithSystemFont(texto3, "Arial", 7); 
    cost1->setTextColor(Color4B::YELLOW);
    cost1->setPosition(400, 20);
    this->addChild(cost1);

    cost2 = Label::createWithSystemFont(texto4, "Arial", 7);
    cost2->setTextColor(Color4B::YELLOW);
    cost2->setPosition(420, 20);
    this->addChild(cost2);

    NormalTowerOption = Sprite::create("TorretaNormalIcon.png"); NormalTowerOption->setPosition(400, 45);
    AdvancedTowerOption = Sprite::create("Torreta2Icon.png"); AdvancedTowerOption->setPosition(420, 45);
    this->addChild(NormalTowerOption, 1);
    this->addChild(AdvancedTowerOption, 1);

    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(EasyMode::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(EasyMode::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(EasyMode::onTouchEnded, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    //Generate Map
    LevelMap = Sprite::create("NivelOficial.png");
    LevelMap->setPosition(Point((visibleSize.width / 2) + origin.x, 150));
    this->addChild(LevelMap);

    mainBase = Sprite::create("LiderArrestado.png");
    mainBase->setPosition(Point((visibleSize.width / 2) + origin.x + 140, (visibleSize.height) - 43));
    mainBase->setTag(25);
    auto fisica = PhysicsBody::createCircle(mainBase->getContentSize().width / 2);
    fisica->setCategoryBitmask(0x02);
    fisica->setCollisionBitmask(0x01);
    fisica->setContactTestBitmask(0xFFFFFFFF);
    fisica->setDynamic(false);
    fisica->setGravityEnable(false);
    mainBase->addComponent(fisica);
    this->addChild(mainBase);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(EasyMode::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    schedule(CC_SCHEDULE_SELECTOR(EasyMode::Fire), 1.5, CC_REPEAT_FOREVER, 1);

    this->scheduleUpdate();

    return true;
}

void EasyMode::update(float dt) {
    for (auto posicionamiento : enemyWave) { //simular que mueren
        if (posicionamiento->getChildByName<Enemy*>("enemySoul")->getHealth()<=0) {
            posicionamiento->setPosition(4500, 4500);
        }
    }

    if (countenemiesdeath == 16) {//check if he defeated all the enemies
        auto esena = GameWonScene::createScene();
        Director::getInstance()->replaceScene(esena);
    }

    if (scoring < 0) {
        scoring = 0;
        texto5 = "Score:    " + std::to_string(scoring);
        score->setString(texto5);
    }
    
}


void EasyMode::Fire(float dt) {
    for (auto m : turrets) {
        for (auto n : enemyWave) {
                log("paso primer cheque");
                if (m->getChildByName<Turret*>("turretData")->EnemyinRango(m, n) == true && m->getChildByName<Turret*>("turretData")->EnemyFueraDeRango(m, n) == false
                    && n->getChildByName<Enemy*>("enemySoul")->getHealth()>0) {
                    log("paso segundo cheque"); 
                        auto bullet = Sprite::create("bala.png");
                        physicsBullet(bullet);
                        bullet->setPosition(m->getPosition());
                        bullet->setTag(20);
                        auto place = MoveTo::create(0.1, n->getPosition());
                        auto remove = Sequence::create(place, RemoveSelf::create(), nullptr);
                        this->addChild(bullet, 1);
                        bullet->runAction(remove);
                        log("paso el ultimo cheque");
                        //auto fireSFX = SimpleAudioEngine::getInstance();
                        //fireSFX->SimpleAudioEngine::playEffect("audio/fire.mp3", false);
                }
                else {

                }

        }
    }

    for (auto m : advancedTurrets) {
        for (auto n : enemyWave) {
            log("paso primer cheque");
            if (m->getChildByName<Turret2*>("turretData")->EnemyinRango2(m, n) == true && m->getChildByName<Turret2*>("turretData")->EnemyFueraDeRango2(m, n) == false
                && n->getChildByName<Enemy*>("enemySoul")->getHealth() > 0) {
                log("paso segundo cheque");
                auto bullet = Sprite::create("bala.png");
                physicsBullet(bullet);
                bullet->setPosition(m->getPosition());
                bullet->setTag(30);
                auto place = MoveTo::create(0.1, n->getPosition());
                auto remove = Sequence::create(place,RemoveSelf::create(), nullptr);
                this->addChild(bullet, 1);
                bullet->runAction(remove);
                log("paso el ultimo cheque");
               // auto fireSFX = SimpleAudioEngine::getInstance();
                //fireSFX->SimpleAudioEngine::playEffect("audio/fire.mp3", false);
            }
            else {

            }

        }
    }
}


void EasyMode::createSpecialTitles() {

    auto sprite = Sprite::create("4piece.png");
    sprite->setPosition(363, 85);
    this->addChild(sprite, 0);
    zonas.push_back(sprite);

    auto sprite2 = Sprite::create("4piece.png");
    sprite2->setPosition(192, 85);
    this->addChild(sprite2, 0);
    zonas.push_back(sprite2);

    auto sprite3 = Sprite::create("2Vertical.png");
    sprite3->setPosition(198, 2);
    this->addChild(sprite3, 0);
    zonas.push_back(sprite3);

    auto sprite4 = Sprite::create("2Vertical.png");
    sprite4->setPosition(276, 2);
    this->addChild(sprite4, 0);
    zonas.push_back(sprite4);

    auto sprite5 = Sprite::create("2Vertical.png");
    sprite5->setPosition(198, 180);
    this->addChild(sprite5, 0);
    zonas.push_back(sprite5);

    auto sprite6 = Sprite::create("2Vertical.png");
    sprite6->setPosition(276, 180);
    this->addChild(sprite6, 0);
    zonas.push_back(sprite6);

    auto sprite7 = Sprite::create("2Horizontal.png");
    sprite7->setPosition(270, 125);
    this->addChild(sprite7, 0);
    zonas.push_back(sprite7);

    auto sprite8 = Sprite::create("2Horizontal.png");
    sprite8->setPosition(281, 205);
    this->addChild(sprite8, 0);
    zonas.push_back(sprite8);

    auto sprite9 = Sprite::create("2Horizontal.png");
    sprite9->setPosition(330, 205);
    this->addChild(sprite9, 0);
    zonas.push_back(sprite9);

}

void EasyMode::spawnEnemyLeft(float dt) {
    auto aux = Director::getInstance();
    auto size = aux->getWinSize();

    auto enemy = Sprite::create("Enemy.png");
    enemy->retain();
    auto Soul = new Enemy();
    Soul->setName("enemySoul");

    enemy->setPosition(Point((size.width / 2) - 55, 0));
    enemy->addChild(Soul);
    enemy->getChildByName<Enemy*>("enemySoul")->setHealth(2);

    physics(enemy);
    auto move = MoveTo::create(3, Point((size.width / 2) - 55, 93));
    auto horizontal = MoveTo::create(3, Point(130, 93));
    auto arriba = MoveTo::create(3, Point(130, 200));
    auto derecha = MoveTo::create(3, Point((size.width / 2) - 30, 200));
    auto arribaDeNuevo = MoveTo::create(3, Point((size.width / 2) - 30, 275));
    auto derechaDeNuevo = MoveTo::create(3, Point((size.width / 2) + 119, 275));
    auto ruta = Sequence::create(move, horizontal, arriba, derecha, arribaDeNuevo, derechaDeNuevo, nullptr);

    enemy->runAction(ruta);


    enemyWave.push_back(enemy);
    this->addChild(enemy);

}

void EasyMode::spawnEnemyRight(float dt) {
    auto aux = Director::getInstance();
    auto size = aux->getWinSize();

    auto malo = Sprite::create("Enemy.png");
    auto Soul = Enemy::create();
    Soul->setName("enemySoul");

    malo->setPosition(Point((size.width / 2) - 25, 0));
    malo->addChild(Soul);
    malo->getChildByName<Enemy*>("enemySoul")->setHealth(3);

    physics(malo);
    auto move = MoveTo::create(2, Point((size.width / 2) - 25, 93));
    auto horizontal = MoveTo::create(2, Point(350, 93));
    auto arriba = MoveTo::create(2, Point(350, 200));
    auto izquierda = MoveTo::create(2, Point((size.width / 2) - 30, 200));
    auto arribaDeNuevo = MoveTo::create(2, Point((size.width / 2) - 30, 275));
    auto derechaDeNuevo = MoveTo::create(2, Point((size.width / 2) + 119, 275));
    auto ruta = Sequence::create(move, horizontal, arriba, izquierda, arribaDeNuevo, derechaDeNuevo, nullptr);

    malo->runAction(ruta);

    enemyWave.push_back(malo);
    this->addChild(malo);
}

void EasyMode::physics(Sprite* sprite) {
    auto hurtbox = PhysicsBody::createCircle(sprite->getContentSize().width / 2);
    hurtbox->setGravityEnable(false);
    hurtbox->setDynamic(true);
    hurtbox->setCategoryBitmask(0x01);
    hurtbox->setCollisionBitmask(0x02);
    hurtbox->setContactTestBitmask(0xFFFFFFFF);
    sprite->addComponent(hurtbox);
}

void EasyMode::physicsBullet(Sprite* sprite) {
    auto hurtbox = PhysicsBody::createCircle(sprite->getContentSize().width / 2);
    hurtbox->setGravityEnable(false);
    hurtbox->setDynamic(true);
    hurtbox->setCategoryBitmask(0x02);
    hurtbox->setCollisionBitmask(0x01);
    hurtbox->setContactTestBitmask(0xFFFFFFFF);
    sprite->addComponent(hurtbox);
}


bool EasyMode::onContactBegin(PhysicsContact& contact)
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
          //  auto BaseHurtSFX = SimpleAudioEngine::getInstance();
           // BaseHurtSFX->SimpleAudioEngine::playEffect("audio/BaseWasHurt.mp3", false);
            log("choco al jefe");
            scoring -= 10000;
            texto5 = "Score:    " + std::to_string(scoring);
            score->setString(texto5);
            health--;
            texto = std::to_string(health);
            vida->setString(texto);

            countenemiesdeath++;
            
            
            if (health == 0) {
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
              //      auto deathSFX = SimpleAudioEngine::getInstance();
                //    deathSFX->SimpleAudioEngine::playEffect("audio/death.mp3", false);
                    scoring += 1000;
                    texto5 = "Score:    " + std::to_string(scoring);
                    score->setString(texto5);
                    
                    log("murio");
                    money += 5;
                    texto2 = std::to_string(money);
                    currency->setString(texto2);
                    countenemiesdeath++;
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
              //      auto BaseHurtSFX = SimpleAudioEngine::getInstance();
                //    BaseHurtSFX->SimpleAudioEngine::playEffect("audio/death.mp3", false);
                    scoring += 1000;
                    texto5 = std::to_string(scoring);
                    score->setString(texto5);

                    log("murio");
                    money += 5;
                    texto2 = std::to_string(money);
                    currency->setString(texto2);
                    countenemiesdeath++;
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
               //     auto deathSFX = SimpleAudioEngine::getInstance();
                 //   deathSFX->SimpleAudioEngine::playEffect("audio/death.mp3", false);
                    scoring += 1000;
                    texto5 = "Score:    " + std::to_string(scoring);
                    score->setString(texto5);

                    log("murio");
                    money += 5;
                    texto2 = std::to_string(money);
                    currency->setString(texto2);
                    countenemiesdeath++;
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
            //        auto deathSFX = SimpleAudioEngine::getInstance();
              //      deathSFX->SimpleAudioEngine::playEffect("audio/death.mp3", false);
                    scoring += 1000;
                    texto5 = std::to_string(scoring);
                    score->setString(texto5);

                    log("murio");
                    money += 5;
                    texto2 = std::to_string(money);
                    currency->setString(texto2);
                    //nodeA->removeFromParentAndCleanup(true);
                    //CC_SAFE_RELEASE_NULL(nodeA);
                    countenemiesdeath++;
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

bool EasyMode::onTouchBegan(Touch* touch, Event* event)
{
    Point touchLoc = touch->getLocation();

    if (NormalTowerOption->getBoundingBox().containsPoint(touchLoc))
    {
        selector = 1;
        return true;
    }
    if (AdvancedTowerOption->getBoundingBox().containsPoint(touchLoc))
    {
        selector = 2;
        return true;
    }

    return false;
}

void EasyMode::onTouchMoved(Touch* touch, Event* event)
{
    if (selector == 1) NormalTowerOption->setPosition(touch->getLocation());

    if (selector == 2) AdvancedTowerOption->setPosition(touch->getLocation());
}

bool EasyMode::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {

    if (selector == 1 && money >= 10) {
        money -= 10;
        texto2 = std::to_string(money);
        currency->setString(texto2);
        Turret* data = new Turret();
        data->setRange(50);
        data->setCont(2);
        auto newSprite = Sprite::create("TorretaNormalIcon.png");
        data->setName("dataTorreta");
        newSprite->addChild(data);
        newSprite->setPosition(touch->getLocation());
        auto radio = DrawNode::create();
        radio->DrawNode::drawCircle(newSprite->getPosition(), data->getRange(), 360, 30, false, Color4F::GRAY);

        this->addChild(newSprite);
        this->addChild(radio);
        turrets.push_back(newSprite);

        NormalTowerOption->setPosition(400, 45);
    }
    else {
        NormalTowerOption->setPosition(400, 45);
    }
    if (selector == 2 && money >= 30) {
        money -= 30;
        texto2 = std::to_string(money);
        currency->setString(texto2);
        Turret2* data = new Turret2();
        data->setRange2(50);
        auto newSprite = Sprite::create("Torreta2Icon.png");

        data->setName("dataTorreta");
        newSprite->addChild(data);

        auto radio = DrawNode::create();
        newSprite->setPosition(touch->getLocation());
        radio->DrawNode::drawCircle(newSprite->getPosition(), 50, 360, 30, false, Color4F::GRAY);
        this->addChild(newSprite);
        advancedTurrets.push_back(newSprite);
        this->addChild(radio);
        AdvancedTowerOption->setPosition(420, 45);
    }
    else {
        AdvancedTowerOption->setPosition(420, 45);
    }
    selector = 0;

    return true;
}

void EasyMode::SumCoins(float dt) {
    scoring++;
    texto5 = "Score:    " + std::to_string(scoring);
    score->setString(texto5);
    
    int m = rand() % 2;

    switch (m) {
    case 0:
        score->setTextColor(Color4B::RED);
        break;
    case 1:
        score->setTextColor(Color4B::BLUE);
        break;
    case 2:
        score->setTextColor(Color4B::WHITE);
        break;

    }

}