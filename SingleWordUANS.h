#ifndef SINGLE_WORD_UANS_H__
#define SINGLE_WORD_UANS_H__

#include "EncodingResultUANS.h"
#include "Frequencies.h"
#include "IndexSubset.h"

class SingleWordUANS
{

private:
	std::vector<IndexSubset> m_IndexSubsets;
	std::map<uint32, EncodingResultUANS*> m_EncodedData;
	CodewordUANS m_LowerBound;

	IndexSubset findIndexSubset(const Symbol& symbol) const;
	IndexSubset findCorrespondingIndexSubset(const CodewordUANS& codeword) const;

public:

	/**Constructor: used for encoding*/
	SingleWordUANS(std::vector<IndexSubset>& indexSubsets)
	{
		m_IndexSubsets = indexSubsets;
	}

	/**Constructor: used for decoding*/
	SingleWordUANS(std::vector<IndexSubset>& indexSubsets, std::map<uint32, EncodingResultUANS*>& encodedData)
		: SingleWordUANS(indexSubsets)
	{
		m_EncodedData = encodedData;
	}
	
	CodewordUANS encode(CodewordUANS& codeword, const Symbol& symbol, EncodingResultUANS* encodingResult, bool& isFirstSymbol);
	CodewordUANS decode(const CodewordUANS codeword, Symbol& symbol) const;


	//get & set follows below here
	void setLowerBound(const CodewordUANS& lowerBound) { m_LowerBound = lowerBound; }
	CodewordUANS getLowerBound(void) const { return m_LowerBound; }
};
#endif