/*
 * PathManager.h
 *
 *  Created on: Apr 8, 2019
 *      Author: tim
 */

#ifndef UTILITY_PATHMANAGER_H_
#define UTILITY_PATHMANAGER_H_

#include <string>

class PathManager
{
private:
	std::string basepath;
public:
	bool initialize();

	std::string getBasepath() const;
	std::string getPluginpath() const;
	std::string getBinarypath() const;
};

#endif /* UTILITY_PATHMANAGER_H_ */
