#pragma once
#ifndef ENCODING_RESULT_UANS_H
#define ENCODING_RESULT_UANS_H
#include "ANSTypes.h"
#include "cogra\types.h"
#include<vector>

using namespace cogra;
/*
* the class EncodingResult:
* holds the modulo rests of normalization and corresponding final state
*/
class EncodingResultUANS
{

private:
	std::vector<uint16> m_ModuloRest;
	CodewordUANS m_finalState;

public:

	EncodingResultUANS(const CodewordUANS& finalState, const std::vector<uint16>& moduloRest) : m_finalState(finalState), m_ModuloRest(moduloRest) {}

	/**default Constructor.*/
	EncodingResultUANS() = default;

	//~EncodingResult();

	void appendModuloRest(const uint16 & moduloRest)
	{
		m_ModuloRest.push_back(moduloRest);
	}

	

	//set & get follows below here:

	CodewordUANS getFinalState() const {return m_finalState;}
	void setFinalState(const CodewordUANS & finalState){m_finalState = finalState;}

	std::vector<uint16> getModuloRest()	{return m_ModuloRest;}
};


#endif




	