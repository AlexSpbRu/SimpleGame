#include "stdafx.h"


#ifdef OPENGL
#include <gl/gl.h>	
#include <gl/glext.h>
#endif

#ifdef OPENGLES20
#include <EGL/egl.h>
#include <GLES2/gl2.h>	
#endif

//#include <string>
//#include <vector>
#include <iostream>
#include <fstream>

#include "Math3D.h"
#include "utils.h"
#include "Shader.h"



Shader::Shader(const char* path_vert_shader, const char* path_frag_shader){
	create_program(path_vert_shader, path_frag_shader);
}

Shader::~Shader(){
	//TODO Figure out what to place here...
}

GLuint Shader::getShader(){
	return _shaderProgram;
}

// Read a shader source from a file
// store the shader source in a std::vector<char>
void Shader::read_shader_src(const char *fname, std::vector<char> &buffer) {
        std::ifstream in;
        in.open(fname, std::ios::binary);

        if(in.is_open()) {
                // Get the number of bytes stored in this file
                in.seekg(0, std::ios::end);
                size_t length = (size_t)in.tellg();

                // Go to start of the file
                in.seekg(0, std::ios::beg);

                // Read the content of the file in a buffer
                buffer.resize(length + 1);
                in.read(&buffer[0], length);
                in.close();
                // Add a valid C - string end
                buffer[length] = '\0';
        }
        else {
                std::cerr << "Unable to open " << fname << " I'm out!" << std::endl;
                return;
        }
}


// Compile a shader
GLuint Shader::load_and_compile_shader(const char *fname, GLenum shaderType) {
        // Load a shader from an external file
        std::vector<char> buffer;
        read_shader_src(fname, buffer);
        const char *src = &buffer[0];
        // Compile the shader
		GLuint shader = compile_shader(src, shaderType);
        return shader;
}

GLuint Shader::compile_shader(const char *programm, GLenum shaderType)
{
	// Compile the shader
    GLuint shader = glCreateShader(shaderType);
	auto err = glGetError();
    glShaderSource(shader, 1, &programm, NULL);
    glCompileShader(shader);
    // Check the result of the compilation
    GLint test;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &test);
    if(!test) {
            std::cerr << "Shader compilation failed with this message:" << std::endl;
            std::vector<char> compilation_log(512);
            glGetShaderInfoLog(shader, compilation_log.size(), NULL, &compilation_log[0]);
            std::cerr << &compilation_log[0] << std::endl;
			std::string strError(compilation_log.begin(), compilation_log.end());
			IO_TRACE("Shader compilation failed with this message: %s\n", &compilation_log[0]);
            return 0;
    }
    return shader;
}

// Create a program from two shaders
GLboolean Shader::create_program_from_file(const char *path_vert_shader, const char *path_frag_shader)
{
	std::vector<char> vert_shader, frag_shader;
	read_shader_src(path_vert_shader, vert_shader);
	read_shader_src(path_frag_shader, frag_shader);
	return create_program(vert_shader.data(), frag_shader.data());
}

GLboolean Shader::create_program(const char *vert_shader, const char *frag_shader)
{
	// Load and compile the vertex and fragment shaders
	_vertexShader = compile_shader(vert_shader, GL_VERTEX_SHADER);
	_fragmentShader = compile_shader(frag_shader, GL_FRAGMENT_SHADER);
	if (_vertexShader == 0 || _fragmentShader == 0)
		return false;
	// Attach the above shader to a program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, _vertexShader);
	glAttachShader(shaderProgram, _fragmentShader);

	// Flag the shaders for deletion
	glDeleteShader(_vertexShader);
	glDeleteShader(_fragmentShader);

	// Link and use the program
	glLinkProgram(shaderProgram);

	_shaderProgram = shaderProgram;
	return true;
}

void Shader::use(){
        glUseProgram(_shaderProgram);
}
