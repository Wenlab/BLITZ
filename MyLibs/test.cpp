#include "test.h"



void test::creatFrames(cv::Scalar stripe,
	cv::Scalar background,
	int wid,
	int gap,
	int angle
	) {


	cv::Mat Rot = cv::getRotationMatrix2D(cv::Point2f(540, 540), angle, 1);

	for (int i = -wid; i < gap; i++) {
		cv::Mat img(1080, 1080, CV_8UC3, background);

		for (int Ptx = i; Ptx < 1080; Ptx += (wid + gap)) {
			cv::Rect rect(Ptx, 0, wid, 1080);
			cv::rectangle(img, rect, stripe, -1);
		}
		cv::warpAffine(img, img, Rot, cv::Size(img.cols, img.rows));

		Frames.push_back(img);

	}

	//iter = Frames.begin();
	totalFrames = wid + gap;
}
void test::showFrame(int go) {
	int go2 = (iter + go) % totalFrames;
	if (go2 < 0) {
		cv::imshow("test", Frames[go2 + totalFrames]);
		iter = go2 + totalFrames;
	}
	else {
		cv::imshow("test", Frames[go2]);
		iter = go2;
	}


	//iter += go;

	cv::waitKey(wait);
}

void test::findFishTail() {
	Arena ArenaObj(1, 0, 0);

	ArenaObj.initialize();
	ArenaObj.predictInitialize();
	cv::VideoCapture cap("D:/working/Data/automl/20210408_2204_h2bg6f_16dpf_1.avi");

	for (int i = 0; i < 2500; i++) {
		ArenaObj.getImgFromVideo(cap);

		ArenaObj.buildBgImg();

	}
	int frame_idx = 0;
	while (1) {

		ArenaObj.getImgFromVideo(cap);
		ArenaObj.buildBgImg();
		ArenaObj.findSingleFish4test();
		ArenaObj.checkBout(frame_idx);
		frame_idx++;
		//cv::imshow("test", Frames[go2]);
	}

}
void test::findFishTail(std::string path) {
	Arena ArenaObj(1, 0, 0);

	ArenaObj.initialize();
	//ArenaObj.predictInitialize();
	saveData saveDataObj;

	cv::VideoCapture cap(path);
	int totalFrameNumber = cap.get(CV_CAP_PROP_FRAME_COUNT);
	saveDataObj.offlineInitialize(180000);

	for (int i = 0; i < 2500; i++) {
		ArenaObj.getImgFromVideo(cap);

		ArenaObj.buildBgImg();

	}
	int frame_idx = 0;
	cap.set(CV_CAP_PROP_POS_FRAMES, 0);

	while (1) {
		if (cap.get(CV_CAP_PROP_POS_FRAMES) == 180000)
			break;
		ArenaObj.getImgFromVideo(cap);
		ArenaObj.buildBgImg();
		ArenaObj.findSingleFish4test();
		//ArenaObj.checkBout(frame_idx);
		saveDataObj.offlineSaveFrame(ArenaObj,frame_idx);
		frame_idx++;
		//cv::imshow("test", Frames[go2]);
	}
	saveDataObj.offlineSaveMat("20210401_1922_h2bg6f_35dpf_2");
}
