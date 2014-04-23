pong
====

A pong game implementation using Allegro 5


CPU Utilization - 

When you use al_rest(0.01) in the bouncer thread the CPU utilization was around 1-3%

When I used a bool ready flag in the bouncer thread, the CPU utilization was around 99%