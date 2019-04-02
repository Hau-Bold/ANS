#ifndef FREQUENCIES_RANS_H__
#define FREQUENCIES_RANS_H__

#include "Frequencies.h"


class FrequenciesRANS : public Frequencies
{
public:

	/*
	* Constructor.
	*/
	FrequenciesRANS(std::vector<uint32>& frequencies, const bool doVerbose):Frequencies(frequencies,doVerbose){}

	void normalizeFrequencies(void) override;
	std::vector<uint32>::iterator getMinimalFrequency(void)override;

};

#endif