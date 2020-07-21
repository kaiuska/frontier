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
//#include "rands.h"
#include "sprite.h"
#include "map.h"

#include "person.h"
#include <algorithm>


MainMenu main_menu;


// TODO: Implement "instanced rendering" of tiles
float vertices[] = {
    1.0f,  0.0f,   // top right
    1.0f, -1.0f,   // bottom rig
    0.0f, -1.0f,   // bottom lef
    0.0f,  0.0f,    // top left 
};

float texcoords[] = {
    1.0f, 1.0f,   // top right
    1.0f, 0.0f,   // bottom rig
    0.0f, 0.0f,   // bottom lef
    0.0f, 1.0f,    // top left 
};
unsigned int indices[] = {
    0, 1, 3, 
    3, 2, 1,
};


glm::ivec2 center; 

int feature_index = 1;

glm::vec3 camera_pos(-0.0f, 0.0f, 1.0f);
glm::vec3 camera_front(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up(0.0f, 1.0f, 0.0f);


Tile tiles[TILESX][TILESY];
Feature features[TILESX][TILESY];


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int wid, int hei);
bool process_input(GLFWwindow *window, Map& map, float dt );

GLFWwindow *create_window();

float key_time = 0.0f;

void init_vertex_arrays();

bool resized = false;

void init_textures();
void init_map(int wid, int hei);


Person player;

//Map map(TILESX, TILESY);



int main()
{

    GLFWwindow * window = create_window();

    if(window == NULL){
        return -1;        
    }

    
    Shader main_shader("shaders/vs_main.vs", "shaders/fs_main.fs");
    Shader text_shader("shaders/vs_text.vs", "shaders/fs_text.fs");


    init_vertex_arrays();
    load_font("textures/font/technicality1.ttf", text_hei);
    init_textures();
    //init_map(TILESX, TILESY);


    //center = glm::ivec2(TILESX/2, TILESY/2);
    printf("center %d %d\n", center.x ,center.y);
    //player.init(center, tiles[(int)center.x][(int)center.y].get_position(), PLAYER);


    float delta_time = 0.0f;
    float last_time = 0.0f;
    float current_time = glfwGetTime();

    main_menu.init(glm::vec2(scr_wid/2-menu_wid, scr_hei/4), glm::vec2(menu_wid, scr_hei*3/4)); 
    

    Map map(TILESX, TILESY);
    


    /* Game Loop **************************************************************/
    while(!glfwWindowShouldClose(window))
    {
        if(resized == true){
            main_menu.resize(glm::vec2(scr_wid/2-menu_wid, scr_hei/4), 
                    glm::vec2(menu_wid, scr_hei*3/4)); 
            printf("resizing\n");
            resized = false;
        }
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        delta_time = current_time - last_time;
        last_time = current_time;  
        process_input(window, map, delta_time);


        map.draw(main_shader);


        text_shader.use();
        text_shader.setMat4("projection", glm::ortho(-scr_wid/2, scr_wid/2, -scr_hei/2, scr_hei/2, -20.0f, 10.0f));
        main_menu.draw(main_shader, text_shader);

        //text_shader.setMat4("projection", projection);
        //text_shader.setMat4("view", glm::mat4(1.0f));
        //render_text(text_shader, "frontier", -scr_wid/2+10, scr_hei/2-text_hei,0.5f, text_color);

        current_time = glfwGetTime();


        glfwSwapBuffers(window);
        glfwWaitEvents();
    }

    glfwTerminate();



    return 0;
}


bool process_input(GLFWwindow *window, Map& map ,float dt)
{

    bool key_pressed = false;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float new_time = glfwGetTime();

    if((new_time - key_time) > 0.00f){

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
            if(feature_list[feature_index] == NO_FEATURE){
                feature_index = 0;    
            }else{
                feature_index++;
            }
            tiles[int(center.x)][int(center.y)].set_feature(feature_list[feature_index]);
            key_pressed = true;
        }
        key_time = glfwGetTime();
    }
    return key_pressed;
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        if(main_menu.is_clicked(glm::vec2(x, y))){
            main_menu.click(glm::vec2(x, y));
        }
    }
}





void framebuffer_size_callback(GLFWwindow* window, int wid, int hei)
{
    //glfwGetWindowSize(window, &w, &h);
    resized = true;
    glViewport(0,0,wid,hei);

    int h, w;
    glfwGetFramebufferSize(window, &w, &h);
    scr_wid = w;
    scr_hei = h;
    
    printf("window size %f, %f\n", scr_wid, scr_hei);
}



void init_vertex_arrays()
{
    unsigned int tile_vao, tile_vbo, tile_ebo, tex_vbo; 
    glGenVertexArrays(1, &tile_vao);
    glGenBuffers(1, &tile_vbo); 
    glGenBuffers(1, &tex_vbo);
    glGenBuffers(1, &tile_ebo);


    glBindVertexArray(tile_vao);

    // setup vertices
    glBindBuffer(GL_ARRAY_BUFFER, tile_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // setup texcoords
    glBindBuffer(GL_ARRAY_BUFFER, tex_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW);
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2*sizeof(float)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    printf("sizeof(indices) %d\n", sizeof(indices));
    // setup indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tile_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);


    sprite_vao = tile_vao;
    sprite_vbo = tile_vbo;

    int i = 0;
    while(tile_list[i] != TileType::NO_TILE) {
        vertex_arrays[tile_list[i]] = tile_vao;
        i++;
    }
    i = 0;
    while(feature_list[i] != FeatureType::NO_FEATURE) {
        vertex_arrays[feature_list[i]] = tile_vao;
        i++;
    }
    i = 0;
    while(control_list[i] != ControlType::NO_CONTROL){
        vertex_arrays[control_list[i]] = tile_vao;
        i++;
    }

    // vertex array for text
    glGenVertexArrays(1, &text_vao);
    glGenBuffers(1, &text_vbo);
    glBindVertexArray(text_vao);
    glBindBuffer(GL_ARRAY_BUFFER, text_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


}

void init_textures()
{
    // TODO: Swap textures out for version without bounding box around them to
    // minimize size, and thus speed (to avoid having to drop un-needed pixels)
    //textures[GREEN_GRASS] = load_texture("textures/grass1-64x74-96dpi.png", 1);
    textures[DEAD_GRASS] = load_texture("textures/tiles/dead_grass1-tall-iso.png", 1);
    textures[WATER] = load_texture("textures/tiles/water1-56x288-iso.png", 1);
    textures[GREEN_GRASS] = load_texture("textures/tiles/green_grass2-tall-iso.png", 1);

    textures[TREE] = load_texture("textures/features/tree1-64x120-iso.png", 1);
    textures[OLD_TREE] = load_texture("textures/features/tree1-64x184-iso.png", 1);
    textures[BUSH] = load_texture("textures/features/blueberry_bush2-64x110-iso.png", 1);
    textures[LONG_GRASS] = load_texture("textures/features/long_grass2-iso.png", 1);
    textures[FERN] = load_texture("textures/features/fern1-64x120-iso.png", 1);

    textures[BUTTON] = load_texture("textures/controls/button1_iso.png", 1);
    textures[MAIN_MENU] = load_texture("textures/controls/main_menu_background.png", 1);

    textures[PLAYER] = load_texture("textures/people/player1-iso.png", 1);

    textures[TILLED_SOIL]  = load_texture("textures/features/agriculture/tilled_soil-iso.png", 1); 
    textures[CORN_STAGE_1] = load_texture("textures/features/agriculture/corn_stage1-iso.png", 1); 
    textures[CORN_STAGE_2] = load_texture("textures/features/agriculture/corn_stage2-iso.png", 1);
    textures[CORN_STAGE_3] = load_texture("textures/features/agriculture/corn_stage3-iso.png", 1);
    textures[CORN_STAGE_4] = load_texture("textures/features/agriculture/corn_stage4-iso.png", 1);
    textures[CORN_STAGE_5] = load_texture("textures/features/agriculture/corn_stage5-iso.png", 1);

    textures[DIRECTION_INDICATOR] = load_texture("textures/people/direction_indicators-iso.png", 1);
}




GLFWwindow *create_window()
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

