#include "utils.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

//#include "rands.h"

bool mouse_clicked = false;;
glm::vec2 click_pos = glm::vec2(0, 0);
glm::vec2 mouse_pos = glm::vec2(0, 0);


const float TURN_LENGTH = 0.05;

float scr_wid = (float)DEFAULT_WID;
float scr_hei = (float)DEFAULT_HEI;

std::map<unsigned int, unsigned int> textures;
std::map<unsigned int, unsigned int> vertex_arrays;

std::map <char, Character> characters;

//float TILE_WID = 64.0f;
//float TILE_HEI = 258.0f;
//
short *rands = NULL;

float TILE_WID = 64.0f;
float TILE_HEI = 256.0f;

const float TILE_SURFACE_HEI = TILE_WID/2;

const float ZOOM_INC = 0.5f;
const float MAX_ZOOM = 16.0f;
const float MIN_ZOOM = 0.5f;


const float VOXEL_HEI = sqrt(TILE_WID * TILE_WID + TILE_WID * TILE_WID)/2; 
const float feature_wid = TILE_WID;
const float feature_hei = 240.0f;


const float ELEVATION_OFFSET = 0.1f;
//const int MAX_ELEVATION = 64;
const int MAX_ELEVATION = 128;
//const int MAX_ELEVATION = 34;
const int MIN_ELEVATION = 0;
const int WATER_LEVEL = 1;



const glm::vec3 text_color(0.1f, 0.1f, 0.1f);
const float text_hei = 40;


unsigned int text_vao;
unsigned int text_vbo;


void render_text(Shader &s, std::string text, float x, float y, float scale, glm::vec3 color)
{
    // activate corresponding render state	
    s.use();
    s.setVec3("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(text_vao);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = characters[*c];

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;
        // update VBO for each character
        float verts[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, text_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


Direction reverse_direction(Direction dir)
{
    if(dir == NORTH)
        dir = SOUTH;
    else if(dir == NORTHEAST)
        dir = SOUTHWEST;
    else if(dir == EAST)
        dir = WEST;
    else if(dir == SOUTHEAST)
        dir = NORTHWEST;
    else if(dir == SOUTH)
        dir = NORTH;
    else if(dir == SOUTHWEST)
        dir = NORTHEAST;
    else if(dir == WEST)
        dir = EAST;
    else if(dir == NORTHWEST)
        dir = SOUTHEAST;

    return dir;
}


// place tile (ndc) 
glm::vec2 place_tile(glm::vec2 cart, float wid, float hei)
{
    return glm::vec2(
            (cart.x-cart.y) * (wid/2.0f), 
            (cart.x+cart.y) * (hei/2.0f));
}



glm::vec3 iso_to_cartesian(glm::vec3 iso)
{
    return glm::vec3 (
            (2 * (iso.y + iso.x)),
            (4 * (iso.y - iso.x)),
            iso.z);
}

glm::vec2 grad(glm::vec2 p){
    glm::vec2 r(((float)rands[int(p.x)%512]/512)*2.0f-1.0f, ((float)rands[int(p.y)%512]/512.0f)*2.0f-1.0f);
    return glm::normalize(r);
}


float fade(float t) {
  return 6.0 * t*t*t*t*t - 15.0 * t*t*t*t + 10.0 * t*t*t;
}

float noise(glm::vec2 p){
    // points
    glm::vec2 p0 = glm::vec2(floor(p.x), floor(p.y));
    glm::vec2 p1 = glm::vec2(p0.x + 1.0, p0.y);
    glm::vec2 p2 = glm::vec2(p0.x,       p0.y + 1.0);
    glm::vec2 p3 = glm::vec2(p0.x + 1.0, p0.y + 1.0);
    

    // gradients at each corner
    glm::vec2 g0 = grad(p0);
    glm::vec2 g1 = grad(p1);
    glm::vec2 g2 = grad(p2);
    glm::vec2 g3 = grad(p3);

    // offset into the cube
    float t0 = p.x - p0.x; 
    float t1 = p.y - p0.y;

    float fade_t0 = fade(t0); 
    float fade_t1 = fade(t1); 
    
    float p0p1 = (1.0 - fade_t0) * glm::dot(g0, (p - p0)) + fade_t0 * glm::dot(g1, (p - p1)); 
    float p2p3 = (1.0 - fade_t0) * glm::dot(g2, (p - p2)) + fade_t0 * glm::dot(g3, (p - p3)); 
    
    return (1.0 - fade_t1) * p0p1 + fade_t1 * p2p3;
}

float vertices[] = {
    1.0f,  0.0f,   // top right
    1.0f, -1.0f,   // bottom rig
    0.0f, -1.0f,   // bottom lef
    0.0f,  0.0f,   // top left 
};

float texcoords[] = {
    1.0f, 1.0f,   // top right
    1.0f, 0.0f,   // bottom rig
    0.0f, 0.0f,   // bottom lef
    0.0f, 1.0f,   // top left 
};

unsigned int indices[] = {
    0, 1, 3, 
    3, 2, 1,
};

//bool contains(glm::vec2 point, glm::vec2 pos, glm::vec2 size)
//{
//    if (pos.x < point.x && point.x < pos.x + size.x && pos.y < point.y && point.y < pos.y + size.y) {
//        return true;
//    }
//    return false;
//}

short *init_rands(int seed_size)
{
    rands = new short[seed_size];
    for (int i = 0; i < seed_size; i++) {
        rands[i] = i;
    }
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(rands, rands + seed_size, std::default_random_engine(seed));
}
//float noise(glm::vec2 p){
//    // points
//    glm::vec2 p0 = glm::vec2(floor(p.x), floor(p.y));
//    glm::vec2 p1 = glm::vec2(p0.x + 1.0, p0.y);
//    glm::vec2 p2 = glm::vec2(p0.x,       p0.y + 1.0);
//    glm::vec2 p3 = glm::vec2(p0.x + 1.0, p0.y + 1.0);
//    
//
//    // gradients at each corner
//    glm::vec2 g0 = grad(p0);
//    glm::vec2 g1 = grad(p1);
//    glm::vec2 g2 = grad(p2);
//    glm::vec2 g3 = grad(p3);
//
//    // offset into the cube
//    float t0 = p.x - p0.x; 
//    float t1 = p.y - p0.y;
//
//    float fade_t0 = fade(t0); 
//    float fade_t1 = fade(t1); 
//    
//    float p0p1 = (1.0 - fade_t0) * glm::dot(g0, (p - p0)) + fade_t0 * glm::dot(g1, (p - p1)); 
//    float p2p3 = (1.0 - fade_t0) * glm::dot(g2, (p - p2)) + fade_t0 * glm::dot(g3, (p - p3)); 
//    
//    return (1.0 - fade_t1) * p0p1 + fade_t1 * p2p3;
//}
