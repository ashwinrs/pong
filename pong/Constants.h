//
//  Constants.h
//  pong
//
//  Created by Ashwin R S on 5/4/14.
//
//

#ifndef pong_Constants_h
#define pong_Constants_h

const float FPS = 30;
const int SCREEN_W = 640;
const int SCREEN_H = 480;

//bouncer size
const int BOUNCER_SIZE = 10;
const int BOUNCER_STEP_SIZE = 4;

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

#ifdef LOG
#define PRINT2(a,b,c) printf(a,b,c)
#else
#define PRINT2(a,b,c)
#endif

#ifdef LOG
#define PRINT3(a,b,c,d) printf(a,b,c,d)
#else
#define PRINT3(a,b,c,d)
#endif


#endif
