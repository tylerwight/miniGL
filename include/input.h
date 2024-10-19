#ifndef INPUT_H
#define INPUT_H
#include "graphics.h"



#define MAX_KEYS 1024
#define MAX_MOUSE_BUTTONS 8


typedef struct {
    int keys[MAX_KEYS];
    int keys_last[MAX_KEYS];
    int mouse_buttons[MAX_MOUSE_BUTTONS];
    int mouse_buttons_last[MAX_MOUSE_BUTTONS];
    double mouse_x, mouse_y;
    double scroll_x, scroll_y;
} input_manager;


typedef enum {
    EVENT_KEY_PRESS,
    EVENT_KEY_RELEASE,
} event_type;

typedef struct {
    event_type type;
    int key;
} event;

void initialize_input(GLFWwindow* window);
void update_input(input_manager *input_manager);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int is_key_down(input_manager *input_manager, int key);
int is_key_pressed(input_manager *input_manager, int key);
int is_key_released(input_manager *input_manager, int key);
int is_mouse_button_down(input_manager *input_manager, int button);
int is_mouse_button_pressed(input_manager *input_manager, int button);
int is_mouse_button_released(input_manager *input_manager, int button);



#endif