#include "Histogram.h"

Histogram * Histogram::m_Instance = nullptr;

Histogram * Histogram::getInstance(uint8* image, const int32& width, const int32& height, const int32& countOfChannels)
{
	if (!m_Instance)
	{
		m_Instance = new Histogram(image, width, height, countOfChannels);
	}
	return m_Instance;
}

void Histogram::deleteInstance(void)
{
	if (m_Instance)
	{
		delete m_Instance;
		m_Instance = NULL;
	}
}

void Histogram::countFrequencies(void)
{
	m_Frequencies.resize(COUNT_OF_SYMBOLS);

	for (int row = 0; row < m_Height; row++)
	{
		std::thread t = this->countFrequencies(row);
		t.join();
	}
}

void Histogram::countPreconditionedFrequencies(void)
{
	m_Frequencies.clear();
	m_Frequencies.resize(COUNT_OF_SYMBOLS);

	for (int row = 0; row < m_Height; row++)
	{
		std::thread t = this->countPreconditionedFrequencies(row);
		t.join();
	}
}

std::thread Histogram::countFrequencies(const int32& row)
{
	if (m_Image)
	{
		return std::thread([&] {

			for (int32 column = 0; column < m_Width; column++)
			{
				int32 counter = (row * m_Width + column) * m_CountOfChannels;

				for (int32 channel = 0; channel < m_CountOfChannels; channel++)
				{
					uint8 value = m_Image[counter + channel];
					m_Frequencies[value]++;
				}
			}
		});
	}
	else
	{
		throw std::invalid_argument("Image is not loaded yet!");
	}
}

std::thread Histogram::countPreconditionedFrequencies(const int32& row)
{
	if (!m_PreconditionedData.empty())
	{
		return std::thread([=]
		{
			for (int column = 0; column < m_Width; column++) {

				int32 counter = (row * m_Width + column) * m_CountOfChannels;

				for (int32 channel = 0; channel < m_CountOfChannels; channel++)
				{
					uint8 value = m_PreconditionedData[counter + channel];

					m_Frequencies[value]++;
				}
			}
		});
	}
	else
	{
		throw std::invalid_argument("Image is not preconditioned yet!");
	}
}

uint32 Histogram::getSumOfFrequencies(void) const
{
	return std::accumulate(m_Frequencies.begin(), m_Frequencies.end(), 0);
}

double Histogram::getEntropy(void) const
{
	double entropy = .0;

	int sumOfFrequencies = getSumOfFrequencies();

	if (m_Frequencies.size() > 0)
	{
		for (auto & element : m_Frequencies)
		{
			if (element != 0)
			{
				double tmpProbability = static_cast<double>(element) / static_cast<double>(sumOfFrequencies);

				entropy += tmpProbability * log(tmpProbability);
			}
		}

		entropy *= (-1.) / log(2);
	}

	return entropy;
}

void Histogram::verbose(const std::string& messageForEntropy, const std::string& messageForSumOfFrequencies) const
{
	std::cout << messageForEntropy << " " << getEntropy() << std::endl;
	std::cout << messageForSumOfFrequencies << " " << getSumOfFrequencies() << std::endl;
}

