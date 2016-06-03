#ifndef __SDFEDIT_COMMON_H__
#define __SDFEDIT_COMMON_H__

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <memory>
#include <sstream>
#include <fstream>
#include <functional>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <CL\cl.h>
#include <CL\cl.hpp>
#include <map>

#define GLM_SWIZZLE 
#include <glm\glm.hpp>

#ifdef _DEBUG
#define ASSERT_EXP(condition, msg) if ((condition) == false) { \
std::cerr << msg << "\n"; \
assert(false); \
} \

#else

#define ASSERT_EXP(condition, msg)

#endif //_DEBUG

#endif //__SDFEDIT_COMMON_H__