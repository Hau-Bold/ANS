#include "Frequencies.h"

uint32 Frequencies::getSumOfFrequencies(void) const
{
	return std::accumulate(m_Frequencies.begin(), m_Frequencies.end(), 0);
}

std::vector<uint32>::iterator Frequencies::getMinimalFrequency(void)
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

std::vector<uint32>::iterator Frequencies::getMaximalFrequency(void)
{
	std::vector<uint32>::const_iterator iteratorAtMaximum = max_element(std::begin(m_Frequencies), std::end(m_Frequencies));

	if (*iteratorAtMaximum > 0)
	{
		/**to cast the const away*/
		return m_Frequencies.erase(iteratorAtMaximum, iteratorAtMaximum);
	}
	else
	{
		throw std::runtime_error("Frequencies not initialized");
	}
}

const uint32 Frequencies::getSumOfFrequencies(const Symbol & place) const
{
	if (!m_Frequencies.empty())
	{
		if ((place > m_Frequencies.size()) || (place < 0))
		{
			throw  std::out_of_range("used index is out of range.");
		}
		std::vector<uint32>::const_iterator iteratorAtPlace = m_Frequencies.begin() + (place);

		return std::accumulate(m_Frequencies.begin(), iteratorAtPlace, 0);
	}
	else
	{
		throw  std::runtime_error("Code alphabet not initialized.");
	}
}




