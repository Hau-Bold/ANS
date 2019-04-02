#include "SingleWordUANS.h"
IndexSubset SingleWordUANS::findIndexSubset(const Symbol& symbol) const
{
	for (int32 position = 0; position < m_IndexSubsets.size(); position++)
	{
		IndexSubset current = m_IndexSubsets[position];

		if (current.getSymbol() == symbol)
		{
			return current;
		}
	}

	throw std::invalid_argument("Invalid argument!\n");
}

IndexSubset SingleWordUANS::findCorrespondingIndexSubset(const CodewordUANS& codeword) const
{
	for (int32 position = 0; position < m_IndexSubsets.size(); position++)
	{
		IndexSubset current = m_IndexSubsets[position];
		int32 moduloRest = current.getModuloRest();
		int32 frequency = current.getFrequency();

		if ((codeword - moduloRest) % frequency == 0)
		{
			return current;
		}
	}

	throw std::invalid_argument("Invalid argument!\n");
}

CodewordUANS SingleWordUANS::encode(CodewordUANS& codeword, const Symbol& symbol, EncodingResultUANS* encodingResult, bool& isFirstSymbol)
{
	IndexSubset indexSubset = findIndexSubset(symbol);

	std::vector<int32> indices = indexSubset.getIndices();

	while (std::find(indices.begin(), indices.end(), codeword) == indices.end())
	{
		if (!isFirstSymbol)
		{
			encodingResult->appendModuloRest(codeword % 2);
		}
		codeword >>= 1;
	}

	if (isFirstSymbol)
	{
		isFirstSymbol = false;
	}

	return codeword * indexSubset.getFrequency() + indexSubset.getModuloRest();
}

CodewordUANS SingleWordUANS::decode(const CodewordUANS codeword, Symbol& symbol) const
{

	IndexSubset indexSubset = findCorrespondingIndexSubset(codeword);
	symbol = indexSubset.getSymbol();

	return (codeword - indexSubset.getModuloRest()) / indexSubset.getFrequency();
}





