#pragma once
#ifndef ENCODING_RESULT_RANS_H__
#define ENCODING_RESULT_RANS_H__

#include "ANSTypes.h"
#include "cogra\types.h"
#include<vector>

using namespace cogra;
/*
* the class EncodingResult:
* holds the modulo rests of normalization and corresponding final state
*/
class EncodingResultRANS
{

private:
	std::vector<std::vector<RansByte>> m_ModuloRest;
	CodewordRANS m_finalState;

public:

	EncodingResultRANS(const CodewordRANS& finalState, const std::vector<std::vector<RansByte>>& moduloRests) : m_finalState(finalState), m_ModuloRest(moduloRests) {}

	/**default Constructor.*/
	EncodingResultRANS() = default;

	//~EncodingResult();

	void appendModuloRest(const std::vector<RansByte>& moduloRestRestsOfSymbol)
	{
		m_ModuloRest.push_back(moduloRestRestsOfSymbol);
	}



	//set & get follows below here:

	CodewordRANS getFinalState() const { return m_finalState; }
	void setFinalState(const CodewordRANS & finalState) { m_finalState = finalState; }

	std::vector<std::vector<RansByte>> getModuloRest() { return m_ModuloRest; }
};
#endif