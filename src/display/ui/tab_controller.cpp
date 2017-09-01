//
//  tab_controller.cpp
//  tempmon
//
//  Created by Kristoffer Andersen on 05/07/2017.
//

#include "tab_controller.h"

using namespace mono::ui;

void TabController::init()
{
    tabbar = 0;
    currentScene = newScene = 0;
}

// MARK: Constructors

TabController::TabController()
{
    init();
}

TabController::TabController(TabBarView &tabBar)
{
    init();
    tabbar = &tabBar;
    tabbar->setButtonPushCallback(this, &TabController::tabBtnPushHandler);
}

// MARK: Scenen Control methods

void TabController::presentScene(SceneController &scene)
{
    newScene = &scene;
    if (currentScene == 0)
    {
        presentNewScene();
    }
    else
    {
        dismissScene();
    }
}

void TabController::presentSceneFor(IconButtonView &btn)
{
    tabbar->setActiveButton(btn);
}

void TabController::presentLeft()
{
    IconButtonView *prev = 0;
    for (std::list<IconButtonView*>::const_iterator it=tabbar->buttonListBegin();
         it != tabbar->buttonListEnd();
         ++it) {
        if (prev == 0)
        {
            prev = *it;
            continue;
        }
        
        if (*it == tabbar->currentActiveButton())
        {
            presentSceneFor(*prev);
            return;
        }
        
        prev = *it;
    }
}

void TabController::presentRight()
{
    IconButtonView *prev = 0;
    for (std::list<IconButtonView*>::const_iterator it=tabbar->buttonListBegin();
         it != tabbar->buttonListEnd();
         ++it) {
        if (prev == 0)
        {
            prev = *it;
            continue;
        }
        
        if (prev == tabbar->currentActiveButton())
        {
            presentSceneFor(**it);
            return;
        }
        
        prev = *it;
    }
}

SceneController* TabController::dismissScene()
{
    if (currentScene != 0)
    {
        currentScene->setDismissCallback(this, &TabController::sceneDismissHandler);
        currentScene->requestDismiss();
        return currentScene;
    }
    else
    {
        return 0;
    }
}

// MARK: Internal Methods

void TabController::tabBtnPushHandler(IconButtonView &btn)
{
    newScene = scnForBtnHandler.call(btn);
    if (newScene == 0)
        return;
    
    if (currentScene != 0)
        dismissScene();
    else
        presentNewScene();
}

void TabController::sceneDismissHandler()
{
    if (currentScene != 0) {
        dismissSceneHandler.call(currentScene);
        if (currentScene->Visible())
            currentScene->hide();
        
        currentScene = 0;
    }
    
    presentNewScene();
}

void TabController::presentNewScene()
{
    if (newScene != 0)
    {
        presentSceneHandler.call(newScene);
        if (!newScene->Visible())
            newScene->show();
        
        currentScene = newScene;
        newScene = 0;
    }
}

// MARK: Accessor Methods

void TabController::setChangeSceneCallback(SceneController* (*cfunc)(const IconButtonView &))
{
    scnForBtnHandler.attach(cfunc);
}

void TabController::setDismissSceneCallback(void (*cfunc)(const SceneController *))
{
    dismissSceneHandler.attach(cfunc);
}

void TabController::setPresentSceneCallback(void (*cfunc)(const SceneController *))
{
    presentSceneHandler.attach(cfunc);
}
