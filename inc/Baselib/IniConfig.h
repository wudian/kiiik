#pragma once

#include "Baselib/Global.h"

class QSettings;

namespace baselib {

class BASELIB_API IniConfig
{
public:
	IniConfig(const std::string& path);
	~IniConfig();

	void SetValue(const std::string& key, const int val);
	void SetValue(const std::string& key, const char val);
	void SetValue(const std::string& key, const float val);
	void SetValue(const std::string& key, const double val);
	void SetValue(const std::string& key, const char* val);
	void SetValue(const std::string& key, std::string& val);
	void SetValue(const std::string& key, bool val);

	int intValue(const std::string& key);
	char charValue(const std::string& key);
	float floatValue(const std::string& key);
	double doubleValue(const std::string& key);
	std::string stringValue(const std::string& key);
	bool boolValue(const std::string& key);

private:
	QSettings* settings_;
};

}