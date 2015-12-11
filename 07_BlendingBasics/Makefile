CC=g++
CFLAGS=-ggdb -Wall

all:BlendingBasics

BlendingBasics:OpenGLControl.o Lin_OpenGLApp.o shaders.o vertexBufferObject.o texture.o walkingCamera.o RenderScene.o
	$(CC) -o $@ $(CFLAGS) $^ -lGLEW -lGLU -lglfw3 -lrt -lXrandr -lXinerama -lXi -lXcursor -lGL -lm -ldl -lXrender -ldrm -lXdamage -lX11-xcb -lxcb-glx -lxcb-dri2 -lxcb-dri3 -lxcb-present -lxcb-sync -lxshmfence -lXxf86vm -lXfixes -lXext -lX11 -lpthread -lxcb -lXau -lXdmcp -lfreeimage
OpenGLControl.o: OpenGLControl.cpp OpenGLControl.h common_header.h

Lin_OpenGLApp.o: Lin_OpenGLApp.cpp Lin_OpenGLApp.h OpenGLControl.h common_header.h

shaders.o: shaders.cpp shaders.h common_header.h

vertexBufferObject.o: vertexBufferObject.cpp vertexBufferObject.h common_header.h

texture.o: texture.cpp texture.h common_header.h

walkingCamera.o: walkingCamera.cpp walkingCamera.h Lin_OpenGLApp.h common_header.h

RenderScene.o: RenderScene.cpp Lin_OpenGLApp.h shaders.h common_header.h

clean:
	rm -f BlendingBasics *.o

