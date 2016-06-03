#include <ScratchApp.h>



#define CL_GL_COMPATIBILITY 0

int main(int argc, char** argv)
{
	/*
	 * Access platform and device info
	 * Create context (possibly with opengl compatibility
	 */
	cl::Platform clPlatform;
	cl::Device clDevice;
	cl::Context clContext;
	cl::CommandQueue clCmdQueue;

	

	return sdfedit::RunApp<sdfedit::ScratchApp>(argc, argv);
}

