#ifndef SHADER_H_
#define SHADER_H_

#include <vector>

class Shader{
private:
	GLuint _shaderProgram = 0;
	GLuint _vertexShader = 0;
	GLuint _fragmentShader = 0;
public:
	Shader() = default;
	Shader(const char* path_vert_shader, const char* path_frag_shader);
	virtual ~Shader();

	void use();
	GLuint getShader();
	GLboolean create_program_from_file(const char *path_vert_shader, const char *path_frag_shader);
	GLboolean create_program( const char *vert_shader, const char *frag_shader);
private:
	GLuint load_and_compile_shader(const char *fname, GLenum shaderType);
	GLuint compile_shader( const char *programm, GLenum shaderType);
	void read_shader_src(const char *fname, std::vector<char> &buffer);
};


#endif /* SHADER_H_*/

