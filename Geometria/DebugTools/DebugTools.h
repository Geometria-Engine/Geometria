#ifndef DEBUGTOOLS_H
#define DEBUGTOOLS_H

#include <signal.h>

#define GDT_ASSERT(x)\
{\
 	if(!(x))\
 	{\
 		std::cerr << "[DebugTools] ASSERT FAILED: " << #x << " @ " << __FILE__ << " (line: " << __LINE__ << ")" << std::endl;\
 		Graphics::Exit();\
 	}\
}

struct DebugTools
{

	static void SignalHandler(int signal)
	{
		printf("Signal %d\n", signal);
		throw "Error!";
	}

};

#define GDT_WATCHPOINT_INIT()\
{\
	typedef void (*SignalHandlerPointer)(int);\
    SignalHandlerPointer previousHandler;\
    previousHandler = signal(SIGSEGV, DebugTools::SignalHandler);\
}

#define GDT_WATCHPOINT_START() std::cout << "[DebugTools] START WATCHPOINT: " << __FILE__ << " (line: " << __LINE__ << ")" << std::endl;  try {
#define GDT_WATCHPOINT_END()\
}\
catch (char* e)\
{\
    printf("Exception Caught: %s\n", e);\
    exit(1);\
}\
std::cout << "[DebugTools] END WATCHPOINT: " << __FILE__ << " (line: " << __LINE__ << ")" << std::endl;

#endif