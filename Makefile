GLAD = deps/glad/glad.c
IMGUI_DIR = deps/imgui
SOURCES = main.cpp ResourceManager.cpp Model.cpp Shader.cpp Texture.cpp BasicObjectRender.cpp
SOURCES += $(GLAD)
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp \
	$(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))  #addsuffix:加后缀; basename:去后缀文件名; notdir: 去前面目录名
INCLUDE = Shader.h Texture.h Camera.h Light.h Mesh.h Model.h Framebuffer.h Skybox.h BasicObjectRender.h
CLANG_FLAG =-I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -I./
LIBS = -lglfw3 -lassimp -framework OpenGL -framework IOKit -framework CoreVideo -framework Cocoa 
EXE = learnOpenGL

#debug:
#	@echo $(OBJS)
all: $(EXE) $(INCLUDE) $(SOURCES)	#当前进度
	clang++ -o $(EXE) $(CLANG_FLAG) $(LIBS) $(OBJS)
	./$(EXE)
########################
#Build rules
########################
main.o: $(INCLUDE) $(SOURCES)
	clang++ -c $(CLANG_FLAG) -o main.o main.cpp
%.o: %.cpp %.h
# $< depend file; $@ target file
# not all .cpp will be compiled, only compile needed
	clang++ -c $(CLANG_FLAG) -o $@ $<
%.o: $(IMGUI_DIR)/%.cpp
	clang++ -c $(CLANG_FLAG) -o $@ $<
%.o: $(IMGUI_DIR)/backends/%.cpp
	clang++ -c $(CLANG_FLAG) -o $@ $<
%.o: former/%.cpp
	clang++ -c $(CLANG_FLAG) -o $@ $<
glad.o: $(GLAD)
	clang++ -c $(CLANG_FLAG) -o $@ $<
$(EXE): $(OBJS)

.PHONY: clean
clean:
	-rm *.o $(EXE)
