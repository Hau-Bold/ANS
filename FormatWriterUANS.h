#ifndef FORMAT_WRITER_UANS_H__
#define FORMAT_WRITER_UANS_H__

#include "FormatWriter.h"

class FormatWriterUANS :public FormatWriter
{
public:
	FormatWriterUANS(std::string& path, std::vector<uint32> general, std::vector<std::vector<uint8>>& preconditionerResult,
		std::vector<uint32> frequencies) :FormatWriter(path, general, preconditionerResult, frequencies)
	{}

	void FormatWriterUANS::prepareEncodingResultForWriting(std::map<uint32, EncodingResultUANS*>& encodedDataUANS);
	void writeTransformedResult(void) const;

};

#endif