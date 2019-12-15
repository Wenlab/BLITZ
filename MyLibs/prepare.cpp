// Include user-defined libraries
#include "prepare.h"

using namespace cv;

Point fishEye1, fishEye2, fishCenter, fishHead,cur_ROIPt;
Point patchPoint[4];
Mat img_findHeadAndCenter, img_setPatch, img_setThreshold, img_setROI;
Mat mask,fishROI;
int threshold_val = 22;
vector<Point> ROIpoint;
std::vector<std::vector<cv::Point>> ROIcontours;

void Prepare::pretreat(Mat curImg)
{
	copyMat(curImg);
	setROI();
	findHeadAndCenter();
	setPatch();
	setThreshold();
}

void Prepare::convertPatchPoint() {
	//midline
	vertices1[0] = float((patchPoint[0].x - 300.0f) / 300.0f);
	vertices2[0] = vertices1[0];
	vertices1[1] = float((300.0f - patchPoint[0].y) / 300.0f);
	vertices2[1] = vertices1[1];
	vertices1[3] = float((patchPoint[1].x - 300.0f) / 300.0f);
	vertices2[3] = vertices1[3];
	vertices1[4] = float((300.0f - patchPoint[1].y) / 300.0f);
	vertices2[4] = vertices1[4];
	//left point
	vertices1[6] = float((patchPoint[2].x - 300.0f) / 300.0f);

	vertices1[7] = float((300.0f - patchPoint[2].y) / 300.0f);

	//right point
	vertices2[6] = float((patchPoint[3].x - 300.0f) / 300.0f);

	vertices2[7] = float((300.0f - patchPoint[3].y) / 300.0f);

}
void Prepare::setHeadAndCenter()
{
	Center = fishCenter;
	Head = fishHead;
}
Point Prepare::getHead()
{
	return Head;
}
Point Prepare::getCenter()
{
	return Center;
}
int Prepare::getThreshold_val()
{
	return threshold_val;
}
Mat Prepare::getMask()
{
	return mask;
}
void Prepare::setLeftPatch(float* leftPatch) 
{
	leftPatch[0] = vertices1[0];
	leftPatch[1] = vertices1[1];
	leftPatch[3] = vertices1[3];
	leftPatch[4] = vertices1[4];
	leftPatch[6] = vertices1[6];
	leftPatch[7] = vertices1[7];

}
void Prepare::setRightPatch(float* rightPatch)
{
	rightPatch[0] = vertices2[0];
	rightPatch[1] = vertices2[1];
	rightPatch[3] = vertices2[3];
	rightPatch[4] = vertices2[4];
	rightPatch[6] = vertices2[6];
	rightPatch[7] = vertices2[7];
}


void copyMat(Mat curImg) {
	img_findHeadAndCenter = curImg.clone();
	img_setPatch = curImg.clone();
	img_setThreshold = curImg.clone();
	img_setROI = curImg.clone();
}

static void on_trackbar_setThreshold(int, void*) {
	int max_val = 255;
	Mat binaryzation = Mat::zeros(img_setThreshold.size(), CV_8UC1);
	threshold(img_setThreshold, binaryzation, threshold_val, max_val, CV_THRESH_BINARY);

	imshow("setThreshold", binaryzation);

}

bool setThreshold() {
	namedWindow("setThreshold", CV_WINDOW_NORMAL);
	createTrackbar("Threshold", "setThreshold", &threshold_val, 255, on_trackbar_setThreshold);
	on_trackbar_setThreshold(threshold_val, 0);
	cout << "Press 'q' to exit." << endl;
	while (char(waitKey(1)) != 'q') {}
	return true;
}

void on_mouse_findHeadAndCenter(int event, int x, int y, int flags, void* ustc) {
	if (event == CV_EVENT_LBUTTONDBLCLK) {
		fishEye1 = Point(x, y);
		circle(img_findHeadAndCenter, fishEye1, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);
		imshow("findHeadAndCenter", img_findHeadAndCenter);
	}
	else if (event == CV_EVENT_RBUTTONDBLCLK) {

		fishEye2 = Point(x, y);
		fishHead = (fishEye1 + fishEye2) / 2;
		circle(img_findHeadAndCenter, fishHead, 2, Scalar(0, 0, 0, 255), CV_FILLED, CV_AA, 0);
		circle(img_findHeadAndCenter, fishEye2, 2, Scalar(0, 255, 0, 0), CV_FILLED, CV_AA, 0);
		imshow("findHeadAndCenter", img_findHeadAndCenter);

	}
	else if (event == CV_EVENT_MBUTTONDOWN) {

		fishCenter = Point(x, y);
		circle(img_findHeadAndCenter, fishCenter, 2, Scalar(0, 0, 255, 0), CV_FILLED, CV_AA, 0);
		imshow("findHeadAndCenter", img_findHeadAndCenter);
		cout << "fishCenter:" << fishCenter.x << ',' << fishCenter.y << endl;
		cout << "fishHead:" << fishHead.x << ',' << fishHead.y << endl;
	}

}

bool findHeadAndCenter() {
	namedWindow("findHeadAndCenter", CV_WINDOW_NORMAL);
	imshow("findHeadAndCenter", img_findHeadAndCenter);
	setMouseCallback("findHeadAndCenter", on_mouse_findHeadAndCenter, 0);
	cout << "Double-click the left mouse button to select one eye." << endl;
	cout << "Double-click the right mouse button to select another eye." << endl;
	cout << "Click the middle mouse button to select center." << endl;
	cout << "Press 'q' to exit." << endl;
	while (char(waitKey(1)) != 'q') {}
	return true;
}

void on_mouse_setPatch(int event, int x, int y, int flags, void* ustc) {
	if (event == CV_EVENT_LBUTTONDBLCLK) {
		patchPoint[0] = Point(x, y);
		circle(img_setPatch, patchPoint[2], 2, Scalar(0, 0, 255, 0), CV_FILLED, CV_AA, 0);
		imshow("setPatch", img_setPatch);
	}
	else if (event == CV_EVENT_RBUTTONDBLCLK) {
		patchPoint[1] = Point(x, y);
		circle(img_setPatch, patchPoint[3], 2, Scalar(0, 0, 0, 255), CV_FILLED, CV_AA, 0);
		imshow("setPatch", img_setPatch);

	}
	else if (event == CV_EVENT_LBUTTONDOWN) {
		patchPoint[2] = Point(x, y);
		circle(img_setPatch, patchPoint[0], 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);
		imshow("setPatch", img_setPatch);
	}
	else if (event == CV_EVENT_RBUTTONDOWN) {
		patchPoint[3] = Point(x, y);
		circle(img_setPatch, patchPoint[1], 2, Scalar(0, 255, 0, 0), CV_FILLED, CV_AA, 0);
		imshow("setPatch", img_setPatch);
		cout << patchPoint[0] << ',' << patchPoint[1] << ',' << patchPoint[2] << ',' << patchPoint[3] << endl;
	}


}

bool setPatch() {
	namedWindow("setPatch", CV_WINDOW_NORMAL);
	imshow("setPatch", img_setPatch);
	setMouseCallback("setPatch", on_mouse_setPatch, 0);
	cout << "Double-click the left mouse button to set the starting point of midline." << endl;
	cout << "Double-click the right mouse button to set the ending point of midline." << endl;
	cout << "Click the left mouse button to set the left point of patch." << endl;
	cout << "Click the right mouse button to set the right point of patch." << endl;
	cout << "Press 'q' to exit." << endl;
	while (char(waitKey(1)) != 'q') {}
	return true;
}

void on_mouse_setROI(int event, int x, int y, int flags, void* ustc)
{
	if (event == CV_EVENT_LBUTTONDBLCLK) {
		cur_ROIPt = Point(x, y);
		circle(img_setROI, cur_ROIPt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);
		imshow("setROI", img_setROI);
		ROIpoint.push_back(cur_ROIPt);
		if (ROIpoint.size() == 4) {
			ROIcontours.push_back(ROIpoint);
			cv::polylines(mask, ROIcontours, true, cv::Scalar(255, 255, 255), 2, cv::LINE_AA);
			cv::fillPoly(mask, ROIcontours, cv::Scalar(255, 255, 255));
			img_setROI.copyTo(fishROI, mask);
			imshow("fishROI", fishROI);
		}
	}
}
bool setROI()
{
	mask = Mat::zeros(img_setROI.size(), CV_8UC1);
	namedWindow("setROI", CV_WINDOW_NORMAL);
	imshow("setROI", img_setROI);

	setMouseCallback("setROI", on_mouse_setROI, 0);
	cout << "Double-click the left mouse button to set the ROI points." << endl;
	cout << "Press 'q' to exit." << endl;
	while (char(waitKey(1)) != 'q') {}
	return true;
}