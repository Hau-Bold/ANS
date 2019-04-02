#ifndef FORMAT_GENERATOR_H
#define FORMAT_GENERATOR_H
#include <cogra\types.h>

#include "ANSConstants.h"
#include "CompressorTypes.h"
#include "EncodingResultUANS.h"
#include "EncodingResultRANS.h"
#include "PreconditionerType.h"
#include "Setting.h"
#include "TransformedResult.h"
#include "Utils.h"

#include <fstream>
#include <ostream>
#include <string>
#include <map>
#include<vector>

using namespace cogra;

class FormatWriter
{
private:

	void writeGeneral(void) const;
	void writeFrequencies(void) const;
	void writePreconditionerResult(void) const;
	std::string m_Path;

	std::vector<uint32> m_Frequencies, m_General;
	std::vector<std::vector<uint8>> m_PreconditionerResult;

	CompressorTypes m_TypeOfCompressor;

	void initOutFile(void);
	virtual void writeTransformedResult(void) const=0;

protected:
	TransformedResult * m_TransformedResult;
	std::ofstream * outFile = nullptr;

public:
	FormatWriter(std::string& path, std::vector<uint32> general, std::vector<std::vector<uint8>>& preconditionerResult,
		std::vector<uint32> frequencies): m_Path(path)
	{
		initOutFile();
		m_General = general;
		m_PreconditionerResult = preconditionerResult;
		m_Frequencies = frequencies;
		m_TypeOfCompressor = general[1] == 0 ? CompressorTypes::RANS : CompressorTypes::UANS;
	}

	/**Destructor*/
	~FormatWriter()
	{
		if (outFile != nullptr)
		{
			outFile->close();
		}
	}

	void write(void) const;

	virtual void prepareEncodingResultForWriting(std::map<uint32, EncodingResultUANS*>& encodedDataRANS) {};
	virtual void prepareEncodingResultForWriting(std::map<uint32, EncodingResultRANS*>& encodedDataRANS) {};
};

#endif