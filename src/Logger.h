#ifndef LOGGER_H_
#define LOGGER_H_

#include <iostream>

namespace Logger
{

enum class LogLevel
{
	INFO, DEBUG, WARNING, ERROR, FATAL, MAX_LEVEL
};

struct endl_t{};
static const struct endl_t endl;

class Logger_
{
public:
	Logger_(LogLevel level, std::ostream& out);
	~Logger_();

	template<typename T>
	Logger_& operator<<(const T& value);
	Logger_& operator<<(const struct endl_t&);

private:
	LogLevel mLevel;
	std::ostream& mStream;
	bool mNewLine;
};

static Logger_ Info(LogLevel::INFO, std::cout);
static Logger_ Debug(LogLevel::DEBUG, std::cout);
static Logger_ Warning(LogLevel::WARNING, std::cout);
static Logger_ Error(LogLevel::ERROR, std::cerr);
static Logger_ Fatal(LogLevel::FATAL, std::cerr);

}

#include "Logger.inl"

#endif /* LOGGER_H_ */
