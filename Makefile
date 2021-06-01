GLAD = deps/glad/glad.c
IMGUI_DIR = deps/imgui
SOURCES += $(GLAD)
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp \
	$(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))  #addsuffix:加后缀; basename:去后缀文件名; notdir: 去前面目录名
INCLUDE = Shader.h
CLANG_FLAG =-I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends
LIBS = -lglfw3 -framework OpenGL -framework IOKit -framework CoreVideo -framework Cocoa 
EXE = learnOpenGL

#debug:
#	@echo $(OBJS)
all: $(EXE) main.o
	clang++ -o $(EXE) $(CLANG_FLAG) $(LIBS) $(OBJS) main.o
	./$(EXE)
2dcolor: colorRectangle.o
	clang++ -o $(EXE) $(LIBS) $(GLAD) colorRectangle.o 
	./$(EXE)
2dtexture: 2DTexture.o
	clang++ -o $(EXE) $(LIBS) $(GLAD) 2DTexture.o 
	./$(EXE)
demowindow: $(OBJS) addImgui.o
	clang++ -o demowindow $(CLANG_FLAG) $(LIBS) $(OBJS) addImgui.o
	./demowindow
transform: $(OBJS) transform.o
	clang++ -o $(EXE) $(CLANG_FLAG) $(LIBS) $(OBJS) transform.o
	./$(EXE)
3d: coordinate.o
	clang++ -o $(EXE) $(CLANG_FLAG) $(LIBS) $(OBJS) coordinate.o
	./$(EXE)


########################
#Build rules
########################
%.o: %.cpp
# $< depend file; $@ target file
# not all .cpp will be compiled, only compile needed
	clang++ -c $(CLANG_FLAG) -o $@ $<
%.o: $(IMGUI_DIR)/%.cpp
	clang++ -c $(CLANG_FLAG) -o $@ $<
%.o: $(IMGUI_DIR)/backends/%.cpp
	clang++ -c $(CLANG_FLAG) -o $@ $<
glad.o: $(GLAD)
	clang++ -c $(CLANG_FLAG) -o $@ $<
$(EXE): $(OBJS)

.PHONY: clean
clean:
	-rm *.o $(EXE)