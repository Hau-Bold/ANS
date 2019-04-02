#pragma once
#ifndef SINGLE_WORD_RANS_H__
#define SINGLE_WORD_RANS_H__

#include "EncodingResultRANS.h"
#include "Frequencies.h"
#include "Utils.h"


class SingleWordRANS
{

private:
	std::map<uint32, EncodingResultRANS*> m_EncodedData;

	Frequencies * m_Frequencies;

	/**sums up frequencies and breaks if sum is greater than codeword*/
	const Symbol lookUp(const CodewordRANS& codeword) const;

public:

	/**
	* Constructor: used for encoding
	*/
	SingleWordRANS(Frequencies* frequencies)
	{
		m_Frequencies = frequencies;
	}

	/**
	* Constructor: used for decoding
	*/
	SingleWordRANS(Frequencies* frequencies, std::map<uint32, EncodingResultRANS*>& encodedData)
		: SingleWordRANS(frequencies)
	{
		m_EncodedData = encodedData;
	}

	CodewordRANS encode(CodewordRANS& codeword, Symbol& symbol, EncodingResultRANS* encodingResult) const;

	CodewordRANS decode(Symbol& symbol, const CodewordRANS codeword) const;
};

#endif