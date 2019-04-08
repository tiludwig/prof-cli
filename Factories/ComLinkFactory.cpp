/*
 * ComLinkFactory.cpp
 *
 *  Created on: Apr 8, 2019
 *      Author: tim
 */

#include <Factories/ComLinkFactory.h>

IComLink* ComLinkFactory::CreateComLink(std::string& type)
{
	IComLink* driver = nullptr;
	if(type == "serial")
	{
		driver = new SerialLink();
	}

	return driver;
}
