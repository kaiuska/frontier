
objects = tile.o feature.o utils.o menu.o button.o shader.o sprite.o person.o map.o frontier_game.o resources.o

libs = -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm -lglfw -lfreetype
cflags = --std=c++17 `pkg-config --cflags freetype2` -g


all: frontier $(objects)


$(objects): %.o : %.cpp                                                   
	$(CC) -c $(libs) $(cflags) $< -o $@

frontier: frontier.cpp glad/glad.c $(objects) 
	g++ $^ $(libs) $(cflags) -o $@

clean:
	rm frontier *.o 
