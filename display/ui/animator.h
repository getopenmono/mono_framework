// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef __mono_ui_animator__
#define __mono_ui_animator__

#include "view.h"
#include "../../application_run_loop_task_interface.h"

namespace mono { namespace ui {
    
    /**
     * @deprecated
     */
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

#endif /* defined(__mono_ui_animator__) */
