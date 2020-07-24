#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <unistd.h>

#include "utils.h"
#include "shader.h"
#include "sprite.h"

extern glm::vec3 button_bg_color;

class Button : public Sprite
{
public:
    Button();
    Button(std::string text, float text_scale, glm::vec2 pos, glm::vec2 size);
    void init(std::string text, float text_scale, glm::vec2 pos, glm::vec2 size);
    void draw(Shader& shader, Shader& text_shader);
    void set_position(glm::vec2 pos);
    glm::vec4 get_rect();
    void click();
    void set_text(const std::string& text);
    


private:
    std::string _text;
    glm::vec2 _text_pos;
    float _text_scale;
};

#endif
