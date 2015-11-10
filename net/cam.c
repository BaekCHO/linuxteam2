#include <cv.h>

#include <highgui.h>




void main()

{

	IplImage* image = 0;

	// CAM 참조

	CvCapture* capture = cvCaptureFromCAM(0);

	// 윈도우 속성, 0 고정

	cvNamedWindow( "CAM", 0 );

	// 윈도우 크기 400*300

	cvResizeWindow( "CAM", 400, 300 );

   

	// cvGrabFrame()함수는 capture 변수로 CAM을 참조, T/F반환

	while(cvGrabFrame( capture )) {

		// CAM에서 가져온 프레임을 image에 저장

		image = cvRetrieveFrame( capture );

		// image 출력

		cvShowImage( "CAM", image );

  

		// ESC 입력시 break

		if( cvWaitKey(10) >= 0 ) break;

	}

	cvReleaseCapture( &capture );

	cvDestroyWindow( "CAM" );

}
