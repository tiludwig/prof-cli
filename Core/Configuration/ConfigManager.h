/*
 * ConfigManager.h
 *
 *  Created on: May 22, 2019
 *      Author: tim
 */

#ifndef CORE_CONFIGURATION_CONFIGMANAGER_H_
#define CORE_CONFIGURATION_CONFIGMANAGER_H_

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <Core/Exceptions/CustomException.h>

class TestConfiguration
{
private:

	std::vector<std::pair<std::string, std::string>> config;
public:
	std::string name;
	void add(const std::string& key, const std::string& value)
	{
		if (key == "testname")
		{
			name = value;
		}

		auto setting = make_pair(key, value);
		config.push_back(setting);
	}

	std::string& operator[](const std::string& key)
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
			std::string message("Unable to find key '");
			message += key;
			message += "' in configuration";
			//in configuration");
			throw CustomException(message);
		}
	}
};

class ConfigManager
{
private:
	std::vector<std::pair<std::string, std::string>> coreSettings;
	std::vector<TestConfiguration> tests;
	int currentTestReadIndex;
public:

	ConfigManager()
	{
		currentTestReadIndex = 0;
	}
	std::string& getCoreSetting(const std::string& key)
	{
		auto value = std::find_if(coreSettings.begin(), coreSettings.end(),
				[&](const std::pair<std::string, std::string>& element)
				{
					return element.first == key;
				});

		if (value != coreSettings.end())
		{
			return value->second;
		}
		else
		{
			std::string message("Unable to find key '");
			message += key;
			message += "' in configuration";
			throw CustomException(message);
		}
	}

	int getTestCount()
	{
		return tests.size();
	}

	TestConfiguration& getNextTest()
	{
		if (tests.size() == 0)
		{
			std::string message("No tests were specified in the configuration file.");
			throw CustomException(message);
		}

		return tests.at(currentTestReadIndex++);
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

	void add(const std::string& section, const std::string& key, const std::string& value)
	{
		if (section == "core")
		{
			auto setting = std::make_pair(key, value);
			coreSettings.push_back(setting);
		}
		else
		{
			if (tests.size() == 0)
			{
				TestConfiguration cfg;
				tests.push_back(cfg);
			}

			TestConfiguration& cfg = tests.at(tests.size() - 1);
			cfg.add(key, value);
		}
	}

	friend void operator>>(std::istream& stream, ConfigManager& cfg)
	{
		std::string line;
		std::string currentSection = "";
		while (getline(stream, line))
		{
			// remove comments
			auto commentIndex = line.find('#');
			if (commentIndex != std::string::npos)
				line.erase(commentIndex);

			auto tokenIndex = line.find('[');
			if (tokenIndex != std::string::npos)
			{
				auto endIndex = line.find(']');
				currentSection = cfg.trim(line.substr(tokenIndex + 1, endIndex - tokenIndex - 1));
				if (currentSection == "test")
				{
					// add new test class
					cfg.tests.push_back(TestConfiguration());
				}
				continue;
			}
			auto separatorIndex = line.find('=');
			if (separatorIndex == std::string::npos)
				continue;

			auto key = cfg.trim(line.substr(0, separatorIndex));
			auto value = cfg.trim(line.substr(separatorIndex + 1, line.size() - separatorIndex));

			cfg.add(currentSection, key, value);
		}
	}
};

#endif /* CORE_CONFIGURATION_CONFIGMANAGER_H_ */
