/*
 * The MIT License
 *
 * Copyright 2016 Robxley.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "Logger.hpp"

#ifdef BHD_USE_FILE_LOG

#include "Assertion.hpp"

namespace BHD
{
	Logger::Logger(const char *filename) : logFile(filename)
	{
		// Check if the file is opened
		if (!logFile.is_open())
		{
			BHD_ASSERT(0);
			return;
		}
		
		logFile << "  ================================" << std::endl;
		logFile << "   Biohazard - Logger File - START " << std::endl;
		logFile << "  ================================" << std::endl << std::endl;
	}
	Logger::~Logger()
	{
		if (logFile.is_open())
		{
			logFile << std::endl;
			logFile << "  ===============================" << std::endl;
			logFile << "   Biohazard - Logger File - END " << std::endl;
			logFile << "  ===============================" << std::endl << std::endl;
		}
	}

	Logger &Logger::instance(const char *filename)
	{
		static Logger instance(filename);
		return instance;
	}

	std::ofstream& Logger::getFileLogger()
	{ 
		BHD_ASSERT(logFile.is_open());
		return logFile;
	}

}

#endif