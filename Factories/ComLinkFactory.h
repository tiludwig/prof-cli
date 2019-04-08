/*
 * ComLinkFactory.h
 *
 *  Created on: Apr 8, 2019
 *      Author: tim
 */

#ifndef FACTORIES_COMLINKFACTORY_H_
#define FACTORIES_COMLINKFACTORY_H_

#include <Components/ComLink/IComLink.h>
#include <Components/ComLink/SerialLink.h>
#include <string>

class ComLinkFactory
{
public:
	static IComLink* CreateComLink(std::string& type);
};

#endif /* FACTORIES_COMLINKFACTORY_H_ */
