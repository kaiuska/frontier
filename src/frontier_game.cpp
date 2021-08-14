#include "frontier_game.h"


float delta_time = 0.0f;
float last_time = 0.0f;
float current_time = glfwGetTime();
bool resized = false;


FrontierGame::FrontierGame() :
    _year(0), _day(0), _hour(0), _min(0), _sec(0), _state(NORMAL)
{
    _window = create_window();

    //_project_dir = getcwd();

    _main_shader.create("shaders/vs_main.vs", "shaders/fs_main.fs");
    _text_shader.create("shaders/vs_text.vs", "shaders/fs_text.fs");

    _state = _window ? NORMAL : CRASH;

    _last_key_time = glfwGetTime();
    init_vertex_arrays();
    load_font("textures/font/technicality1.ttf", text_hei);
    init_textures();


    if (!rands) {
        init_rands(256);
    }

    _map.create(0, 0, TILESX, TILESY);

    _main_menu.init(glm::vec2(scr_wid/2-menu_wid, scr_hei/4), glm::vec2(menu_wid, scr_hei*3.0f/4.0f));

    _projection = glm::ortho(
                -(float)scr_wid/2, 
                (float)scr_wid/2, 
                -(float)scr_hei/2, 
                (float)scr_hei/2, 
                -20.0f, 10.0f); 
}


void FrontierGame::play()
{
    _state = NORMAL;
    int new_feature = NO_FEATURE; 
    _main_menu.show_action_prompt();
    while(!glfwWindowShouldClose(_window) && _state != CRASH)
    {

        while ((delta_time = current_time - last_time) < 0.1) {
            current_time = glfwGetTime();
        }

        last_time = current_time;  

        if(resized){
            _main_menu.resize(glm::vec2(scr_wid/2-menu_wid, scr_hei/4), glm::vec2(menu_wid, scr_hei*3/4)); 
            resized = false;
        }

        draw();

        glfwWaitEvents();
        process_input(_window, _map, delta_time);

        current_time = glfwGetTime();
    }
}


ActionType FrontierGame::process_input(GLFWwindow *window, Map& map ,float dt)
{

    bool recieved_input = false;
    bool key_pressed = false;


    ActionType action = ActionType::NONE;
    if (mouse_clicked) {
        action = (ActionType)_main_menu.click(click_pos);
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {    // quit
        glfwSetWindowShouldClose(window, true);
        _state = NORMAL;
    } if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){    // wait
        _state = NORMAL;
    } if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){         // quit
        _main_menu.hide_action_prompt();
        _state = NORMAL;
    }
    
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){       // turn left
        map.turn_player(LEFT);
    }if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){      // turn right
        map.turn_player(RIGHT);
    }if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){     // move forward
        map.move_player(FORWARD);
    }if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){     // move back
        map.move_player(BACKWARD);
    }

    if(glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS){// zoom out
        map.zoom(0.5f);
    }if(glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS){ // zoom in
        map.zoom(-0.5f);
    }


    if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS || action == CHOP){       // chop
        printf("Action: Chop\n");
        chop_action();
        _state = NORMAL;
        action = CHOP;
    }if(glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS || action == BUILD){      // build
        printf("Action: Build\n");
        build_action();
        _state = NORMAL;
        action = BUILD;
    }if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS || action == PLANT){      // plant
        printf("Action: Plant\n");
        _state = NORMAL;
        action = PLANT;
    }

    if (key_pressed) {
        _last_key_time = glfwGetTime();
        _state = NORMAL;
    }

    return action;
}



bool FrontierGame::build_action() 
{
    _main_menu.hide_action_prompt();
    _main_menu.show_construction_prompt();
    draw();
    FeatureType new_feature = _main_menu.get_construction(_window);

    //printf("get construction %d  %s\n", new_feature, feature_defs[(FeatureType)new_feature].name.c_str());
    if (new_feature != NO_FEATURE && feature_defs[new_feature].buildable) {
        _map.change_feature(_map.adjacent_tile(), new_feature);
    } else {
        return false;
    }
    return true;
}

bool FrontierGame::chop_action() 
{
    int existing_feature = _map.get_feature(_map.adjacent_tile());
    // if existing_feature is is "natural" block of features
    if (200 <= existing_feature && existing_feature <= 399) { 
        _map.change_feature(_map.adjacent_tile(), NO_FEATURE);
        return true;
    } else {
        return false;
    }
}


bool FrontierGame::running()
{
    return _state != CRASH ? true : false;
}


GLFWwindow *FrontierGame::create_window()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(DEFAULT_HEI,DEFAULT_WID, "frontier", NULL,NULL);
    if(!window){
        std::cerr << "failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGL()){
        std::cerr << "failed to initialize GLAD" << std::endl;
        return NULL;
    }

    glViewport(0, 0, DEFAULT_HEI, DEFAULT_WID);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    int h, w;
    glfwGetFramebufferSize(window, &w, &h);
    scr_wid = w;
    scr_hei = h;

    return window;
}



void FrontierGame::framebuffer_size_callback(GLFWwindow* window, int wid, int hei)
{
    //glfwGetWindowSize(window, &w, &h);
    //glViewport(0,0,wid,hei);
    
    int h, w;
    glfwGetFramebufferSize(window, &w, &h);
    scr_wid = w;
    scr_hei = h;
    glViewport(0, 0 ,wid ,hei);
    
    resized = true;
    printf("resize %f, %f\n", scr_wid, scr_hei);
}

void FrontierGame::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    //printf("mouse button pressed\n");

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        printf("mouse click %lf %lf\n", x, y);
        click_pos.x = x;
        click_pos.y = y;
        mouse_clicked = true;

    }
}

void FrontierGame::clock_tick()
{
    _sec++;
    if(_sec >= 60){
        _sec = 0;
        _min++;
    }
    if(_min >= 60){
        _min = 0;
        _hour++;
    }
    if(_hour >= 24){
        _hour = 0;
        _day++;
    }
    if(_day >= 365){
        _day = 0;
        _year++;
    }
    //printf("time: %d %d %d %d %d\n", _year, _day, _hour, _min, _sec);
}






void FrontierGame::draw()
{
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 projection = glm::ortho(
                    -(float)scr_wid/2, 
                    (float)scr_wid/2, 
                    -(float)scr_hei/2, 
                    (float)scr_hei/2, 
                    -20.0f, 10.0f); 

        _map.draw(_main_shader);

        _text_shader.use();
        _text_shader.setMat4("projection", glm::ortho(-scr_wid/2, scr_wid/2, -scr_hei/2, scr_hei/2, -20.0f, 10.0f));
        _main_menu.draw(_main_shader, _text_shader);

        _text_shader.setMat4("projection", projection);
        _text_shader.setMat4("view", glm::mat4(1.0f));
        render_text(_text_shader, "frontier", -scr_wid/2+10, scr_hei/2-text_hei,0.5f, text_color);

        glfwSwapBuffers(_window);

}



