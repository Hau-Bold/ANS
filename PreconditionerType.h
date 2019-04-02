#ifndef PRECONDITIONER_TYPE_H
#define PRECONDITIONER_TYPE_H

/*enum to define the mode how image should be preconditioned*/
enum PreconditionerType
{
	NONE, //no filtering
	SUB, // difference to left pixel
	UP,// difference to upper pixel
	AVERAGE2,// difference to average of upper and left pixel
	AVERAGE3,//difference to average of left,up,and right pixel
	AVERAGE4,//difference to average of left,up,right and below pixel
	AVERAGE5,//difference to average of left,leftUp,up,right and below pixel
	AVERAGE6,//difference to average of left,leftUp,up,rightUp,right and below pixel
	AVERAGE7,//difference to average of left,leftUp,up,rightUp,right, rightBelow and below pixel
	AVERAGE8,//difference to average of left,leftUp,up,rightUp,right,rightBelow,below and leftBelow pixel
	PAETH // Paeth-Prediktor
};
#endif 
