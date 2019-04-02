#include "FormatWriterUANS.h"

void FormatWriterUANS::writeTransformedResult(void) const
{
	const std::vector<CodewordUANS> finalStates = m_TransformedResult->getFinalStatesUANS();
	std::vector<uint8> bitsUANS = m_TransformedResult->getBitsUANS();

	const std::vector<uint32> sizes = m_TransformedResult->getSizesUANS();

	const uint32 countOfModuloRests = bitsUANS.size();
	const int32 size = finalStates.size();

	//no need to write count of final states, it corresponds to the dimensions of the image

	(*outFile).write(reinterpret_cast<const char*>(finalStates.data()), sizeof(uint32)*size);
	(*outFile).write(reinterpret_cast<const char*>(&countOfModuloRests), sizeof(uint32));
	(*outFile).write(reinterpret_cast<const char*>(bitsUANS.data()), sizeof(uint8)*countOfModuloRests);
	(*outFile).write(reinterpret_cast<const char*>(sizes.data()), sizeof(uint32)*sizes.size());
}

void FormatWriterUANS::prepareEncodingResultForWriting(std::map<uint32, EncodingResultUANS*>& encodedDataUANS)
{
	std::vector<uint16> moduloRestsTotal;
	std::vector<CodewordUANS> finalStates;
	std::vector<uint32> sizes;
	std::map<uint32, EncodingResultUANS*>::iterator iter = encodedDataUANS.begin();

	while (iter != encodedDataUANS.end())
	{
		EncodingResultUANS* encodingResult = iter->second;

		CodewordUANS finalState = encodingResult->getFinalState();
		std::vector<uint16> moduloRests = encodingResult->getModuloRest();
		sizes.emplace_back(moduloRests.size());

		for (auto& moduloRest : moduloRests)
		{
			moduloRestsTotal.emplace_back(moduloRest);
		}

		finalStates.emplace_back(finalState);

		iter++;
	}

	m_TransformedResult = new TransformedResult(finalStates, sizes);

	uint8 rest = (moduloRestsTotal.size()) % 8;
	std::vector<uint8>bitsUANS = Utils::bitMaskToUint8(moduloRestsTotal);
	bitsUANS.emplace_back(rest);
	m_TransformedResult->setBitsUANS(bitsUANS);
}