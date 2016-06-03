#ifndef __CL_EVALUATOR_H__
#define __CL_EVALUATOR_H__

#include <Common.h>

#define CL_VERBOSE 1

#ifdef _DEBUG

#define CL_ASSERT(status, msg) if (status != CL_SUCCESS) { \
std::cerr << msg << "\nError code: " << status << "\n"; \
assert(false); \
} \

#else

#define CL_ASSERT(status, msg)

#endif //_DEBUG

#ifdef CL_VERBOSE

#define CL_LOG(msg) std::cout << msg

#else

#define CL_LOG(msg)

#endif //CL_VERBOSE

namespace sdfedit
{
	namespace ecl
	{
		enum class GraphicsAPIBridge
		{
			NONE,
			OGL,
			D3D
		};

		/*
		 * Not sure this can be abstracted to any backend (CPU, CUDA, etc...)
		 */
		class CLEvaluatorOptions
		{
		public:
			CLEvaluatorOptions();
			virtual ~CLEvaluatorOptions();
			static CLEvaluatorOptions Default();

			GraphicsAPIBridge graphicsAPIBridge;
		};

		/*
		 * Evaluates a list of SDF edits into a texture that can be rendered on screen
		 * Offers methods to add / remove edits
		 * Possibly offer methods to triangulate sdf edits into a mesh for export / rendering purposes
		 * TODO: define interface for it. This is the particular implementation of an evaluator that uses OpenCL backend
		 */
		class CLEvaluator
		{
		public:
		
			typedef std::shared_ptr<CLEvaluator> Ptr;
		
			CLEvaluator(CLEvaluatorOptions const& options = CLEvaluatorOptions::Default());
			virtual ~CLEvaluator();

		private:

			// Helper to create OpenCL context and command queue (only one at the moment)
			// It queries platforms and devices, and picks the "best" available
			void setupCL();

			// Helper to clean up all resources used by this class, including OpenCL stuff
			void teardownCL();

			void compileCLProgram(std::vector<std::string> const& filenames, std::string const& includePath = "");

			CLEvaluatorOptions	m_options;
			bool				m_ready;
			cl::Platform		m_platform;
			cl::Device			m_device;
			cl::Context			m_context;
			cl::CommandQueue	m_cmdQueue;

			cl::Program			m_program; //only one at the moment, put all kernerls into it

			std::map<std::string, cl::Kernel> m_kernels;

		};
	}
}

#endif //__CL_EVALUATOR_H__