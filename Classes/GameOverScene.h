
#ifndef __GAMEOVERSCENE_H__
#define __GAMEOVERSCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include <cocostudio/SimpleAudioEngine.h>
#include <iostream>

USING_NS_CC;
USING_NS_CC_EXT;


class GameOverScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    bool didWon;
    // implement the "static create()" method manually
    CREATE_FUNC(GameOverScene);

    Menu* Return;
    Sprite* Entity;
    Sprite* Cartel;
    
    void BackToMenu(Ref* sender);
    void menuCloseCallback(Ref* pSender);
};

#endif  __GAMEOVERSCENE_H__

