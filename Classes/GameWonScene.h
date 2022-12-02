#ifndef __GAMEWONSCENE_H__
#define __GAMEWONSCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include <cocostudio/SimpleAudioEngine.h>
#include <iostream>

USING_NS_CC;
USING_NS_CC_EXT;


class GameWonScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    int coin;
    // implement the "static create()" method manually
    CREATE_FUNC(GameWonScene);
    Sprite* fondo;
    Menu* Return;
    Sprite* Entity;
    Sprite* Cartel;
    Label* mensaje;

    void updateLogo(float dt);
    void BackToMenu(Ref* sender);
    void menuCloseCallback(Ref* pSender);
};

#endif  __GAMEWONSCENE_H__

