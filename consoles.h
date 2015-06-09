//
//  consoles.h
//  Mono
//
//  Created by Kristoffer Andersen on 27/05/15.
//
//

#ifndef __spiTest__consoles__
#define __spiTest__consoles__

#include <stdio.h>


namespace mono {
    
    /** Print an error on screen or console */
    void error(const char *msg);
    
    /** Print a warning on screen or console */
    void warning(const char *msg);
    
    class Console
    {
    public:
        FILE *fptr;
        
        Console(FILE *f);
        
        Console& operator<<(const char *txt);
        Console& operator<<(int integer);
        Console& operator<<(char integer);
        Console& operator<<(float number);
        
    };
    
//    Console& operator<<(Console &console, const char *txt);
//    Console& operator<<(Console &console, int integer);
//    Console& operator<<(Console &console, char integer);
//    Console& operator<<(Console &console, float number);
    
    static Console Error(stderr);
    static Console Warn(stdout);
    static Console Debug(stdout);
}

#endif /* defined(__spiTest__consoles__) */
