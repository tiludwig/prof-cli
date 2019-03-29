/*
 * RingBuffer.h
 *
 *  Created on: 28.03.2019
 *      Author: Tim
 */

#ifndef CORE_BUFFER_RINGBUFFER_H_
#define CORE_BUFFER_RINGBUFFER_H_

#include <stdint.h>

template <class T, uint32_t N>
class RingBuffer
{
private:
	T internalBuffer[N];
	uint32_t head;
	uint32_t tail;
	uint32_t overflowCount;
	uint32_t peak;
public:
	RingBuffer();
	void append(T value);
	T read();
	T peakRead();

	bool isEmpty();

	bool isFull();

	void skip(uint32_t amount);



	uint32_t size();
	uint32_t capacity();

	T* getCurrentItemAddress();
	uint32_t getIndex();
};

#include <Core/Buffer/RingBuffer.impl.h>

#endif /* CORE_BUFFER_RINGBUFFER_H_ */
