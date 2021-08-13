#ifndef _SPRITE_H_
#define _SPRITE_H_
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "utils.h"

#define SCALE_TO_TEX -1

extern unsigned int sprite_vao;
extern unsigned int sprite_vbo;

class Sprite
{
public:
    Sprite();
    Sprite(glm::vec2 pos, glm::vec2 size, unsigned int texID);
    void create(glm::vec2 pos, glm::vec2 size, unsigned int texID);
    void create(glm::vec2 pos, glm::vec2 size, unsigned int texID, int subtex);
    virtual void draw(Shader shader);

    void set_size(float x, float y);
    void set_size(glm::vec2 size);
    //void resize(glm::vec2 pos, glm::vec2 size);
    glm::vec2 get_size();


    virtual void set_position(float x, float y);
    virtual void set_position(glm::vec2 pos);

    glm::vec2 get_position();

    glm::vec2 get_tex_dimensions();

    void set_textureID(unsigned int texID);
    void set_vaoID(unsigned int vaoID);

    bool contains(glm::vec2 mouse);

    //virtual void click();
    //void set_elevation(int elevation);
    void set_subtex(int subtex);
    void set_highlight(glm::vec4 highlight);

protected:
    glm::vec2 _pos;
    glm::vec2 _size;
    unsigned int _textureID;
    unsigned int _vaoID;
    glm::vec4 _highlight;
    //int _elevation;
    glm::vec2 _tex_dimensions;
    int _subtex;
    //glm::vec4 _texture_rect;
    glm::mat4 _projection;
};


#endif
