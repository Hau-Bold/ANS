#include "FormatWriter.h"

void FormatWriter::initOutFile(void)
{
	outFile = new std::ofstream(Utils::generatePath(m_Path, ANSConstants::FORMAT_ENDING), std::ios::out | std::ofstream::binary);
}

void FormatWriter::writeGeneral() const
{
	uint32 countOfGeneralArguments = m_General.size();

	(*outFile).write(reinterpret_cast<const char*>(&countOfGeneralArguments), sizeof(uint32));

	for (uint32 element : m_General)
	{
		(*outFile).write(reinterpret_cast<char*>(&element), sizeof(uint32));
	}
}

void FormatWriter::writePreconditionerResult(void) const
{
	for (int32 channel = 0; channel < m_PreconditionerResult.size(); channel++)
	{
		(*outFile).write(reinterpret_cast<const char*>(m_PreconditionerResult[channel].data()), sizeof(uint8)*m_PreconditionerResult[channel].size());
	}
}

void FormatWriter::writeFrequencies(void) const
{
	(*outFile).write(reinterpret_cast<const char*>(m_Frequencies.data()), sizeof(uint32)*m_Frequencies.size());
}

void FormatWriter::write(void) const
{
	writeGeneral();
	writeFrequencies();
	writePreconditionerResult();
	writeTransformedResult();

	outFile->close();
}