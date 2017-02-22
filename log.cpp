#include "stdafx.h"
#include "log.h"
#include "datetime.h"

Log::Log()
{
}

void Log::Write(const char * message)
{
	wof.open("app.log", std::fstream::out | std::fstream::app);
	dtm::datetime now;
	wof << "[" << now << "] " << message << "\n";
	wof.close();
}

Log::~Log()
{
}
