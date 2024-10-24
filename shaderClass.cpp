#include"shaderClass.h"

std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}
Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	//create vertex shder object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//attach vertex shader source to the vertex shader object 
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	//compile the vertex shader into machine code
	glCompileShader(vertexShader);
	//check if shader compiled successfully
	compilerErrors(vertexShader, "VERTEX");

	//create fragment shader object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//attach fragment shader source to the fragment shader object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	//complie the vertex shaderinto machine code
	glCompileShader(fragmentShader);

	//check if shader compiled successfully
	compilerErrors(fragmentShader, "FRAGMENT");

	//create shader program object and get its reference
	ID= glCreateProgram();

	//attach the vertex and fragment shader to the shader program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	//wrap-up/link ll the shader together into the shader program
	glLinkProgram(ID);

	//check if shader compiled successfully
	compilerErrors(ID,"PROGRAM");

	//delete the now useless vertex and fragment shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

 }
 
void Shader::Activate()
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}
void Shader::compilerErrors(unsigned int shader, const char* type)
{
	GLint hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for :" << type << "\n" << std::endl;
		}
		
	}
	else
	{
		glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for :" << type << "\n" << std::endl;
		}
	}
}