#include "input.h"
#include "minigl.h"


void initialize_input(GLFWwindow* window) {
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_FALSE);

}


void update_input(input_manager *input_manager) {
    
    memcpy(input_manager->keys_last, input_manager->keys, sizeof(input_manager->keys));
    memcpy(input_manager->mouse_buttons_last, input_manager->mouse_buttons, sizeof(input_manager->mouse_buttons));
    // Reset scroll offsets after processing
    input_manager->scroll_x = 0;
    input_manager->scroll_y = 0;
}

dispatch_event(event* event){

}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    minigl_engine *ctx_engine = glfwGetWindowUserPointer(window);
    input_manager *ctx_input_manager = &(ctx_engine->engine_input_manager);

    if (key >= 0 && key < MAX_KEYS) {
        if (action == GLFW_PRESS) {
            ctx_input_manager->keys[key] = GLFW_PRESS;
        } else if (action == GLFW_RELEASE) {
            ctx_input_manager->keys[key] = GLFW_RELEASE;
        }
    }

}




void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    minigl_engine *ctx_engine = glfwGetWindowUserPointer(window);
    input_manager *ctx_input_manager = &ctx_engine->engine_input_manager;

    if (button >= 0 && button < MAX_MOUSE_BUTTONS) {
        if (action == GLFW_PRESS) {
            ctx_input_manager->mouse_buttons[button] = GLFW_PRESS;
        } else if (action == GLFW_RELEASE) {
            ctx_input_manager->mouse_buttons[button] = GLFW_RELEASE;
        }
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    minigl_engine *ctx_engine = glfwGetWindowUserPointer(window);
    input_manager *ctx_input_manager = &ctx_engine->engine_input_manager;

    ctx_input_manager->mouse_x = xpos;
    ctx_input_manager->mouse_y = ypos;
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    minigl_engine *ctx_engine = glfwGetWindowUserPointer(window);
    input_manager *ctx_input_manager = &ctx_engine->engine_input_manager;

    ctx_input_manager->scroll_x = xoffset;
    ctx_input_manager->scroll_y = yoffset;
}


int is_key_down(input_manager *input_manager, int key) {
    return input_manager->keys[key] == GLFW_PRESS;
}

int is_key_pressed(input_manager *input_manager, int key) {
    // printf("key: %d\n", input_manager->keys[key]);
    // printf("key_last: %d\n", input_manager->keys_last[key]);
    return input_manager->keys[key] == GLFW_PRESS && input_manager->keys_last[key] != GLFW_RELEASE;
}

int is_key_released(input_manager *input_manager, int key) {
    return input_manager->keys[key] == GLFW_RELEASE && input_manager->keys_last[key] == GLFW_PRESS;
}

int is_mouse_button_down(input_manager *input_manager, int button) {
    return input_manager->mouse_buttons[button] == GLFW_PRESS;
}

int is_mouse_button_pressed(input_manager *input_manager, int button) {
    return input_manager->mouse_buttons[button] == GLFW_PRESS && input_manager->mouse_buttons_last[button] == GLFW_RELEASE;
}

int is_mouse_button_released(input_manager *input_manager, int button) {
    return input_manager->mouse_buttons[button] == GLFW_RELEASE && input_manager->mouse_buttons_last[button] == GLFW_PRESS;
}









