struct Button_State {
	bool is_down, changed;

};

enum {
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_W,
	BUTTON_S,
	BUTTON_COUNT// this will have the integer value of the number of buttons ( i.e. 4 in this case ) 
};

struct Input {
	Button_State buttons[BUTTON_COUNT];
};