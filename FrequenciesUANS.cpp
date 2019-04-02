#include "FrequenciesUANS.h"

void FrequenciesUANS::normalizeFrequencies(void)
{
	const auto sum = getSumOfFrequencies();
	const auto powerOfTwo = Utils::findNearestPowerOfTwo(sum);//u may use other constants for scaling...

	replaceFrequenciesByClosestPowerOfTwo();

	while (getSumOfFrequencies() != powerOfTwo)
	{
		const uint32 tmp = getSumOfFrequencies();

		if (tmp > powerOfTwo)
		{
			const auto maximalFrequency = getMaximalFrequency();
			const auto position = std::distance(m_Frequencies.begin(), maximalFrequency);
			auto maximum = *maximalFrequency;
			maximum = maximum >> 1;
			m_Frequencies[position] = maximum;
		}
		else
		{
			const auto minimalFrequency = getMinimalFrequency();
			const auto position = std::distance(m_Frequencies.begin(), minimalFrequency);
			auto minimum = *minimalFrequency;
			minimum = minimum << 1;
			m_Frequencies[position] = minimum;
		}
	}
}

void FrequenciesUANS::replaceFrequenciesByClosestPowerOfTwo(void)
{
	if (!m_Frequencies.empty())
	{
		int pos = 0;
		for (uint32 & element : m_Frequencies)
		{
			if (element != 0)
			{
				element = Utils::findNearestPowerOfTwo(element);
			}
			pos++;
		}

	}
	else
	{
		throw  std::runtime_error("Code alphabet not initialized.");
	}
}

std::vector<FrequenciesUANS::SymbolsWithSameFrequency> FrequenciesUANS::getSymbolsWithSameFrequency(void) const
{
	uint32 sum = getSumOfFrequencies();
	std::vector<SymbolsWithSameFrequency> response;
	std::list<uint32> lst_Frequency;
	std::vector<uint8> vctr_Symbol;

	for (uint16 symbol = 0; symbol < COUNT_OF_SYMBOLS; symbol++)
	{
		auto frequency = m_Frequencies[symbol];
		/**check if current frequency is in lst_Frequencies*/
		bool frequencyIsContained = (std::find(lst_Frequency.begin(), lst_Frequency.end(), frequency)) != lst_Frequency.end();

		if ((!frequencyIsContained) && (frequency > 0))
		{
			lst_Frequency.push_back(frequency);
			vctr_Symbol.push_back(symbol);

			for (int16 i = symbol + 1; i < COUNT_OF_SYMBOLS; i++)
			{
				uint32 elementToCompare = m_Frequencies[i];
				if (frequency == elementToCompare)
				{
					vctr_Symbol.push_back(static_cast<uint8>(i));
				}
			}
			response.emplace_back(SymbolsWithSameFrequency(sum / frequency, vctr_Symbol));
			vctr_Symbol.clear();
		}
	}
	std::sort(response.begin(), response.end(), isLessThan());

	return response;
}

std::vector<FrequenciesUANS::SymbolsWithSameFrequency> FrequenciesUANS::generateSymbolsWithModuloRests(std::vector<FrequenciesUANS::SymbolsWithSameFrequency>& receiving) const
{
	std::vector<FrequenciesUANS::SymbolsWithSameFrequency>response;
	
	for (int position = 0; position < receiving.size(); position++)
	{
		auto current = receiving[position];

		auto powerOfTwo = current.m_PowerOfTwo;

		int countOfSymbols = current.m_Symbols.size();
		current.m_ModuloRests.resize(countOfSymbols);

		response.emplace_back(current);

		auto currentModuloRests = current.m_ModuloRests;

		for (int i = position + 1; i < receiving.size(); i++)
		{
			SymbolsWithSameFrequency next = receiving[i];
			std::vector<uint32> nextModuloRests = next.m_ModuloRests;

			for (int j = 0; j < currentModuloRests.size(); j++)
			{
				int32 curr = currentModuloRests[j];

				for (int k = 0; k < nextModuloRests.size(); k++)
				{
					uint32 n = nextModuloRests[k];

					if ((n - curr) % powerOfTwo == 0)
					{
						nextModuloRests.erase(nextModuloRests.begin() + k);
						//k has be replaced:
						k--;
					}
				}
			}

			next.m_ModuloRests = nextModuloRests;
			receiving[i] = next;
		}
	}

	return response;
}

std::vector<IndexSubset> FrequenciesUANS::generateIndexSubsets(const std::vector<FrequenciesUANS::SymbolWithModuloRest>& receiving) const
{
	std::vector<IndexSubset> response;

	for (auto& element : receiving)
	{
		response.emplace_back(IndexSubset(element.m_PowerOfTwo, element.m_ModuloRest, element.m_Symbol, m_LowerBound));
	}

	return response;
}

void FrequenciesUANS::checkLowerBoundOfNormalizedInterval(const std::vector<FrequenciesUANS::SymbolWithModuloRest> & symbolsWithCorrectModuloRest, const bool doVerbose)
{

	SymbolWithModuloRest symbolWithMaximalFrequency = symbolsWithCorrectModuloRest[symbolsWithCorrectModuloRest.size() - 1];

	if (int32(BASIS_UANS * m_LowerBound) < symbolWithMaximalFrequency.m_PowerOfTwo + symbolWithMaximalFrequency.m_ModuloRest)
	{
		int32 lowerBound = 16;

		while (int32(BASIS_UANS * lowerBound) < symbolWithMaximalFrequency.m_PowerOfTwo + symbolWithMaximalFrequency.m_ModuloRest)
		{
			lowerBound <<= 1;
		}

		/**no representative in normalized intervall*/
		setLowerBound(lowerBound);

		if (doVerbose)
		{
			printf("L is now %i \n", m_LowerBound);
		}
	}
}

std::vector<FrequenciesUANS::SymbolWithModuloRest> FrequenciesUANS::getvctrSymbolAndModuloRest(const std::vector<FrequenciesUANS::SymbolsWithSameFrequency> & receiving) const 
{

	std::vector<FrequenciesUANS::SymbolWithModuloRest> response;
	for (auto & tmp : receiving)
	{
		for (uint32 pos = 0; pos < tmp.m_Symbols.size(); pos++)
		{
			response.emplace_back(SymbolWithModuloRest(tmp.m_PowerOfTwo, tmp.m_Symbols[pos], tmp.m_ModuloRests[pos]));
		}
	}

	return response;
}

std::vector<IndexSubset> FrequenciesUANS::organizeIndexSubsets(void)
{
	/**yields all symbols with same frequencies*/
	auto symbolsWithSameFrequency = getSymbolsWithSameFrequency();

	/*yields the symbols and the correponding modulorests**/
	auto symbolsWithCorrectModuloRest = generateSymbolsWithModuloRests(symbolsWithSameFrequency);
	auto vctrSymbolAndModuloRest = getvctrSymbolAndModuloRest(symbolsWithCorrectModuloRest);
	checkLowerBoundOfNormalizedInterval(vctrSymbolAndModuloRest, m_DoVerbose);

	return generateIndexSubsets(vctrSymbolAndModuloRest);
}

std::vector<uint32>::iterator FrequenciesUANS::getMinimalFrequency(void)
{
	uint32 minimumGlobal = 1;
	std::vector<uint32>::iterator iterator = m_Frequencies.begin();

	while (*iterator <= 1)
	{
		iterator++;
	}

	uint32 minimum = *iterator;


	while (iterator != m_Frequencies.end())
	{
		uint32 current = *iterator;

		if ((current < minimum) && (current >= minimumGlobal))
		{
			minimum = *iterator;
		}

		iterator++;
	}


	iterator = m_Frequencies.begin();

	while (*iterator != minimum)
	{
		iterator++;
	}

	return iterator;
}
