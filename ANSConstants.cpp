#include"ANSConstants.h"

/**constants for entropy*/
const std::string ANSConstants::ENTROPIE = "Entropy:";
const std::string ANSConstants::ENTROPIE_NORMALIZED = "Entropy normalized:";
const std::string ANSConstants::PRECONDITIONED_ENTROPIE = "Preconditioned entropy:";
const std::string ANSConstants::PRECONDITIONED_ENTROPIE_NORMALIZED = "Preconditioned entropy normalized:";

/**constants for sum of frequencies*/
const std::string ANSConstants::SUM_OF_FREQUENCIES = "Sum of frequencies";
const std::string ANSConstants::SUM_OF_NORMALIZED_FREQUENCIES = "Sum of frequencies";
const std::string ANSConstants::SUM_OF_PRECONDITIONED_FREQUENCIES = "Sum of preconditioned frequencies";
const std::string ANSConstants::SUM_OF_PRECONDITIONED_FREQUENCIES_NORMALIZED = "Sum of preconditioned frequencies normalized";

//constants for binary file.
const std::string ANSConstants::FORMAT_ENDING = ".bin";
const std::string ANSConstants::FORMAT_ENDING_ENCODING = ".ppm";

std::string ANSConstants::MESSAGE_DURATION_OF_PRECONDITIONING = "Precondition took %.2f seconds\n";
std::string ANSConstants::MESSAGE_DURATION_OF_POSTCONDITIONING = "Postcondition took %.2f seconds\n";
std::string ANSConstants::MESSAGE_DURATION_OF_COMPRESSION = "Compression took %.2f seconds\n";

std::string ANSConstants::MESSAGE_DURATION_OF_READING_BINARY_FILE = "Reading binary file took %.2f seconds\n";
std::string ANSConstants::MESSAGE_DURATION_OF_DECOMPRESSION = "Decompression took %.2f seconds\n";
