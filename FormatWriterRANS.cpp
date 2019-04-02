#include "FormatWriterRANS.h"

void FormatWriterRANS::prepareEncodingResultForWriting(std::map<uint32, EncodingResultRANS*>& encodedDataRANS)
{
	std::vector<RansByte> moduloRestsTotal;
	std::vector<CodewordRANS> finalStates;
	std::vector<uint16> sizes;
	std::vector<uint16> sizesCorrespondingToItem;
	std::map<uint32, EncodingResultRANS*>::iterator iter = encodedDataRANS.begin();

	while (iter != encodedDataRANS.end())
	{
		EncodingResultRANS* encodingResult = iter->second;

		CodewordRANS finalState = encodingResult->getFinalState();

		std::vector<std::vector<RansByte>> moduloRests = encodingResult->getModuloRest();
		sizes.emplace_back(moduloRests.size());

		for (int id = 0; id < moduloRests.size(); id++)
		{
			std::vector<RansByte> tempEncodingSteps = moduloRests[id];

			for (auto& element : tempEncodingSteps)
			{
				moduloRestsTotal.emplace_back(element);
			}
			sizesCorrespondingToItem.emplace_back(tempEncodingSteps.size());
		}

		finalStates.emplace_back(finalState);

		iter++;
	}

	m_TransformedResult = new TransformedResult(finalStates, sizes);

	uint8 rest = (moduloRestsTotal.size()) % 8;
	std::vector<uint8>encodingStepsRANS = Utils::bitMaskToUint8(sizesCorrespondingToItem);
	encodingStepsRANS.emplace_back(rest);

	m_TransformedResult->setEncodingStepsRANS(encodingStepsRANS);

	m_TransformedResult->setBitsRANS(moduloRestsTotal);
}

void FormatWriterRANS::writeTransformedResult(void) const
{
	const std::vector<CodewordRANS> finalStates = m_TransformedResult->getFinalStatesRANS();

	std::vector<RansByte> bitsRANS = m_TransformedResult->getBitsRANS();

	const std::vector<uint16> sizes = m_TransformedResult->getSizesRANS();

	const std::vector<uint8> encodingStepsRANS = m_TransformedResult->getEncodingStepsRANS();

	const uint64 countOfModuloRests = bitsRANS.size();
	const uint32 size = finalStates.size();
	const uint64 countOfEncodingSteps = encodingStepsRANS.size();

	//no need to write count of final states, it corresponds to the dimensions of the image

	(*outFile).write(reinterpret_cast<const char*>(sizes.data()), sizeof(uint16)*sizes.size());
	(*outFile).write(reinterpret_cast<const char*>(finalStates.data()), sizeof(uint64)*size);

	(*outFile).write(reinterpret_cast<const char*>(&countOfModuloRests), sizeof(uint64));
	(*outFile).write(reinterpret_cast<const char*>(bitsRANS.data()), sizeof(RansByte)*countOfModuloRests);

	(*outFile).write(reinterpret_cast<const char*>(&countOfEncodingSteps), sizeof(uint64));
	(*outFile).write(reinterpret_cast<const char*>(encodingStepsRANS.data()), sizeof(uint8)*countOfEncodingSteps);
}