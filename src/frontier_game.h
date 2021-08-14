#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <map>

#include "utils.h"
#include "shader.h"
#include "tile.h"
#include "feature.h"
#include "button.h"
#include "menu.h"
#include "sprite.h"
#include "map.h"

#include "person.h"
#include <algorithm>


class FrontierGame
{
public:
    FrontierGame();
    void play();
    void draw();
    

    bool running();
    bool build_action();
    bool chop_action();
    void clock_tick();
private:
    GLFWwindow *create_window();
    ActionType process_input(GLFWwindow *window, Map& map ,float dt);

    static void framebuffer_size_callback(GLFWwindow* window, int wid, int hei);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

private:
    std::string _project_dir;
    GLFWwindow *_window;

    Shader _main_shader;
    Shader _text_shader;
    Map _map;
    MainMenu _main_menu;

    GameState _state;

    glm::mat3 _projection;

    float _last_key_time;
    int _year;
    int _day;
    int _hour;
    int _min;
    int _sec;
};
