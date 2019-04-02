#include "FrequenciesRANS.h"

void FrequenciesRANS::normalizeFrequencies(void)
{
	const uint32 sum = getSumOfFrequencies();
	const uint32 powerOfTwo = M_RANS;
		
		
		//Utils::findNearestPowerOfTwo(sum);

	for (auto& element : m_Frequencies)
	{
		if (element != 0)
		{
			auto temp = element;
			const double  relativeFrequency = static_cast<double>(element) / static_cast<double>(sum);

			const double scaledFrequency = ceil(relativeFrequency * powerOfTwo + 0.5);

			/*updating value*/
			element = scaledFrequency;
		}
	}

	// at this point scaledFrequencies do not sum to nearestPowerOfTwo, due to rounding errors they don't

	while (getSumOfFrequencies() != powerOfTwo)
	{
		const auto tmpSum = getSumOfFrequencies();

		if (tmpSum > powerOfTwo)
		{
			const std::vector<uint32>::iterator maximalFrequency = getMaximalFrequency();
			const auto position = std::distance(m_Frequencies.begin(), maximalFrequency);

			uint32 maximum = *maximalFrequency;
			maximum -= 1;

			m_Frequencies[position] = maximum;
		}
		else
		{
			const std::vector<uint32>::iterator minimalFrequency = getMinimalFrequency();
			const auto position = std::distance(m_Frequencies.begin(), minimalFrequency);

			uint32 minimum = *minimalFrequency;
			minimum += 1;

			m_Frequencies[position] = minimum;
		}
	}
}

std::vector<uint32>::iterator FrequenciesRANS::getMinimalFrequency(void)
{
	uint32 minimumGlobal = 2;
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


