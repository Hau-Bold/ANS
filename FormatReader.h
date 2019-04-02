#ifndef FORMAT_READER_H__
#define FORMAT_READER_H__
#include <cogra\types.h>

#include "ANSTypes.h"
#include "CompressorTypes.h"
#include "EncodingResultRANS.h"

#include <fstream>
#include <istream>
#include <string>
#include <iostream>
#include <map>
#include <vector>


using namespace cogra;


/**the class FormatReader*/
class FormatReader
{
protected:
	std::ifstream * inFile = nullptr;

public:

	/*
	*Constructor.
	*
	*@param path - the path of the file to read
	*/
	FormatReader(std::string& path){
		inFile = new std::ifstream(path, std::ios::in | std::ios::binary);
	}


	void readGeneral(std::vector<uint32>& generalArguments,uint32& countOfGeneralArguments);
	void readPreconditionerResult(const int32& size, const int32& m_CountOfChannels, std::vector<std::vector<uint8>>& m_PreconditionResult) const;
	void readFrequencies(std::vector<uint32>& frequencies) const;
	

	std::ifstream * getReader()const { return inFile; }
};


#endif