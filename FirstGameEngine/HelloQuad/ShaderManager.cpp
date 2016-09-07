#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
}

GLuint ShaderManager::loadShader(const char * file, GLenum shaderType)
{
	char* fileContents;
	std::ifstream inFileShader(file, std::ios::binary);
	if (inFileShader.is_open())
	{
		inFileShader.seekg(0, std::ios::end);
		unsigned int length = (unsigned int)inFileShader.tellg();
		inFileShader.seekg(0, std::ios::beg);

		fileContents = new char[length + 1];

		inFileShader.read(fileContents, length);
		fileContents[length] = '\0';

		inFileShader.close();
	}


	GLuint shaderIndex;

	shaderIndex = glCreateShader(shaderType);

	glShaderSource(shaderIndex, 1, &fileContents, 0);
	
	glCompileShader(shaderIndex);

	delete[] fileContents;

	GLint compileStatus;
	glGetShaderiv(shaderIndex, GL_COMPILE_STATUS, &compileStatus);

	if (compileStatus == 0)
	{
		GLchar* errorLog;
		GLint logLength;

		glGetShaderiv(shaderIndex, GL_INFO_LOG_LENGTH, &logLength);
		
		errorLog = new GLchar[logLength];

		glGetShaderInfoLog(shaderIndex, logLength, 0, errorLog);

		std::cout << "SHADER ERROR: " << errorLog << std::endl;

		glDeleteShader(shaderIndex);
		delete[] errorLog;

		return 0;
	}
	

	return shaderIndex;
}

GLuint ShaderManager::loadShaders(const char * vertexFile, const char * fragmentFile)
{
	GLuint vertIndex = loadShader(vertexFile, GL_VERTEX_SHADER);
	GLuint fragIndex = loadShader(fragmentFile, GL_FRAGMENT_SHADER);

	if (vertIndex == 0 || fragIndex == 0) return 0;

	GLuint shaderProgramIndex = glCreateProgram();

	glAttachShader(shaderProgramIndex, vertIndex);
	glAttachShader(shaderProgramIndex, fragIndex);

	glLinkProgram(shaderProgramIndex);

	GLint compileStatus;
	glGetProgramiv(shaderProgramIndex, GL_LINK_STATUS, &compileStatus);

	if (compileStatus == 0)
	{
		GLchar* errorLog;
		GLint logLength;

		glGetProgramiv(shaderProgramIndex, GL_INFO_LOG_LENGTH, &logLength);

		errorLog = new GLchar[logLength];

		glGetProgramInfoLog(shaderProgramIndex, logLength, 0, errorLog);

		std::cout << "PROGRAM ERROR: " << errorLog << std::endl;

		glDeleteProgram(shaderProgramIndex);
		delete[] errorLog;

		return 0;
	}

	return shaderProgramIndex;
}




