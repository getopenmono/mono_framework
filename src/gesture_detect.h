

#ifndef gesture_detect_hpp
#define gesture_detect_hpp

#include <mono.h>
#include <FunctionPointer.h>

class GestureDetect : public mono::TouchResponder {
    
    enum Positions {
        LEFT,
        RIGHT,
        TOP,
        BOTTOM
    };
    
    bool gestureInProgress;
    Positions Begin, End;
    mono::geo::Point beginPos, endPos;
    
    void processMove(bool isScreenCoords);
    
public:
    
    mbed::FunctionPointer slideLeft, slideRight;
    
    GestureDetect();
    
    void RespondTouchBegin(mono::TouchEvent &event);
    void RespondTouchEnd(mono::TouchEvent &event);
};

#endif /* gesture_detect_hpp */
