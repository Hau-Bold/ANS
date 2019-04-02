#include"FormatReader.h"

void  FormatReader::readGeneral(std::vector<uint32>& generalArguments,uint32& countOfGeneralArguments)
{

	(*inFile).read(reinterpret_cast<char*>(&countOfGeneralArguments), sizeof(uint32));

	generalArguments.resize(countOfGeneralArguments);
	(*inFile).read(reinterpret_cast<char*>(generalArguments.data()), sizeof(uint32)*countOfGeneralArguments);
}

void FormatReader::readFrequencies(std::vector<uint32> & frequencies) const
{
	frequencies.resize(COUNT_OF_SYMBOLS);
	(*inFile).read(reinterpret_cast<char*>(frequencies.data()), sizeof(uint32)*COUNT_OF_SYMBOLS);
}

void FormatReader::readPreconditionerResult(const int32& size, const int32& countOfChannels, std::vector<std::vector<uint8>>& preconditionResult) const
{
	for (int32 channel = 0; channel < countOfChannels; channel++)
	{
		std::vector<uint8> tmp(size);
		(*inFile).read(reinterpret_cast<char*>(tmp.data()), sizeof(uint8)*size);
		preconditionResult.emplace_back(tmp);
	}
}





