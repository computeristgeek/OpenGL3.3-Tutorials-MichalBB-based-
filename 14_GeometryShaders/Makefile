CC=g++ -I/usr/include/freetype2/
CFLAGS=-ggdb -Wall

all:GeometryShaders

GeometryShaders:OpenGLControl.o Lin_OpenGLApp.o shaders.o vertexBufferObject.o texture.o flyingCamera.o static_geometry.o freeTypeFont.o skybox.o fog.o dirLight.o pointLight.o RenderScene.o
	$(CC) -o $@ $(CFLAGS) $^ -lGLEW -lGLU -lglfw3 -lrt -lXrandr -lXinerama -lXi -lXcursor -lGL -lm -ldl -lXrender -ldrm -lXdamage -lX11-xcb -lxcb-glx -lxcb-dri2 -lxcb-dri3 -lxcb-present -lxcb-sync -lxshmfence -lXxf86vm -lXfixes -lXext -lX11 -lpthread -lxcb -lXau -lXdmcp -lfreeimage -lfreetype
OpenGLControl.o: OpenGLControl.cpp OpenGLControl.h common_header.h

Lin_OpenGLApp.o: Lin_OpenGLApp.cpp Lin_OpenGLApp.h OpenGLControl.h common_header.h

shaders.o: shaders.cpp shaders.h common_header.h

vertexBufferObject.o: vertexBufferObject.cpp vertexBufferObject.h common_header.h

texture.o: texture.cpp texture.h common_header.h

flyingCamera.o: flyingCamera.cpp flyingCamera.h Lin_OpenGLApp.h common_header.h

static_geometry.o: static_geometry.cpp static_geometry.h vertexBufferObject.h common_header.h

freeTypeFont.o: freeTypeFont.cpp freeTypeFont.h texture.h shaders.h vertexBufferObject.h common_header.h
	$(CC) -c freeTypeFont.cpp -o freeTypeFont.o

skybox.o: skybox.cpp skybox.h texture.h vertexBufferObject.h common_header.h

fog.o: fog.cpp fog.h shaders.h common_header.h

pointLight.o: pointLight.cpp pointLight.h shaders.h common_header.h

dirLight.o: dirLight.cpp dirLight.h shaders.h common_header.h

RenderScene.o: RenderScene.cpp Lin_OpenGLApp.h shaders.h static_geometry.h freeTypeFont.h flyingCamera.h skybox.h dirLight.h common_header.h
	$(CC) -c RenderScene.cpp -o RenderScene.o

clean:
	rm -f GeometryShaders *.o
