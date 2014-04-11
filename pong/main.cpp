//
//  main.cpp
//  pong
//
//  Created by Ashwin R S on 4/6/14.
//
//

#include <stdio.h>
#include <allegro5/allegro.h>

class DATA{
public:
    ALLEGRO_MUTEX   *mutex;
    ALLEGRO_COND    *cond;
    float           posiX;
    float           posiY;
    bool            modi_X;
    bool            ready;
    
    DATA() :    mutex(al_create_mutex()),
                cond(al_create_cond()),
                posiX(0),
                posiY(0),
                modi_X(false),
                ready(false) {}
    
    ~DATA(){
        al_destroy_mutex(mutex);
        al_destroy_cond(cond);
    }
};

const float FPS        = 30;
const int SCREEN_W     = 640;
const int SCREEN_H     = 480;
const int BOUNCER_SIZE = 32;

static void *Func_Thread(ALLEGRO_THREAD *thr, void *arg);

int main(int argc, char **argv) {
    ALLEGRO_DISPLAY     *display       = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue   = NULL;
    ALLEGRO_TIMER       *timer         = NULL;
    ALLEGRO_BITMAP      *bouncer       = NULL;
    ALLEGRO_THREAD      *thread_1      = NULL;
    ALLEGRO_THREAD      *thread_2      = NULL;
    
    bool redraw = true;
    
    if(!al_init()){
        fprintf(stderr, "failed to initialize allegro");
        return -1;
    }
    
    if(!al_install_mouse()){
        fprintf(stderr, "failed ti initialize mouse");
        return -1;
    }
    
    timer = al_create_timer(1.0/FPS);
    
    if (!timer) {
        fprintf(stderr, "failed to initialize timer!");
        return -1;
    }
    
    display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display) {
        fprintf(stderr, "failed to create display");
        return -1;
    }
    
    bouncer = al_create_bitmap(BOUNCER_SIZE, BOUNCER_SIZE);
    if (!bouncer) {
        fprintf(stderr, "failed to create bouncer bitmap");
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }
    
    al_set_target_bitmap(bouncer);
    al_clear_to_color(al_map_rgb(255,255,255));
    al_set_target_bitmap(al_get_backbuffer(display));
    event_queue = al_create_event_queue();
    
    if(!event_queue){
        fprintf(stderr, "failed to create event");
        al_destroy_bitmap(bouncer);
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }
    
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();
    al_start_timer(timer);
    
    DATA data;
    
    thread_1 = al_create_thread(Func_Thread, &data);
    al_start_thread(thread_1);
    
    al_lock_mutex(data.mutex);
    while (!data.ready) {
        al_wait_cond(data.cond, data.mutex);
    }
    al_unlock_mutex(data.mutex);
    
    al_lock_mutex(data.mutex);
    data.modi_X = true;
    data.ready = false;
    al_unlock_mutex(data.mutex);
    
    thread_2 = al_create_thread(Func_Thread, &data);
    al_start_thread(thread_2);
    
    al_lock_mutex(data.mutex);
    while (!data.ready) {
        al_wait_cond(data.cond, data.mutex);
    }
    al_unlock_mutex(data.mutex);
    
    while (1) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        
        
        
    }
    
     
    
    
    
    
    
    
    
    
    
    return NULL;

}
