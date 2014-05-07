//
//  Bouncer.h
//  pong
//
//  Created by Ashwin R S on 5/4/14.
//
//

#ifndef pong_Bouncer_h
#define pong_Bouncer_h

#include "Constants.h"

//bouncer size
const int BOUNCER_SIZE = 10;
const int BOUNCER_STEP_SIZE = 6;

class BOUNCER{
public:
    ALLEGRO_MUTEX   *mutex;
    ALLEGRO_COND    *cond;
    float           bouncer_x;
    float           bouncer_y;
    float           bouncer_dx;
    float           bouncer_dy;
    bool            ready;
    
    BOUNCER() : mutex(al_create_mutex()),
    cond(al_create_cond()),
    bouncer_x(SCREEN_W / 2.0 - BOUNCER_SIZE / 2.0),
    bouncer_y(SCREEN_H / 2.0 - BOUNCER_SIZE / 2.0),
    bouncer_dx(-4.0),
    bouncer_dy(-4.0),
    ready(false){}
    
    ~BOUNCER(){
        al_destroy_mutex(mutex);
        al_destroy_cond(cond);
    }
};


#endif
