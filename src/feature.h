#ifndef _FEATURE_H_
#define _FEATURE_H_
#include <vector>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "utils.h"
#include "sprite.h"


extern const float voxel_hei;
//extern std::map<unsigned int, unsigned int> textures;
//extern std::map<unsigned int, unsigned int> vertex_arrays;


class Feature : public Sprite
{
public:
    Feature();
    Feature(glm::vec2 pos, FeatureType feature);
    void create(glm::vec2 pos, FeatureType feature);
    virtual void draw(Shader shader);

    void set_elevation(int elevation);
    void set_position(glm::vec2 pos);
    FeatureType get_type();

private:
    FeatureType _type;
    int _elevation;
};




#endif
