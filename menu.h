#ifndef _MAIN_MENU_H_
#define _MAIN_MENU_H_
#include "utils.h"
#include "shader.h"
#include "button.h"
#include "sprite.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class MainMenu : public Sprite
{
public:
    MainMenu();
    MainMenu(glm::vec2 pos, glm::vec2 size);
    void init(glm::vec2 pos, glm::vec2 size);
    void draw(Shader& shader, Shader& text_shader);
    void resize(glm::vec2 pos, glm::vec2 size);
    //bool is_clicked(glm::vec2 mouse);
    void click(glm::vec2 mouse);
private:
    Button _button;
};


#endif
