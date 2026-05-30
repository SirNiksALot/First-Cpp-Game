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
int player_1_score, player_2_score;

float ball_p_x, ball_p_y, ball_dp_x = 100, ball_dp_y = 0 , ball_half_size = 1; 

void simulate_player(float* p, float* dp, float ddp, float dt) {
    /*
    *dp is basically player_dp which is speed/velocity
    *p is basically player_p which is position
    
    */

    ddp -= *dp * 10.f; // add friction ( by damping the acceleration in every frame , even in the absence of user input )

    *p = *p + *dp * dt + ddp * dt * dt * .5f; // s = ut + (1/2)a(t^2)  ( to calculate the position change for current frame ) 
    *dp = *dp + ddp * dt; // v = u + at  ( this line is required to set the velocity for the next frame's movement ) 


    // Collision Detection Condition :
    if (*p + player_half_size_y > arena_half_size_y) { // collided with top arena boundary 
        *p = arena_half_size_y - player_half_size_y; // set the object pos to the arena upper limit ( boundary condition i.e touching the arena ) 
        *dp = 0; // 
        /*
        Note :

        1. *dp = 0 ===> setting the speed to 0 doesn't do anything visually , setting *p ( position ) directly so as to be touching the boundary of the arena itself arrests the velocity . But,
        I think we do this so that the *dp ( speed )  value doesn't get too big or out of control inside memory

        2. *dp *= -2 0 ===> inverting the speed can be used to invert velocity direction to show a "bouncing off the wall" effect. 
        We multiply it by 2 to give a exagerated invertion / bouncing effect

        */
    }
    else if (*p - player_half_size_y < -arena_half_size_y) { // collided with bottom arena boundary 
        *p = -arena_half_size_y + player_half_size_y; // set the object pos to arena lower limit ( boundary condition i.e touching the arena) 
        *dp = 0;
    }

}


bool aabb_vs_aabb(float ball_p_x, float ball_p_y , float player_p_x , float player_p_y) {
    return (ball_p_x + ball_half_size > player_p_x - player_half_size_x && // ball right edge beyond player left edge 
        ball_p_x - ball_half_size < player_p_x + player_half_size_x && // ball left edge before player right edge 
        ball_p_y + ball_half_size > player_p_y - player_half_size_y && // ball top edge above player bottom edge 
        ball_p_y - ball_half_size < player_p_y + player_half_size_y); // ball bottom edge below player top edge 
}

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


    //  ------------------------------------------------------ Simulate player 1 ------------------------------------------------------------
    float player_1_ddp = 0.f;
    if (is_down(BUTTON_UP)) player_1_ddp += 2000 ;
    if (is_down(BUTTON_DOWN)) player_1_ddp -= 2000 ;

    simulate_player(&player_1_p, &player_1_dp, player_1_ddp, dt);

    //   ------------------------------------------------------- Simulate player 2 -----------------------------------------------------------
    float player_2_ddp = 0.f;
    if (is_down(BUTTON_W)) player_2_ddp += 2000;
    if (is_down(BUTTON_S)) player_2_ddp -= 2000;

    simulate_player(&player_2_p, &player_2_dp, player_2_ddp, dt);
    

    //   -------------------------------------------------------- Simulate Ball -------------------------------------------------------------
    {

        ball_p_x += ball_dp_x * dt; // dist  = speed x time then , pos += dist  ( we dont use equations of motion here because no acceleration required , only constant velocity ) 
        ball_p_y += ball_dp_y * dt;


        // COLLISION # 1 : collision with player 1 . ( Note : 80 is the x pos of centre of player 1 , and player_1_p is the y pos of the centre of player 1 ) 
        if (aabb_vs_aabb(ball_p_x, ball_p_y, 80, player_1_p)) {
            ball_p_x = 80 - player_half_size_x - ball_half_size; // set the ball position to the limit ( boundary i.e touching the player ) 
            ball_dp_x *= -1; // invert x axis velocity to bounce back from the player 
            ball_dp_y = (ball_p_y - player_1_p) + player_1_dp * 0.75f; // custom physics for better "feel" 

            /*

            Explanation for custom physics mechanics : ( ball_p_y - player_1_p ) +  player_1_dp * 0.75f;

            1.)  ball_p_y - player_1_p  ==> to give the bal some vertical velocity based on where on the player the ball collided . ( therefore ,
            if you hit the player in the top half , the ball gets some upward velocity , llly if the ball hits the player in its bottom half it gets some downward velocity )

            2.)  player_1_dp * 0.75f ==> to give the ball some y axis velocity based on the player's velocity . ( .75 for scaling )
            */
        }


        // COLLISION # 2 : collision with player 2 . ( Note : -80 is the x pos of centre of player 1, and player_1_p is the y pos of the centre of player 1)
        else if (aabb_vs_aabb(ball_p_x, ball_p_y, -80, player_2_p)) {
            ball_p_x = -80 + player_half_size_x + ball_half_size; // set the ball position to the limit ( boundary i.e touching the player ) 
            ball_dp_x *= -1; // invert x axis velocity to bounce back from the player 
            ball_dp_y = player_2_dp * 0.75f; // to give the ball some y axis velocity based on the player's velocity . ( .75 for scaling ) 
        }


        // COLLISION # 3 : collision with arena boundaries 
        if (ball_p_y + ball_half_size > arena_half_size_y) { // collided with top boundary 
            ball_p_y = arena_half_size_y - ball_half_size;
            ball_dp_y *= -1; // invert vertical velocity for reflection effect
        }
        else if (ball_p_y - ball_half_size < -arena_half_size_y) { // collided with bottom boundary 
            ball_p_y = -arena_half_size_y + ball_half_size;
            ball_dp_y *= -1;
        }
        else if (ball_p_x + ball_half_size > arena_half_size_x) { // collided with right boundary 
            ball_p_x = 0;
            ball_p_y = 0;
            ball_dp_x *= -1;
            ball_dp_y = 0;
            player_2_score++;
        }
        else if (ball_p_x - ball_half_size < -arena_half_size_x) { // collided with left boundary 
            ball_p_x = 0;
            ball_p_y = 0;
            ball_dp_x *= -1;
            ball_dp_y = 0;
            player_1_score++;

        }
    }


    // ----------------------------------------------------- render rectangles -----------------------------------------------------
    draw_number(player_2_score, -10, 40, 1.f, 0xbbffbb);
    draw_number(player_1_score, 10, 40, 1.f, 0xbbffbb);

    draw_rect(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 0xffffff); // ball 
    draw_rect(80, player_1_p, 2.5, 12, 0xff0000); // player 1
    draw_rect(-80, player_2_p, 2.5 , 12, 0xff0000); // player 2 



};