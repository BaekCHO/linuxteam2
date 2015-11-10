#include <stdio.h>

#include <math.h>

#include "cv.h"

#include "highgui.h"

int main() {

	printf("Hello \r\n");

	cvNamedWindow("Test",CV_WINDOW_AUTOSIZE);

	IplImage* image=0;

	image=cvLoadImage("image.jpg", CV_LOAD_IMAGE_COLOR);

	if(image==0)

		printf("image doesn't exist");

	else

	{

		cvShowImage("Test", image);

		cvWaitKey(0);

		cvReleaseImage(&image);

	}

	return 0;

}
