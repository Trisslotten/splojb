#include "logger.hpp"


namespace
{
	bool show_output;
	bool show_error;
	bool show_debug;
	bool show_warning;
}

void Log::whatToShow(bool _error, bool _debug, bool _warning)
{
	show_error = _error;
	show_debug = _debug;
	show_warning = _warning;
}

void Log::showOutput(bool show)
{
	show_output = show;
}

void Log::showError(bool show)
{
	show_error = show;
}

void Log::showDebug(bool show)
{
	show_debug = show;
}

void Log::showWarning(bool show)
{
	show_warning = show;
}

void Log::print(const std::string& msg)
{
	if (show_output)
	{
		std::cout << msg;
	}
}

void Log::error(const std::string& msg)
{
	if (show_error)
	{
		print("ERROR: " + msg);
	}
}

void Log::debug(const std::string & msg)
{
	if (show_debug)
	{
		print("DEBUG: " + msg);
	}
}

void Log::warning(const std::string & msg)
{
	if (show_warning)
	{
		print("WARNING: " + msg);
	}
}

void Log::println(const std::string & msg)
{
	print(msg + "\n");
}

void Log::errorln(const std::string & msg)
{
	error(msg + "\n");
}

void Log::debugln(const std::string & msg)
{
	debug(msg + "\n");
}

void Log::warningln(const std::string & msg)
{
	warning(msg + "\n");
}

