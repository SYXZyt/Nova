#pragma once
#include <string>

#ifdef NOVA_ENABLED

#ifdef _WIN32
#include <Windows.h>
#endif

#include <ctime>
#include <cassert>
#include <queue>
#include <thread>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

namespace Nova
{
	/// @brief Class which will handle logging messages. Uses a thread to ensure performance
	class Logger final
	{
		/// @brief The thread which will handle printing messages
		static std::thread logThread;

		/// @brief Is the thread currently running or not
		static volatile bool isActive;

		/// @brief Where to store messages which have not yet been logged
		static std::queue<std::string> messages;

		/// @brief The file stream to output messages
		static std::ofstream* out;

		/// @brief Get a message from the queue and then log it to the console
		static void LogMessage()
		{
			if (messages.empty())
				return;

			std::string message = messages.front();
			messages.pop();

			std::cout << message << '\n';

#ifdef _WIN32
			OutputDebugStringA(message.c_str());
			OutputDebugStringA("\r\n");
#endif

			if (out)
			{
				(*out) << message << '\n';
				out->flush();
			}
		}

		/// @brief The main loop which the logger uses. Should run on a thread otherwise it will block the entire application
		static void LogLoop()
		{
			while (isActive)
				LogMessage();

			//Now that the thread is over, we need to clear the queue
			//This is to print any messages that are still left unprinted
			while (!messages.empty())
				LogMessage();
		}

	public:
		/// @brief Log an [INFO] message
		/// @param message The message to log
		static void Log(const std::string& message)
		{
			messages.push("[INFO]" + message);
		}

		/// @brief Log a [TODO] message
		/// @param message The message to log
		static void LogTodo(const std::string& message)
		{
			messages.push("[TODO]" + message);
		}

		/// @brief Log a [WARNING] message
		/// @param message The message to log
		static void LogWarning(const std::string& message)
		{
			messages.push("[WARNING]" + message);
		}

		/// @brief Log an [ERROR] message
		/// @param message The message to log
		static void LogError(const std::string& message)
		{
			messages.push("[ERROR]" + message);
		}

		/// @brief Log an [ASSERT] message
		/// @param message The message to log
		static void LogAssert(const std::string& message)
		{
			messages.push("[ASSERT]" + message);
		}

		/// @brief Log a message with no type prefix
		/// @param message The message to log
		static void LogRaw(const std::string& message)
		{
			messages.push(message);
		}

		/// @brief Log a newline
		static void NewLine()
		{
			messages.push("\r\n");
		}

		/// @brief Initalises the logger. Should be called at the beginning of main
		static void Begin(bool logToFile = false)
		{
			//If the logger has already begun, we can log an error message
			if (isActive)
			{
				LogError("Nova has already been started");
				return;
			}

			std::time_t now = std::time(nullptr);
			std::tm* now_tm = std::localtime(&now);

			std::stringstream ss;
			ss << "Nova "
				<< (now_tm->tm_year + 1900) << '-'
				<< std::setw(2) << std::setfill('0') << (now_tm->tm_mon + 1) << '-'
				<< std::setw(2) << std::setfill('0') << now_tm->tm_mday << ' '
				<< std::setw(2) << std::setfill('0') << now_tm->tm_hour << '-'
				<< std::setw(2) << std::setfill('0') << now_tm->tm_min << '-'
				<< std::setw(2) << std::setfill('0') << now_tm->tm_sec << ".txt";

			if (logToFile)
				out = new std::ofstream(ss.str());

			isActive = true;
			logThread = std::thread(Logger::LogLoop);
			Log("Nova has begun");
		}

		/// @brief Shuts down the logger. Should be called at the end of main
		static void End()
		{
			//Ideally we would log an error here, but nova is not active so we can't. I don't like the idea of throwing over this
			//It isn't a fatal error so we don't need to throw. Suppose we could do a debug assert
			assert(isActive || "Nova has already ended. Forgot call to Nova::Begin()?");

			Log("Nova has ended");
			isActive = false;
			logThread.join();

			out->close();

			delete out;
			out = nullptr;
		}
	};

	inline std::thread Logger::logThread;
	inline volatile bool Logger::isActive = false;
	inline std::queue<std::string> Logger::messages;
	inline std::ofstream* Logger::out = nullptr;
}

#endif

namespace Nova
{
	/// @brief Initalises the logger. Should be called at the beginning of main
	inline static void Begin(bool useFile = false)
	{
#ifdef NOVA_ENABLED
		Logger::Begin(useFile);
#endif
	}

	/// @brief Shuts down the logger. Should be called at the end of main
	inline static void End()
	{
#ifdef NOVA_ENABLED
		Logger::End();
#endif
	}

	/// @brief Log an [INFO] message
	/// @param message The message to log
	inline void Log(const std::string& msg)
	{
#ifdef NOVA_ENABLED
		Logger::Log(msg);
#endif
	}

	/// @brief Log an [ERROR] message
	/// @param message The message to log
	inline void LogError(const std::string& msg)
	{
#ifdef NOVA_ENABLED
		Logger::LogError(msg);
#endif
	}

	/// @brief Log a [WARNING] message
	/// @param message The message to log
	inline void LogWarning(const std::string& msg)
	{
#ifdef NOVA_ENABLED
		Logger::LogWarning(msg);
#endif
	}

	/// @brief Log a [TODO] message
	/// @param message The message to log
	inline void LogTodo(const std::string& msg)
	{
#ifdef NOVA_ENABLED
		Logger::LogTodo(msg);
#endif
	}

	/// @brief Log an [ASSERT] message
	/// @param message The message to log
	inline void LogAssert(const std::string& msg)
	{
#ifdef NOVA_ENABLED
		Logger::LogAssert(msg);
#endif
	}

	/// @brief Log a message with no type prefix
	/// @param message The message to log
	inline void LogRaw(const std::string& msg)
	{
#ifdef NOVA_ENABLED
		Logger::LogRaw(msg);
#endif
	}

	/// @brief Log a newline
	inline void NewLine()
	{
#ifdef NOVA_ENABLED
		Logger::NewLine();
#endif
	}
}