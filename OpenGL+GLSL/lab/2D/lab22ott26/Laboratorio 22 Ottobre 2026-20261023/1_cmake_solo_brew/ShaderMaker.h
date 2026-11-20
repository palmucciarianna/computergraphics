#pragma once
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>

class ShaderMaker
{
    public:
		static GLuint createProgram(char* vertexfilename, char *fragmentfilename);
		static char* readShaderSource(const char* shaderFile);
	
    private:
		ShaderMaker() { }	
};