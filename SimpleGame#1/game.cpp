#define is_down(b) input->buttons[b].is_down // when key is down / pressed 
#define pressed(b) (input->buttons[b].is_down  && input->buttons[b].changed ) // when the key is *first*  pressed after *not being in a pressed state*
#define released(b) (!input->buttons[b].is_down  && input->buttons[b].changed) // when key is released after *being in a pressed state* 
/*
Illustration :
Key at different times t0 , t1 , t2 , t3 , t4 , t5 is as follows : 

  t0  t1  t2  t3  t4  t5
| 0 | 1 | 1 | 1 | 0 | 0 | 

Here at : 
t0 ==> is_down = false and changed = false 
t1 ==> is_down = true and changed = true ---> PRESSED state 
t2 ==> is_down = true and changed = false
t3 ==> is_down = true and changed = false
t4 ==> is_down = false and changed = true -->  RELEASED state
t5 ==> is_down = false and changed = false
*/


float player_1_p , player_1_dp , player_2_p, player_2_dp;
float arena_half_size_x = 85, arena_half_size_y = 45; 
float player_half_size_x = 2.5, player_half_size_y = 12;

float ball_p_x, ball_p, y, ball_dp_x = 100, ball_dp_y = 100; 

void simulate_game(Input* input , float dt) {
    
    clear_screen(0xff5500);
    draw_rect(0,0, arena_half_size_x, arena_half_size_y, 0xffaa33);
    
    /*
    Note : 
    1. if (pressed(BUTTON_UP)) player_pos_y +=1.f;    ===> this moves the position of the rect when key is pressed only 
    2. if (is_down(BUTTON_UP)) player_pos_y += speed * dt; ===> this moves the position of the rect when key is pressed and held .
    PS : refer key state macros to understand the states
    */


    /*
    Variables : 
    1. player_ddp ===> acceleration : der(der(position)) w.r.t time . **This is what we modify with user input**
    2. player_dp ==> velocity : der(position) w.r.t time
    3. player_p ==> position

    */


    //  ------------------------------------------------------ player 1 ------------------------------------------------------------
    float player_1_ddp = 0.f;
    if (is_down(BUTTON_UP)) player_1_ddp += 2000 ;
    if (is_down(BUTTON_DOWN)) player_1_ddp -= 2000 ;

    
    player_1_ddp -= player_1_dp * 10.f; // add friction ( by damping the acceleration in every frame , even in the absence of user input )

    player_1_p = player_1_p + player_1_dp * dt   + player_1_ddp * dt * dt * .5f  ; // s = ut + (1/2)a(t^2)  ( to calculate the position change for current frame ) 
    player_1_dp = player_1_dp + player_1_ddp * dt ; // v = u + at  ( this line is required to set the velocity for the next frame's movement ) 

    /*
    Collision Detection Condition : object position + object half size > half size of arena ( i.e. the object is found to be poking out of the arena ) 
    */
    if (player_1_p + player_half_size_y > arena_half_size_y) { 
        player_1_p = arena_half_size_y - player_half_size_y; // move the object to the boundary limit ( position + half size ) ( boundary condition ) 
        player_1_dp = 0 ; // 
        /*
        Note : 

        1. player_1_dp = 0 ===> doesn't do anything visually , setting player_1_p directly so as to be touching the boundary of the arena itself arrests the velocity . But,
        I think we do this so that the player_1_dp value doesn't get too big or out of control inside memory

        2. player_1_dp *= -2 0 ===> use to invert velocity direction to show a "bouncing off the wall" effect.We multiply it by 2 to give a exagerated invertion / bouncing effect
        */
    } else if (player_1_p - player_half_size_y < - arena_half_size_y) {
        player_1_p = - arena_half_size_y + player_half_size_y; // move the object to the boundary limit ( position + half size ) ( boundary condition ) 
        player_1_dp = 0 ;
    }






    //   ------------------------------------------------------- player 2 -----------------------------------------------------------
    float player_2_ddp = 0.f;
    if (is_down(BUTTON_W)) player_2_ddp += 2000;
    if (is_down(BUTTON_S)) player_2_ddp -= 2000;

    player_2_ddp -= player_2_dp * 10.f; // add friction ( by damping the acceleration in every frame , even in the absence of user input ) 

    player_2_p = player_2_p + player_2_dp * dt + player_2_ddp * dt * dt * .5f; // s = ut + (1/2)a(t^2)  ( to calculate the position change for current frame ) 
    player_2_dp = player_2_dp + player_2_ddp * dt; // v = u + at  ( this line is required to set the velocity for the next frame's movement ) 
    
    // Collision detection Condition 
    if (player_2_p + player_half_size_y > arena_half_size_y) {
        player_2_p = arena_half_size_y - player_half_size_y; // move the object to the boundary limit ( position + half size ) ( boundary condition ) 
        player_2_dp = 0; 
    }
    else if (player_2_p - player_half_size_y < -arena_half_size_y) {
        player_2_p = -arena_half_size_y + player_half_size_y; // move the object to the boundary limit ( position + half size ) ( boundary condition ) 
        player_2_dp = 0;
    }
    

    //   -------------------------------------------------------- Ball -------------------------------------------------------------
    




    // ----------------------------------------------------- render rectangles -----------------------------------------------------
    draw_rect(0, 0, 1, 1, 0xffffff); // ball 
    draw_rect(80, player_1_p, 2.5, 12, 0xff0000); // player 1
    draw_rect(-80, player_2_p, 2.5 , 12, 0xff0000); // player 2 



};