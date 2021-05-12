#include "resources.h"


std::map<FeatureType, FeatureDef> feature_defs;
//type                          type,                       texture,                name,                   subtex, passable, buildable

FeatureType feature_list[] = {
    // wild
    TREE,
    OLD_TREE,
    BUSH,
    FERN,
    LONG_GRASS,


    // structures
    CABIN_WALL,
    
    CABIN_WALL_NORTH_CORNER,
    CABIN_WALL_NORTHEAST,
    CABIN_WALL_EAST_CORNER,
    CABIN_WALL_SOUTHEAST,
    CABIN_WALL_SOUTH_CORNER,
    CABIN_WALL_SOUTHWEST,
    CABIN_WALL_WEST_CORNER,
    CABIN_WALL_NORTHWEST,


    // agriculture
    CORN_STAGE_1,
    CORN_STAGE_2,
    CORN_STAGE_3,
    CORN_STAGE_4,
    CORN_STAGE_5,
    NO_FEATURE
};



ControlType control_list[] = {
    MAIN_MENU,
    BUTTON,
    NO_CONTROL
};



TileType tile_list[] = {
    GREEN_GRASS,
    DEAD_GRASS,
    WATER,
    NO_TILE
};


PersonType person_list[] = {
    PLAYER
};



void init_textures()
{
    textures[DEAD_GRASS]    = load_texture("textures/tiles/dead_grass1-tall-iso.png", 1);
    textures[TILLED_SOIL]   = load_texture("textures/tiles/tilled_soil-iso.png", 1); 
    textures[WATER]         = load_texture("textures/tiles/water3-64x286-iso.png", 1);
    textures[GREEN_GRASS]   = load_texture("textures/tiles/green_grass2-tall-iso.png", 1);
    textures[TREE]          = load_texture("textures/features/tree1-64x120-iso.png", 1);
    textures[OLD_TREE]      = load_texture("textures/features/tree1-64x184-iso.png", 1);
    textures[BUSH]          = load_texture("textures/features/blueberry_bush2-64x110-iso.png", 1);
    textures[LONG_GRASS]    = load_texture("textures/features/long_grass2-iso.png", 1);
    textures[FERN]          = load_texture("textures/features/fern1-64x120-iso.png", 1);
    textures[BUTTON]        = load_texture("textures/controls/button1_iso.png", 1);
    textures[MAIN_MENU]     = load_texture("textures/controls/main_menu_background.png", 1);
    textures[PLAYER]        = load_texture("textures/people/player1-iso.png", 1);
    textures[CORN]          = load_texture("textures/features/agriculture/corn-iso.png", 1);
    textures[CABIN_WALL]    = load_texture("textures/features/building/log_cabin_walls-iso.png", 1);
    textures[DIRECTION_INDICATOR] = load_texture("textures/people/direction_indicators-iso.png", 1);
    

    feature_defs[ TREE                    ] =  { TREE,                     textures[TREE],         "Tree",                 0,          0,  false }; 
    feature_defs[ OLD_TREE                ] =  { OLD_TREE,                 textures[OLD_TREE],     "Old Tree",             0,          0,  false }; 
    feature_defs[ BUSH                    ] =  { BUSH,                     textures[BUSH],         "Bush",                 0,          1,  false }; 
    feature_defs[ FERN                    ] =  { FERN,                     textures[FERN],         "Sword Fern",           0,          1,  false }; 
    feature_defs[ LONG_GRASS              ] =  { LONG_GRASS,               textures[LONG_GRASS],   "Long Grass",           0,          1,  false }; 

    feature_defs[ CABIN_WALL_NORTH_CORNER ] =  { CABIN_WALL_NORTH_CORNER,  textures[CABIN_WALL],   "Cabin Wall N",     NORTH,       0,  true }; 
    feature_defs[ CABIN_WALL_NORTHEAST    ] =  { CABIN_WALL_NORTHEAST,     textures[CABIN_WALL],   "Cabin Wall NE",   NORTHEAST,    0,  true }; 
    feature_defs[ CABIN_WALL_EAST_CORNER  ] =  { CABIN_WALL_EAST_CORNER,   textures[CABIN_WALL],   "Cabin Wall E",      EAST,       0,  true }; 
    feature_defs[ CABIN_WALL_SOUTHEAST    ] =  { CABIN_WALL_SOUTHEAST,     textures[CABIN_WALL],   "Cabin Wall SE",   SOUTHEAST,    0,  true }; 
    feature_defs[ CABIN_WALL_SOUTH_CORNER ] =  { CABIN_WALL_SOUTH_CORNER,  textures[CABIN_WALL],   "Cabin Wall S",     SOUTH,       0,  true }; 
    feature_defs[ CABIN_WALL_SOUTHWEST    ] =  { CABIN_WALL_SOUTHWEST,     textures[CABIN_WALL],   "Cabin Wall SW",   SOUTHWEST,    0,  true }; 
    feature_defs[ CABIN_WALL_WEST_CORNER  ] =  { CABIN_WALL_WEST_CORNER,   textures[CABIN_WALL],   "Cabin Wall W",      WEST,       0,  true }; 
    feature_defs[ CABIN_WALL_NORTHWEST    ] =  { CABIN_WALL_NORTHWEST,     textures[CABIN_WALL],   "Cabin Wall NW",   NORTHWEST,    0,  true }; 


    feature_defs[ CORN_STAGE_0            ] =  { CORN_STAGE_0,     textures[CORN],   "Sweet Corn",   0,  1, false }; 
    feature_defs[ CORN_STAGE_1            ] =  { CORN_STAGE_1,     textures[CORN],   "Sweet Corn",   1,  1, false }; 
    feature_defs[ CORN_STAGE_2            ] =  { CORN_STAGE_2,     textures[CORN],   "Sweet Corn",   2,  1, false }; 
    feature_defs[ CORN_STAGE_3            ] =  { CORN_STAGE_3,     textures[CORN],   "Sweet Corn",   3,  1, false }; 
    feature_defs[ CORN_STAGE_4            ] =  { CORN_STAGE_4,     textures[CORN],   "Sweet Corn",   4,  1, false }; 
    feature_defs[ CORN_STAGE_5            ] =  { CORN_STAGE_5,     textures[CORN],   "Sweet Corn",   5,  1, false }; 

 
}










// overwrites map::characters with characters from font
void load_font(const char* font, int hei)
{
    FT_Library ft;
    if(FT_Init_FreeType(&ft)){
        fprintf(stderr, "ERROR::FREETYPE: Failed to init FreeType Library\n");
        return;
    }
    FT_Face face;
    if(FT_New_Face(ft, font, 0, &face)){
        fprintf(stderr, "ERROR::FREETYPE: Failed to load font %s\n", font);
        return;
    }
    FT_Set_Pixel_Sizes(face, 0, hei);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for(unsigned char ch = 0; ch < 128; ch++){
        if(FT_Load_Char(face, ch, FT_LOAD_RENDER)){
            fprintf(stderr, "ERROR::FREETYPE: Failed to load glyph \"%c\"\n", ch);
            continue;
        }
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                    face->glyph->bitmap.rows, 0, GL_RED,GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        Character character = {
                texture, 
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };

        characters.insert(std::pair<char, Character>(ch, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}


unsigned int load_texture(const char *fname, int alpha)                                 
{                                                                                       
    stbi_set_flip_vertically_on_load(true);
    unsigned int texture;                                                               
    glGenTextures(1, &texture);                                                         
    glBindTexture(GL_TEXTURE_2D, texture);                                              
    // load and generate the texture                                                    
    int width, height, nrChannels;                                                      
    unsigned char *data = stbi_load(fname, &width, &height, &nrChannels, 0);            
    if (data)                                                                           
    {                                                                                   
        if(alpha == 0)                                                                  
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        else                                                                            
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);                                                
                                                                                        
        // set the texture wrapping/filtering options (on the currently bound texture ob
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);                   
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);                   
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);               
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);               
    }                                                                                   
    else                                                                                
    {                                                                                   
        std::cerr << "Failed to load texture " << fname << std::endl;                   
        exit(EXIT_FAILURE);                                                             
    }                                                                                   
    stbi_image_free(data);                                                              
    return texture;                                                                     
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

glm::vec2 get_tex_dimensions(unsigned int textureID)
{
    int w, h;
    glBindTexture(GL_TEXTURE_2D, textureID);
    glActiveTexture(GL_TEXTURE0);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    return glm::vec2(w, h); 
}


