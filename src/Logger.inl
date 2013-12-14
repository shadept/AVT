// #include "Logger.h"

namespace Logger
{

inline Logger_::Logger_(LogLevel level, std::ostream& out) :
		mLevel(level), mStream(out), mNewLine(true)
{
}

inline Logger_::~Logger_()
{
}

template<typename T>
inline Logger_& Logger_::operator <<(const T& value)
{
#ifdef NDEBUG
	if(mLevel != LogLevel::DEBUG)
	{
#endif
		static std::string levels[(unsigned) LogLevel::MAX_LEVEL] = { "INFO", "DEBUG", "WARNING", "ERROR", "FATAL" };
		if (mNewLine)
		{
			mStream << "[" << levels[(unsigned) mLevel] << "] ";
			mNewLine = false;
		}
		mStream << value;
#ifdef NDEBUG
	}
#endif
	return (*this);
}

inline Logger_& Logger_::operator <<(const struct endl_t&)
{
#ifdef NDEBUG
	if(mLevel != LogLevel::DEBUG)
	{
#endif
		if (mNewLine == false)
		{
			mStream << "\n";
			mStream.flush();
			mNewLine = true;
		}
		if (mLevel == LogLevel::FATAL)
			exit(-1);
#ifdef NDEBUG
	}
#endif
	return (*this);
}

}
