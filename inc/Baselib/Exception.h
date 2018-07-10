#pragma once

#include <string>
#include <sstream>
#include <exception>
#include "Baselib/Global.h"

namespace baselib {

#define ThrowStringError(msg) throw baselib::Exception(msg).SourceInfo(__FILE__, __LINE__)

#define ThrowError(msg) do { std::stringstream ss; ss << msg; ThrowStringError(ss.str()); } while (0);

class BASELIB_API Exception : public std::exception
{
public:
	Exception() : msg_(""), file_(""), line_(0), desc_("") {};
	Exception(const std::string& msg) : msg_(msg), file_(""), line_(0), desc_("") {};
	virtual ~Exception() throw() {}

	std::string Msg() const { return msg_; }

	Exception& SourceInfo(const char *file, int line);
	virtual const char * what() const throw();

private:
	std::string msg_;
	std::string file_;
	int line_;
	mutable std::string desc_;
};

}