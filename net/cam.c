#include <cv.h>

#include <highgui.h>




void main()

{

	IplImage* image = 0;

	// CAM ����

	CvCapture* capture = cvCaptureFromCAM(0);

	// ������ �Ӽ�, 0 ����

	cvNamedWindow( "CAM", 0 );

	// ������ ũ�� 400*300

	cvResizeWindow( "CAM", 400, 300 );

   

	// cvGrabFrame()�Լ��� capture ������ CAM�� ����, T/F��ȯ

	while(cvGrabFrame( capture )) {

		// CAM���� ������ �������� image�� ����

		image = cvRetrieveFrame( capture );

		// image ���

		cvShowImage( "CAM", image );

  

		// ESC �Է½� break

		if( cvWaitKey(10) >= 0 ) break;

	}

	cvReleaseCapture( &capture );

	cvDestroyWindow( "CAM" );

}
