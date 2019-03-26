#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;

Point testHead,  testCenter;
vector<Point> testTail(9);
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
	double dist = abs(BA.y*P.x - BA.x*P.y + B.cross(A)) / norm(BA);
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
Point findCenter(vector<Point>& contour) {
	Moments M = moments(contour);
	Point center = Point(M.m10 / M.m00, M.m01 / M.m00);
	return center;
}


Mat grayImg, binImg, closeImg, edgeImg,smoothImg;

//This function return two midpoint of a minBoundingBox
bool findMidpoint(vector<Point>& contour, vector<Point>& midPoint, bool isLong) {
	RotatedRect minBoundingBox = minAreaRect(contour);
	Point2f vertices[4];
	minBoundingBox.points(vertices);
	if ((minBoundingBox.size.width >= minBoundingBox.size.height)^isLong) {
		midPoint[0] = (vertices[0] + vertices[1])*0.5;
		midPoint[1] = (vertices[2] + vertices[3])*0.5;
	}
	else {
		midPoint[0] = (vertices[0] + vertices[3])*0.5;
		midPoint[1] = (vertices[1] + vertices[2])*0.5;
	}
	return true;
}

/*This function return a set of radian to describe the fishtailing motion*/

bool fishAngleAnalysis(Mat fishImg,vector<double>& fishAngle) {
	//Find the contour of fish
	vector<vector<Point>> allContours, fishContours;
	cvtColor(fishImg, grayImg, COLOR_RGB2GRAY);
	GaussianBlur(grayImg, smoothImg, Size(5, 5), 0, 0);
	Mat element = getStructuringElement(MORPH_RECT, Size(50, 50));
	Canny(smoothImg, edgeImg, 25, 75);
	morphologyEx(edgeImg, closeImg, MORPH_CLOSE, element);

	findContours(closeImg, allContours, CV_RETR_LIST, CHAIN_APPROX_NONE);
	for (int i = 0; i < allContours.size(); i++) {
		if (contourArea(allContours[i]) < 150000 && contourArea(allContours[i]) > 50000)
			fishContours.push_back(allContours[i]);
	}
	if (fishContours.size() != 1) {
		cout << "Can't find contour of fish!Area of all contours:";
		for (int i = 0; i < allContours.size(); i++) {
			cout << contourArea(allContours[i]) << ',';
		}
		cout << endl;
		return false;
	}

	//Find the contour of head and tail
	vector<Point> boundingBox_midpoint(2);
	findMidpoint(fishContours[0], boundingBox_midpoint, true);
	vector<Point> fishHeadContour, fishTailContour;
	vector<int> indices = findPtsLineIntersectContour(fishContours[0], boundingBox_midpoint[0], boundingBox_midpoint[1]);
	vector<vector<Point>> contourHalves(2);

	contourHalves[0].insert(contourHalves[0].end(), fishContours[0].begin() + indices[0], fishContours[0].begin() + indices[1]);
	contourHalves[1].insert(contourHalves[1].end(), fishContours[0].begin() + indices[1], fishContours[0].end());
	contourHalves[1].insert(contourHalves[1].end(), fishContours[0].begin(), fishContours[0].begin() + indices[0]);

	vector<double> areas(2);
	if (contourHalves[0].size()*contourHalves[1].size() == 0) {
		cout << "Can't find the head or tail!" << endl;
		return false;
	}

	areas[0] = contourArea(contourHalves[0]);
	areas[1] = contourArea(contourHalves[1]);
	if (areas[0] > areas[1]) {
		fishHeadContour = contourHalves[0];
		fishTailContour = contourHalves[1];
	}
	else {
		fishHeadContour = contourHalves[1];
		fishTailContour = contourHalves[0];
	}

	//Find a point to describe the head
	Point fishCenter = findCenter(fishContours[0]),fishHead,nearTail,farTail;
	vector<Point> headBoundingBox_midpoint(2), tailBoundingBox_midpoint(2), fishTailVector(9);
	findMidpoint(fishHeadContour, headBoundingBox_midpoint, false);
	findMidpoint(fishTailContour, tailBoundingBox_midpoint, false);
	if (norm(fishCenter - headBoundingBox_midpoint[0]) < norm(fishCenter - headBoundingBox_midpoint[1]))
		fishHead = headBoundingBox_midpoint[1];
	else
		fishHead = headBoundingBox_midpoint[0];

	//Find some points to describe the tail
	if (norm(fishCenter - tailBoundingBox_midpoint[0]) < norm(fishCenter - tailBoundingBox_midpoint[1])) {
		farTail = tailBoundingBox_midpoint[1];
		nearTail = tailBoundingBox_midpoint[0];
	}
	else {
		farTail = tailBoundingBox_midpoint[0];
		nearTail = tailBoundingBox_midpoint[1];
	}

	Point tailAxis= farTail - nearTail;
	for (int i = 0; i < 9; i++) {
		Point curPt1,curPt2;
		curPt1 = nearTail + (i+1) * tailAxis*0.1;
		curPt2.x = tailAxis.y;
		curPt2.y = -tailAxis.x;
		vector<int> tailIndices = findPtsLineIntersectContour(fishTailContour, curPt1+curPt2, curPt1-curPt2);
		fishTailVector[i] = (fishTailContour[tailIndices[0]] + fishTailContour[tailIndices[1]])*0.5-fishCenter;
		testTail[i] = (fishTailContour[tailIndices[0]] + fishTailContour[tailIndices[1]])*0.5;//for test
	}


	//Calculate the angle
	Point fishHeadVector;
	fishHeadVector = fishCenter - fishHead;
	for (int i = 0; i < 9; i++) {
		double sinfi;
		sinfi = (fishHeadVector.x*fishTailVector[i].y - fishTailVector[i].x*fishHeadVector.y) / (norm(fishHeadVector)*norm(fishTailVector[i]));
		fishAngle[i] = asin(sinfi);
	}
	//just for test
	testHead = fishHead;
	testCenter = fishCenter;
	return true;
}



int main() {
	VideoCapture capture("F:\\FishData\\test3.avi");
	namedWindow("output", CV_WINDOW_NORMAL);
	namedWindow("edgeImg", CV_WINDOW_NORMAL);
	namedWindow("closeImg", CV_WINDOW_NORMAL);
	while (1) {
		Mat curImg;
		capture >> curImg;
		if (curImg.empty())
			break;
		vector<double> test(9);
		fishAngleAnalysis(curImg,test);
		for (int i = 0; i < 9; i++) {
			cout << test[i] << ',';
			circle(curImg, testTail[i],6, Scalar(255, 0, 0),-1);
		}
		cout << endl;

		line(curImg, testHead, testCenter, Scalar(0, 0, 255), 3, 4);

		imshow("output", curImg);
		imshow("closeImg", closeImg);
		imshow("edgeImg", edgeImg);

		waitKey();
	}


	getchar();
	return 0;
}
