CC=g++
CFLAGS=-g -Wall

all:CreatingOpenGL3_3Window

CreatingOpenGL3_3Window:OpenGLControl.o Lin_OpenGLApp.o RenderScene.o
	$(CC) $(CFLAGS) $@ $^ -lGL -lGLEW -lglm -lGLUT -lpthread
OpenGLControl.o: OpenGLControl.cpp OpenGLControl.h common_header.h

Lin_OpenGLApp.o: Lin_OpenGLApp.cpp Lin_OpenGLApp.h OpenGLControl.h common_header.h

RenderScene.o: RenderScene.cpp Lin_OpenGLApp.h common_header.h

clean:
	rm -f CreatingOpenGL3_3Window *.o

