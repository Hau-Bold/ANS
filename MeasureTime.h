#ifndef MEASURE_TIME_H
#define MEASURE_TIME_H
#include<iostream>
#include<math.h>
#include<string>
#include<time.h>

/**the class MeasureTime*/
class MeasureTime
{

private:

	static MeasureTime  * m_Instance;
	double startTime,statTime_tmp;

	/*
	*Destructor: default
	*/
	~MeasureTime(void) = default;

	/*
	*Constructor: default
	*/
	MeasureTime(void) = default;

public:

	/**yields an instance if this class*/
	static MeasureTime * getInstance(void);


	/**starts the clock*/
	void start(void);

	/**yields the elapsed time
	*
	*@param message - notification for client
	*/
	void getDuration(std::string & message);

	/**stops the clock*/
    void stop(void);
};

#endif
