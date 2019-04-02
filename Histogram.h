#pragma once
#ifndef HISTOGRAM_H
#define HISTOGRAM_H
#include "ANSTypes.h"
#include<algorithm>
#include <iostream>
#include<numeric>
#include<thread>
#include<unordered_map>
#include <vector>

using namespace cogra;
/*
* class Histogram:
* holds the code alphabet in form of an unordered map
* and provides the ability to compute code alphabet's entropy.
*/
class Histogram
{

private:

	static Histogram * m_Instance;
	std::vector<uint32> m_Frequencies;
	std::vector<uint8> m_PreconditionedData;
	uint8 * m_Image;
	int32 m_Width, m_Height, m_CountOfChannels;

	/*constructor.
	*
	*@param image - the image
	*
	*/
	Histogram::Histogram(uint8* image, const int32& width, const int32& height, const int32& countOfChannels) :m_Image(image), m_Width(width), m_Height(height), m_CountOfChannels(countOfChannels) {}

	/*
	* yields the code alphabet's entropy
	*/
	double getEntropy(void) const;

	/**yields the sum of the frequencies*/
	uint32 getSumOfFrequencies(void) const;

	/*
	* counts the frequencies of the image of all pixels in same row.
	*
	*@param row - the row
	*/
	std::thread countFrequencies(const int32& row);

	/*
	* counts the frequencies of the image of all pixels in same row.
	*
	*@param row - the row
	*/
	std::thread countPreconditionedFrequencies(const int32& row);

public:

	/*to delete  dynamically allocated memory*/
	void deleteInstance();

	/**
	* yields an instance of Histogram
	*/
	static Histogram * getInstance(uint8* image, const int32& width, const int32& height, const int32& countOfChannels);


	/**counts the frequencies of pixelvalues of the image*/
	void countFrequencies(void);

	/**counts the frequencies of the preconditioned image*/
	void countPreconditionedFrequencies(void);



	/**sends desired values to console
	*
	*@param messageForEntropy - the message for the entropy
	*
	*@param messageForSumOfFrequencies - the message for the sum of frequencies
	*/

	void verbose(const std::string& messageForEntropy, const std::string& messageForSumOfFrequencies) const;

	/**get & set follows below here*/

	void Histogram::setFrequencies(const std::vector<uint32>& frequencies)
	{
		m_Frequencies = frequencies;
	}

	std::vector<uint32> getFrequencies() const
	{
		return m_Frequencies;
	}

	void setPreconditionedData(const std::vector<uint8> & preconditionedData)
	{
		m_PreconditionedData = preconditionedData;
	}

};

#endif