
void clear_screen(u32 color) {
    u32* pixel = (u32*)render_state.memory; // type casting buffer void pointer to u32 pointer to represent pixel value pointer 
    for (int y = 0; y < render_state.height; y++) {
        for (int x = 0; x < render_state.width;x++) {
            *pixel++ = color; // setting pixel value and incrementing
        }
    }
}


void draw_rect_in_pixels(int x0 , int y0 , int x1, int y1 , u32 color) {

    x0 = clamp(0, x0, render_state.width); // start x
    x1 = clamp(0, x1, render_state.width); // start y 
    y0 = clamp(0, y0, render_state.height); // end x
    y1 = clamp(0, y1, render_state.height); // end y 
    
    for (int y = y0; y < y1; y++) {
        u32* pixel = (u32*)render_state.memory + x0 + y*render_state.width;
        for (int x = x0; x < x1 ;x++) {
            *pixel++ = color; // setting pixel value and incrementing
        }
    }
}

/*
### Intuition behind initialization of pointer on the vertical axis for drawing rectangle

NOTE : 
1. The drawing happens by initializing the pointer vertically ( with " +  y*render_state.width;  " ) and horizontally ( with " + x0 " )  and drawing/setting pixels that lie on that horizontal line between limits x0 and x1 . Basically,
the rectangle is drawn by drawing horizontal lines from bottom to top .
1. we already know that the " + x0 " is to move the pointer into position in the x axis 

This " +  y*render_state.width; " is there so that the pointer can move up the y axis . 
In order to do that the pointer needs to skip an entire screen width i.e. render_state.width 


Illustration ( of how to move the pointer up vertically  ) : 

|-|X| | | | | | |
| |O|-|-|-|-|-|-|

x ==> final pointer pos
O ==> initial pointer pos
- ==> skipped pixels

The above illustration therefore shows you how we move the pointer up  , to begin drawing the horizontal line above the current one 
*/

float render_scale = 0.01f; // just as a utility to avoid typing floats for x , y , half_size_x and half_size_y 

void draw_rect(float x, float y, float half_size_x, float half_size_y , u32 color ) {

    // scaling according to window size 
    x *= render_state.height*render_scale; //  read this as " x % of height of window will be set as x " 
    y *= render_state.height*render_scale;
    half_size_x *= render_state.height * render_scale; //  read this as " half_size_x % of height of window will be set as half_size_x " 
    half_size_y *= render_state.height * render_scale; 

    // for centering the rectangle ( 0,0 will therefore now be the centre of the screen instead of the bottom left corner ) 
    x += render_state.width / 2; 
    y += render_state.height / 2; 


    /*
    * 
    The Variables and what they are supposed to represent : 

    x ==> centre point x 
    y ==> centre point y 
    half_size_x ==> half width 
    half_size_y ==> half height 

    Note : Note that the coordinate system we're using here where the centre 0,0 points to the centre of the screen .
    --------------------------
    |          |             |
    |          |             |
    |----------|-------------|
    |          |(0,0)        |
    |          |             |
    --------------------------
    */



    // change to pixels 
    int x0 = x - half_size_x;
    int x1 = x + half_size_x;
    int y0 = y - half_size_y;
    int y1 = y + half_size_y;

    draw_rect_in_pixels(x0, y0, x1, y1, color);



}