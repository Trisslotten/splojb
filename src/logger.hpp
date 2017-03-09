#pragma once

#include <string>
#include <iostream>

namespace Log
{

	void whatToShow(bool _error, bool _debug, bool _warning);
	void showOutput(bool show);
	void showError(bool show);
	void showDebug(bool show);
	void showWarning(bool show);

	void print(const std::string& msg);
	void error(const std::string& msg);
	void debug(const std::string& msg);
	void warning(const std::string& msg);

	void println(const std::string& msg);
	void errorln(const std::string& msg);
	void debugln(const std::string& msg);
	void warningln(const std::string& msg);
}