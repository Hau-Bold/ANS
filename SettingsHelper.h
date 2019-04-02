#ifndef SETTINGS_HELPER_H
#define SETTINGS_HELPER_H

#include<iostream>

/**the class SettingsHelper*/
class SettingsHelper
{

public:
	/*
	*
	*Constructor.
	*/
	SettingsHelper()
	{
		std::cout << " ENCODING:" << std::endl;
		std::cout << std::endl;
		std::cout << " Command line arguments in mode of encoding must correspond the following form:" << std::endl;
		std::cout << " (path) (preconditioner) (compressor) (mode of compressor) (verbose)" << std::endl;
		std::cout << " path - path to image that is desired to be compressed " << std::endl;
		std::cout << " preconditioner - select an integer between 0 and 10 " << std::endl;
		std::cout << "0 - NONE " << std::endl;
		std::cout << "1 - SUB " << std::endl;
		std::cout << "2 - UP " << std::endl;
		std::cout << "3 - AVERAGE2 " << std::endl;
		std::cout << "4 - AVERAGE3 " << std::endl;
		std::cout << "5 - AVERAGE4 " << std::endl;
		std::cout << "6 - AVERAGE5 " << std::endl;
		std::cout << "7 - AVERAGE6 " << std::endl;
		std::cout << "8 - AVERAGE7 " << std::endl;
		std::cout << "9 - AVERAGE8 " << std::endl;
		std::cout << "10 - PAETH " << std::endl;
		std::cout << " compressor - 0 for rans, 1 for uans " << std::endl;
		std::cout << "optional: verbose - -v if verbosing is desired" << std::endl;
		std::cout << std::endl;
		std::cout << " DECODING:" << std::endl;
		std::cout << std::endl;
		std::cout << " Command line arguments in mode of decoding must correspond the following form:" << std::endl;
		std::cout << " (path)  (verbose)" << std::endl;
		std::cout << " path - path to binary file that is desired to be uncompressed " << std::endl;
		std::cout << "optional: verbose - -v if verbosing is desired" << std::endl;
	}
};


#endif