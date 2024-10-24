#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H


#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	//reference id of the shader programe
	GLuint ID;
	//constructor that build the shader program from 2 different shsderd
	Shader(const char* vertexFile, const char* fragmentFile);
	//activate the shader program
	void Activate();
	//deletes the shader program
	void Delete();
private:
	// check if the different shader have compiler properly
	void compilerErrors(unsigned int shader, const char* type);
};

#endif // !SHADER_CLASS_H
