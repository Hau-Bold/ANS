#ifndef PRECONDITIONER_H
#define PRECONDITIONER_H
#include "ANSTypes.h"
#include "PreconditionerType.h"
#include<future>
#include<vector>
#include<ppl.h>

using namespace cogra;

/** the class Preconditioner
* - holds the mode to precondition
* - holds the image (and it's necessary data) on which the precondition should be applied
* - used as an singleon
*/
class Preconditioner
{
private:

	static Preconditioner * m_Instance;
	PreconditionerType m_PreconditionerMode;
	/**holds the unsigned int data of the image*/
	Symbol * m_Image;
	/**holds the signed int data of the image after the preconditioning*/
	std::vector<Symbol> m_PreConditionedData;
	//holds the response after using postcondition
	std::vector<Symbol> m_PostConditionedData;
	int32 m_Width, m_Height, m_CountOfChannels;

	/**Constructor.
	*
	*@param preconditionerMode - the preconditionerMode
	*
	*@param image - the image
	*/
	Preconditioner(const PreconditionerType& preconditionerMode, uint8 * image, const int32& width, const int32& height, const int32& countOfChannels);


	Preconditioner(const PreconditionerType& preconditionerMode,const int32& width, const int32& height, const int32& countOfChannels);

	/**
	*copies the pixel values of the row in the preconditioned data
	*
	*@param row - the row
	*
	*/
	void precondition_NONE(const int32& row);

	/**
	*copies the pixel values of the row in the postconditioned data
	*
	*@param row - the row
	*
	*/
	void postcondition_NONE(const int32& row);

	/**
	*runs from left to right in the selected row and replaces current pixel value belonging to channel by difference to left pixel.
	*
	*@param row - the index of the pixel row
	*
	*@param channel - the channel
	*
	*@ return - the pixel on the right edge which is needed to undone this preconditioning.
	*/
	Symbol precondition_SUB(const int32& row, const int8& channel);

	/**
	*reverts the precondition_SUB at the specific row of the channel
	*
	*@param row - the row
	*
	*@param receiving - the pixel value at the right end of the row
	*
	*@param channel - the channel
	*
	*/
	void postcondition_SUB(const int32& row, const Symbol& receiving, const int8& channel);

	/**
	*runs from up to down over alle pixels at selected channel and selected column and replaces current pixel value by difference to upper pixel value.
	*
	*@param column - the column
	*
	*@param channel - the channel
	*
	*@ return - the pixel at channel at bottom of column
	*/
	Symbol precondition_UP(const int32& column, const int8& channel);

	/**
	*reverts the precondition_up at specific column and channel
	*
	*@param column - the column
	*
	*@param receiving - the pixel at channel at bottom of column (return statement of precondition_UP)
	*
	*/
	void postcondition_UP(const int32& column, const Symbol& receiving, const int8& channel);

	/**
	*runs from left to right in channel of selected row and replaces current pixel by difference to average of left and up pixel.
	*
	*@param row - the index of the pixel row
	*
	*@param channel - the channel
	*
	*@ return - the pixel on the right edge which is needed to undone this preconditioning.
	*/
	Symbol precondition_AVERAGE2(const int32& row, const int8& channel);

	/**
	*reverts the precondition_AVERAGE2 at the specific row
	*
	*@param row - the row
	*
	*@param receiving - the pixel at the bottom
	*
	*/
	void postcondition_AVERAGE2(const int32& row, const Symbol& receiving, const int8& channel);

	/**
	*runs from left to right in channel of selected row and replaces current pixel by
	* difference to average of left, up and right pixel.
	*
	*@param row - the index of the pixel row
	*
	*@param channel - the channel
	*
	*@ return - the pixel on the right edge which is needed to undone this preconditioning.
	*/
	Symbol precondition_AVERAGE3(const int32& row, const int8& channel);

	/**
	*reverts the precondition_AVERAGE3 in channel at the specific row
	*
	*@param row - the row
	*
	*@param receiving - the pixel at the bottom
	*
	*@param channel - the color channel
	*
	*/
	void postcondition_AVERAGE3(const int32& row, const Symbol& receiving, const int8& channel);

	/**
	*runs from up to down in selected channel of selected column and
	*replaces current pixel by difference to average of left, up, right and below pixel.
	*
	*@param row - the index of the pixel row
	*
	*@param channel - the color channel
	*
	*@return - the pixel on the right edge which is needed to undo this preconditioning.
	*/
	Symbol precondition_AVERAGE4(const int32& column, const int8& channel);

	/**
	*reverts the precondition_AVERAGE4 at the specific channel
	*
	*@param channel - the channel
	*
	*@param receiving - the result of preconditioning at the bottom for the specific channel
	*
	*/
	void postcondition_AVERAGE4(const std::vector<Symbol>& receiving, const int8& channel);

	/**
	*runs from up to down in selected channel of selected column and
	*replaces current pixel by difference to average of left,leftUp,up,right and below pixel.
	*
	*@param row - the index of the pixel row
	*
	*@param channel - the color channel
	*
	*@return - the pixel on the right edge which is needed to undo this preconditioning.
	*/
	Symbol precondition_AVERAGE5(const int32& column, const int8& channel);

	/**
	*reverts the precondition_AVERAGE5 at the specific channel
	*
	*@param channel - the channel
	*
	*@param receiving - the result of preconditioning at the bottom for the specific channel
	*
	*/
	void postcondition_AVERAGE5(const std::vector<Symbol>& receiving, const int8& channel);

	/**
	*runs from up to down in selected channel of selected column and
	*replaces current pixel by difference to average of left,leftUp,up,rightUp,right and below pixel.
	*
	*@param row - the index of the pixel row
	*
	*@param channel - the color channel
	*/
	void precondition_AVERAGE6(const int32& column, const int8& channel);

	/**
	*reverts the precondition_AVERAGE6 at the specific channel
	*
	*@param channel - the channel
	*
	*@param receiving - the result of preconditioning at the bottom for the specific channel
	*
	*/
	void postcondition_AVERAGE6(const int8& channel);

	/**
	*runs from up to down in selected channel of selected column and
	*replaces current pixel by difference to average of left,leftUp,up,rightUp,right and below pixel.
	*
	*@param row - the index of the pixel row
	*
	*@param channel - the color channel
	*/
	void precondition_AVERAGE7(const int32& column, const int8& channel);

	/**
	*reverts the precondition_AVERAGE6 at the specific channel
	*
	*@param channel - the channel
	*
	*@param receiving - the result of preconditioning at the bottom for the specific channel
	*
	*/
	void postcondition_AVERAGE7(const int8& channel);

	/**
	*runs from up to down in selected channel of selected column and
	*replaces current pixel by difference to average of left,leftUp,up,rightUp,right and below pixel.
	*
	*@param row - the index of the pixel row
	*
	*@param channel - the color channel
	*/
	void precondition_AVERAGE8(const int32& column, const int8& channel);

	/**
	*reverts the precondition_AVERAGE6 at the specific channel
	*
	*@param channel - the channel
	*
	*@param receiving - the result of preconditioning at the bottom for the specific channel
	*
	*/
	void postcondition_AVERAGE8(const int8& channel);

	/**applys the paeth preconditioning on the channel in the selected row
	*
	*@param row - the row
	*
	*@param channel -the channel
	*
	*@return - the first pixel value of selected channel in selected row  
	*/
	Symbol precondition_PAETH(const int32& row, const int8& channel);

	/**
	*reverts the precondition paeth in the selected row an channel
	*
	*@param row - the row
	*
	*@param receiving - the result of applying paeth preconditioning on channel in row 
	*
	*@param channel - the color channel
	*/
	void postcondition_PAETH(const int32& row, const Symbol& receiving, const int8& channel);

	/**
	*computes the paeth predictor from left, up , leftUp pixel
	*
	*@param left - the left pixel value
	*
	*@param up - the above pixel value
	*
	*@param leftUp - the left above pixel value
	*
	*/
	int16 paethPredictor(const int16& left,const int16& up,const int16& leftUp);


public:


	/**yields an instance of preconditioner */
	static Preconditioner * getInstance(const PreconditionerType& preconditionerMode, uint8 * imageData, const int32& width, const int32& height, const int32& countOfChannels);

	/**yields an instance of preconditioner */
	static Preconditioner * getInstance(const PreconditionerType& preconditionerMode,const int32& width, const int32& height, const int32& countOfChannels);

	/**Destructor.*/
	~Preconditioner() {};

	/**to delete the instance*/
	void deleteInstance();

	std::vector<std::vector<Symbol>> executePreCondition();

	void executePostCondition(std::vector<std::vector<Symbol>> receiving);

	/**
	*yields the result for average preconditioning the color channel with more than five neighbours
	*
	*@param channel - the channel 
	*/
	std::vector<Symbol> getResultForHigherAverage(const uint8& channel);

	/**
	*sets the necessary pixel values in selected color channel for reverting average preconditioning with more than five neighbours
	*
	*@param receiving - the pixels that must be set for undoing preconditioning
	*
	*@param channel - the channel
	*/
	void setResultForHigherAverage(const std::vector<Symbol>& receiving, const uint8& channel);

	

	/**get & set follows below here*/

	Symbol * getImageData()
	{
		return m_Image;
	}

	std::vector<Symbol> getPreConditionedData()
	{
		return m_PreConditionedData;
	}

	void setData(std::vector<Symbol> & data)
	{
		m_PreConditionedData = data;
	}

	std::vector<Symbol> getPostConditionedData()
	{
		return m_PostConditionedData;
	}
};
#endif
