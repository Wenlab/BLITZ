// Include user-defined libraries
#include "fishAnalysis.h"

using namespace cv;

int predict_left(double* boutStart) {
	Py_Initialize();
	if (Py_IsInitialized() == 0) {
		cout << "Py_Initialize failed." << endl;
	}
	PyObject* pModule = PyImport_ImportModule("predict");
	if (pModule == NULL)
		cout << "Py_ImportModule failed." << endl;
	PyObject* pFunc = PyObject_GetAttrString(pModule, "predict_left");
	PyObject* PyList = PyList_New(40);
	PyObject* ArgList = PyTuple_New(1);
	for (int Index_i = 0; Index_i < PyList_Size(PyList); Index_i++) {
		PyList_SetItem(PyList, Index_i, PyFloat_FromDouble(boutStart[Index_i]));
	}
	PyTuple_SetItem(ArgList, 0, PyList);
	PyObject* pReturn = NULL;
	pReturn = PyObject_CallObject(pFunc, ArgList);
	int result;
	PyArg_Parse(pReturn, "i", &result);
	cout << "predict:" << result << endl;
	Py_Finalize();
	return result;

}
int predict_right(double* boutStart) {
	Py_Initialize();
	if (Py_IsInitialized() == 0) {
		cout << "Py_Initialize failed." << endl;
	}
	PyObject* pModule = PyImport_ImportModule("predict");
	if (pModule == NULL)
		cout << "Py_ImportModule failed." << endl;
	PyObject* pFunc = PyObject_GetAttrString(pModule, "predict_right");
	PyObject* PyList = PyList_New(40);
	PyObject* ArgList = PyTuple_New(1);
	for (int Index_i = 0; Index_i < PyList_Size(PyList); Index_i++) {
		PyList_SetItem(PyList, Index_i, PyFloat_FromDouble(boutStart[Index_i]));
	}
	PyTuple_SetItem(ArgList, 0, PyList);
	PyObject* pReturn = NULL;
	pReturn = PyObject_CallObject(pFunc, ArgList);
	int result;
	PyArg_Parse(pReturn, "i", &result);
	cout << "predict:" << result << endl;
	Py_Finalize();
	return result;

}

/*Find the closest point on the contour to the reference point, return the index findClosestPt*/
int findClosestPt(vector<Point>& contour, Point point)
{
	double minDist = 1000000;
	double tempDist = 0;
	Point temp;
	int goodIndex = 0;
	for (int i = 0; i < contour.size(); i++)
	{
		temp = contour[i];
		tempDist = norm(contour[i] - point);

		if (tempDist < minDist)
		{
			goodIndex = i;
			minDist = tempDist;
		}
	}

	return goodIndex;
}

/* Get the Euclidean distance from point P to line AB */
double getPt2LineDistance(Point2f P, Point2f A, Point2f B)
{
	Point2f BA = B - A; // the vector from A to B
	//Point2f PA = P - A; // the vector from A to P
	double dist = abs(BA.y * P.x - BA.x * P.y + B.cross(A)) / norm(BA);
	return dist;
}

/* Find 2 intersection points of a line (AB) and contour */
vector<int> findPtsLineIntersectContour(vector<Point>& contour, Point2f A, Point2f B)
{
	vector<int> goodIndices(2);
	Point2f curPt; // current point
	vector<Point> ptList; // store potential intersection points
	vector<int> idxList; // store indices of potential intersection points
	double distThre = 1.0; // threshold to decide whether it is an intersection pt
	for (int i = 0; i < contour.size(); i++)
	{
		curPt = contour[i];
		double pt2line = getPt2LineDistance(curPt, A, B);
		if (pt2line < distThre)
		{
			ptList.push_back(contour[i]);
			idxList.push_back(i);
		}

	}

	// assign intersection points to each side
	int idxA = findClosestPt(ptList, A); // the one closest to A
	int idxB = findClosestPt(ptList, B); // the one closest to B

	if (idxA < idxB)
	{
		goodIndices[0] = idxList[idxA];
		goodIndices[1] = idxList[idxB];
	}
	else
	{
		goodIndices[0] = idxList[idxB];
		goodIndices[1] = idxList[idxA];
	}


	return goodIndices;

}

/*This function return a radian to describe the fishtailing motion */
bool FixedFish::fishAngleAnalysis(int counter) {
	//Find the contour of fish
	Mat binaryzation;
	double  max_val = 255, maxFishArea = 150000, minFishArea = 1000;
	Point topEnd, tailPt_a, tailPt_b;
	vector<vector<Point>> allContours, fishContours;

	fishImg.copyTo(ROIimage, fishROI);
	threshold(ROIimage, binaryzation, threshold_val, max_val, CV_THRESH_BINARY);
	//imshow("test", ROIimage);
	//imshow("test2", binaryzation);
	findContours(binaryzation, allContours, CV_RETR_LIST, CHAIN_APPROX_NONE);
	for (int i = 0; i < allContours.size(); i++) {
		if (contourArea(allContours[i]) < maxFishArea && contourArea(allContours[i]) > minFishArea)
			fishContours.push_back(allContours[i]);
	}
	for (int i = 0; i < allContours.size(); i++) {
		cout << contourArea(allContours[i]) << ',';
	}
	if (fishContours.size() != 1) {
		cout << "Can't find contour of fish!Area of all contours:";
		cout << fishContours.size() << endl;
		for (int i = 0; i < allContours.size(); i++) {
			cout << contourArea(allContours[i]) << ',';
		}
		cout << endl;
		return false;
	}

	//Find the tail of fish

	double Pt2center = norm(fishContours[0][0] - fishCenter);
	topEnd = fishContours[0][0];

	for (int i = 1; i < fishContours[0].size(); i++)
	{
		double curPt2center = norm(fishContours[0][i] - fishCenter);
		if (Pt2center < curPt2center) {
			topEnd = fishContours[0][i];
			Pt2center = curPt2center;
			//circle(fishImg, topEnd, 1, Scalar(255), -1);

		}


	}
	Point tailAxis = topEnd - fishCenter;
	tailPt_a = fishCenter + tailAxis * 9 / 10 + Point(tailAxis.y, -tailAxis.x) / 4;
	tailPt_b = fishCenter + tailAxis * 9 / 10 + Point(-tailAxis.y, tailAxis.x) / 4;
	vector<int> fishTail = findPtsLineIntersectContour(fishContours[0], tailPt_a, tailPt_b);

	//Calculate the angle
	Point fishHeadVector, fishTailVector;
	fishHeadVector = fishCenter - fishHead;
	fishTailVector = (fishContours[0][fishTail[0]] + fishContours[0][fishTail[1]]) / 2 - fishCenter;
	double sinfi;
	sinfi = -(fishHeadVector.x * fishTailVector.y - fishTailVector.x * fishHeadVector.y) / (norm(fishHeadVector) * norm(fishTailVector));

	tailingAngle[counter] = asin(sinfi);
	if (tailingAngle[counter] > 0.2 || tailingAngle[counter] < -0.2) {
		tailingFlag[counter] = 1;
		
	}
	
	//*fishTail_return = (fishContours[0][fishTail[0]] + fishContours[0][fishTail[1]]) / 2;
	//drawContours(fishImg, fishContours, -1, Scalar(255),2);
	//imshow("contour", fishImg);
	return true;
}


void FixedFish::getImgFromVideo(cv::VideoCapture cap)
{
	cap >> fishImgFromVideo;
	cvtColor(fishImgFromVideo, fishImg, CV_BGR2GRAY);
}
void FixedFish::getMat(Mat curImg)
{
	fishImg=curImg;
}

void FixedFish::resetCount()
{
	count = 0;
}

int FixedFish::getCount()
{
	return count;
}

void FixedFish::addCount(int increase)
{
	count = count + increase;
}

void FixedFish::predictDirection() {
	if (leftTrue) {
		predict_left(&tailingAngle[count - boutFrames]);
		//TODO:save data
	}
	else {
		predict_right(&tailingAngle[count - boutFrames]);
		//TODO:save data
	}
}