//
//  main.h
//  pong
//
//  Created by Ashwin R S on 4/14/14.
//
//

#ifndef pong_main_h
#define pong_main_h

const float FPS = 30;
const int SCREEN_W = 640;
const int SCREEN_H = 480;

//bouncer size
const int BOUNCER_SIZE = 10;

//pad size
const int PAD_W = 80;
const int PAD_H = 8;
const float PAD_STEP_SIZE = 8;

//keyboard keys
enum MYKEYS {
    KEY_LEFT, KEY_RIGHT
};

//macros
#define LOG 0

#ifdef LOG
    #define PRINTA(a) printf(a)
#else
    #define PRINTA(a)
#endif

#ifdef LOG
#define PRINT1(a,b) printf(a,b)
#else
#define PRINT1(a,b)
#endif


#endif
