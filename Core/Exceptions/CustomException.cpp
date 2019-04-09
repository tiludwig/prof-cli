/*
 * ArgumentException.cpp
 *
 *  Created on: Apr 9, 2019
 *      Author: tim
 */

#include <Core/Exceptions/CustomException.h>

CustomException::CustomException(const std::string& message)
{
	errorMessage = message;
	errorCode = 0;
}

CustomException::CustomException(int errorCode, const std::string& message)
{
	this->errorCode = errorCode;
	this->errorMessage = message;
}

const char* CustomException::getMessage() const
{
	return errorMessage.c_str();
}

const int CustomException::getErrorCode() const
{
	return errorCode;
}

