#ifndef FORMAT_READER_UANS_H__
#define FORMAT_READER_UANS_H__

#include "EncodingResultUANS.h"
#include "TransformedResult.h"
#include "Utils.h"

#include <fstream>
#include <istream>
#include <string>
#include <iostream>
#include <map>
#include <vector>

class FromatReaderUANS
{

private:
	std::ifstream& m_InFile;

public:
	FromatReaderUANS(std::ifstream& inFile):m_InFile(inFile) {  }

	TransformedResult* readTransformedResult(int32& size);
};

#endif