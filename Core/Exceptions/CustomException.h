/*
 * ArgumentException.h
 *
 *  Created on: Apr 9, 2019
 *      Author: tim
 */

#ifndef DEBUG_CORE_EXCEPTIONS_CUSTOMEXCEPTION_H_
#define DEBUG_CORE_EXCEPTIONS_CUSTOMEXCEPTION_H_

#include <exception>
#include <string>

class CustomException : public std::exception
{
private:
	int errorCode;
	std::string errorMessage;
public:
	CustomException(const std::string& message);
	CustomException(int errorCode, const std::string& message);

	const char* getMessage() const;
	const int getErrorCode() const;
};

#endif /* DEBUG_CORE_EXCEPTIONS_CUSTOMEXCEPTION_H_ */
