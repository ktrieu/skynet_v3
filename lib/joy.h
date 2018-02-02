#define MAX_BUTTONS 30

typedef struct {
	int last;
	int curr;
	TVexJoysticks buttonId;
} Button;

typedef struct {
	Button buttons[MAX_BUTTONS];
	int numButtons;
} JoyState;

JoyState joyState;

void joy_add_button(TVexJoysticks buttonId) {
	if (joyState.numButtons < MAX_BUTTONS) {
		joyState.buttons[joyState.numButtons].buttonId = buttonId;
		joyState.numButtons++;
	}
	else {
		//maybe display an error
	}
}

Button* joy_get_button(TVexJoysticks buttonId) {
	for (int i = 0; i < joyState.numButtons; i++) {
		if (joyState.buttons[i].buttonId == buttonId) {
			return &joyState.buttons[i];
		}
	}
	return NULL;
}

void joy_init() {
	//controller one
	joy_add_button(Btn7U);
	joy_add_button(Btn7D);
	joy_add_button(Btn7L);
	joy_add_button(Btn7R);
	joy_add_button(Btn8U);
	joy_add_button(Btn8D);
	joy_add_button(Btn8L);
	joy_add_button(Btn8R);
	joy_add_button(Btn5U);
	joy_add_button(Btn5D);
	joy_add_button(Btn6U);
	joy_add_button(Btn6D);
	//controller two
	joy_add_button(Btn7UXmtr2);
	joy_add_button(Btn7DXmtr2);
	joy_add_button(Btn7LXmtr2);
	joy_add_button(Btn7RXmtr2);
	joy_add_button(Btn8UXmtr2);
	joy_add_button(Btn8DXmtr2);
	joy_add_button(Btn8LXmtr2);
	joy_add_button(Btn8RXmtr2);
	joy_add_button(Btn5UXmtr2);
	joy_add_button(Btn5DXmtr2);
	joy_add_button(Btn6UXmtr2);
	joy_add_button(Btn6DXmtr2);
}

void joy_poll_input() {
	for (int i = 0; i < joyState.numButtons; i++) {
		Button* current = &joyState.buttons[i];
		current->last = current->curr;
		current->curr = vexRT[current->buttonId];
	}
}

bool joy_is_clicked(TVexJoysticks buttonId) {
	Button* button = joy_get_button(buttonId);
	return (button->curr != button->last && button->curr == false);
}
