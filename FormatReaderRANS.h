#ifndef FORMAT_READER_RANS_H__
#define FORMAT_READER_RANS_H__

#include "EncodingResultRANS.h"
#include "TransformedResult.h"

#include <fstream>
#include <istream>
#include <string>
#include <iostream>
#include <map>
#include <vector>

class FromatReaderRANS
{
private:
	std::ifstream& m_InFile;

public:
	FromatReaderRANS(std::ifstream& inFile) :m_InFile(inFile) {}

	TransformedResult* readTransformedResult(int32& size);
};

#endif