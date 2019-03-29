/*
 * Buffer.impl.h
 *
 *  Created on: Mar 29, 2019
 *      Author: tim
 */

#ifdef CORE_BUFFER_BUFFER_H_

template <class T, uint32_t N>
Buffer<T, N>::Buffer()
{
	index = 0;
	overflowCount = 0;
}

template <class T, uint32_t N>
bool Buffer<T, N>::isEmpty()
{
	return (index == 0);
}

template <class T, uint32_t N>
bool Buffer<T, N>::isFull()
{
	return (index == N);
}

template <class T, uint32_t N>
uint32_t Buffer<T, N>::count()
{
	return index;
}

template <class T, uint32_t N>
uint32_t Buffer<T, N>::capacity()
{
	return N;
}

template <class T, uint32_t N>
void Buffer<T, N>::clear()
{
	index = 0;
	overflowCount = 0;
}

template <class T, uint32_t N>
T* Buffer<T, N>::data()
{
	return internalBuffer;
}

template <class T, uint32_t N>
void Buffer<T, N>::append(T& value)
{
	if(isFull()){
		overflowCount++;
		return;
	}

	internalBuffer[index++] = value;
}

template <class T, uint32_t N>
T& Buffer<T, N>::operator[](uint32_t idx)
{
	return internalBuffer[idx];
}

#endif
