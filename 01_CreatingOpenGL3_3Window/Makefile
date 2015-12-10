CC=g++
CFLAGS=-ggdb -Wall

all:CreatingOpenGL3_3Window

CreatingOpenGL3_3Window:OpenGLControl.o Lin_OpenGLApp.o RenderScene.o
	$(CC) -o $@ $(CFLAGS) $^ -lGLEW -lGLU -lglfw3 -lrt -lXrandr -lXinerama -lXi -lXcursor -lGL -lm -ldl -lXrender -ldrm -lXdamage -lX11-xcb -lxcb-glx -lxcb-dri2 -lxcb-dri3 -lxcb-present -lxcb-sync -lxshmfence -lXxf86vm -lXfixes -lXext -lX11 -lpthread -lxcb -lXau -lXdmcp 
OpenGLControl.o: OpenGLControl.cpp OpenGLControl.h common_header.h

Lin_OpenGLApp.o: Lin_OpenGLApp.cpp Lin_OpenGLApp.h OpenGLControl.h common_header.h

RenderScene.o: RenderScene.cpp Lin_OpenGLApp.h common_header.h

clean:
	rm -f CreatingOpenGL3_3Window *.o

