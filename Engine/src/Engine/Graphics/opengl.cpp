#include "pch.h"

namespace MyEngine
{
    void GLAPIENTRY MessageCallback(GLenum source,
                                    GLenum type,
                                    GLuint id,
                                    GLenum severity,
                                    GLsizei length,
                                    const GLchar* message,
                                    const void* userParam)
    {
        std::string error = "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n" +
            std::to_string(type) + "\n" +
            std::to_string(severity) + "\n" +
            message;

        LOG_DEBUG(error);
    }
    void GlfwErrorCallback(int error, const char* description)
    {
        LOG_DEBUG(description);
    }

    bool CheckOpenGLError() 
	{
		GLenum errorEnum = glGetError();
		// from: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetError.xml
		switch (errorEnum)
		{
		case GL_NO_ERROR:
			return false;
			break;
		case GL_INVALID_ENUM:
			LOG_DEBUG("GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.");
			break;
		case GL_INVALID_VALUE:
			LOG_DEBUG("GL_INVALID_VALUE: A numeric argument is out of range.");
			break;
		case GL_INVALID_OPERATION:
			LOG_DEBUG("GL_INVALID_OPERATION: The specified operation is not allowed in the current state.");
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			LOG_DEBUG("GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.");
			break;
		case GL_OUT_OF_MEMORY:
			LOG_DEBUG("GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.");
			break;
		case GL_STACK_UNDERFLOW:
			LOG_DEBUG("GL_STACK_UNDERFLOW: An attempt has been made to perform an operation that would cause an internal stack to underflow.");
			break;
		case GL_STACK_OVERFLOW:
			LOG_DEBUG("GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would cause an internal stack to overflow.");
			break;
		default:
			LOG_DEBUG("Unknown error");
			break;
		};

		return true;	// There -WAS- an error
	}
}
