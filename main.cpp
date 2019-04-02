#include <cogra/types.h>
#include<future>
#include <string>
#include <thread>
#include <vector>
#include <map>

#include "ANSConstants.h"
#include "ANSTypes.h"
#include "CompressorTypes.h"
#include "CryptionType.h"
#include "EncodingResultRANS.h"
#include "EncodingResultUANS.h"
#include "FormatWriter.h"
#include "FormatWriterUANS.h"
#include "FormatWriterRANS.h"
#include "FormatReader.h"
#include "FormatReaderRANS.h"
#include "FormatReaderUANS.h"
#include "Frequencies.h"
#include "FrequenciesRANS.h"
#include "FrequenciesUANS.h"
#include "Histogram.h"
#include "IndexSubset.h"
#include "MeasureTime.h"
#include "Preconditioner.h"
#include "PreconditionerType.h"
#include "Setting.h"
#include "SettingsHelper.h"
#include "SingleWordRANS.h"
#include "SingleWordUANS.h"
#include "TransformedResult.h"

using namespace cogra;

//! the data of loaded image
uint8* image = nullptr;

//!dimensions of loaded image
int32 m_Width, m_Height, m_CountOfChannels;

//!holds the result of uans encoding
std::map<uint32, EncodingResultUANS*>m_EncodedDataUANS;

//!holds the result of rans encoding
std::map<uint32, EncodingResultRANS*>m_EncodedDataRANS;

/*!holds arguments of setting, i.e. used preconditioner,
 used compressor, dimensions of image*/
std::vector<uint32> general;

//!count of used arguments
uint32 countOfGeneralArguments;

//! at end of compression, bytes may be concluded (i.e. a bitmask to an uint8) 
TransformedResult * transformedResult = nullptr;

//!path to binary analogue;result of compression is there
std::string pathToBinary;

Setting * m_Setting = nullptr;
Histogram * m_Histogram = nullptr;
FormatWriter * formatWriter = nullptr;
FormatReader * formatReader = nullptr;
Frequencies * frequencies = nullptr;
//! the frequencies for coding
std::vector<uint32> m_Frequencies;

Preconditioner * m_Preconditioner = nullptr;

//!preconditioned image (which will be compressed) 
std::vector<uint8> m_PreconditionedData;

//!result of preconditioning
std::vector<std::vector<uint8>> m_PreconditionResult;

SingleWordUANS* singleWordUANS = nullptr;
SingleWordRANS * singleWordRANS = nullptr;


std::vector<uint8> m_DecodedData;
PreconditionerType m_TypeOfPreconditioner;
CompressorTypes m_TypeOfCompressor;



int32 m_PreconditionerSize = 0, m_LowerBound, m_Size;

void loadGeneralArguments(const bool isDecoding, std::vector<uint32>& generalArguments);
Frequencies* loadFrequencies(std::vector<uint32> frequencies, const bool doVerbose);

EncodingResultUANS* encodeUANS(const uint32& row);
std::thread decodeUANS(const uint32& row, EncodingResultUANS* encodedData);

EncodingResultRANS* encodeRANS(const uint32& row);
std::thread decodeRANS(const uint32& row, EncodingResultRANS* encodingResult);

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;
	system("color 02");

	m_Setting = Setting::getInstance(argc, argv);

	if (m_Setting->getIsSettingOk())
	{
		switch (m_Setting->getTypeOfCryption())
		{
		case ENCODE:
		general = m_Setting->getGeneralArguments();
		loadGeneralArguments(false, general);

		image = Utils::loadImage(m_Setting->getPath(), m_Width, m_Height, m_CountOfChannels);

		if (m_CountOfChannels == 4)
		{
			printf("Writing image back to disc is possible!");
		}

		general.emplace_back(m_Width);
		general.emplace_back(m_Height);
		general.emplace_back(m_CountOfChannels);

		m_Histogram = Histogram::getInstance(image, m_Width, m_Height, m_CountOfChannels);
		m_Histogram->countFrequencies();

		if (m_Setting->getDoVerbose())
		{
			m_Histogram->verbose(ANSConstants::ENTROPIE, ANSConstants::SUM_OF_FREQUENCIES);
		}

		frequencies = loadFrequencies(m_Histogram->getFrequencies(), m_Setting->getDoVerbose());

		m_Preconditioner = Preconditioner::getInstance(m_TypeOfPreconditioner, image, m_Width, m_Height, m_CountOfChannels);

		if (m_Preconditioner)
		{
			m_PreconditionResult = m_Preconditioner->executePreCondition();

			if (m_TypeOfPreconditioner != PreconditionerType::NONE)
			{
				general.emplace_back(m_PreconditionResult[0].size());
			}

			if (m_Setting->getDoVerbose())
			{
				MeasureTime::getInstance()->getDuration(ANSConstants::MESSAGE_DURATION_OF_PRECONDITIONING);
			}

			m_PreconditionedData = m_Preconditioner->getPreConditionedData();
			m_Histogram->setPreconditionedData(m_PreconditionedData);
			m_Histogram->countPreconditionedFrequencies();

			if (m_Setting->getDoVerbose())
			{
				m_Histogram->verbose(ANSConstants::PRECONDITIONED_ENTROPIE, ANSConstants::SUM_OF_PRECONDITIONED_FREQUENCIES);
			}

			frequencies->setFrequencies(m_Histogram->getFrequencies());
			frequencies->normalizeFrequencies();
			m_Histogram->setFrequencies(frequencies->getFrequencies());

			if (m_Setting->getDoVerbose())
			{
				m_Histogram->verbose(ANSConstants::PRECONDITIONED_ENTROPIE_NORMALIZED, ANSConstants::SUM_OF_PRECONDITIONED_FREQUENCIES_NORMALIZED);
			}
		}

		/**not longer needed*/
		m_Histogram->deleteInstance();

		if (m_TypeOfCompressor == RANS)
		{
			singleWordRANS = new SingleWordRANS(frequencies);

			CodewordRANS codeword = LOWER_BOUND_RANS;

			for (uint32 row = 0; row < m_Height; row++)
			{
				m_EncodedDataRANS.emplace(row, std::async(std::launch::async, [=]
				{
					return encodeRANS(row);
				}).get());
			}
		}

		else if (m_TypeOfCompressor == UANS)
		{
			frequencies->setLowerBound(LOWER_BOUND_UANS);
			singleWordUANS = new SingleWordUANS(frequencies->organizeIndexSubsets());
			singleWordUANS->setLowerBound(frequencies->getLowerBound());

			for (uint32 row = 0; row < m_Height; row++)
			{
				m_EncodedDataUANS.emplace(row, std::async(std::launch::async, [=]
				{
					return encodeUANS(row);
				}).get());
			}
			general.emplace_back(singleWordUANS->getLowerBound());//this is not necessary if RANS is used, lower bound there is fixed
		}

		if (m_Setting->getDoVerbose())
		{
			MeasureTime::getInstance()->getDuration(ANSConstants::MESSAGE_DURATION_OF_COMPRESSION);
		}

		switch (m_TypeOfCompressor)
		{
		case UANS:
			formatWriter = new FormatWriterUANS(m_Setting->getPath(), general, m_PreconditionResult, frequencies->getFrequencies());
			formatWriter->prepareEncodingResultForWriting(m_EncodedDataUANS);
			formatWriter->write();
			break;

		case RANS:
			formatWriter = new FormatWriterRANS(m_Setting->getPath(), general, m_PreconditionResult, frequencies->getFrequencies());
			formatWriter->prepareEncodingResultForWriting(m_EncodedDataRANS);
			formatWriter->write();
			break;

		default:
			//do nothing
			break;

		}

		Utils::printFileSize(Utils::generatePath(m_Setting->getPath(), ANSConstants::FORMAT_ENDING));

		break;

	case DECODE:
		//std::string path = Utils::generatePath(m_Setting->getPath(), ANSConstants::FORMAT_ENDING);
		formatReader = new FormatReader(m_Setting->getPath());//use setting
		formatReader->readGeneral(general, countOfGeneralArguments);
		loadGeneralArguments(true, general);

		formatReader->readFrequencies(m_Frequencies);
		frequencies = loadFrequencies(m_Frequencies, m_Setting->getDoVerbose());

		if (m_TypeOfPreconditioner != PreconditionerType::NONE)
		{
			formatReader->readPreconditionerResult(m_PreconditionerSize, m_CountOfChannels, m_PreconditionResult);
		}

		if (m_TypeOfCompressor == UANS)
		{
			FromatReaderUANS * readerUANS = new FromatReaderUANS(*(formatReader->getReader()));
			transformedResult = readerUANS->readTransformedResult(m_Size);
			m_EncodedDataUANS =Utils::prepareEncodingResultForDecodingUANS(transformedResult);
		}
		else if (m_TypeOfCompressor == RANS)
		{
			FromatReaderRANS * readerRANS = new FromatReaderRANS(*(formatReader->getReader()));
			transformedResult = readerRANS->readTransformedResult(m_Size);
			m_EncodedDataRANS = Utils::prepareEncodingResultForDecodingRANS(transformedResult);
		}

		if (m_Setting->getDoVerbose())
		{
			MeasureTime::getInstance()->getDuration(ANSConstants::MESSAGE_DURATION_OF_READING_BINARY_FILE);
		}

		/**start decoding*/
		if (m_TypeOfCompressor == RANS)
		{
			singleWordRANS = new SingleWordRANS(frequencies, m_EncodedDataRANS);
			std::map<uint32, EncodingResultRANS*>::iterator iterator = m_EncodedDataRANS.begin();

			m_DecodedData.resize(m_Width * m_Height * m_CountOfChannels);

			EncodingResultRANS* tmp = iterator->second;

			while (iterator != m_EncodedDataRANS.end())
			{
				std::thread t1 = decodeRANS(iterator->first, iterator->second);
				t1.join();
				iterator++;
			}
		}
		else if (m_TypeOfCompressor == UANS)
		{
			frequencies->setLowerBound(m_LowerBound);
			singleWordUANS = new SingleWordUANS(frequencies->organizeIndexSubsets(), m_EncodedDataUANS);
			singleWordUANS->setLowerBound(frequencies->getLowerBound());
			m_DecodedData.resize(m_Width * m_Height * m_CountOfChannels);

			std::map<uint32, EncodingResultUANS*>::iterator iterator = m_EncodedDataUANS.begin();
			int32 row = iterator->first;
			EncodingResultUANS* tmp = iterator->second;

			while (iterator != m_EncodedDataUANS.end())
			{
				std::thread t1 = decodeUANS(iterator->first, iterator->second);
				t1.join();
				iterator++;
			}
		}

		if (m_Setting->getDoVerbose())
		{
			MeasureTime::getInstance()->getDuration(ANSConstants::MESSAGE_DURATION_OF_DECOMPRESSION);
		}

		/**post condition*/
		m_Preconditioner = Preconditioner::getInstance(m_TypeOfPreconditioner, m_Width, m_Height, m_CountOfChannels);
		m_Preconditioner->setData(m_DecodedData);
		m_Preconditioner->executePostCondition(m_PreconditionResult);

		if (m_Setting->getDoVerbose())
		{
			MeasureTime::getInstance()->getDuration(ANSConstants::MESSAGE_DURATION_OF_POSTCONDITIONING);
		}

		Utils::writePPMImage(Utils::generatePath(m_Setting->getPath(), ANSConstants::FORMAT_ENDING_ENCODING), m_Preconditioner->getPostConditionedData(), m_Width, m_Height, m_CountOfChannels);

		default:
			//do nothing
			break;
		}

		if (m_Setting->getDoVerbose())
		{
			MeasureTime::getInstance()->stop();
		}
	}
	else
	{
		new SettingsHelper();
	}

	system("pause");
	return 0;
}

void loadGeneralArguments(const bool isDecoding, std::vector<uint32>& generalArguments)
{
	m_TypeOfPreconditioner = PreconditionerType(generalArguments[0]);
	m_TypeOfCompressor = CompressorTypes(generalArguments[1]);

	if (isDecoding)
	{
		m_Width = generalArguments[2];
		m_Height = generalArguments[3],
			m_CountOfChannels = generalArguments[4];
		m_Size = m_Height;

		if (m_TypeOfCompressor == RANS)
		{
			if (countOfGeneralArguments == 6)
			{
				m_PreconditionerSize = generalArguments[5];
			}
		}

		else if (m_TypeOfCompressor == UANS)
		{
			if (countOfGeneralArguments == 6)
			{
				m_LowerBound = generalArguments[5];
			}

			else if (countOfGeneralArguments == 7)
			{
				m_PreconditionerSize = generalArguments[5];
				m_LowerBound = generalArguments[6];
			}
		}
	}
}

Frequencies* loadFrequencies(std::vector<uint32> frequencies, const bool doVerbose)
{
	Frequencies* response = nullptr;

	switch (m_TypeOfCompressor)
	{
	case RANS:
		response = new FrequenciesRANS(frequencies, doVerbose);
		break;

	case UANS:
		response = new FrequenciesUANS(frequencies, doVerbose);
		break;

	default:
		//do nothing
		break;
	}
	return response;
}

EncodingResultUANS* encodeUANS(const uint32& row)
{
	EncodingResultUANS* response = new EncodingResultUANS();

	CodewordUANS codeword = singleWordUANS->getLowerBound();
	bool isFirstSymbol = true;
	int32 counter = row * m_Width * m_CountOfChannels;

	for (uint32 column = 0; column < m_Width; column++) {

		/*reading symbol*/

		for (uint32 channel = 0; channel < m_CountOfChannels; channel++)
		{
			Symbol symbol = m_PreconditionedData[counter + channel];

			/**new codeword result at each iteration*/
			codeword = singleWordUANS->encode(codeword, symbol, response, isFirstSymbol);
		}

		counter += m_CountOfChannels;
	}

	response->setFinalState(codeword);

	return response;
}

std::thread decodeUANS(const uint32& row, EncodingResultUANS* encodedData)
{
	return std::thread([&] {

		std::vector<uint16> moduloRest = encodedData->getModuloRest();
		int32 countOfRests = static_cast<int32>(moduloRest.size()) - 1;
		uint32 column = m_Width - 1;
		int32 counter = (row * m_Width + column) * m_CountOfChannels + m_CountOfChannels - 1;

		CodewordUANS currentState = encodedData->getFinalState();
		Symbol symbol;

		do {

			while (currentState < m_LowerBound) {

				/** normalize current state */
				uint8 read = static_cast<uint8>(moduloRest[countOfRests]);
				currentState = (currentState << 1) + read;
				countOfRests--;
			}

			currentState = singleWordUANS->decode(currentState, symbol);

			m_DecodedData[counter] = symbol;

			/**to iterate trough row in reverse order */
			counter--;

		} while ((countOfRests > 0) && (counter != -1));
	});
}

EncodingResultRANS* encodeRANS(const uint32& row)
{
	CodewordRANS codeword = BASIS_RANS;
	EncodingResultRANS* response = new EncodingResultRANS();

	int32 counter = row * m_Width * m_CountOfChannels;

	for (int32 column = 0; column < m_Width; column++) {

		for (uint32 countOfChannels = 0; countOfChannels < m_CountOfChannels; countOfChannels++)
		{
			Symbol symbol = m_PreconditionedData[counter + countOfChannels];
			codeword = singleWordRANS->encode(codeword, symbol, response);
		}

		counter += m_CountOfChannels;
	}

	response->setFinalState(codeword);
	return response;
}

std::thread decodeRANS(const uint32& row, EncodingResultRANS* encodingResult)
{
	return std::thread([&] {
	CodewordRANS currentState = encodingResult->getFinalState();

	std::vector<std::vector<RansByte>> moduloRest_16 = encodingResult->getModuloRest();

	int32 countOfRests = static_cast<int32>(moduloRest_16.size() - 1);

	/**start at right!*/
	uint32 column = m_Width - 1;
	int32 counter = (row * m_Width + column) * m_CountOfChannels + m_CountOfChannels - 1;

	do {
		/** normalize current state */
		std::vector<RansByte> rests = moduloRest_16[countOfRests];
		countOfRests--;
		for (int i = rests.size() - 1; i >= 0; i--) {

			RansByte rest = rests[i];
			currentState = BASIS_RANS * currentState + rest;
		}
		Symbol symbol;
		currentState = singleWordRANS->decode(symbol, currentState);
		m_DecodedData[counter] = symbol;

		/**to iterate trough row in reverse order*/
		counter--;

	} while ((counter > -1) && (countOfRests > -1));
	});
}