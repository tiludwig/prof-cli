/*
 * Buffer.h
 *
 *  Created on: Mar 29, 2019
 *      Author: tim
 */

#ifndef CORE_BUFFER_BUFFER_H_
#define CORE_BUFFER_BUFFER_H_

#include <stdint.h>

template <class T, uint32_t N>
class Buffer
{
private:
	T internalBuffer[N];
	uint32_t index;
	uint32_t overflowCount;
public:
	Buffer();

	bool isEmpty();
	bool isFull();

	uint32_t count();
	uint32_t capacity();

	void clear();
	T* data();

	void append(T& value);

	T& operator[](uint32_t idx);
};

#include "Buffer.impl.h"

#endif /* CORE_BUFFER_BUFFER_H_ */
