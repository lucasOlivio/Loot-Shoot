#pragma once

#include <string>

namespace MyEngine
{
	class WindowsUtils
	{
	public:
		//Returns the last Win32 error, in string format. Returns an empty string if there is no error.
		static std::string GetLastErrorAsString();
	};
}
