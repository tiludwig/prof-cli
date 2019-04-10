/*
 * Payload.h
 *
 *  Created on: Apr 9, 2019
 *      Author: tim
 */

#ifndef CORE_PAYLOAD_PAYLOAD_H_
#define CORE_PAYLOAD_PAYLOAD_H_

#include <vector>
#include <cassert>
#include <stdint.h>
#include <string>

class Payload
{
private:
	std::vector<char> internalBuffer;
public:
	template<typename T>
	void add(T value)
	{
		static_assert(sizeof(T) <= 4 );

		if (sizeof(T) == 1)
		{
			internalBuffer.push_back(value);
			return;
		}

		unsigned int* ptrValue = (unsigned int*) &value;

		for (unsigned int i = 0; i < sizeof(T); i++)
		{
			unsigned char temp = (*ptrValue >> (8 * i)) & 0xFF;
			internalBuffer.push_back(temp);
		}
	}

	void add(const char* value)
	{
		while (*value != '\0')
		{
			internalBuffer.push_back(*value++);
		}
		internalBuffer.push_back('\0');
	}

	void add(const char* buffer, uint32_t size)
	{
		for(uint32_t i = 0; i < size; i++)
		{
			internalBuffer.push_back(buffer[i]);
		}
	}

	void add(const std::string& value)
	{
		add(value.c_str());
	}

	char* data()
	{
		return internalBuffer.data();
	}

	char operator[](uint32_t index)
	{
		return internalBuffer[index];
	}
};

#endif /* CORE_PAYLOAD_PAYLOAD_H_ */
