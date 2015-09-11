//
//  animator.h
//  displaySimTest
//
//  Created by Kristoffer Lyder Andersen on 11/09/15.
//
//

#ifndef __displaySimTest__animator__
#define __displaySimTest__animator__

#include "view.h"
#include "../../application_run_loop_task_interface.h"

namespace mono { namespace ui {
    
    class Animator : public IRunLoopTask
    {
    protected:
        View *view;
        
        geo::Point origin;
        geo::Point destination;
        
        geo::Point moveVector;
        
        bool active;
        
        void taskHandler();
        
    public:
        
        Animator(View *view);
        
        
        void setMoveVector(geo::Point vec);
        
        void setDestination(geo::Point dest);
        
        /**
         * Begin the animation of the view
         *
         * @brief Start the animation
         */
        void Start();
        
        void Pause();
        
        void Reset();
    };
    
} }

#endif /* defined(__displaySimTest__animator__) */
