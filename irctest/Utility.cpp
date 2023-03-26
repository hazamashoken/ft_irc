#include "Utility.hpp"

#include <sstream>
#include <sys/time.h>

/*
 * convert an int to a string
 */
std::string itos(int i)
{
	std::stringstream ss;

	ss << i;
	return (ss.str());
}

/*
 * get the current time in milliseconds
 */
time_t millis()
{
	struct timeval tv;
	time_t ms;

	gettimeofday(&tv, NULL);
	ms = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (ms);
}
