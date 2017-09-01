

#ifndef tab_controller_h
#define tab_controller_h

#include <list>
#include <scene_controller.h>
#include "tabbar_view.h"
#include <FunctionPointer.h>

namespace mono { namespace ui {
    
    class TabController
    {
    protected:
        TabBarView *tabbar;
        SceneController *currentScene, *newScene;
        mbed::FunctionPointerArg1<SceneController*, const IconButtonView&> scnForBtnHandler;
        mbed::FunctionPointerArg1<void, const SceneController*> presentSceneHandler;
        mbed::FunctionPointerArg1<void, const SceneController*> dismissSceneHandler;
        
        void init();
        
        void tabBtnPushHandler(IconButtonView &btn);
        void sceneDismissHandler();
        void sceneHideHandler(const SceneController &);
        void presentNewScene();
        
    public:
        
        TabController();
        
        TabController(TabBarView &tabBar);
        
        // MARK: Scene Control
        
        void presentScene(SceneController &scene);
        
        void presentSceneFor(IconButtonView &btn);
        
        void presentLeft();
        
        void presentRight();
        
        SceneController* dismissScene();
        
        // MARK: Accessor Methods
        
        void setChangeSceneCallback(SceneController*(*cfunc)(const IconButtonView&));
        
        template <typename Context>
        void setChangeSceneCallback(Context *cnxt, SceneController*(Context::*memptr)(const IconButtonView&))
        {
            scnForBtnHandler.attach<Context>(cnxt, memptr);
        }
        
        void setDismissSceneCallback(void(*cfunc)(const SceneController*));
        
        template <typename Context>
        void setDismissSceneCallback(Context *cnxt, void(Context::*memptr)(const SceneController*))
        {
            dismissSceneHandler.attach<Context>(cnxt, memptr);
        }
        
        void setPresentSceneCallback(void(*cfunc)(const SceneController*));
        
        template <typename Context>
        void setPresentSceneCallback(Context *cnxt, void(Context::*memptr)(const SceneController*))
        {
            presentSceneHandler.attach<Context>(cnxt, memptr);
        }
    };
    
} }

#endif /* tab_controller_h */
