#include "FormatReaderRANS.h"

TransformedResult * FromatReaderRANS::readTransformedResult(int32 & size)
{
	std::vector<CodewordRANS> finalStates;
	finalStates.resize(size);

	std::vector<uint16> sizes(size);
	uint64 countOfModuloRests, countOfEncodingSteps;

	(m_InFile).read(reinterpret_cast<char*>(sizes.data()), sizeof(uint16)*size);
	(m_InFile).read(reinterpret_cast<char*>(finalStates.data()), sizeof(uint64)*size);
	(m_InFile).read(reinterpret_cast<char*>(&countOfModuloRests), sizeof(uint64));

	std::vector<RansByte> bitRANS(countOfModuloRests);
	(m_InFile).read(reinterpret_cast<char*>(bitRANS.data()), sizeof(RansByte)*countOfModuloRests);

	(m_InFile).read(reinterpret_cast<char*>(&countOfEncodingSteps), sizeof(uint64));
	std::vector<uint8> encodingStepsRANS(countOfEncodingSteps);
	(m_InFile).read(reinterpret_cast<char*>(encodingStepsRANS.data()), sizeof(uint8)*countOfEncodingSteps);

	return new TransformedResult(finalStates, sizes, bitRANS, encodingStepsRANS);

	m_InFile.close();
}

