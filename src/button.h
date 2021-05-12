#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <unistd.h>

//#include "types.h"
#include "utils.h"
#include "shader.h"
#include "sprite.h"


class Button : public Sprite
{
public:
    Button();
    Button(std::string text, float text_scale, glm::vec2 pos, glm::vec2 size, int action);
    Button(std::string text, float text_scale, glm::vec2 pos, glm::vec2 size, int action, unsigned int texID, int subtex);
    void init(std::string text, float text_scale, glm::vec2 pos, glm::vec2 size, int action);
    virtual void draw(Shader& shader, Shader& text_shader);
    void set_position(glm::vec2 pos);
    glm::vec4 get_rect();
    void click();
    void set_text(const std::string& text);
    std::string get_text();
    void set_image(unsigned int texID, int subtex);
    int get_action();
    
    void set_selected(bool selected);


private:
    bool _selected;
    std::string _text;
    int _action;
    glm::vec2 _text_pos;
    float _text_scale;
    Sprite _image;
    bool _has_image;
};

#endif
