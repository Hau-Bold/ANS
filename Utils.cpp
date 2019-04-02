#include"Utils.h"

std::string Utils::convertCharToString(char receiving)
{
	std::stringstream stringStream;
	std::string response;

	stringStream << receiving;
	stringStream >> response;

	return response;
}

void  Utils::replaceAll(std::string & receiving, char toReplace, char replacement)
{
	std::replace(receiving.begin(), receiving.end(), toReplace, replacement);
}

uint8 * Utils::loadImage(const std::string& path, int32& width, int32& height, int32& countOfChannels)
{
	stbi_uc* image = stbi_load(path.c_str(), &width, &height, &countOfChannels, 0);

	if (image)
	{
		printFileSize(path);


		double countOfPixels = static_cast<double>(width * height * countOfChannels);
		printf("%.*lf KB loaded for compression.\n", 3, countOfPixels / 1000);
		return static_cast<cogra::uint8*>(image);
	}
	else
	{
		throw new std::invalid_argument("path " + path + "is not correct");
	}
}

void Utils::testValidity(PreconditionerType preconditionerMode, uint8 * val1, std::vector<uint8> & val2, int32 width, int32 height, int32 countOfChannels)
{

	bool isValid = true;
	switch (preconditionerMode)
	{
	case NONE:
	case SUB:
	case AVERAGE2:
	case AVERAGE3:
	case PAETH:
		for (int row = 0; row < height; row++) {
			for (int column = 0; column < width; column++) {

				int counter = (row * width + column) * countOfChannels;

				for (int8 channel = 0; channel < countOfChannels; channel++)
				{
					uint8 tmp1 = val1[counter + channel];
					uint8 tmp2 = val2[counter + channel];

					if (!tmp1 == tmp2)
					{
						std::cout << "false values at column: " << std::to_string(column) << " false values at row: " << std::to_string(row) << " channel:" << std::to_string(channel) << std::endl;
						std::cout << std::to_string(tmp1) << " : " << std::to_string(tmp2) << std::endl;
						isValid = false;
					}
				}
			}
		}

		std::cout << "State: " << std::to_string(isValid) << std::endl;
		break;

	case UP:
	case AVERAGE4:
	case AVERAGE5:
	case AVERAGE6:
	case AVERAGE7:
	case AVERAGE8:

		for (int32 column = 0; column < width; column++)
		{

			for (int32 row = 0; row < height; row++)
			{

				//access current pixel
				int32 counter = row * width + column;
				for (int16 channel = 0; channel < countOfChannels; channel++)
				{
					uint8 tmp1 = val1[counter + channel];
					uint8 tmp2 = val2[counter + channel];

					if (!(tmp1 == tmp2))
					{
						std::cout << "false values at column: " << std::to_string(column) << " false values at row: " << std::to_string(row) << " channel:" << std::to_string(channel) << std::endl;
						std::cout << std::to_string(tmp1) << " : " << std::to_string(tmp2) << std::endl;

						isValid = false;
					}
				}
			}
		}

		std::cout << "State: " << std::to_string(isValid) << std::endl;
		break;

	default:

		throw new std::invalid_argument("Preconditioner mode <" + std::to_string(preconditionerMode) + "> not implemented yet");
		break;
	}

}

uint32 Utils::findNearestPowerOfTwo(const int & receiving)
{
	int lowerBound = 1;
	int upperBound = 2;
	int tmp = receiving;

	while (tmp > 1)
	{
		tmp >>= 1;
		upperBound <<= 1;
	}

	lowerBound = upperBound >> 1;

	return std::abs(upperBound - receiving) > std::abs(lowerBound - receiving)
		? static_cast<uint32>(lowerBound) : static_cast<uint32>(upperBound);
}

int Utils::findLastIndexOf(const std::string & receiving, char occurence)
{
	int response = -1;

	for (int position = 0; position < receiving.length(); position++)
	{
		if (receiving[position] == occurence)
		{
			response = position;
			break;
		}
	}

	return response;
}

bool Utils::checkResource(std::string & path)
{
	int indexOfDot = findLastIndexOf(path, '.');
	int length = static_cast<int>(path.length());

	std::string ending = path.substr(indexOfDot, length);

	return (ending == ANSConstants::FORMAT_ENDING);
}

std::vector<uint8> Utils::bitMaskToUint8(std::vector<uint16>& bitVector)
{
	std::vector<uint8> response;

	const auto countOfElements = bitVector.size();
	const auto n = countOfElements / 8;
	const auto rest = countOfElements % 8;

	if (bitVector.size() > 0)
	{
		int k = 0;

		while (k < n)
		{
			uint8 binary = 0;
			for (int i = 0; i < 8; i++)
			{
				binary += (bitVector[(8 * k) + i] * (1 << i));
			}
			response.emplace_back(binary);

			k++;
		}

		for (int i = 0; i < rest; i++)
		{
			response.emplace_back(bitVector[countOfElements - rest + i]);
		}

		return response;
	}
	else
	{
		return response;
	}
}

std::vector<uint8> Utils::uint8ToBitMask(uint8& number)
{
	std::vector<uint8>response(8);
	int pos = 0;
	uint8 res = number;
	while (res > 0)
	{
		response[pos] = res % 2;
		res >>= 1;
		pos++;
	}

	return response;
}

void Utils::writePPMImage(const std::string & path, const std::vector<uint8>& data, const uint32 & width, const uint32 & height, const uint32 & countOfChannels)
{
	std::ofstream outFile;

	outFile.open(path, std::ios::out | std::ios::binary);

	outFile << "P6\n" << width << " " << height << "\n255\n";
	for (uint32 row = 0; row < height; ++row) {
		for (uint32 column = 0; column < width; column++)
		{
			int32 counter = (row * width + column) * countOfChannels;

			for (uint32 channel = 0; channel < countOfChannels; channel++)
			{
				outFile << (unsigned char)(data[counter + channel]);
			}
		}
	}
}

std::string Utils::generatePath(const std::string& path, const std::string& ending)
{
	size_t indexOfDot = findLastIndexOf(path, '.');
	size_t length = path.length();

	std::string pathToBinaryFormat = path.substr(0, indexOfDot);

	return pathToBinaryFormat + ending;
}

void Utils::printFileSize(const std::string& path)
{
	struct stat filestatus;
	stat(path.c_str(), &filestatus);
	std::cout << "compressed to " << filestatus.st_size << " bytes\n";
}

std::map<uint32, EncodingResultUANS*> Utils::prepareEncodingResultForDecodingUANS(TransformedResult * transformedResult)
{
	std::map<uint32, EncodingResultUANS*>response;
	std::vector<CodewordUANS> finalStates = transformedResult->getFinalStatesUANS();

	std::vector<uint32> sizes = transformedResult->getSizesUANS();

	/**get count of untouched bits in moduloRestsTotal*/

	std::vector<uint8> moduloRestsTotal = transformedResult->getBitsUANS();
	size_t size = moduloRestsTotal.size();//last place
	size_t rest = moduloRestsTotal[size - 1];
	moduloRestsTotal.resize(size - 1);

	std::vector<uint8> moduloRestsUANS;
	//ie the last count of remain unchained.....
	int k = 0;
	for (int k = 0; k < size - 1 - rest; k++)
	{
		std::vector<uint8> bitMask = uint8ToBitMask(moduloRestsTotal[k]);

		for (auto& element : bitMask)
		{
			moduloRestsUANS.emplace_back(element);
		}
	}

	for (auto i = size - 1 - rest; i < size - 1; i++)
	{
		moduloRestsUANS.emplace_back(moduloRestsTotal[i]);
	}

	for (auto position = 0; position < sizes.size(); position++)
	{
		std::vector<uint16> moduloRestsCorrespondingToItem;
		uint32 currentSize = sizes[position];
		CodewordUANS finalState = finalStates[position];
		static int startPoint = 0;

		for (uint32 i = 0; i < currentSize; i++)
		{
			moduloRestsCorrespondingToItem.emplace_back(moduloRestsUANS[startPoint + i]);
		}
		response.emplace(std::make_pair(position, new EncodingResultUANS(finalState, moduloRestsCorrespondingToItem)));

		startPoint += currentSize;
	}
	return response;
}

std::map<uint32, EncodingResultRANS*> Utils::prepareEncodingResultForDecodingRANS(TransformedResult * transformedResult)
{
	std::map<uint32, EncodingResultRANS*>response;
	std::vector<CodewordRANS> finalStates = transformedResult->getFinalStatesRANS();
	std::vector<uint16> sizes = transformedResult->getSizesRANS();
	std::vector<RansByte> bitRANS = transformedResult->getBitsRANS();
	std::vector<uint8> encodingStepsRANS = transformedResult->getEncodingStepsRANS();

	//the sizes for a symbol
	std::vector<uint16> sizesCorrespondingToItem;

	for (int i = 0; i < encodingStepsRANS.size(); i++)
	{
		std::vector<uint8>bitMask= uint8ToBitMask(encodingStepsRANS[i]);
		for (auto& element : bitMask)
		{
			sizesCorrespondingToItem.emplace_back(element);
		}
	}
	int counter = 0;
	int counterRest=0;
	for (int position = 0;position < sizes.size(); position++)
	{
		uint16 currentSize = sizes[position];//the count of vectors of vectors

		std::vector<std::vector<RansByte>> moduloRests(currentSize);

		for (int id = 0; id < currentSize; id++)
		{
			uint16 sizeCorrespondingToItem = sizesCorrespondingToItem[counter +id];

			std::vector<RansByte> tempEncodingSteps(sizeCorrespondingToItem);

			for (int k = 0; k < sizeCorrespondingToItem; k++)
			{
				tempEncodingSteps[k] = bitRANS[counterRest + k];
			}
			moduloRests[id]=tempEncodingSteps;
			counterRest += sizeCorrespondingToItem;
		}
		response.emplace(std::make_pair(position, new EncodingResultRANS(finalStates[position], moduloRests)));

		counter += currentSize;
	}

	return response;
}