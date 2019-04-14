/*
 * Configuration.cpp
 *
 *  Created on: Apr 8, 2019
 *      Author: tim
 */

#include <Core/Configuration/Configuration.h>
#include <Core/Exceptions/CustomException.h>
#include <algorithm>

using namespace std;
Configuration::Configuration()
{
}

Configuration::~Configuration()
{
}

std::string& Configuration::getValue(const std::string& key)
{
	auto value = find_if(config.begin(), config.end(), [&](const std::pair<std::string, std::string>& element)
	{
		return element.first == key;
	});

	if (value != config.end())
	{
		return value->second;
	}
	else
	{
		throw CustomException("Unable to find key in configuration");
	}
}

void Configuration::add(const std::string& key, const std::string& value)
{
	auto setting = make_pair(key, value);
	config.push_back(setting);
}

std::string& Configuration::operator[](const std::string& key)
{
	return getValue(key);
}

std::string ltrim(const std::string& text)
{
	auto firstChar = text.find_first_not_of(" \t");
	if (firstChar == std::string::npos)
	{
		return text;
	}
	return text.substr(firstChar, text.size() - firstChar);
}

std::string rtrim(const std::string& text)
{
	auto lastChar = text.find_last_not_of(" \t");
	if (lastChar == std::string::npos)
	{
		return text;
	}
	return text.substr(0, lastChar + 1);
}

std::string trim(const std::string& text)
{
	return rtrim(ltrim(text));
}

void operator>>(std::istream& stream, Configuration& cfg)
{
	string line;
	string currentSection = "";
	while (getline(stream, line))
	{
		// remove comments
		auto commentIndex = line.find('#');
		if (commentIndex != string::npos)
			line.erase(commentIndex);

		auto tokenIndex = line.find('[');
		if (tokenIndex != string::npos)
		{
			auto endIndex = line.find(']');
			currentSection = trim(line.substr(tokenIndex + 1, endIndex - tokenIndex - 1));
			continue;
		}
		auto separatorIndex = line.find('=');
		if (separatorIndex == string::npos)
			continue;

		auto key = trim(line.substr(0, separatorIndex));
		auto value = trim(line.substr(separatorIndex + 1, line.size() - separatorIndex));
		cfg.add(currentSection + '.' + key, value);
	}
}

unsigned int Configuration::getIterations()
{
	return std::stoul(getValue("measurement.iterations"));
}

unsigned int Configuration::getUiUpdateFrequency()
{
	return std::stoul(getValue("core.ui-update-freq"));
}

std::string& Configuration::getSuTName()
{
	return getValue("measurement.sut-name");
}

std::string& Configuration::getInputProviderName()
{
	return getValue("measurement.input-provider");
}

std::string& Configuration::getComdriverType()
{
	return getValue("core.comdriver");
}

