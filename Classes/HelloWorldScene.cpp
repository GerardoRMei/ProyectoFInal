#include "HelloWorldScene.h"
#include "EasyMode.h"
#include "RegMode.h"
#include "HardMode.h"
#include "cocos2d.h"
#include <cocostudio/SimpleAudioEngine.h>

USING_NS_CC;
using namespace CocosDenshion;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }
   //auto audio = SimpleAudioEngine::getInstance();
   //audio->stopBackgroundMusic(true);
   //audio->playBackgroundMusic("audio/MenuTheme.mp3", false);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    FondoDePantalla = Sprite::create("background.png");
    FondoDePantalla->setAnchorPoint(Vec2::ZERO);
    FondoDePantalla->setPosition(0, 0);


    this->addChild(FondoDePantalla,0);

    
    Logo = Sprite::create("Logo.png");
    Logo->setPosition(Point((visibleSize.width / 2) + origin.x, (visibleSize.height)-100));
    this->addChild(Logo);

    Lider = Sprite::create("Lider.png");
    Lider->setPosition(Point((visibleSize.width / 2) + origin.x+150, (visibleSize.height) - 100));
    this->addChild(Lider);
    
    Cop = Sprite::create("GuardCop.png");
    Cop->setPosition(Point((visibleSize.width / 2) + origin.x - 150, (visibleSize.height) - 100));
    this->addChild(Cop);

    auto buttonEZ = MenuItemImage::create("GameMode0.png","GameMode0Activated.png",CC_CALLBACK_1(HelloWorld::Easy, this));
    buttonEZ->setPosition(Point((visibleSize.width / 2) +origin.x-100, (visibleSize.height)-200));

    auto buttonReg = MenuItemImage::create("GameMode1.png", "GameMode1Activated.png", CC_CALLBACK_1(HelloWorld::Reg, this));
    buttonReg->setPosition(Point((visibleSize.width / 2) + origin.x, (visibleSize.height) - 200));

    auto buttonHard = MenuItemImage::create("GameMode2.png", "GameMode2Activated.png", CC_CALLBACK_1(HelloWorld::Hard, this));
    buttonHard->setPosition(Point((visibleSize.width / 2) + origin.x + 100, (visibleSize.height) - 200));

    auto *menu = Menu::create(buttonEZ, buttonReg, buttonHard, NULL);
    menu->setPosition(Point(0, 0));
    this->addChild(menu);


    return true;
 }


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    int a = 0;
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

void HelloWorld::Easy(Ref* sender)
{
    auto esena = EasyMode::createScene();   
    Director::getInstance()->replaceScene(TransitionFade::create(4,esena));
}

void HelloWorld::Reg(Ref* sender)
{
    auto a = RegMode::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(4, a));
}

void HelloWorld::Hard(Ref* sender)
{
    auto b = HardMode::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(4, b));
}

