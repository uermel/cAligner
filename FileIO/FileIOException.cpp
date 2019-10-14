#include "FileIOException.h"
#include <string.h>
#include "../MKLog/MKLog.h"

FileIOException::FileIOException()
	: mFileName(), mMessage()
{
	MKLOG("FileIO Exception thrown!");
}

FileIOException::~FileIOException() throw()
{

}

FileIOException::FileIOException(string aMessage)
	: mFileName(), mMessage(aMessage)
{
	MKLOG("FileIO Exception: " + aMessage);
}

FileIOException::FileIOException(string aFileName, string aMessage)
	: mFileName(aFileName), mMessage(aMessage)
{
	MKLOG("FileIO Exception: " + aMessage);

	stringstream ss;
	ss << "Could not access file '";
	ss << mFileName << "'. " << mMessage << endl;
	mMessage = ss.str();
}

const char* FileIOException::what() const throw()
{
	return mMessage.c_str();
}

string FileIOException::GetMessage() const throw()
{
	return mMessage;
}
