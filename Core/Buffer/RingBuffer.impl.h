/*
 * RingBuffer.cpp
 *
 *  Created on: 28.03.2019
 *      Author: Tim
 */

#include <Core/Buffer/RingBuffer.h>

template <class T, uint32_t N>
RingBuffer<T, N>::RingBuffer()
{
	head = 0;
	tail = 0;
	overflowCount = 0;
	peak = 0;
}

template<class T, uint32_t N>
void RingBuffer<T, N>::append(T value)
{
	if (isFull())
	{
		overflowCount++;
		return;
	}

	uint32_t writeIndex = (tail++) & (N - 1);
	internalBuffer[writeIndex] = value;
}

template<class T, uint32_t N>
T RingBuffer<T, N>::read()
{
	if (isEmpty())
	{
		return 0;
	}

	uint32_t readIndex = (head++) & (N - 1);
	return internalBuffer[readIndex];
}

template<class T, uint32_t N>
T RingBuffer<T, N>::peakRead()
{
	if (peak == tail)
	{
		return 0;
	}

	uint32_t readIndex = (peak++) & (N - 1);
	return internalBuffer[readIndex];
}

template<class T, uint32_t N>
bool RingBuffer<T, N>::isEmpty()
{
	return (head == tail);
}

template<class T, uint32_t N>
bool RingBuffer<T, N>::isFull()
{
	return (size() == N);
}

template<class T, uint32_t N>
uint32_t RingBuffer<T, N>::size()
{
	return (tail - head);
}

template<class T, uint32_t N>
uint32_t RingBuffer<T, N>::capacity()
{
	return N;
}

template<class T, uint32_t N>
T* RingBuffer<T, N>::getCurrentItemAddress()
{
	return &internalBuffer[head];
}

template<class T, uint32_t N>
uint32_t RingBuffer<T, N>::getIndex()
{
	return head;
}

template <class T, uint32_t N>
void RingBuffer<T,N>::skip(uint32_t amount)
{
	head += amount;
}
