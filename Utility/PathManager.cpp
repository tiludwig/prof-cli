/*
 * PathManager.cpp
 *
 *  Created on: Apr 8, 2019
 *      Author: tim
 */

#include <Utility/PathManager.h>

#include <unistd.h>
#include <climits>

bool PathManager::initialize()
{
	char buf[PATH_MAX];
	auto pathLength = readlink("/proc/self/exe", buf, PATH_MAX);

	basepath.append(buf, 0, pathLength);
	auto index = basepath.find_last_of('/');
	if (index == std::string::npos)
	{
		printf("Unable to get base path\n");
		return false;
	}
	basepath.erase(index);

	index = basepath.find_last_of('/');
	if (index == std::string::npos)
	{
		printf("Unable to get base path\n");
		return false;
	}
	basepath.erase(index);

	return true;
}

std::string PathManager::getBasepath() const
{
	return basepath;
}

std::string PathManager::getPluginpath() const
{
#ifdef DEBUG
	return "/home/tim/repos/dev/imu_emulator/Debug/";
#else
	return basepath + "/plugins/";
#endif
}

std::string PathManager::getBinarypath() const
{
#ifdef DEBUG
	return basepath + "/Debug/";
#else
	return basepath + "/bin/";
#endif
}
