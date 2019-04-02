#include "FormatReaderUANS.h"

TransformedResult * FromatReaderUANS::readTransformedResult(int32 & size)
{
	std::vector<CodewordUANS> finalStates;
	finalStates.resize(size);

	std::vector<uint32> sizes(size);
	uint32 countOfModuloRests;

	(m_InFile).read(reinterpret_cast<char*>(finalStates.data()), sizeof(uint32)*size);
	TransformedResult* transformedResult = new TransformedResult(finalStates);

	(m_InFile).read(reinterpret_cast<char*>(&countOfModuloRests), sizeof(uint32));

	std::vector<uint8> bitUANS(countOfModuloRests);
	(m_InFile).read(reinterpret_cast<char*>(bitUANS.data()), sizeof(uint8)*countOfModuloRests);
	transformedResult->setBitsUANS(bitUANS);

	(m_InFile).read(reinterpret_cast<char*>(sizes.data()), sizeof(uint32)*sizes.size());

	transformedResult->setSizesUANS(sizes);

	return transformedResult;
}