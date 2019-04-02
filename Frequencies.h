#pragma once
#ifndef FREQUENCIES_H
#define FREQUENCIES_H
#include "ANSTypes.h"
#include "PreconditionerType.h"
#include "CompressorTypes.h"
#include "IndexSubset.h"
#include "Utils.h"

#include<algorithm>
#include<iostream>
#include<iterator>
#include<math.h>
#include<numeric>
#include<unordered_map>
#include <vector>

class Frequencies
{

private:

	PreconditionerType m_PreconditionerType;
	CompressorTypes m_CompressionType;

protected:

	std::vector<uint32> m_Frequencies;
	CodewordUANS m_LowerBound;
	bool m_DoVerbose;

	virtual std::vector<uint32>::iterator getMinimalFrequency(void)=0;
	std::vector<uint32>::iterator getMaximalFrequency(void);

public:

	Frequencies(std::vector<uint32>& frequencies, const bool doVerbose) : m_Frequencies(frequencies), m_DoVerbose(doVerbose) {};

	virtual void normalizeFrequencies(void) = 0;
	virtual std::vector<IndexSubset> organizeIndexSubsets(void) { return std::vector<IndexSubset>(0); }

	uint32 getSumOfFrequencies(void) const;
	const uint32 getSumOfFrequencies(const Symbol& place) const;


	/**get & set follows below here*/
	std::vector<uint32> getFrequencies(void) const { return m_Frequencies; }

	void setFrequencies(std::vector<uint32>& frequencies) { m_Frequencies = frequencies; }
	void setLowerBound(const CodewordUANS& lowerBound) { m_LowerBound = lowerBound; }
	CodewordUANS getLowerBound(void) const { return m_LowerBound; }
};
#endif