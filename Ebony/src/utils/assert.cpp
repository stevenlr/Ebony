#include "stdafx.h"
#include "utils/assert.h"

using namespace std;

namespace ebony {

AssertFailureException::AssertFailureException(const char *test, const char *file, int line, const string &msg) noexcept
{
	stringstream sstr;

	sstr << "Assertion failure in " << file << ":l." << line << ": " << test << ", " << msg;
	_exceptionWhat = sstr.str();
}

AssertFailureException::AssertFailureException(const char *test, const char *file, int line) noexcept
{
	stringstream sstr;

	sstr << "Assertion failure in " << file << ":l." << line << ": " << test;
	_exceptionWhat = sstr.str();
}

const char* AssertFailureException::what() const noexcept
{
	return _exceptionWhat.c_str();
}

}
