//
//  GameState.h
//  pong
//
//  Created by Ashwin R S on 5/4/14.
//
//

#ifndef pong_GameState_h
#define pong_GameState_h

class GameState{
public:
    ALLEGRO_MUTEX   *mutex;
    int             state;
    
    GameState() : mutex(al_create_mutex),
    state(0){}
    
    ~GameState(){
        al_destroy_mutex(mutex);
    }
};


#endif
