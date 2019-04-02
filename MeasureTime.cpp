#include"MeasureTime.h"


MeasureTime  * MeasureTime::m_Instance=nullptr;


MeasureTime * MeasureTime::getInstance(void)
{
	if (!m_Instance)
	{
		m_Instance = new MeasureTime();
	}

	return m_Instance;
}

void MeasureTime::start(void)
{
	startTime = clock();
	statTime_tmp = startTime;
}

void MeasureTime::getDuration(std::string & message)
{
	double duration = clock() - statTime_tmp;
	duration = duration / CLOCKS_PER_SEC;  // rescale to seconds

	printf(message.c_str(), duration);
	statTime_tmp = clock();
}

void MeasureTime::stop(void)
{

	double duration = clock() - startTime;
	duration = duration / CLOCKS_PER_SEC;  // rescale to seconds

	std::cout << "  duration = " << duration << " sec." << std::endl;

	delete m_Instance;
}
