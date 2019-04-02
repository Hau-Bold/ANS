#include "Preconditioner.h"
#include <stdexcept>
#include<iostream>

Preconditioner * Preconditioner::m_Instance = NULL;

Preconditioner * Preconditioner::getInstance(const PreconditionerType& preconditionerMode, uint8 * image, const int32& width, const int32& height, const int32& countOfChannels)
{
	if (!m_Instance)
	{
		m_Instance = new Preconditioner(preconditionerMode, image, width, height, countOfChannels);
	}

	return m_Instance;
}


Preconditioner * Preconditioner::getInstance(const PreconditionerType& preconditionerMode, const int32& width, const int32& height, const int32& countOfChannels)
{
	if (!m_Instance)
	{
		m_Instance = new Preconditioner(preconditionerMode, width, height, countOfChannels);
	}

	return m_Instance;
}

Preconditioner::Preconditioner(const PreconditionerType& preconditionerMode, uint8 * image, const int32& width, const int32& height, const int32& countOfChannels)
{
	m_PreconditionerMode = preconditionerMode;
	m_Image = image;
	m_Width = width;
	m_Height = height;
	m_CountOfChannels = countOfChannels;
}

Preconditioner::Preconditioner(const PreconditionerType& preconditionerMode, const int32& width, const int32& height, const int32& countOfChannels)
{
	m_PreconditionerMode = preconditionerMode;
	m_Width = width;
	m_Height = height;
	m_CountOfChannels = countOfChannels;
}

void Preconditioner::deleteInstance()
{
	if (m_Instance)
	{
		delete m_Instance;
		m_Instance = NULL;
	}
}

std::vector<std::vector<Symbol>> Preconditioner::executePreCondition()
{
	m_PreConditionedData.resize(m_Width * m_Height * m_CountOfChannels);
	std::vector<std::vector<Symbol>> response;

	switch (m_PreconditionerMode)
	{

	case NONE:

		concurrency::parallel_for(int32(0), m_Height, [&](int32 row)
		{
			precondition_NONE(row);
		});

		break;

	case SUB:

		response.resize(m_CountOfChannels);

		for (int8 channel = 0; channel < m_CountOfChannels; channel++)
		{
			std::vector<Symbol> res_temp(m_Height);

			for (int32 row = 0; row < m_Height; row++)
			{
				res_temp[row] = std::async(std::launch::async, [=]
				{
					return precondition_SUB(row, channel);
				}).get();
			}

			response[channel] = res_temp;
		}

		break;

	case UP:

		response.resize(m_CountOfChannels);

		for (uint8 channel = 0; channel < m_CountOfChannels; channel++)
		{
			std::vector<Symbol> res_temp(m_Width);

			for (int32 column = 0; column < m_Width; column++)
			{
				res_temp[column] = std::async(std::launch::async, [=]
				{
					return precondition_UP(column, channel);
				}).get();
			}

			response[channel] = res_temp;
		}

		break;

	case AVERAGE2:

		response.resize(m_CountOfChannels);

		for (int32 channel = 0; channel < m_CountOfChannels; channel++)
		{
			std::vector<Symbol> res_temp(m_Height);

			concurrency::parallel_for(int32(0), m_Height, [&](int32 row)
			{
				res_temp[row] = precondition_AVERAGE2(row, channel);
			});

			response[channel] = res_temp;
		}

		break;

	case AVERAGE3:

		response.resize(m_CountOfChannels);

		for (int32 channel = 0; channel < m_CountOfChannels; channel++)
		{
			std::vector<Symbol> res_temp(m_Height);

			concurrency::parallel_for(int32(0), m_Height, [&](int32 row)
			{
				res_temp[row] = precondition_AVERAGE3(row, channel);
			});

			response[channel] = res_temp;
		}

		break;

	case AVERAGE4:

		response.resize(m_CountOfChannels);

		for (uint8 channel = 0; channel < m_CountOfChannels; channel++)
		{
			std::vector<Symbol> res_temp(m_Width);

			concurrency::parallel_for(int32(0), m_Width, [&](int32 column)
			{
				res_temp[column] = precondition_AVERAGE4(column, channel);
			});

			response[channel] = res_temp;
		}

		break;

	case AVERAGE5:

		response.resize(m_CountOfChannels);

		for (uint8 channel = 0; channel < m_CountOfChannels; channel++)
		{
			std::vector<Symbol> res_temp(m_Width);

			concurrency::parallel_for(int32(0), m_Width, [&](int32 column)
			{
				res_temp[column] = precondition_AVERAGE5(column, channel);
			});

			response[channel] = res_temp;
		}

		break;

	case AVERAGE6:

		response.resize(m_CountOfChannels);

		for (uint8 channel = 0; channel < m_CountOfChannels; channel++)
		{
			response[channel] = getResultForHigherAverage(channel);

			concurrency::parallel_for(int32(0), m_Width, [&](int32 column)
			{
				precondition_AVERAGE6(column, channel);
			});
		}

		break;

	case AVERAGE7:

		response.resize(m_CountOfChannels);

		for (uint8 channel = 0; channel < m_CountOfChannels; channel++)
		{
			response[channel] = getResultForHigherAverage(channel);

			concurrency::parallel_for(int32(0), m_Width, [&](int32 column)
			{
				precondition_AVERAGE7(column, channel);
			});
		}

		break;

	case AVERAGE8:

		response.resize(m_CountOfChannels);

		for (uint8 channel = 0; channel < m_CountOfChannels; channel++)
		{
			response[channel] = getResultForHigherAverage(channel);

			concurrency::parallel_for(int32(0), m_Width, [&](int32 column)
			{
				precondition_AVERAGE8(column, channel);
			});
		}

		break;

	case PAETH:

		response.resize(m_CountOfChannels);

		for (int32 channel = 0; channel < m_CountOfChannels; channel++)
		{
			std::vector<Symbol> res_temp(m_Height);

			concurrency::parallel_for(int32(0), m_Height, [&](int32 row)
			{
				res_temp[row] = precondition_PAETH(row, channel);
			});

			response[channel] = res_temp;
		}

		break;

	default:

		throw new std::invalid_argument("Preconditioner mode " + std::to_string(m_PreconditionerMode) + " not implemented yet.");

		break;
	}

	return response;
}

void Preconditioner::executePostCondition(std::vector<std::vector<Symbol>> receiving)
{
	m_PostConditionedData.resize(m_Width * m_Height * m_CountOfChannels);

	switch (m_PreconditionerMode)
	{
	case NONE:

		concurrency::parallel_for(int32(0), m_Height, [&](int32 row)
		{
			postcondition_NONE(row);
		});

		break;

	case SUB:

		concurrency::parallel_for(int32(0), m_CountOfChannels, [&](int32 channel)
		{
			std::vector<Symbol> resultOfChannel = receiving[channel];

			concurrency::parallel_for(int32(0), m_Height, [&](int32 row)
			{
				postcondition_SUB(row, resultOfChannel[row], channel);
			});
		});

		break;

	case UP:

		for (int8 channel = m_CountOfChannels - 1; channel > -1; channel--)
		{
			std::vector<Symbol> resultOfChannel = receiving[channel];

			concurrency::parallel_for(int32(0), m_Width, [&](int32 column)
			{
				postcondition_UP(column, resultOfChannel[column], channel);
			});
		}

		break;

	case AVERAGE2:

		concurrency::parallel_for(int32(0), m_CountOfChannels, [&](int32 channel)
		{
			std::vector<Symbol> resultOfChannel = receiving[channel];
			for (int32 row = 0; row < m_Height; row++)
			{
				postcondition_AVERAGE2(row, resultOfChannel[row], channel);
			}
		});

		break;


	case AVERAGE3:

		concurrency::parallel_for(int32(0), m_CountOfChannels, [&](int32 channel)
		{
			std::vector<Symbol> resultOfChannel = receiving[channel];
			for (int32 row = 0; row < m_Height; row++)
			{
				postcondition_AVERAGE3(row, resultOfChannel[row], channel);
			}
		});

		break;

	case AVERAGE4:

		concurrency::parallel_for(int32(0), m_CountOfChannels, [&](int32 channel)
		{
			std::vector<Symbol> resultOfChannel = receiving[channel];
			postcondition_AVERAGE4(resultOfChannel, channel);
		});

		break;

	case AVERAGE5:

		concurrency::parallel_for(int32(0), m_CountOfChannels, [&](int32 channel)
		{
			std::vector<Symbol> resultOfChannel = receiving[channel];
			postcondition_AVERAGE5(resultOfChannel, channel);
		});

		break;

	case AVERAGE6:

		concurrency::parallel_for(int32(0), m_CountOfChannels, [&](int32 channel)
		{
			setResultForHigherAverage(receiving[channel], channel);
			postcondition_AVERAGE6(channel);
		});

		break;

	case AVERAGE7:

		concurrency::parallel_for(int32(0), m_CountOfChannels, [&](int32 channel)
		{
			setResultForHigherAverage(receiving[channel], channel);
			postcondition_AVERAGE7(channel);
		});

		break;

	case AVERAGE8:

		concurrency::parallel_for(int32(0), m_CountOfChannels, [&](int32 channel)
		{
			setResultForHigherAverage(receiving[channel], channel);
			postcondition_AVERAGE8(channel);
		});

		break;

	case PAETH:

		concurrency::parallel_for(int32(0), m_CountOfChannels, [&](int32 channel)
		{
			std::vector<Symbol> resultOfChannel = receiving[channel];

			for (int32 row = 0; row < m_Height; row++)
			{
				postcondition_PAETH(row, resultOfChannel[row], channel);
			}
		});

		break;

	default:

		throw new std::invalid_argument("Preconditioner mode " + std::to_string(m_PreconditionerMode) + " not implemented yet.");

		break;
	}
}

void Preconditioner::precondition_NONE(const int32 & row)
{

	for (int32 column = 0; column < m_Width; column++)
	{
		int32 counter = (row * m_Width + column) * m_CountOfChannels;

		for (int32 channel = 0; channel < m_CountOfChannels; channel++)
		{
			m_PreConditionedData[counter + channel] = m_Image[counter + channel];
		}
	}
}

void Preconditioner::postcondition_NONE(const int32 & row)
{
	for (int32 column = 0; column < m_Width; column++)
	{
		int32 counter = (row * m_Width + column) * m_CountOfChannels;

		for (int8 channel = 0; channel < m_CountOfChannels; channel++)
		{
			m_PostConditionedData[counter + channel] = m_PreConditionedData[counter + channel];
		}
	}
}

Symbol Preconditioner::precondition_SUB(const int32 & row, const int8 & channel)
{
	Symbol response = 0;
	int16 current = 0, noOverflow = 0;
	int32 counter = 0;

	/*first pixel remains unchanged, i.e it is zero*/
	for (int32 column = 1; column < m_Width; column++)
	{
		/**process pixels*/
		counter = (row * m_Width + column) * m_CountOfChannels;

		current = static_cast<int16>(m_Image[counter + channel]);
		noOverflow = current - static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);

		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);

		if ((column == m_Width - 1))
		{
			//save last pixel value
			response = static_cast<Symbol>(current);
		}
	}

	return response;
}

void Preconditioner::postcondition_SUB(const int32& row, const Symbol& receiving, const int8& channel)
{

	/**setting initial value of counter*/
	int32 counter = (row * m_Width + m_Width - 1) * m_CountOfChannels;
	int16 noOverflow = 0;

	//process last pixel of row
	m_PostConditionedData[counter + channel] = receiving;

	for (auto column = m_Width - 1; column > 0; column--)
	{
		/**access current pixel*/
		counter = (row * m_Width + column) * m_CountOfChannels;

		//compute correct value (value before preconditioning)
		noOverflow = ((int16)m_PostConditionedData[counter + channel]) - ((int16)m_PreConditionedData[counter + channel]);

		m_PostConditionedData[counter - m_CountOfChannels + channel] = static_cast<Symbol>(noOverflow);
	}
}

Symbol Preconditioner::precondition_UP(const int32& column, const int8& channel)
{
	Symbol response = 0;
	int16 current = 0, noOverflow = 0;
	int32 counter = 0, counterUp = 0;

	for (int32 row = 0; row < m_Height; row++)
	{
		//access current pixel
		counter = (row * m_Width + column)* m_CountOfChannels;
		counterUp = counter - m_Width * m_CountOfChannels;
		current = static_cast<int16>(m_Image[counter + channel]);
		noOverflow = current - (row > 0 ? static_cast<int16>(m_Image[counterUp + channel]) : 0);
		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);

		if (row == m_Height - 1)
		{
			response = static_cast<Symbol>(current);
		}
	}

	return response;
}

void Preconditioner::postcondition_UP(const int32& column, const Symbol& receiving, const int8& channel)
{

	int32 counter = ((m_Height - 1)* m_Width + column) * m_CountOfChannels, counterBelow = 0;
	int16 noOverflow = 0;

	m_PostConditionedData[counter + channel] = receiving;

	for (int32 row = m_Height - 2; row >= 0; row--)
	{
		counter = (row * m_Width + column)* m_CountOfChannels;
		counterBelow = counter + m_Width * m_CountOfChannels;
		noOverflow = static_cast<int16>(m_PostConditionedData[counterBelow + channel]) - static_cast<int16>(m_PreConditionedData[counterBelow + channel]);

		m_PostConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);
	}
}

Symbol Preconditioner::precondition_AVERAGE2(const int32 & row, const int8 & channel)
{
	/** Instead of computing
	 x^pre := x^data - floor((x^left + x^up) / 2)
	 we compute
	 x^pre : = 2 *x ^ data - (x^left + x ^ up)
	 without overflow*/

	Symbol response = 0;
	int16 current = 0, noOverflow = 0, up = 0, left = 0, average = 0;
	int32 counter = 0, counterUp = 0;

	if (row == 0)
	{
		for (int32 column = 1; column < m_Width; column++)
		{
			counter = column * m_CountOfChannels;

			current = static_cast<int16>(m_Image[counter + channel]);

			/**compute 2* current - left without overflow*/
			noOverflow = 2 * current - static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);

			/**save overflow*/
			m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);

			if (column == m_Width - 1)
			{
				response = static_cast<Symbol>(current);
			}
		}
	}
	else
	{
		/**row > 0*/
		for (int32 column = 1; column < m_Width; column++)
		{

			//process all other pixels
			counter = (row * m_Width + column) * m_CountOfChannels;
			counterUp = ((row - 1) * m_Width + column) * m_CountOfChannels;
			//left
			current = m_Image[counter + channel];
			up = static_cast<int16>(m_Image[counterUp + channel]);
			left = static_cast<int16>(m_Image[counter + channel - m_CountOfChannels]);
			average = up + left;
			noOverflow = 2 * current - average;

			m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);

			if ((column == m_Width - 1))
			{
				response = static_cast<Symbol>(current);
			}
		}
	}

	return response;
}



void Preconditioner::postcondition_AVERAGE2(const int32 & row, const Symbol & receiving, const int8 & channel)
{

	/**since the row r+1 depends on row r, function is not parallalizable*/
	/**
	we compute
	x^post (=x^left) : = 2 *x ^ data - x^pre -  x^up
	without overflow to revert the precondition*/


	int16 up = 0, current = 0, noOverflow = 0;
	int32 counter = (row * m_Width + m_Width - 1) * m_CountOfChannels, counterUp = 0;

	//at beginnig: save receiving
	m_PostConditionedData[counter + channel] = receiving;
	if (row == 0)
	{
		for (int32 column = m_Width - 1; column > 0; column--)
		{
			counter = column * m_CountOfChannels;

			/**compute 2* postData - data without overflow*/
			noOverflow = 2 * (static_cast<int16>(m_PostConditionedData[counter + channel])) - static_cast<int16>(m_PreConditionedData[counter + channel]);

			/**save overflow*/
			m_PostConditionedData[counter - m_CountOfChannels + channel] = static_cast<Symbol>(noOverflow);
		}
	}
	else
	{
		/**row > 0*/
		for (int32 column = m_Width - 1; column > 0; column--)
		{
			counter = (row* m_Width + column) * m_CountOfChannels;
			counterUp = ((row - 1)* m_Width + column) * m_CountOfChannels;
			up = static_cast<int16>(m_PostConditionedData[counterUp + channel]);
			current = static_cast<int16>(m_PostConditionedData[counter + channel]);
			noOverflow = 2 * current - static_cast<int16>(m_PreConditionedData[counter + channel]) - up;
			m_PostConditionedData[counter - m_CountOfChannels + channel] = static_cast<Symbol>(noOverflow);
		}
	}
}


uint8 Preconditioner::precondition_AVERAGE3(const int32 & row, const int8 & channel)
{
	/** Instead of computing
	x^pre : = x^data - floor((x^left + x^up + x^right) / 3)
	we compute
	x^pre : = 3 *x ^ data - (x^left + x ^ up + x^right)
	without overflow*/
	Symbol  response = 0;
	int16 current = 0, left = 0, up = 0, right = 0, noOverflow = 0;
	int32 counter = 0, counterUp = 0;

	if (row == 0)
	{
		for (int32 column = 1; column < m_Width - 1; column++)
		{

			/**process inner pixels*/
			counter = column * m_CountOfChannels;
			current = static_cast<int16>(m_Image[counter + channel]);
			left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
			right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);
			noOverflow = 3 * current - (left + right);
			m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);
		}


		/**process right border*/
		counter = (m_Width - 1)* m_CountOfChannels;
		current = static_cast<int16>(m_Image[counter + channel]);
		left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
		noOverflow = 3 * current - left;
		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);

		/**saving response*/
		response = static_cast<Symbol>(current);
	}
	else
	{
		/**row > 0*/
		/**process inner pixels*/
		for (int32 column = 1; column < m_Width - 1; column++)
		{
			counter = (row * m_Width + column) * m_CountOfChannels;
			counterUp = ((row - 1)*m_Width + column) * m_CountOfChannels;
			current = static_cast<int16>(m_Image[counter + channel]);
			left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
			up = static_cast<int16>(m_Image[counterUp + channel]);
			right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);
			noOverflow = 3 * current - (left + up + right);
			m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);
		}

		/**process right border*/
		counter = (row * m_Width + m_Width - 1) * m_CountOfChannels;
		counterUp = ((row - 1)*m_Width + m_Width - 1) * m_CountOfChannels;
		current = static_cast<int16>(m_Image[counter + channel]);
		left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
		up = static_cast<int16>(m_Image[counterUp + channel]);
		noOverflow = 3 * current - (left + up);
		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);

		response = static_cast<Symbol>(current);
	}

	return response;
}

void Preconditioner::postcondition_AVERAGE3(const int32 & row, const Symbol & receiving, const int8 & channel)
{
	/**since the row r+1 depends on row r, function is not parallalizable*/

	/**
	we compute
	x^Post (=x^left) : = 3 *x ^ data -  x^up - x^right - x^Pre
	without overflow to revert the precondition*/

	int32 counter = (row * m_Width + m_Width - 1) * m_CountOfChannels, counterUp = 0;
	int16 current = 0, right = 0, up = 0, noOverflow = 0;

	//at beginnig: save receiving
	m_PostConditionedData[counter + channel] = receiving;

	if (row == 0)
	{
		noOverflow = 3 * ((int16)m_PostConditionedData[counter + channel]) - ((int16)m_PreConditionedData[counter + channel]);
		m_PostConditionedData[counter - m_CountOfChannels + channel] = static_cast<Symbol>(noOverflow);

		/**process inner pixels*/
		for (int32 column = m_Width - 2; column > 0; column--)
		{
			counter = column * m_CountOfChannels;
			current = static_cast<int16>(m_PostConditionedData[counter + channel]);
			right = static_cast<int16>(m_PostConditionedData[counter + m_CountOfChannels + channel]);
			noOverflow = 3 * current - right - (int16)m_PreConditionedData[counter + channel];
			m_PostConditionedData[counter - m_CountOfChannels + channel] = static_cast<Symbol>(noOverflow);
		}

	}
	else
	{
		//row > 0;
		counterUp = ((row - 1)* m_Width + m_Width - 1)*m_CountOfChannels;
		current = static_cast<int16>(m_PostConditionedData[counter + channel]);
		up = static_cast<int16>(m_PostConditionedData[counterUp + channel]);
		noOverflow = 3 * current - up - static_cast<int16>(m_PreConditionedData[counter + channel]);
		m_PostConditionedData[counter - m_CountOfChannels + channel] = static_cast<Symbol>(noOverflow);


		for (int32 column = m_Width - 2; column > 0; column--)
		{
			counter = (row*m_Width + column) * m_CountOfChannels;
			counterUp = ((row - 1)*m_Width + column) * m_CountOfChannels;
			current = static_cast<int16>(m_PostConditionedData[counter + channel]);
			right = static_cast<int16>(m_PostConditionedData[counter + m_CountOfChannels + channel]);
			up = static_cast<int16>(m_PostConditionedData[counterUp + channel]);
			noOverflow = 3 * current - (up + right + static_cast<int16>(m_PreConditionedData[counter + channel]));
			m_PostConditionedData[counter - m_CountOfChannels + channel] = static_cast<Symbol>(noOverflow);
		}
	}
}

uint8 Preconditioner::precondition_AVERAGE4(const int32 & column, const int8 & channel)
{
	/** Instead of computing
	x^pre : = x^data - floor((x^left + x^up + x^right + x^below) / 4)
	we compute
	x^pre : = 4 *x ^ data - (x^left + x ^ up + x^right + + x^below)
	without overflow*/
	Symbol  response = 0;
	int16 current = 0, left = 0, up = 0, right = 0, below = 0, noOverflow = 0;
	int32 counter = 0, counterUp = 0, counterBelow = 0;

	if (column == 0)
	{

		counter = column * m_CountOfChannels;
		counterBelow = (m_Width + column) * m_CountOfChannels;

		current = static_cast<int16>(m_Image[counter + channel]);
		right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);
		below = static_cast<int16>(m_Image[counterBelow + channel]);
		noOverflow = 4 * current - (right + below);

		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);


		for (int32 row = 1; row < m_Height - 1; row++)
		{
			counter = (row * m_Width + column) * m_CountOfChannels;
			counterUp = ((row - 1)*m_Width + column) * m_CountOfChannels;
			counterBelow = ((row + 1)*m_Width + column) * m_CountOfChannels;

			current = static_cast<int16>(m_Image[counter + channel]);
			up = static_cast<int16>(m_Image[counterUp + channel]);
			right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);
			below = static_cast<int16>(m_Image[counterBelow + channel]);
			noOverflow = 4 * current - (up + right + below);

			m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);
		}

		/**process below border, i.e. row = m_Height - 1*/
		counter = ((m_Height - 1) * m_Width + column) * m_CountOfChannels;
		counterUp = ((m_Height - 2)*m_Width + column) * m_CountOfChannels;
		current = static_cast<int16>(m_Image[counter + channel]);
		up = static_cast<int16>(m_Image[counterUp + channel]);
		right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);
		noOverflow = 4 * current - (up + right);
		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);

		response = static_cast<Symbol>(current);
	}


	else if ((column > 0) && (column < m_Width - 1))
	{
		/**process all inner pixels*/
		/**process first pixel, i.e. row=0*/

		counter = column * m_CountOfChannels;
		counterBelow = (m_Width + column) * m_CountOfChannels;

		current = static_cast<int16>(m_Image[counter + channel]);
		left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
		right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);
		below = static_cast<int16>(m_Image[counterBelow + channel]);
		noOverflow = 4 * current - (left + right + below);

		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);


		for (int32 row = 1; row < m_Height - 1; row++)
		{
			counter = (row * m_Width + column) * m_CountOfChannels;
			counterUp = ((row - 1)*m_Width + column) * m_CountOfChannels;
			counterBelow = ((row + 1)*m_Width + column) * m_CountOfChannels;

			current = static_cast<int16>(m_Image[counter + channel]);
			left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
			up = static_cast<int16>(m_Image[counterUp + channel]);
			right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);
			below = static_cast<int16>(m_Image[counterBelow + channel]);
			noOverflow = 4 * current - (left + up + right + below);

			m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);
		}

		/**process below border, i.e. row = m_Height - 1*/
		counter = ((m_Height - 1) * m_Width + column) * m_CountOfChannels;
		counterUp = ((m_Height - 2)*m_Width + column) * m_CountOfChannels;
		current = static_cast<int16>(m_Image[counter + channel]);
		left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
		up = static_cast<int16>(m_Image[counterUp + channel]);
		right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);
		noOverflow = 4 * current - (left + up + right);
		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);

		response = static_cast<Symbol>(current);
	}

	else
	{
		/**process right border*/
		/**column == m_Width - 1;*/

		/**process first pixel, i.e. row=0*/
		counter = column * m_CountOfChannels;
		counterBelow = (m_Width + column) * m_CountOfChannels;

		current = static_cast<int16>(m_Image[counter + channel]);
		left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
		below = static_cast<int16>(m_Image[counterBelow + channel]);
		noOverflow = 4 * current - (left + below);

		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);


		for (int32 row = 1; row < m_Height - 1; row++)
		{
			counter = (row * m_Width + column) * m_CountOfChannels;
			counterUp = ((row - 1)*m_Width + column) * m_CountOfChannels;
			counterBelow = ((row + 1)*m_Width + column) * m_CountOfChannels;

			current = static_cast<int16>(m_Image[counter + channel]);
			left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
			up = static_cast<int16>(m_Image[counterUp + channel]);
			below = static_cast<int16>(m_Image[counterBelow + channel]);
			noOverflow = 4 * current - (left + up + below);

			m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);
		}

		/**process below border, i.e. row = m_Height - 1*/
		counter = ((m_Height - 1) * m_Width + column) * m_CountOfChannels;
		counterUp = ((m_Height - 2)*m_Width + column) * m_CountOfChannels;
		current = static_cast<int16>(m_Image[counter + channel]);
		left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
		up = static_cast<int16>(m_Image[counterUp + channel]);
		noOverflow = 4 * current - (left + up);
		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);

		response = static_cast<Symbol>(current);
	}

	return response;
}

void Preconditioner::postcondition_AVERAGE4(const std::vector<Symbol> & receiving, const int8 & channel)
{

	/**since the row r-1 depends on row r && r+1, function is not parallalizable*/
	/**
	we compute
	x^Post ´(=x^up) := 4 *x ^ data -  x^left - x^right - x^Pre
	without overflow to revert the precondition*/
	int32 counter = 0, counterBelow = 0, counterUp = 0;
	int16 current = 0, left = 0, right = 0, up = 0, below = 0, noOverflow = 0;

	/**processing last pixelrow*/
	for (int32 column = 0; column < m_Width; column++)
	{
		counter = ((m_Height - 1) * m_Width + column) * m_CountOfChannels;

		/**saving receiving of channel*/
		m_PostConditionedData[counter + channel] = receiving[column];
	}

	for (int32 row = m_Height - 1; row > 0; row--)
	{
		for (int32 column = 0; column < m_Width; column++)
		{
			counter = (row * m_Width + column) * m_CountOfChannels;
			counterUp = ((row - 1) * m_Width + column) * m_CountOfChannels;
			counterBelow = ((row + 1) * m_Width + column) * m_CountOfChannels;

			left = column == 0 ? 0 : static_cast<int16>(m_PostConditionedData[counter - m_CountOfChannels + channel]);
			right = column == m_Width - 1 ? 0 : static_cast<int16>(m_PostConditionedData[counter + m_CountOfChannels + channel]);
			below = row == m_Height - 1 ? 0 : static_cast<int16>(m_PostConditionedData[counterBelow + channel]);
			current = static_cast<int16>(m_PostConditionedData[counter + channel]);

			noOverflow = 4 * current - (left + right + below + static_cast<int16>(m_PreConditionedData[counter + channel]));
			m_PostConditionedData[counterUp + channel] = static_cast<Symbol>(noOverflow);
		}
	}

}

Symbol Preconditioner::precondition_AVERAGE5(const int32 & column, const int8 & channel)
{
	/** Instead of computing
	x^pre : = x^data - floor((x^left + x^leftUp  + x^up + x^right + x^below) / 5)
	we compute
	x^pre : = 5 *x ^ data - (x^left + x^leftUp + x^up  + x^right + x^below)
	without overflow*/
	Symbol  response = 0;
	int16 current = 0, left = 0, leftUp = 0, up = 0, right = 0, below = 0, noOverflow = 0;
	int32 counter = 0, counterUp = 0, counterBelow = 0;

	if (column == 0)
	{

		counter = column * m_CountOfChannels;
		counterBelow = (m_Width + column) * m_CountOfChannels;

		current = static_cast<int16>(m_Image[counter + channel]);
		right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);
		below = static_cast<int16>(m_Image[counterBelow + channel]);
		noOverflow = 5 * current - (right + below);

		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);


		for (int32 row = 1; row < m_Height - 1; row++)
		{
			counter = (row * m_Width + column) * m_CountOfChannels;
			counterUp = ((row - 1)*m_Width + column) * m_CountOfChannels;
			counterBelow = ((row + 1)*m_Width + column) * m_CountOfChannels;

			current = static_cast<int16>(m_Image[counter + channel]);
			up = static_cast<int16>(m_Image[counterUp + channel]);
			right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);
			below = static_cast<int16>(m_Image[counterBelow + channel]);
			noOverflow = 5 * current - (up + right + below);

			m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);
		}

		/**process below border, i.e. row = m_Height - 1*/
		counter = ((m_Height - 1) * m_Width + column) * m_CountOfChannels;
		counterUp = ((m_Height - 2)*m_Width + column) * m_CountOfChannels;
		current = static_cast<int16>(m_Image[counter + channel]);
		up = static_cast<int16>(m_Image[counterUp + channel]);
		right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);
		noOverflow = 5 * current - (up + right);
		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);

		response = static_cast<Symbol>(current);
	}


	else if ((column > 0) && (column < m_Width - 1))
	{
		/**process all inner pixels*/
		/**process first pixel, i.e. row=0*/

		counter = column * m_CountOfChannels;
		counterBelow = (m_Width + column) * m_CountOfChannels;

		current = static_cast<int16>(m_Image[counter + channel]);
		left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
		right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);
		below = static_cast<int16>(m_Image[counterBelow + channel]);
		noOverflow = 5 * current - (left + right + below);

		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);


		for (int32 row = 1; row < m_Height - 1; row++)
		{
			counter = (row * m_Width + column) * m_CountOfChannels;
			counterUp = ((row - 1)*m_Width + column) * m_CountOfChannels;
			counterBelow = ((row + 1)*m_Width + column) * m_CountOfChannels;

			current = static_cast<int16>(m_Image[counter + channel]);
			left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
			leftUp = static_cast<int16>(m_Image[counterUp - m_CountOfChannels + channel]);
			up = static_cast<int16>(m_Image[counterUp + channel]);
			right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);
			below = static_cast<int16>(m_Image[counterBelow + channel]);
			noOverflow = 5 * current - (left + leftUp + up + right + below);

			m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);
		}

		/**process below border, i.e. row = m_Height - 1*/
		counter = ((m_Height - 1) * m_Width + column) * m_CountOfChannels;
		counterUp = ((m_Height - 2)*m_Width + column) * m_CountOfChannels;
		current = static_cast<int16>(m_Image[counter + channel]);
		left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
		leftUp = static_cast<int16>(m_Image[counterUp - m_CountOfChannels + channel]);
		up = static_cast<int16>(m_Image[counterUp + channel]);
		right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);
		noOverflow = 5 * current - (left + leftUp + up + right);
		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);

		response = static_cast<Symbol>(current);
	}

	else
	{
		/**process right border*/
		/**column == m_Width - 1;*/

		/**process first pixel, i.e. row=0*/
		counter = column * m_CountOfChannels;
		counterBelow = (m_Width + column) * m_CountOfChannels;

		current = static_cast<int16>(m_Image[counter + channel]);
		left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
		below = static_cast<int16>(m_Image[counterBelow + channel]);
		noOverflow = 5 * current - (left + below);

		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);


		for (int32 row = 1; row < m_Height - 1; row++)
		{
			counter = (row * m_Width + column) * m_CountOfChannels;
			counterUp = ((row - 1)*m_Width + column) * m_CountOfChannels;
			counterBelow = ((row + 1)*m_Width + column) * m_CountOfChannels;

			current = static_cast<int16>(m_Image[counter + channel]);
			left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
			leftUp = static_cast<int16>(m_Image[counterUp - m_CountOfChannels + channel]);
			up = static_cast<int16>(m_Image[counterUp + channel]);
			below = static_cast<int16>(m_Image[counterBelow + channel]);
			noOverflow = 5 * current - (left + leftUp + up + below);

			m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);
		}

		/**process below border, i.e. row = m_Height - 1*/
		counter = ((m_Height - 1) * m_Width + column) * m_CountOfChannels;
		counterUp = ((m_Height - 2)*m_Width + column) * m_CountOfChannels;
		current = static_cast<int16>(m_Image[counter + channel]);
		left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
		leftUp = static_cast<int16>(m_Image[counterUp - m_CountOfChannels + channel]);
		up = static_cast<int16>(m_Image[counterUp + channel]);
		noOverflow = 5 * current - (left + leftUp + up);
		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);

		response = static_cast<Symbol>(current);
	}

	return response;
}

void Preconditioner::postcondition_AVERAGE5(const std::vector<Symbol>& receiving, const int8 & channel)
{
	/**
	we compute
	x^Post ´(=x^up) := 5 *x ^ data -  x^left - x^leftUp - x^right - x^below - x^Pre
	without overflow to revert the precondition*/
	int32 counter = 0, counterBelow = 0, counterUp = 0;
	int16 current = 0, left = 0, leftUp = 0, right = 0, below = 0, noOverflow = 0;

	/**processing last pixelrow*/
	for (int32 column = 0; column < m_Width; column++)
	{
		counter = ((m_Height - 1) * m_Width + column) * m_CountOfChannels;

		/**saving receiving of channel*/
		m_PostConditionedData[counter + channel] = receiving[column];
	}

	for (int32 row = m_Height - 1; row > 0; row--)
	{
		for (int32 column = 0; column < m_Width; column++)
		{
			counter = (row * m_Width + column) * m_CountOfChannels;
			counterUp = ((row - 1) * m_Width + column) * m_CountOfChannels;
			counterBelow = ((row + 1) * m_Width + column) * m_CountOfChannels;

			left = column == 0 ? 0 : static_cast<int16>(m_PostConditionedData[counter - m_CountOfChannels + channel]);
			leftUp = column == 0 ? 0 : static_cast<int16>(m_PostConditionedData[counterUp - m_CountOfChannels + channel]);
			right = column == m_Width - 1 ? 0 : static_cast<int16>(m_PostConditionedData[counter + m_CountOfChannels + channel]);
			below = row == m_Height - 1 ? 0 : static_cast<int16>(m_PostConditionedData[counterBelow + channel]);
			current = static_cast<int16>(m_PostConditionedData[counter + channel]);

			noOverflow = 5 * current - (left + leftUp + right + below + (int16)m_PreConditionedData[counter + channel]);
			m_PostConditionedData[counterUp + channel] = static_cast<Symbol>(noOverflow);
		}
	}
}

void Preconditioner::precondition_AVERAGE6(const int32 & column, const int8 & channel)
{
	/** Instead of computing
	x^pre : = x^data - floor((x^left + x^leftUp  + x^up + x^rightUp + x^right + x^below) / 6)
	we compute
	x^pre : = 6 *x ^ data - (x^left + x^leftUp + x^up + x^rightUp  + x^right + x^below)
	without overflow*/
	int16 current = 0, left = 0, leftUp = 0, up = 0, rightUp = 0, right = 0, below = 0, noOverflow = 0;
	int32 counter = 0, counterUp = 0, counterBelow = 0;

	if (column == 0)
	{

		counter = column * m_CountOfChannels;
		counterBelow = (m_Width + column) * m_CountOfChannels;

		current = static_cast<int16>(m_Image[counter + channel]);
		right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);
		below = static_cast<int16>(m_Image[counterBelow + channel]);

		noOverflow = 6 * current - (right + below);
		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);


		for (int32 row = 1; row < m_Height - 1; row++)
		{
			counter = (row * m_Width + column) * m_CountOfChannels;
			counterUp = ((row - 1)*m_Width + column) * m_CountOfChannels;
			counterBelow = ((row + 1)*m_Width + column) * m_CountOfChannels;

			current = static_cast<int16>(m_Image[counter + channel]);
			up = static_cast<int16>(m_Image[counterUp + channel]);
			rightUp = static_cast<int16>(m_Image[counterUp + m_CountOfChannels + channel]);
			right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);
			below = static_cast<int16>(m_Image[counterBelow + channel]);

			noOverflow = 6 * current - (up + rightUp + right + below);
			m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);
		}

		/**process below border, i.e. row = m_Height - 1*/
		counter = ((m_Height - 1) * m_Width + column) * m_CountOfChannels;
		counterUp = ((m_Height - 2)*m_Width + column) * m_CountOfChannels;
		current = static_cast<int16>(m_Image[counter + channel]);
		up = static_cast<int16>(m_Image[counterUp + channel]);
		rightUp = static_cast<int16>(m_Image[counterUp + m_CountOfChannels + channel]);
		right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);

		noOverflow = 6 * current - (up + rightUp + right);
		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);
	}


	else if ((column > 0) && (column < m_Width - 1))
	{
		/**process all inner pixels*/
		/**process first pixel, i.e. row=0*/

		counter = column * m_CountOfChannels;
		counterBelow = (m_Width + column) * m_CountOfChannels;

		current = static_cast<int16>(m_Image[counter + channel]);
		left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
		right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);
		below = static_cast<int16>(m_Image[counterBelow + channel]);

		noOverflow = 6 * current - (left + right + below);
		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);


		for (int32 row = 1; row < m_Height - 1; row++)
		{
			counter = (row * m_Width + column) * m_CountOfChannels;
			counterUp = ((row - 1)*m_Width + column) * m_CountOfChannels;
			counterBelow = ((row + 1)*m_Width + column) * m_CountOfChannels;

			current = static_cast<int16>(m_Image[counter + channel]);
			left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
			leftUp = static_cast<int16>(m_Image[counterUp - m_CountOfChannels + channel]);
			up = static_cast<int16>(m_Image[counterUp + channel]);
			rightUp = static_cast<int16>(m_Image[counterUp + m_CountOfChannels + channel]);
			right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);
			below = static_cast<int16>(m_Image[counterBelow + channel]);

			noOverflow = 6 * current - (left + leftUp + up + rightUp + right + below);
			m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);
		}

		/**process below border, i.e. row = m_Height - 1*/
		counter = ((m_Height - 1) * m_Width + column) * m_CountOfChannels;
		counterUp = ((m_Height - 2)*m_Width + column) * m_CountOfChannels;
		current = static_cast<int16>(m_Image[counter + channel]);
		left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
		leftUp = static_cast<int16>(m_Image[counterUp - m_CountOfChannels + channel]);
		up = static_cast<int16>(m_Image[counterUp + channel]);
		rightUp = static_cast<int16>(m_Image[counterUp + m_CountOfChannels + channel]);
		right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);

		noOverflow = 6 * current - (left + leftUp + up + rightUp + right);
		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);
	}

	else
	{
		/**process right border*/
		/**column == m_Width - 1;*/

		/**process first pixel, i.e. row=0*/
		counter = column * m_CountOfChannels;
		counterBelow = (m_Width + column) * m_CountOfChannels;

		current = static_cast<int16>(m_Image[counter + channel]);
		left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
		below = static_cast<int16>(m_Image[counterBelow + channel]);
		noOverflow = 6 * current - (left + below);

		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);


		for (int32 row = 1; row < m_Height - 1; row++)
		{
			counter = (row * m_Width + column) * m_CountOfChannels;
			counterUp = ((row - 1)*m_Width + column) * m_CountOfChannels;
			counterBelow = ((row + 1)*m_Width + column) * m_CountOfChannels;

			current = static_cast<int16>(m_Image[counter + channel]);
			left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
			leftUp = static_cast<int16>(m_Image[counterUp - m_CountOfChannels + channel]);
			up = static_cast<int16>(m_Image[counterUp + channel]);
			below = static_cast<int16>(m_Image[counterBelow + channel]);

			noOverflow = 6 * current - (left + leftUp + up + below);
			m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);
		}

		/**process below border, i.e. row = m_Height - 1*/
		counter = ((m_Height - 1) * m_Width + column) * m_CountOfChannels;
		counterUp = ((m_Height - 2)*m_Width + column) * m_CountOfChannels;
		current = static_cast<int16>(m_Image[counter + channel]);
		left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
		leftUp = static_cast<int16>(m_Image[counterUp - m_CountOfChannels + channel]);
		up = static_cast<int16>(m_Image[counterUp + channel]);
		noOverflow = 6 * current - (left + leftUp + up);
		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);
	}
}

void Preconditioner::postcondition_AVERAGE6(const int8 & channel)
{
	/**
	we compute
	x^Post ´(=x^rightUp) := 6 *x ^ data -  x^left - x^leftUp - x^up- x^right - x^below - x^Pre
	without overflow to revert the precondition
	-writing pixel values to execute this postcondition is made by setResultForAverage - i.e. last pixel row and first pixel column are already set
	*/
	int32 counter = 0, counterBelow = 0, counterUp = 0;
	int16 current = 0, leftUp = 0, left = 0, up = 0, right = 0, below = 0;
	int16 noOverflow = 0;

	for (int32 row = m_Height - 1; row > 0; row--)
	{
		for (int32 column = 0; column < m_Width - 1; column++)
		{
			counter = (row * m_Width + column) * m_CountOfChannels;
			counterUp = ((row - 1) * m_Width + column) * m_CountOfChannels;
			counterBelow = ((row + 1) * m_Width + column) * m_CountOfChannels;

			left = column == 0 ? 0 : static_cast<int16>(m_PostConditionedData[counter - m_CountOfChannels + channel]);
			leftUp = column == 0 ? 0 : static_cast<int16>(m_PostConditionedData[counterUp - m_CountOfChannels + channel]);
			up = row == 0 ? 0 : static_cast<int16>(m_PostConditionedData[counterUp + channel]);
			right = column == m_Width - 1 ? 0 : static_cast<int16>(m_PostConditionedData[counter + m_CountOfChannels + channel]);
			below = row == m_Height - 1 ? 0 : static_cast<int16>(m_PostConditionedData[counterBelow + channel]);
			current = static_cast<int16>(m_PostConditionedData[counter + channel]);

			noOverflow = 6 * current - (left + leftUp + up + right + below + (int16)m_PreConditionedData[counter + channel]);
			m_PostConditionedData[counterUp + m_CountOfChannels + channel] = static_cast<Symbol>(noOverflow);
		}
	}
}

void Preconditioner::precondition_AVERAGE7(const int32 & column, const int8 & channel)
{
	/** Instead of computing
	x^pre : = x^data - floor((x^left + x^leftUp  + x^up + x^rightUp + x^right + rightBelow + x^below) / 7)
	we compute
	x^pre : = 7 *x ^ data - (x^left + x^leftUp + x^up + x^rightUp  + x^right  + belowRight + x^below)
	without overflow*/
	int16 current = 0, left = 0, leftUp = 0, up = 0, rightUp = 0, right = 0, rightBelow = 0, below = 0, noOverflow = 0;
	int32 counter = 0, counterUp = 0, counterBelow = 0;

	if (column == 0)
	{
		counter = column * m_CountOfChannels;
		counterBelow = (m_Width + column) * m_CountOfChannels;

		current = static_cast<int16>(m_Image[counter + channel]);
		right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);
		rightBelow = static_cast<int16>(m_Image[counterBelow + m_CountOfChannels + channel]);
		below = static_cast<int16>(m_Image[counterBelow + channel]);

		noOverflow = 7 * current - (right + rightBelow + below);
		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);

		for (int32 row = 1; row < m_Height - 1; row++)
		{
			counter = (row * m_Width + column) * m_CountOfChannels;
			counterUp = ((row - 1)*m_Width + column) * m_CountOfChannels;
			counterBelow = ((row + 1)*m_Width + column) * m_CountOfChannels;

			current = static_cast<int16>(m_Image[counter + channel]);
			up = static_cast<int16>(m_Image[counterUp + channel]);
			rightUp = static_cast<int16>(m_Image[counterUp + m_CountOfChannels + channel]);
			right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);
			rightBelow = static_cast<int16>(m_Image[counterBelow + m_CountOfChannels + channel]);
			below = static_cast<int16>(m_Image[counterBelow + channel]);

			noOverflow = 7 * current - (up + rightUp + right + rightBelow + below);
			m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);
		}

		/**process below border, i.e. row = m_Height - 1*/
		counter = ((m_Height - 1) * m_Width + column) * m_CountOfChannels;
		counterUp = ((m_Height - 2)*m_Width + column) * m_CountOfChannels;
		current = static_cast<int16>(m_Image[counter + channel]);
		up = static_cast<int16>(m_Image[counterUp + channel]);
		rightUp = static_cast<int16>(m_Image[counterUp + m_CountOfChannels + channel]);
		right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);

		noOverflow = 7 * current - (up + rightUp + right);
		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);
	}


	else if ((column > 0) && (column < m_Width - 1))
	{
		/**process all inner pixels*/
		/**process first pixel, i.e. row=0*/

		counter = column * m_CountOfChannels;
		counterBelow = (m_Width + column) * m_CountOfChannels;

		current = static_cast<int16>(m_Image[counter + channel]);
		left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
		right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);
		rightBelow = static_cast<int16>(m_Image[counterBelow + m_CountOfChannels + channel]);
		below = static_cast<int16>(m_Image[counterBelow + channel]);

		noOverflow = 7 * current - (left + right + rightBelow + below);
		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);


		for (int32 row = 1; row < m_Height - 1; row++)
		{
			counter = (row * m_Width + column) * m_CountOfChannels;
			counterUp = ((row - 1)*m_Width + column) * m_CountOfChannels;
			counterBelow = ((row + 1)*m_Width + column) * m_CountOfChannels;

			current = static_cast<int16>(m_Image[counter + channel]);
			left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
			leftUp = static_cast<int16>(m_Image[counterUp - m_CountOfChannels + channel]);
			up = static_cast<int16>(m_Image[counterUp + channel]);
			rightUp = static_cast<int16>(m_Image[counterUp + m_CountOfChannels + channel]);
			right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);
			rightBelow = static_cast<int16>(m_Image[counterBelow + m_CountOfChannels + channel]);
			below = static_cast<int16>(m_Image[counterBelow + channel]);

			noOverflow = 7 * current - (left + leftUp + up + rightUp + right + rightBelow + below);
			m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);
		}

		/**process below border, i.e. row = m_Height - 1*/
		counter = ((m_Height - 1) * m_Width + column) * m_CountOfChannels;
		counterUp = ((m_Height - 2)*m_Width + column) * m_CountOfChannels;
		current = static_cast<int16>(m_Image[counter + channel]);
		left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
		leftUp = static_cast<int16>(m_Image[counterUp - m_CountOfChannels + channel]);
		up = static_cast<int16>(m_Image[counterUp + channel]);
		rightUp = static_cast<int16>(m_Image[counterUp + m_CountOfChannels + channel]);
		right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);

		noOverflow = 7 * current - (left + leftUp + up + rightUp + right);
		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);
	}

	else
	{
		/**process right border*/
		/**column == m_Width - 1;*/

		/**process first pixel, i.e. row=0*/
		counter = column * m_CountOfChannels;
		counterBelow = (m_Width + column) * m_CountOfChannels;

		current = static_cast<int16>(m_Image[counter + channel]);
		left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
		below = static_cast<int16>(m_Image[counterBelow + channel]);
		noOverflow = 7 * current - (left + below);

		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);


		for (int32 row = 1; row < m_Height - 1; row++)
		{
			counter = (row * m_Width + column) * m_CountOfChannels;
			counterUp = ((row - 1)*m_Width + column) * m_CountOfChannels;
			counterBelow = ((row + 1)*m_Width + column) * m_CountOfChannels;

			current = static_cast<int16>(m_Image[counter + channel]);
			left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
			leftUp = static_cast<int16>(m_Image[counterUp - m_CountOfChannels + channel]);
			up = static_cast<int16>(m_Image[counterUp + channel]);
			below = static_cast<int16>(m_Image[counterBelow + channel]);

			noOverflow = 7 * current - (left + leftUp + up + below);
			m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);
		}

		/**process below border, i.e. row = m_Height - 1*/
		counter = ((m_Height - 1) * m_Width + column) * m_CountOfChannels;
		counterUp = ((m_Height - 2)*m_Width + column) * m_CountOfChannels;
		current = static_cast<int16>(m_Image[counter + channel]);
		left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
		leftUp = static_cast<int16>(m_Image[counterUp - m_CountOfChannels + channel]);
		up = static_cast<int16>(m_Image[counterUp + channel]);
		noOverflow = 7 * current - (left + leftUp + up);
		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);

	}
}

void Preconditioner::postcondition_AVERAGE7(const int8 & channel)
{
	/**
	we compute
	x^Post ´(=x^rightUp) := 7 *x ^ data -  x^left - x^leftUp - x^up- x^right - x^rightBelow - x^below - x^Pre
	without overflow to revert the precondition
	-writing pixel values to execute this postcondition is made by setResultForAverage - i.e. last pixel row and first pixel column are already set
	*/
	int32 counter = 0, counterBelow = 0, counterUp = 0;
	int16 current = 0, leftUp = 0, left = 0, up = 0, right = 0, rightBelow = 0, below = 0;
	int16 noOverflow = 0;

	for (int32 row = m_Height - 1; row > 0; row--)
	{
		for (int32 column = 0; column < m_Width - 1; column++)
		{
			counter = (row * m_Width + column) * m_CountOfChannels;
			counterUp = ((row - 1) * m_Width + column) * m_CountOfChannels;
			counterBelow = ((row + 1) * m_Width + column) * m_CountOfChannels;

			left = column == 0 ? 0 : static_cast<int16>(m_PostConditionedData[counter - m_CountOfChannels + channel]);
			leftUp = column == 0 ? 0 : static_cast<int16>(m_PostConditionedData[counterUp - m_CountOfChannels + channel]);
			up = row == 0 ? 0 : static_cast<int16>(m_PostConditionedData[counterUp + channel]);
			right = column == m_Width - 1 ? 0 : static_cast<int16>(m_PostConditionedData[counter + m_CountOfChannels + channel]);
			rightBelow = ((column == m_Width - 1) || (row == m_Height - 1)) ? 0 : static_cast<int16>(m_PostConditionedData[counterBelow + m_CountOfChannels + channel]);
			below = row == m_Height - 1 ? 0 : static_cast<int16>(m_PostConditionedData[counterBelow + channel]);
			current = static_cast<int16>(m_PostConditionedData[counter + channel]);

			noOverflow = 7 * current - (left + leftUp + up + right + rightBelow + below + (int16)m_PreConditionedData[counter + channel]);
			m_PostConditionedData[counterUp + m_CountOfChannels + channel] = static_cast<Symbol>(noOverflow);
		}
	}
}


void Preconditioner::precondition_AVERAGE8(const int32 & column, const int8 & channel)
{
	/** Instead of computing
	x^pre : = x^data - floor((x^left + x^leftUp  + x^up + x^rightUp + x^right + x^rightBelow + x^below + x^leftBelow) / 8)
	we compute
	x^pre : = 8 *x ^ data - (x^left + x^leftUp  + x^up + x^rightUp + x^right + x^rightBelow + x^below + x^leftBelow)
	without overflow*/
	int16 current = 0, left = 0, leftUp = 0, up = 0, rightUp = 0, right = 0, rightBelow = 0, below = 0, leftBelow = 0, noOverflow = 0;
	int32 counter = 0, counterUp = 0, counterBelow = 0;

	if (column == 0)
	{
		counter = column * m_CountOfChannels;
		counterBelow = (m_Width + column) * m_CountOfChannels;

		current = static_cast<int16>(m_Image[counter + channel]);
		right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);
		rightBelow = static_cast<int16>(m_Image[counterBelow + m_CountOfChannels + channel]);
		below = static_cast<int16>(m_Image[counterBelow + channel]);

		noOverflow = 8 * current - (right + rightBelow + below);
		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);


		for (int32 row = 1; row < m_Height - 1; row++)
		{
			counter = (row * m_Width + column) * m_CountOfChannels;
			counterUp = ((row - 1)*m_Width + column) * m_CountOfChannels;
			counterBelow = ((row + 1)*m_Width + column) * m_CountOfChannels;

			current = static_cast<int16>(m_Image[counter + channel]);
			up = static_cast<int16>(m_Image[counterUp + channel]);
			rightUp = static_cast<int16>(m_Image[counterUp + m_CountOfChannels + channel]);
			right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);
			rightBelow = static_cast<int16>(m_Image[counterBelow + m_CountOfChannels + channel]);
			below = static_cast<int16>(m_Image[counterBelow + channel]);

			noOverflow = 8 * current - (up + rightUp + right + rightBelow + below);
			m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);
		}

		/**process below border, i.e. row = m_Height - 1*/
		counter = ((m_Height - 1) * m_Width + column) * m_CountOfChannels;
		counterUp = ((m_Height - 2)*m_Width + column) * m_CountOfChannels;
		current = static_cast<int16>(m_Image[counter + channel]);
		up = static_cast<int16>(m_Image[counterUp + channel]);
		rightUp = static_cast<int16>(m_Image[counterUp + m_CountOfChannels + channel]);
		right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);

		noOverflow = 8 * current - (up + rightUp + right);
		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);
	}


	else if ((column > 0) && (column < m_Width - 1))
	{
		/**process all inner pixels*/
		/**process first pixel, i.e. row=0*/

		counter = column * m_CountOfChannels;
		counterBelow = (m_Width + column) * m_CountOfChannels;

		current = static_cast<int16>(m_Image[counter + channel]);
		left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
		right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);
		rightBelow = static_cast<int16>(m_Image[counterBelow + m_CountOfChannels + channel]);
		below = static_cast<int16>(m_Image[counterBelow + channel]);
		leftBelow = static_cast<int16>(m_Image[counterBelow - m_CountOfChannels + channel]);

		noOverflow = 8 * current - (left + right + rightBelow + below + leftBelow);
		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);


		for (int32 row = 1; row < m_Height - 1; row++)
		{
			counter = (row * m_Width + column) * m_CountOfChannels;
			counterUp = ((row - 1)*m_Width + column) * m_CountOfChannels;
			counterBelow = ((row + 1)*m_Width + column) * m_CountOfChannels;

			current = static_cast<int16>(m_Image[counter + channel]);
			left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
			leftUp = static_cast<int16>(m_Image[counterUp - m_CountOfChannels + channel]);
			up = static_cast<int16>(m_Image[counterUp + channel]);
			rightUp = static_cast<int16>(m_Image[counterUp + m_CountOfChannels + channel]);
			right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);
			rightBelow = static_cast<int16>(m_Image[counterBelow + m_CountOfChannels + channel]);
			below = static_cast<int16>(m_Image[counterBelow + channel]);
			leftBelow = static_cast<int16>(m_Image[counterBelow - m_CountOfChannels + channel]);

			noOverflow = 8 * current - (left + leftUp + up + rightUp + right + rightBelow + below + leftBelow);
			m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);
		}

		/**process below border, i.e. row = m_Height - 1*/
		counter = ((m_Height - 1) * m_Width + column) * m_CountOfChannels;
		counterUp = ((m_Height - 2)*m_Width + column) * m_CountOfChannels;
		current = static_cast<int16>(m_Image[counter + channel]);
		left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
		leftUp = static_cast<int16>(m_Image[counterUp - m_CountOfChannels + channel]);
		up = static_cast<int16>(m_Image[counterUp + channel]);
		rightUp = static_cast<int16>(m_Image[counterUp + m_CountOfChannels + channel]);
		right = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);

		noOverflow = 8 * current - (left + leftUp + up + rightUp + right);
		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);
	}

	else
	{
		/**process right border*/
		/**column == m_Width - 1;*/

		/**process first pixel, i.e. row=0*/
		counter = column * m_CountOfChannels;
		counterBelow = (m_Width + column) * m_CountOfChannels;

		current = static_cast<int16>(m_Image[counter + channel]);
		left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
		below = static_cast<int16>(m_Image[counterBelow + channel]);
		leftBelow = static_cast<int16>(m_Image[counterBelow - m_CountOfChannels + channel]);
		noOverflow = 8 * current - (left + below + leftBelow);

		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);


		for (int32 row = 1; row < m_Height - 1; row++)
		{
			counter = (row * m_Width + column) * m_CountOfChannels;
			counterUp = ((row - 1)*m_Width + column) * m_CountOfChannels;
			counterBelow = ((row + 1)*m_Width + column) * m_CountOfChannels;

			current = static_cast<int16>(m_Image[counter + channel]);
			left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
			leftUp = static_cast<int16>(m_Image[counterUp - m_CountOfChannels + channel]);
			up = static_cast<int16>(m_Image[counterUp + channel]);
			below = static_cast<int16>(m_Image[counterBelow + channel]);
			leftBelow = static_cast<int16>(m_Image[counterBelow - m_CountOfChannels + channel]);

			noOverflow = 8 * current - (left + leftUp + up + below + leftBelow);
			m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);
		}

		/**process below border, i.e. row = m_Height - 1*/
		counter = ((m_Height - 1) * m_Width + column) * m_CountOfChannels;
		counterUp = ((m_Height - 2)*m_Width + column) * m_CountOfChannels;
		current = static_cast<int16>(m_Image[counter + channel]);
		left = static_cast<int16>(m_Image[counter - m_CountOfChannels + channel]);
		leftUp = static_cast<int16>(m_Image[counterUp - m_CountOfChannels + channel]);
		up = static_cast<int16>(m_Image[counterUp + channel]);
		noOverflow = 8 * current - (left + leftUp + up);
		m_PreConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);
	}
}

void Preconditioner::postcondition_AVERAGE8(const int8 & channel)
{
	/**
	we compute
	x^Post ´(=x^rightUp) := 8 *x ^ data -  x^left - x^leftUp - x^up- x^right - x^rightBelow - x^below - x^leftBelow - x^Pre
	without overflow to revert the precondition
	-writing pixel values to execute this postcondition is made by setResultForAverage - i.e. last pixel row and first pixel column are already set
	*/
	int32 counter = 0, counterBelow = 0, counterUp = 0;
	int16 current = 0, leftUp = 0, left = 0, up = 0, right = 0, rightBelow = 0, below = 0, leftBelow = 0;
	int16 noOverflow = 0;

	for (int32 row = m_Height - 1; row > 0; row--)
	{
		for (int32 column = 0; column < m_Width - 1; column++)
		{
			counter = (row * m_Width + column) * m_CountOfChannels;
			counterUp = ((row - 1) * m_Width + column) * m_CountOfChannels;
			counterBelow = ((row + 1) * m_Width + column) * m_CountOfChannels;

			left = column == 0 ? 0 : static_cast<int16>(m_PostConditionedData[counter - m_CountOfChannels + channel]);
			leftUp = column == 0 ? 0 : static_cast<int16>(m_PostConditionedData[counterUp - m_CountOfChannels + channel]);
			up = row == 0 ? 0 : static_cast<int16>(m_PostConditionedData[counterUp + channel]);
			right = column == m_Width - 1 ? 0 : static_cast<int16>(m_PostConditionedData[counter + m_CountOfChannels + channel]);
			rightBelow = ((column == m_Width - 1) || (row == m_Height - 1)) ? 0 : static_cast<int16>(m_PostConditionedData[counterBelow + m_CountOfChannels + channel]);
			below = row == m_Height - 1 ? 0 : static_cast<int16>(m_PostConditionedData[counterBelow + channel]);
			leftBelow = ((column == 0) || (row == m_Height - 1)) ? 0 : static_cast<int16>(m_PostConditionedData[counterBelow - m_CountOfChannels + channel]);
			current = static_cast<int16>(m_PostConditionedData[counter + channel]);

			noOverflow = 8 * current - (left + leftUp + up + right + rightBelow + below + leftBelow + static_cast<int16>(m_PreConditionedData[counter + channel]));
			m_PostConditionedData[counterUp + m_CountOfChannels + channel] = static_cast<Symbol>(noOverflow);
		}
	}
}


Symbol Preconditioner::precondition_PAETH(const int32 & row, const int8 & channel)
{
	Symbol response;
	int32 counter = 0, counterUp = 0;
	int16 up = 0, left = 0, leftUp = 0, current = 0, noOverflow = 0;

	if (row == 0)
	{
		for (int32 column = 0; column < m_Width - 1; column++)
		{
			counter = column * m_CountOfChannels;
			left = static_cast<int16>(m_Image[counter + channel]);
			current = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);
			int16 predictor = paethPredictor(left, up, leftUp);
			m_PreConditionedData[counter + m_CountOfChannels + channel] = static_cast<Symbol>(current - predictor);

			if (column == 0)
			{
				response = static_cast<Symbol>(left);
			}
		}

		return response;
	}
	else
	{
		/**row > 0*/
		for (int32 column = 0; column < m_Width - 1; column++)
		{
			counter = (row*m_Width + column)*m_CountOfChannels;
			counterUp = ((row - 1)*m_Width + column)*m_CountOfChannels;
			up = static_cast<int16>(m_Image[counterUp + m_CountOfChannels + channel]);
			left = static_cast<int16>(m_Image[counter + channel]);
			leftUp = static_cast<int16>(m_Image[counterUp + channel]);
			current = static_cast<int16>(m_Image[counter + m_CountOfChannels + channel]);
			int16 predictor = paethPredictor(left, up, leftUp);
			m_PreConditionedData[counter + m_CountOfChannels + channel] = static_cast<Symbol>(current - predictor);

			if (column == 0)
			{
				response = static_cast<Symbol>(left);
			}
		}
	}

	return response;
}

void Preconditioner::postcondition_PAETH(const int32 & row, const Symbol & receiving, const int8 & channel)
{
	int32 counter = (row * m_Width) * m_CountOfChannels, counterUp = 0;
	int16 noOverflow = 0, left = 0, up = 0, leftUp = 0, current = 0, paeth = 0;

	/**saving initial the received value*/
	m_PostConditionedData[counter + channel] = receiving;

	if (row == 0)
	{
		for (int32 column = 1; column < m_Width; column++)
		{
			/**access current pixel*/
			counter = column * m_CountOfChannels;
			left = static_cast<int16>(m_PostConditionedData[counter - m_CountOfChannels + channel]);
			int16  val = static_cast<int16>(m_PreConditionedData[counter + channel]) + paethPredictor(left, 0, 0);
			m_PostConditionedData[counter + channel] = static_cast<Symbol>(val);
		}
	}
	else
	{
		for (int32 column = 1; column < m_Width; column++)
		{
			counter = (row * m_Width + column) * m_CountOfChannels;
			counterUp = ((row - 1) * m_Width + column) * m_CountOfChannels;
			left = static_cast<int16>(m_PostConditionedData[counter - m_CountOfChannels + channel]);
			up = static_cast<int16>(m_PostConditionedData[counterUp + channel]);
			leftUp = static_cast<int16>(m_PostConditionedData[counterUp - m_CountOfChannels + channel]);
			noOverflow = paethPredictor(left, up, leftUp) + static_cast<int16>(m_PreConditionedData[counter + channel]);
			m_PostConditionedData[counter + channel] = static_cast<Symbol>(noOverflow);
		}
	}
}

int16 Preconditioner::paethPredictor(const int16 & left, const int16 & up, const int16 & leftUp)
{
	int16 predictor = left + up - leftUp;
	int16 diffLeft = std::abs(predictor - left);
	int16 diffUp = std::abs(predictor - up);
	int16 diffLeftUp = std::abs(predictor - leftUp);

	if ((diffLeft <= diffUp) && (diffLeft <= diffLeftUp))
	{
		return left;
	}
	else if ((diffUp <= diffLeftUp))
	{
		return up;
	}
	return leftUp;
}

std::vector<Symbol> Preconditioner::getResultForHigherAverage(const uint8 & channel)
{
	std::vector<Symbol> response;
	response.resize(m_Height + m_Width - 1);
	int32 counter = 0;

	for (int32 row = 0; row < m_Height; row++)
	{
		counter = row * m_Width * m_CountOfChannels;
		response[row] = m_Image[counter + channel];
	}

	for (int32 column = 1; column < m_Width; column++)
	{
		counter = ((m_Height - 1) * m_Width + column) * m_CountOfChannels;
		response[m_Height - 1 + column] = m_Image[counter + channel];
	}

	return response;
}

void Preconditioner::setResultForHigherAverage(const std::vector<Symbol>& receiving, const uint8 & channel)
{
	int32 counter = 0;
	for (int32 row = 0; row < m_Height; row++)
	{
		counter = row * m_Width * m_CountOfChannels;
		m_PostConditionedData[counter + channel] = receiving[row];
	}

	for (int32 column = 1; column < m_Width; column++)
	{
		counter = ((m_Height - 1) * m_Width + column) * m_CountOfChannels;
		m_PostConditionedData[counter + channel] = receiving[m_Height - 1 + column];
	}
}