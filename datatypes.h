/*
 * datatypes.h
 *
 *  Created on: Apr 5, 2019
 *      Author: tim
 */

#ifndef DATATYPES_H_
#define DATATYPES_H_

#include <stdint.h>

extern "C" struct buffer_t
{
	char* data;
	uint32_t size;
	uint32_t index;
};

extern "C" struct result_t
{
	uint32_t cycles;
};

#endif /* DATATYPES_H_ */
