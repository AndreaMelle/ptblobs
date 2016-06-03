#include <CLEvaluator.h>

using namespace sdfedit;
using namespace sdfedit::ecl;

CLEvaluatorOptions::CLEvaluatorOptions()
	: graphicsAPIBridge(GraphicsAPIBridge::NONE)
{ }

CLEvaluatorOptions::~CLEvaluatorOptions() { }

CLEvaluatorOptions CLEvaluatorOptions::Default() { return CLEvaluatorOptions(); }

CLEvaluator::CLEvaluator(CLEvaluatorOptions const& options)
	: m_options(options)
{
	ASSERT_EXP(m_options.graphicsAPIBridge == GraphicsAPIBridge::NONE || m_options.graphicsAPIBridge == GraphicsAPIBridge::OGL, "Unsuppoorted graphics API integration.");
	this->setupCL();
}

CLEvaluator::~CLEvaluator()
{
	this->teardownCL();
}

void CLEvaluator::setupCL()
{
	if (m_ready) return;

	cl_int status;

	std::vector<cl::Platform> platforms;
	status = cl::Platform::get(&platforms);
	CL_ASSERT(status, "Could not find OpenCL platform");

	CL_LOG("Platforms: \n\n");

	for (auto platform : platforms)
	{
		std::vector<cl::Device> devices;

		std::string platformName = platform.getInfo<CL_PLATFORM_NAME>();
		std::string platformVendor = platform.getInfo<CL_PLATFORM_VENDOR>();
		CL_LOG(platformName << " " << platformVendor << "\n");

		status = platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
		CL_ASSERT(status, "Could not find OpenCL devices");

		CL_LOG("Devices: \n");

		for (auto device : devices)
		{
			std::string deviceName = device.getInfo<CL_DEVICE_NAME>();
			std::string deviceVendor = device.getInfo<CL_DEVICE_VENDOR>();

			cl_uint deviceMaxComputeUnits = device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
			size_t deviceMaxWorkGroupSize = device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();

			CL_LOG("\t" << deviceName << " " << deviceVendor << "\n");
			CL_LOG("\tMax compute units: " << deviceMaxComputeUnits << "\n");
			CL_LOG("\tMax work-group size: " << deviceMaxWorkGroupSize << "\n");
		}

		CL_LOG("\n");

		if (devices.size() > 0 && (platformName.find("NVIDIA") != std::string::npos || platformVendor.find("NVIDIA") != std::string::npos))
		{
			m_platform = platform;
			m_device = devices[0];
		}
	}

	CL_LOG("\n");

	/*
	* Create context and queue.
	* TODO: we can create more than one queue, why is it useful?
	*/

	//TODO: we don't want to write code like this
	if (m_options.graphicsAPIBridge == GraphicsAPIBridge::NONE)
	{
		m_context = cl::Context({ m_device }, NULL, NULL, NULL, &status);
	}
	else if (m_options.graphicsAPIBridge == GraphicsAPIBridge::OGL)
	{
#ifdef _WIN32

		// FIXME: will fail if at this stage there's no OpenGL or Window created yet. 
		cl_context_properties properties[] = {
			CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
			CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
			CL_CONTEXT_PLATFORM, (cl_context_properties)m_platform(),
			0
		};

		m_context = cl::Context({ m_device }, properties, NULL, NULL, &status);

#else
		ASSERT_EXP(false, "Graphics API integration with OpenCL not supported on this platform.");
#endif
	}

	CL_ASSERT(status, "Could not create OpenCL context.");

	m_cmdQueue = cl::CommandQueue(m_context, m_device, CL_QUEUE_PROFILING_ENABLE, &status);
	CL_ASSERT(status, "Could not create command queue.");

	// TODO: Here we need to compile the program we needs with all the kernels
	// We are probably keeping kernels in a resource manager in a map
	
	//this->compileCLProgram();
	
	// Then we need to allocate all memory buffers we need

	// For instance, for the final output image
	cl::Image2DGL imgBuffer;

	m_ready = true;
}

void CLEvaluator::compileCLProgram(std::vector<std::string> const& filenames, std::string const& includePath)
{
	cl::Program::Sources sources;
	cl::Program m_program = nullptr;

	for (auto filename : filenames)
	{
		std::ifstream programFile(filename);
		std::string programStr(std::istreambuf_iterator<char>(programFile), (std::istreambuf_iterator<char>()));
		sources.push_back(std::make_pair(programStr.c_str(), programStr.length() + 1));
	}

	m_program = cl::Program(m_context, sources);

	std::stringstream buildOptions;

	if(includePath.length() > 0)
	{
		buildOptions << " -I " << includePath;
	}

	cl_int status = m_program.build({ m_device }, buildOptions.str().c_str());

#ifdef _DEBUG

	if (status != CL_SUCCESS)
	{
		std::string log = out_program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(out_device);
		std::cerr << log << "\n";
		assert(false);
	}

#endif
}

void CLEvaluator::teardownCL()
{
	if (!m_ready) return;
	// We don't have to do anything because we are using the C++ API?
	m_ready = false;
}