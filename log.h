#pragma once
#include <fstream>

class Log
{
public:
	Log();
	virtual void Write(const char * message);
	virtual ~Log();
protected:
	std::ofstream wof;
};