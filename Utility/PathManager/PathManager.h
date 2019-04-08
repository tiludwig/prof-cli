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
protected:
	std::string basepath;
public:
	virtual ~PathManager();
	virtual bool initialize();

	virtual std::string getBasepath() const;
	virtual std::string getPluginpath() const;
	virtual std::string getBinarypath() const;
	virtual std::string getDatapath() const;
};

#endif /* UTILITY_PATHMANAGER_H_ */
