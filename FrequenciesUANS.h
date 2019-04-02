#ifndef FREQUENCIES_UANS_H__
#define FREQUENCIES_UANS_H__

#include "Frequencies.h"
#include "IndexSubset.h"

class FrequenciesUANS : public Frequencies
{
private:

	void replaceFrequenciesByClosestPowerOfTwo(void);

	/*
	*the struct SymbolsWithSameFrequency
	*
	*represents all symbols with same frequency and possible modulo rests (for executing UABS encoding)
	*/
	struct SymbolsWithSameFrequency
	{
		uint32 m_PowerOfTwo;//this is a power of two since frequency at this state are normalized
		std::vector<Symbol> m_Symbols;
		std::vector<uint32> m_ModuloRests;

		/**Constructor: initializes possible modulo rests.
		*
		*@param powerOfTwo - the frequency
		*
		*@param symbols - the symbols with same frequency
		*/
		SymbolsWithSameFrequency(const uint32& powerOfTwo, const std::vector<Symbol>& symbols) :m_PowerOfTwo(powerOfTwo), m_Symbols(symbols) {

			m_ModuloRests.resize(m_PowerOfTwo);

			for (uint32 position = 0; position < m_PowerOfTwo; position++)
			{
				m_ModuloRests[position] = position;
			}
		}
	};

	/*
	*the struct SymbolWithModuloRest
	*
	*represent a symbol with it's frequency and it's modulo rest 	*/
	struct SymbolWithModuloRest
	{
		int32 m_PowerOfTwo;
		Symbol m_Symbol;
		int32 m_ModuloRest;
		SymbolWithModuloRest(const int32 & frequency, const Symbol & symbol, const int32 & moduloRest) :m_PowerOfTwo(frequency), m_Symbol(symbol), m_ModuloRest(moduloRest) {}

	};

	struct isLessThan
	{
		inline bool operator()(const SymbolsWithSameFrequency & sameFreq1, const SymbolsWithSameFrequency & sameFreq2)
		{
			return sameFreq1.m_PowerOfTwo < sameFreq2.m_PowerOfTwo;
		}
	};

	std::vector<SymbolsWithSameFrequency> getSymbolsWithSameFrequency(void) const;

	std::vector<SymbolsWithSameFrequency> generateSymbolsWithModuloRests(std::vector<SymbolsWithSameFrequency>& receiving) const;

	std::vector<SymbolWithModuloRest> getvctrSymbolAndModuloRest(const std::vector<SymbolsWithSameFrequency>& receiving) const;

	std::vector<IndexSubset> generateIndexSubsets(const std::vector<SymbolWithModuloRest>& symbolsWithSameFrequency) const;

	void checkLowerBoundOfNormalizedInterval(const std::vector<SymbolWithModuloRest>& symbolsWithCorrectModuloRest, const bool doVerbose);

public:

	/*
	* Constructor.
	*
	* @param frequencies - the frequencies
	*/
	FrequenciesUANS(std::vector<uint32>& frequencies, const bool doVerbose) : Frequencies(frequencies, doVerbose) {}

	void normalizeFrequencies(void) override;

	std::vector<IndexSubset> organizeIndexSubsets(void) override;

	std::vector<uint32>::iterator getMinimalFrequency(void) override;


};
#endif