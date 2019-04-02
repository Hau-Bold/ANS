#ifndef ANS_CONSTANTS_H__
#define ANS_CONSTANTS_H__
#include <string>

/**the class ANSConstants*/
class ANSConstants
{
public:

	/**constants for entropy*/
	static const std::string ENTROPIE;
	static const std::string ENTROPIE_NORMALIZED;
	static const std::string PRECONDITIONED_ENTROPIE;
	static const std::string PRECONDITIONED_ENTROPIE_NORMALIZED;

	/**constants for sum of frequencies*/
	static const std::string SUM_OF_FREQUENCIES;
	static const std::string SUM_OF_NORMALIZED_FREQUENCIES;
	static const std::string SUM_OF_PRECONDITIONED_FREQUENCIES;
	static const std::string SUM_OF_PRECONDITIONED_FREQUENCIES_NORMALIZED;

	//constants for binary file.
	static const std::string FORMAT_ENDING;
	static const std::string FORMAT_ENDING_ENCODING;

	//miscellaneous
	static std::string MESSAGE_DURATION_OF_PRECONDITIONING;
	static std::string MESSAGE_DURATION_OF_POSTCONDITIONING;
	static std::string MESSAGE_DURATION_OF_COMPRESSION;
	static std::string MESSAGE_DURATION_OF_DECOMPRESSION;
	static std::string MESSAGE_DURATION_OF_READING_BINARY_FILE;

};

#endif