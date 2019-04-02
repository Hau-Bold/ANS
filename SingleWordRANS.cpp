#include "SingleWordRANS.h"

const Symbol SingleWordRANS::lookUp(const CodewordRANS& codeword) const {
	int32 currentSumOfFrequencies = 0;
	Symbol symbol = 0;

	for (int32 i = 0; i < m_Frequencies->getFrequencies().size(); i++)
	{
		if (currentSumOfFrequencies + (m_Frequencies->getFrequencies()[i]) <= codeword)
		{
			currentSumOfFrequencies += (m_Frequencies->getFrequencies()[i]);
			symbol++;
		}
		else
		{
			return symbol;
		}
	}
}

CodewordRANS SingleWordRANS::encode(CodewordRANS& codeword, Symbol& symbol, EncodingResultRANS* encodingResult) const
{
	/** compute Bs */
	const uint32 Bs = m_Frequencies->getSumOfFrequencies(symbol);
	const uint32 Fs = m_Frequencies->getFrequencies()[symbol];

	codeword = M_RANS * floor((double)codeword / (double)Fs) + Bs + (codeword % Fs);

	std::vector<RansByte> rests;
	while (codeword >= UPPER_BOUND_RANS)
	{
		const uint64 rest = codeword % BASIS_RANS;//uint16
		rests.emplace_back(rest);
		codeword = double(codeword) / double(BASIS_RANS);
	}
	encodingResult->appendModuloRest(rests);

	return codeword;
}

CodewordRANS SingleWordRANS::decode(Symbol& symbol, const CodewordRANS codeword) const
{
	const CodewordRANS R = codeword % M_RANS;
	symbol = lookUp(R);

	/** compute the next currentState */
	const uint32 Fs = m_Frequencies->getFrequencies()[symbol];
	const uint32 Bs = m_Frequencies->getSumOfFrequencies(symbol);
	return Fs * floor((double)codeword / (double)M_RANS) + R - Bs;
}




