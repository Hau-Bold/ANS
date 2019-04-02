#pragma once
#ifndef UTILS_H
#define UTILS_H

#include "ANSTypes.h"
#include "ANSConstants.h"
#include "EncodingResultUANS.h"
#include "EncodingResultRANS.h"
#include "Histogram.h"
#include "PreconditionerType.h"
#include "TransformedResult.h"

#include <algorithm>
#include <cogra/types.h>
#include <fstream>
#include <iostream>
#include <map>
#include <stb/stb_image.h>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>


class Utils
{
public:

	static std::string convertCharToString(char);
	
	static void replaceAll(std::string& receiving, char toReplace, char replacement);

	static uint8 * loadImage(const std::string& path, int32& width, int32& height, int32& countOfChannels);

	static void testValidity(PreconditionerType preconditionerMode, uint8 * val1, std::vector<uint8> & val2, int32 width, int32 height, int32 countOfChannels);

	/*
	*yields the last index of an occurence in a string
	*
	*@param receiving - the string that should be examined to find the last index of occurence
	*
	*@param occurence - the char, that index of  occurence is requested
	*
	*@param occurence - the char, that last index of occurence is requested
	*
	*@return - index of last occurence
	*/
	static int findLastIndexOf(const std::string& receiving, char occurence);

	/**
	*yields the power of 2 which is closest to receiving
	*
	*@param receiving - the number that should be approximated by a power of two
	*
	*@return the power of 2  which is closest to receiving
	*/
	static uint32 findNearestPowerOfTwo(const int& receiving);

	static bool checkResource(std::string& path);

	static void writePPMImage(const std::string& path, const std::vector<uint8>& data, const uint32& width, const uint32& height, const uint32& countOfChannels);

	static std::vector<uint8> bitMaskToUint8(std::vector<uint16>& bitVector);

	static std::vector<uint8> uint8ToBitMask(uint8& number);

	static std::string generatePath(const std::string& path, const std::string& ending);

	static void printFileSize(const std::string& path);

	static std::map<uint32, EncodingResultUANS*> prepareEncodingResultForDecodingUANS(TransformedResult* transformedResult);

	static std::map<uint32, EncodingResultRANS*> prepareEncodingResultForDecodingRANS(TransformedResult* transformedResult);
};
#endif