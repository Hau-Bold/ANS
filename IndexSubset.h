#ifndef INDEXSUBSET_H__
#define INDEXSUBSET_H__

#include "ANSTypes.h"

#include <cogra\types.h>
#include <vector>

using namespace cogra;

class IndexSubset
{
private:
	uint32 m_Frequency;
	uint32 m_ModuloRest;
	Symbol m_Symbol;
	std::vector<int32> m_Indices;

public:

	IndexSubset(const uint32& frequency, const uint32& moduloRest, const Symbol& symbol, const CodewordUANS& param_L) : m_Frequency(frequency), m_ModuloRest(moduloRest), m_Symbol(symbol)
	{
			int index = static_cast<int>(ceil(static_cast<float>(param_L - moduloRest) / static_cast<float>(frequency)));

			int upperBound = floor(static_cast<float>(BASIS_UANS * param_L - 1 - moduloRest) / static_cast<float>(frequency));

			while (index <= upperBound)
			{
				m_Indices.push_back(index);
				index++;
			}
	};

	/*
	*Copy Constrcutor.
	*
	*@param indexSubset - the indexSubset to copy
	*/
	IndexSubset(const IndexSubset& indexSubset)
	{
		m_Frequency = indexSubset.m_Frequency;
		m_ModuloRest = indexSubset.m_ModuloRest;
		m_Symbol = indexSubset.m_Symbol;
		m_Indices = std::move(indexSubset.m_Indices);
	};

	IndexSubset(void) = default;

	//get & set follows below here
	Symbol getSymbol(void) const { return m_Symbol; }
	uint32 getModuloRest(void)const { return m_ModuloRest; }
	uint32 getFrequency(void) const { return m_Frequency; }
	std::vector<int32> getIndices(void) const { return m_Indices; }
};
#endif