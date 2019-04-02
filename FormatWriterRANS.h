#ifndef FORMAT_WRITER_RANS_H__
#define FORMAT_WRITER_RANS_H__
#include "FormatWriter.h"

class FormatWriterRANS :public FormatWriter
{
public:

	FormatWriterRANS(std::string& path, std::vector<uint32> general, std::vector<std::vector<uint8>>& preconditionerResult,
		std::vector<uint32> frequencies) :FormatWriter(path, general, preconditionerResult, frequencies)
	{}

	void prepareEncodingResultForWriting(std::map<uint32, EncodingResultRANS*>& encodedDataRANS);

	void writeTransformedResult(void) const;
};

#endif