#include "EasyMode.h"
#include "HelloWorldScene.h"
#include "GameWonScene.h"
#include "ui/CocosGUI.h"
#include <cocostudio/SimpleAudioEngine.h>
#include <iostream>


USING_NS_CC;
USING_NS_CC_EXT;

using namespace CocosDenshion;



Scene* GameWonScene::createScene()
{
    return GameWonScene::create();

}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}


// on "init" you need to initialize your instance
bool GameWonScene::init()
{
    //////////////////////////////
    // 1. super init first

    if (!Scene::init())
    {
        return false;
    }
    coin = 0;
    /*
    auto musica = SimpleAudioEngine::getInstance();
    musica->stopBackgroundMusic(true);
    musica->playBackgroundMusic("audio/VictoryTheme.mp3", true);
    */
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    fondo = Sprite::create("background.png");
    fondo->setAnchorPoint(Vec2::ZERO);
    fondo->setPosition(0, 0);
    this->addChild(fondo);

    Cartel = Sprite::create("GameOverLogo.png");
    Cartel->setPosition(Point((visibleSize.width / 2) + origin.x + 100, (visibleSize.height) - 100));
    this->addChild(Cartel);

    Entity = Sprite::create("LiderArrestadoIconVersion.png");
    Entity->setPosition(Point((visibleSize.width / 2) + origin.x - 50, (visibleSize.height / 2) + origin.y));
    this->addChild(Entity);

    auto buttonReturn = MenuItemImage::create("BackToMenuLogo.png", "BackToMenuLogo.png", CC_CALLBACK_1(GameWonScene::BackToMenu, this));
    buttonReturn->setPosition(Point((visibleSize.width / 2) + origin.x + 100, (visibleSize.height) - 200));
    auto* menu = Menu::create(buttonReturn, NULL);
    menu->setPosition(Point(0, 0));
    this->addChild(menu);

    mensaje = Label::createWithSystemFont("You might have won today, but It will be different next time", "Arial", 16);
    mensaje->setPosition(Point((visibleSize.width / 2) + origin.x, (visibleSize.height)-20));
    mensaje->setTextColor(Color4B::BLACK);
    this->addChild(mensaje);

    schedule(CC_SCHEDULE_SELECTOR(GameWonScene::updateLogo), 0.6, CC_REPEAT_FOREVER, 0);


    return true;
}

void GameWonScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    int a = 0;
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

void GameWonScene::BackToMenu(Ref* sender) {
    auto esena = HelloWorld::createScene();
    Director::getInstance()->replaceScene(esena);
}

void GameWonScene::updateLogo(float dt) {
    if (coin % 2 == 0) {
        mensaje->setTextColor(Color4B::BLACK);
    }
    else {
        mensaje->setTextColor(Color4B::BLUE);
    }
    coin++;
}