#ifndef __SDFEDIT_SHADER_H__
#define __SDFEDIT_SHADER_H__

#include <Common.h>

namespace sdfedit
{
    namespace Shader
    {
        GLuint FromFile(const char* filename,
						GLenum shaderType,
#ifdef _DEBUG
						bool checkErrors = true);
#else
						bool checkErrors = false);
#endif
        
        GLuint FromString(const char* source,
						  GLenum shaderType,
#ifdef _DEBUG
						  bool checkErrors = true);
#else
						  bool checkErrors = false);
#endif
        
        GLuint Link(const GLuint* shaders,
                          int shaderCount,
                          bool deleteShaders,
#ifdef _DEBUG
                          bool checkErrors = true);
#else
                          bool checkErrors = false);
#endif
    }
}

#endif //__SDFEDIT_SHADER_H__