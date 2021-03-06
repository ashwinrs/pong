//
//  main.cpp
//  pong
//
//  Created by Ashwin R S on 4/6/14.
//
//

#include <stdio.h>

#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>


#include "main.h" //this should be included first
#include "Logging.h"
#include "Bouncer.h"
#include "GameState.h"
#include "Constants.h"


using namespace std;

//Global variables
ALLEGRO_DISPLAY     *display       = NULL;
ALLEGRO_EVENT_QUEUE *event_queue   = NULL;
ALLEGRO_TIMER       *timer         = NULL;
ALLEGRO_BITMAP      *bouncer_bitmap= NULL;
ALLEGRO_BITMAP      *pad_bitmap    = NULL;

GameState gameState;

float pad_x = SCREEN_W / 2.0 - PAD_W / 2.0;
float pad_y = SCREEN_H - PAD_H / 2.0;

//Function prototypes
int initialize();
void destroy_all_objects();
void show_lost_window();
static void *Bouncer_Thread(ALLEGRO_THREAD *thr, void *arg);


int main(int argc, char **argv) {
    
    ALLEGRO_THREAD *thread_1   = NULL;
    BOUNCER bouncer;
    
    bool key[4] = { false, false};
    bool redraw = true;
    bool doexit = false;
    
    if(initialize()){
        return -1;
    }
    
    //Give the pad yellow color
    al_set_target_bitmap(pad_bitmap);
    al_clear_to_color(al_map_rgb(255, 255, 0));
    
    //Give the bouncer pink color
    al_set_target_bitmap(bouncer_bitmap);
    al_clear_to_color(al_map_rgb(255, 0, 255));
    
    al_set_target_bitmap(al_get_backbuffer(display));
    
    //Register sources to event queue
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    
    //Thread creation
    thread_1 = al_create_thread(Bouncer_Thread, &bouncer);
    al_start_thread(thread_1);
    
    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();
    al_start_timer(timer);
    
    while (!doexit) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        
        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }else if (ev.type == ALLEGRO_EVENT_TIMER) {
            if(key[KEY_LEFT] && pad_x > 0 ){
                pad_x -= PAD_STEP_SIZE;
            }
            if (key[KEY_RIGHT] && pad_x <= (SCREEN_W - PAD_W - PAD_STEP_SIZE) ) {
                pad_x += PAD_STEP_SIZE;
            }   
            redraw = true;
            
        }else if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
            switch(ev.keyboard.keycode) {
                case ALLEGRO_KEY_LEFT:
                    key[KEY_LEFT] = true;
                    break;
                    
                case ALLEGRO_KEY_RIGHT:
                    key[KEY_RIGHT] = true;
                    break;
            }
        }else if (ev.type == ALLEGRO_EVENT_KEY_UP){
            switch(ev.keyboard.keycode) {
                case ALLEGRO_KEY_LEFT:
                    key[KEY_LEFT] = false;
                    break;
                    
                case ALLEGRO_KEY_RIGHT:
                    key[KEY_RIGHT] = false;
                    break;
                
                case ALLEGRO_KEY_ESCAPE:
                    doexit = true;
                    break;
            }
        }
        
        if(redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            
            al_lock_mutex(bouncer.mutex);
            float X = bouncer.bouncer_x;
            float Y = bouncer.bouncer_y;
            bouncer.ready = false;
            
            //notify bouncer thread that values have been copied
            al_broadcast_cond(bouncer.cond);
            
            al_unlock_mutex(bouncer.mutex);
            
            if (gameState.state == 1) {
                show_lost_window();
                destroy_all_objects();
                return 0;
            }
            
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_bitmap(pad_bitmap, pad_x, pad_y, 0);
            al_draw_bitmap(bouncer_bitmap, X, Y, 0);
            
            al_flip_display();
        }
    }
    
    //destroy all objects before exiting
    destroy_all_objects();
    
    return 0;
}

/*
 This function is run on the thread
 The main function of this thread is that it calculates
 the (x,y) coordinates of the bouncer bitmap.
 */
static void *Bouncer_Thread(ALLEGRO_THREAD *thr, void *arg){
    
    BOUNCER *bouncer = (BOUNCER*) arg;
    
    while (!al_get_thread_should_stop(thr)) {
        al_lock_mutex(bouncer->mutex);
        //add ready==false
        if(bouncer->ready == false){
            //changes the diffs sign if it detects the coordinates is reaching end of window
            if(bouncer->bouncer_x < 0 || bouncer->bouncer_x > SCREEN_W - BOUNCER_SIZE) {
                bouncer->bouncer_dx = -bouncer->bouncer_dx;
            }
            
            if(bouncer->bouncer_y < 0){
                bouncer->bouncer_dy = -bouncer->bouncer_dy;
            }else if(bouncer->bouncer_y > SCREEN_H - BOUNCER_SIZE - PAD_H){
                //The bouncer has reached the end of the screen
                //check pad location.
                //if pad is below bouncer
                if ((bouncer->bouncer_x >= pad_x ) && ( bouncer->bouncer_x <= (pad_x+PAD_W) )) {
                    bouncer->bouncer_dy = -bouncer->bouncer_dy;
                }
                else{
                //if pad is not below the bouncer
                    gameState.state = 1;
                }
            }
                
            
            bouncer->bouncer_x += bouncer->bouncer_dx;
            bouncer->bouncer_y += bouncer->bouncer_dy;
            
            bouncer->ready=true;
            
            //Make this thread to go to sleep until main thread reads it.
            al_wait_cond(bouncer->cond, bouncer->mutex);
        }
        al_unlock_mutex(bouncer->mutex);
    }
    return NULL;
}


//This function initializes all the allegro objects
int initialize(){
    if(!al_init()){
        fprintf(stderr, "failed to initialize allegro");
        return -1;
    }
    
    if(!al_install_keyboard()) {
        fprintf(stderr, "failed to initialize the keyboard!\n");
        return -1;
    }
    
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
        fprintf(stderr, "failed to create timer!\n");
        return -1;
    }
    
    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display) {
        fprintf(stderr, "failed to create display!\n");
        al_destroy_timer(timer);
        return -1;
    }
    
    bouncer_bitmap = al_create_bitmap(BOUNCER_SIZE, BOUNCER_SIZE);
    if(!bouncer_bitmap) {
        fprintf(stderr, "failed to create bouncer bitmap!\n");
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }
    
    pad_bitmap = al_create_bitmap(PAD_W, PAD_H);
    if(!pad_bitmap) {
        fprintf(stderr, "failed to create bouncer bitmap!\n");
        al_destroy_bitmap(bouncer_bitmap);
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }
    
    event_queue = al_create_event_queue();
    if(!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        al_destroy_bitmap(pad_bitmap);
        al_destroy_bitmap(bouncer_bitmap);
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }
    return 0;
}

void show_lost_window(){
    const char* message       = "You lost :(";
    const char* content_title = "Game Over";
    const char* window_title  = "GAME OVER";

    al_show_native_message_box(al_get_current_display(),
                                      window_title,
                                      content_title,
                                      message, NULL,
                                      ALLEGRO_MESSAGEBOX_ERROR);

    destroy_all_objects();
    exit(0);
}

void destroy_all_objects(){
    //Destroy all objects before exiting
    al_destroy_bitmap(bouncer_bitmap);
    al_destroy_bitmap(pad_bitmap);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
}

