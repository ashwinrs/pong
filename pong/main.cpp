//
//  main.cpp
//  pong
//
//  Created by Ashwin R S on 4/6/14.
//
//

#include <stdio.h>
#include "main.h"
#include <allegro5/allegro.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    ALLEGRO_DISPLAY     *display       = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue   = NULL;
    ALLEGRO_TIMER       *timer         = NULL;
    ALLEGRO_BITMAP      *bouncer       = NULL;
    ALLEGRO_BITMAP      *pad           = NULL;
    
    bool redraw = true;
    
    if(!al_init()){
        fprintf(stderr, "failed to initialize allegro");
        return -1;
    }
    
    
    return 0;
}