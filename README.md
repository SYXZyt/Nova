## Nova Multi-threaded Logger
Nova is a header-only, multi-threaded C++ logger. It will take in messages and log them to the console.
The concept of Nova is to allow messages to be quickly logged to the console, without causing slow down, due to the consoles slow speed.

## Usage
To use Nova, simply call a begin and end function activate the logger. Then call the log functions to begin logging messages.
Nova also requires a macro to be set in order to be used. This allows you to only allow Nova to run on debug builds.
The log functions used will check this macro so you don't have to worry about checking it. Below is an example programming showing what to do and what not to do.

```cpp
#define NOVA_ENABLE //Allows the logger to be included
#include <Nova/Nova.h>

int main(int, char**)
{
	//If this is not called, any messages logged will wait in a buffer for the logger to be activated
	Nova::Begin();

	Nova::Log("Hello from Nova =D"); //Prints "[INFO] Hello from Nova =D"
	Nova::LogWarning("I am warning the user of potentially incorrect behaviour");
	Nova::LogError("Oh no. Something went wrong");
	Nova::LogTodo("I need to implement something here");
	Nova::LogAssert("I can write my own assert function to call this so I have Nova handled assert messages");
	Nova::LogRaw("Anything I write here will not have a prefix such as [INFO] or [ERROR]");

	Nova::Logger::Log("This is bad!. If NOVA_ENABLED is not set, this will be an error, whereas the lines above will not");

	//Any messages logged after this will not print, until Begin is called again
	Nova::End();
}
```

You can also write custom functions to print to Nova
```cpp

struct Vec2
{
	float x;
	float y;
}

void LogVec2(const Vec2& vec2)
{
	std::string message = "X: " + std::to_string(vec2.x);
	message += ", Y: " + std::to_string(vec2.y);
	Nova::Log(message);
}

```