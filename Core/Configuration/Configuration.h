/*
 * Configuration.h
 *
 *  Created on: Apr 8, 2019
 *      Author: tim
 */

#ifndef CORE_CONFIGURATION_CONFIGURATION_H_
#define CORE_CONFIGURATION_CONFIGURATION_H_

#include <iostream>
#include <string>
#include <vector>

class Configuration
{
private:
	std::vector<std::pair<std::string, std::string>> config;
public:
	Configuration();
	~Configuration();

	void loadFromFile(const std::string& filename);

	std::string& getValue(const std::string& key);
	std::string& operator[](const std::string& key);

	void add(const std::string& key, const std::string& value);

	friend void operator>>(std::istream&, Configuration&);

	unsigned int getIterations();
	unsigned int getUiUpdateFrequency();
	std::string& getTaskName();
	std::string& getInputProviderName();
	std::string& getComdriverType();

	bool getValueIfExists(const std::string& key, std::string& value);
};

#endif /* CORE_CONFIGURATION_CONFIGURATION_H_ */
