#ifndef EBONY_UTILS_ASSERT_H_
#define EBONY_UTILS_ASSERT_H_

#include <exception>
#include <string>

namespace ebony {

class AssertFailureException : public std::exception {
public:
	AssertFailureException(const char *test, const char *file, int line, const std::string &msg) noexcept;
	AssertFailureException(const char *test, const char *file, int line) noexcept;
	virtual ~AssertFailureException() = default;

	AssertFailureException(const AssertFailureException &e) = default;
	AssertFailureException& operator=(const AssertFailureException &e) = default;

	virtual const char* what() const noexcept;

private:
	std::string _exceptionWhat;
};

}

#define ASSERT(test, msg)												\
	do { if (!(test)) {													\
		throw AssertFailureException(#test, __FILE__, __LINE__, msg);	\
	}} while(false)

#ifndef NDEBUG

	#define DEBUG_ASSERT(test, msg)											\
		do { if (!(test)) {													\
			throw AssertFailureException(#test, __FILE__, __LINE__, msg);	\
		}} while(false)

#else

	#define DEBUG_ASSERT(test, msg)	

#endif

#endif
