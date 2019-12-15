// Include user-defined libraries
#include "talk2camera.h"

using namespace cv;


void Camera::initialize()
{
	PylonInitialize();
	EPixelType pixelFormat(PixelType_Mono8);
	// create an instant camera object
	cam.Attach(CTlFactory::GetInstance().CreateFirstDevice());
	
	cam.Open();

	cam.MaxNumBuffer = 1000;
	cam.Width.SetValue(frameWidth);
	cam.Height.SetValue(frameHeight);
	cam.OffsetX = offSetX;
	cam.OffsetY = offSetY;

	cam.AcquisitionFrameRateEnable.SetValue(true);
	cam.AcquisitionFrameRate.SetValue(frameRate);
	cam.ExposureTime.SetValue(exposureTime);
	formatConverter.OutputPixelFormat = pixelFormat;

	cam.StartGrabbing();
	cout << "Camera initialization succeeded." << endl << endl;

}

bool Camera::grabPylonImg()
{
	Pylon::CImageFormatConverter formatConverter;
	// Error handling
	try {
		cam.RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);
		if (!ptrGrabResult->GrabSucceeded()){
			cout << "Error: " << ptrGrabResult->GetErrorCode() << " " << ptrGrabResult->GetErrorDescription() << endl;
			return false;
		}
			


		formatConverter.Convert(pylonImg, ptrGrabResult);
		opencvImg = cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC1, (uint8_t*)pylonImg.GetBuffer());
	}
	catch (const GenericException & e) {
		cerr << "An exception occurred." << endl
			<< e.GetDescription() << endl;
	}

	

	return true;
}

/*
Pylon::CGrabResultPtr Camera::getPtr2buffer()
{
	return ptrGrabResult;
}
*/
Mat Camera::getMat() {
	return opencvImg;
}
int Camera::getFrameRate() {
	return frameRate;
};
Size Camera::getFrameSize() {
	return Size(frameWidth, frameHeight);
};