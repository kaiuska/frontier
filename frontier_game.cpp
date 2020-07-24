#include "frontier_game.h"


float delta_time = 0.0f;
float last_time = 0.0f;
float current_time = glfwGetTime();


FrontierGame::FrontierGame() 
{
    _window = create_window();

    _main_shader.create("shaders/vs_main.vs", "shaders/fs_main.fs");
    _text_shader.create("shaders/vs_text.vs", "shaders/fs_text.fs");

    if(_window == NULL){
        _running = false;
        return;        
    }else{
        _running = true;
    }
    _last_key_time = glfwGetTime();

    init_vertex_arrays();
    load_font("textures/font/technicality1.ttf", text_hei);
    init_textures();
    _map.create(TILESX, TILESY);
}


void FrontierGame::play()
{
    while(!glfwWindowShouldClose(_window))
    {
        /*
        if(resized == true){
            main_menu.resize(glm::vec2(scr_wid/2-menu_wid, scr_hei/4), 
                    glm::vec2(menu_wid, scr_hei*3/4)); 
            printf("resizing\n");
            resized = false;
        }
        */

        delta_time = current_time - last_time;
        last_time = current_time;  
        process_input(_window, _map, delta_time);
        draw();


        current_time = glfwGetTime();


        glfwWaitEvents();
    }


}

void FrontierGame::draw()
{
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        _map.draw(_main_shader);

        _text_shader.use();
        _text_shader.setMat4("projection", glm::ortho(-scr_wid/2, scr_wid/2, -scr_hei/2, scr_hei/2, -20.0f, 10.0f));
        //main_menu.draw(_main_shader, _text_shader);

        //text_shader.setMat4("projection", projection);
        //text_shader.setMat4("view", glm::mat4(1.0f));
        //render_text(text_shader, "frontier", -scr_wid/2+10, scr_hei/2-text_hei,0.5f, text_color);

        glfwSwapBuffers(_window);

}

bool FrontierGame::process_input(GLFWwindow *window, Map& map ,float dt)
{

    bool key_pressed = false;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float new_time = glfwGetTime();

    if((new_time - _last_key_time) > TURN_LENGTH){

        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            map.turn_player(LEFT);
            //if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
            //}else{ TODO: Use ctrl key to switch A and D from turn to strafe
            //}
        }if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            map.turn_player(RIGHT);

        }if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){ // move left
            map.move_player(FORWARD);
            key_pressed = true;
        }if(glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS){
            map.zoom(0.5f);
            key_pressed = true;
        }if(glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS){
            map.zoom(-0.5f);
            key_pressed = true;
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
            key_pressed = true;
        }
        _last_key_time = glfwGetTime();
    }
    return key_pressed;
}


bool FrontierGame::running()
{
    return _running;
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

    //glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendEquation(GL_FUNC_SUBTRACT);
    //
    int h, w;
    glfwGetFramebufferSize(window, &w, &h);
    scr_wid = w;
    scr_hei = h;

    return window;
}



void FrontierGame::framebuffer_size_callback(GLFWwindow* window, int wid, int hei)
{
    //glfwGetWindowSize(window, &w, &h);
    //resized = true;
    glViewport(0,0,wid,hei);

    int h, w;
    glfwGetFramebufferSize(window, &w, &h);
    scr_wid = w;
    scr_hei = h;
    
    printf("window size %f, %f\n", scr_wid, scr_hei);
}

void FrontierGame::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    printf("mouse button pressed\n");

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        /*
        if(main_menu.is_clicked(glm::vec2(x, y))){
            main_menu.click(glm::vec2(x, y));
        }
        */
    }
}
