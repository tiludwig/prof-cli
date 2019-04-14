/*
 * DebugPathManager.cpp
 *
 *  Created on: Apr 8, 2019
 *      Author: tim
 */

#include <Utility/PathManager/DebugPathManager.h>

std::string DebugPathManager::getPluginpath() const
{
	return basepath + "/Debug/plugins/";
}

std::string DebugPathManager::getBinarypath() const
{
	return basepath + "/Debug/";
}

std::string DebugPathManager::getDatapath() const
{
	return basepath + "/Debug/plugins/";
}
