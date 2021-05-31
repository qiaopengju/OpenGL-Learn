glad = /Users/kkyou/Development/Lib/glad/src/glad.c
source = main.cpp 
object := $(source:%.cpp=%.o)
source += $(glad)
head = Shader.h

learnOpenGL: $(object)
	clang++ -o learnOpenGL -lglfw3 -framework OpenGL -framework IOKit -framework CoreVideo -framework Cocoa $(object) $(glad)
	./learnOpenGL

2dcolor: colorRectangle.cpp
	clang++ -o learnOpenGL -lglfw3 -framework OpenGL -framework IOKit -framework CoreVideo -framework Cocoa colorRectangle.cpp $(glad)

2dtexture: 2DTexture.cpp
	clang++ -o learnOpenGL -lglfw3 -framework OpenGL -framework IOKit -framework CoreVideo -framework Cocoa 2DTexture.cpp $(glad)

$(object:%.o): $(object:%.cpp)

.PHONY: clean
clean:
	-rm $(object) learnOpenGL