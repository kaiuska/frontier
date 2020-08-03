#include "utils.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

//#include "rands.h"

bool mouse_clicked = false;;
glm::vec2 click_pos = glm::vec2(0, 0);

const float TURN_LENGTH = 0.05;

float scr_wid = (float)DEFAULT_WID;
float scr_hei = (float)DEFAULT_HEI;

std::map<unsigned int, unsigned int> textures;
std::map<unsigned int, unsigned int> vertex_arrays;

std::map <char, Character> characters;

//float TILE_WID = 64.0f;
//float TILE_HEI = 258.0f;

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
const int MAX_ELEVATION = 34;
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
  return t*t*t*(t*(t*6.0 - 15.0) + 10.0);
}
float noise(glm::vec2 p){
    glm::vec2 p0 = glm::vec2(floor(p.x), floor(p.y));
    glm::vec2 p1 = p0 + glm::vec2(1.0, 0.0);
    glm::vec2 p2 = p0 + glm::vec2(0.0, 1.0);
    glm::vec2 p3 = p0 + glm::vec2(1.0, 1.0);

    glm::vec2 g0 = grad(p0);
    glm::vec2 g1 = grad(p1);
    glm::vec2 g2 = grad(p2);
    glm::vec2 g3 = grad(p3);

    float t0 = p.x - p0.x; // the x offset in to the tile 
    float fade_t0 = fade(t0); /* Used for interpolation in horizontal direction */
    
    float t1 = p.y - p0.y;
    float fade_t1 = fade(t1); /* Used for interpolation in vertical direction. */
    
    /* Calculate dot products and interpolate.*/
    float p0p1 = (1.0 - fade_t0) * glm::dot(g0, (p - p0)) + fade_t0 * glm::dot(g1, (p - p1)); /* between upper two lattice points */
    float p2p3 = (1.0 - fade_t0) * glm::dot(g2, (p - p2)) + fade_t0 * glm::dot(g3, (p - p3)); /* between lower two lattice points */
    
    /* Calculate final result */
    return (1.0 - fade_t1) * p0p1 + fade_t1 * p2p3;
}

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

short rands[] = {
142,7,29,18,9,101,78,4,31,45,
188,1,64,36,219,59,107,242,52,50,
163,236,220,141,90,65,41,202,200,43,
144,174,225,114,140,125,139,39,216,230,
100,104,95,81,55,67,145,147,93,135,
110,63,128,30,255,51,42,146,20,178,
68,159,232,160,190,227,181,214,106,222,
28,127,237,57,49,2,24,129,243,150,
8,94,248,176,173,143,165,224,112,12,
88,87,102,192,15,235,14,247,133,229,
46,187,221,77,6,123,13,183,137,206,
209,33,233,231,26,199,80,196,53,91,
98,148,166,48,238,126,96,175,254,136,
161,226,207,37,244,109,149,76,158,240,
66,171,16,213,212,211,118,203,120,47,
155,3,251,184,23,228,217,115,62,10,210,
185,201,157,83,11,154,69,92,97,130,
84,249,252,22,234,152,19,32,208,246,
138,186,151,72,105,73,168,218,116,82,
177,179,162,198,180,132,204,34,111,99,
131,74,56,38,61,197,113,172,108,70,205,
25,253,21,40,215,241,182,156,194,103,
17,89,60,86,54,245,85,195,75,121,
117,5,79,250,134,153,223,122,27,170,
71,44,239,35,169,58,167,119,193,124,
191,164,189,0,
142,7,29,18,9,101,78,4,31,45,
188,1,64,36,219,59,107,242,52,50,
163,236,220,141,90,65,41,202,200,43,
144,174,225,114,140,125,139,39,216,230,
100,104,95,81,55,67,145,147,93,135,
110,63,128,30,255,51,42,146,20,178,
68,159,232,160,190,227,181,214,106,222,
28,127,237,57,49,2,24,129,243,150,
8,94,248,176,173,143,165,224,112,12,
88,87,102,192,15,235,14,247,133,229,
46,187,221,77,6,123,13,183,137,206,
209,33,233,231,26,199,80,196,53,91,
98,148,166,48,238,126,96,175,254,136,
161,226,207,37,244,109,149,76,158,240,
66,171,16,213,212,211,118,203,120,47,
155,3,251,184,23,228,217,115,62,10,210,
185,201,157,83,11,154,69,92,97,130,
84,249,252,22,234,152,19,32,208,246,
138,186,151,72,105,73,168,218,116,82,
177,179,162,198,180,132,204,34,111,99,
131,74,56,38,61,197,113,172,108,70,205,
25,253,21,40,215,241,182,156,194,103,
17,89,60,86,54,245,85,195,75,121,
117,5,79,250,134,153,223,122,27,170,
71,44,239,35,169,58,167,119,193,124,
191,164,189,0
};

