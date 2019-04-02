#ifndef TRANSFORMED_RESULT_H__
#define TRANSFORMED_RESULT_H__

#include "ANSTypes.h"
#include "cogra\types.h"

#include <vector>
#include <iostream>


using namespace cogra;

class TransformedResult
{
private:
	std::vector<uint8> m_BitsUANS;
	std::vector<RansByte> m_BitsRANS;
	std::vector<CodewordUANS> m_FinalStatesUANS;
	std::vector<CodewordRANS> m_FinalStatesRANS;
	std::vector<uint8> m_EncodingStepsRANS;
	std::vector<uint32> m_SizesUANS;
	std::vector<uint16> m_SizesRANS;

public:

	/**
	Constructor:
	used for writing the result of encoding
	*/
	TransformedResult(const std::vector<CodewordUANS>& finalStates, std::vector<uint32> sizes) : m_FinalStatesUANS(finalStates), m_SizesUANS(sizes)
	{}

	/**
	Constructor:
	used for writing the result of encoding
	*/
	TransformedResult(const std::vector<CodewordRANS>& finalStates, std::vector<uint16> sizes) : m_FinalStatesRANS(finalStates), m_SizesRANS(sizes)
	{}

	/**
	Constructor:
	used for reading the result of decoding
	*/
	TransformedResult(const std::vector<CodewordUANS>& finalStates) : m_FinalStatesUANS(finalStates)
	{}

	/**
	Constructor:
	used for reading the result of decoding
	*/
	TransformedResult(const std::vector<CodewordRANS>& finalStates,const std::vector<uint16> sizes, const std::vector<RansByte>& bitsRANS, const std::vector<uint8>& encodingStepsRANS)
		: m_FinalStatesRANS(finalStates), m_SizesRANS(sizes), m_BitsRANS(bitsRANS), m_EncodingStepsRANS(encodingStepsRANS)
	{}


	//get & set follows below here
	std::vector<CodewordUANS> getFinalStatesUANS(void) const { return m_FinalStatesUANS; }
	std::vector<CodewordRANS> getFinalStatesRANS(void) const { return m_FinalStatesRANS; }

	void setBitsUANS(const std::vector<uint8>& bitsUANS) { m_BitsUANS = bitsUANS; }
	std::vector<uint8> getBitsUANS(void) const { return m_BitsUANS; }

	void setBitsRANS(const std::vector<RansByte>& bitsRANS) { m_BitsRANS = bitsRANS; }
	std::vector<RansByte> getBitsRANS(void) const { return m_BitsRANS; }

	void setSizesUANS(const std::vector<uint32>& sizes) { m_SizesUANS = sizes; }
	std::vector<uint32> getSizesUANS(void) const { return m_SizesUANS; }

	void setSizesRANS(const std::vector<uint16>& sizes) { m_SizesRANS = sizes; }
	std::vector<uint16> getSizesRANS(void) const { return m_SizesRANS; }

	void setEncodingStepsRANS(const std::vector<uint8>& encodingStepsRANS) { m_EncodingStepsRANS = encodingStepsRANS; }
	std::vector<uint8> getEncodingStepsRANS(void) const { return m_EncodingStepsRANS; }
};
#endif