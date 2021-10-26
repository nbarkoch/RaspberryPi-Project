// ConsoleCV2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include "HttpCreateAlert.h"
#include <ctime>
#include <thread>

//#include <windows.h>

using namespace cv;
using namespace std;

const char * NGROK_URL = "https://3595-217-132-230-110.ngrok.io";



Mat yellowFilterSpots(const Mat &src)
{
	Mat frame_HSV, frame_threshold;

	// Convert from BGR to HSV colorspace
	cvtColor(src, frame_HSV, COLOR_BGR2HSV);
	// Detect the object based on HSV Range Values
	inRange(frame_HSV, Scalar(20, 10, 240), Scalar(70, 240, 255), frame_threshold);
	blur(frame_threshold, frame_threshold, Size(10, 10));
	cv::threshold(frame_threshold, frame_threshold, 30, 255, 0);
	//imshow("?", frame_threshold);

	// Show the frames
	return frame_threshold;
}



std::vector<cv::Vec4i> createPolygon(std::vector<cv::Vec4i> lines, Mat src)
{
	// finding the central lines
	int miX1 = lines[0][0];
	int miY1 = lines[0][1];
	int miX2 = lines[0][2];
	int miY2 = lines[0][3];
	int maX1 = miX1;
	int maY1 = miY1;
	int maX2 = miX2;
	int maY2 = miY2;
	int min_index = 0;
	int max_index = 0;
	for (int i = 1; i < lines.size(); ++i)
	{
		int x1 = lines[i][0];
		int y1 = lines[i][1];
		int x2 = lines[i][2];
		int y2 = lines[i][3];
		if (miX1 > x1 && miX2 > x2)
		{
			miX1 = x1;
			miY1 = y2;
			miX2 = x2;
			miY2 = y2;
			min_index = i;
		}
		if (maX1 < x1 && maX2 < x2)
		{
			maX1 = x1;
			maY1 = y2;
			maX2 = x2;
			maY2 = y2;
			max_index = i;
		}
	}

	// we found the 2 closest to edges lines
	int x1 = miX1 < miX2 ? miX1 : miX2;
	int x2 = maX1 > maX2 ? maX1 : maX2;

	int centralX = (x1 + x2) / 2;

	int leftClose = lines[min_index][0] < lines[min_index][2] ? lines[min_index][0] : lines[min_index][2];
	int rightClose = lines[max_index][0] < lines[max_index][2] ? lines[max_index][0] : lines[max_index][2];


	int leftIndexClose, rightIndexClose;
	leftIndexClose = rightIndexClose = 0;
	// from the central we look for the 2 closest lines
	// one closest from right
	// second closest from left
	for (int i = 1; i < lines.size(); ++i)
	{
		int xI = lines[i][0] < lines[i][2] ? lines[i][0] : lines[i][2];
		if (xI > leftClose && xI <= centralX)
		{
			leftClose = xI;
			leftIndexClose = i;
		}
		if (xI < rightClose && xI >= centralX)
		{
			rightClose = xI;
			rightIndexClose = i;
		}
	}

	auto resLines = std::vector<cv::Vec4i>(2);
	resLines[0] = lines[leftIndexClose];
	resLines[1] = lines[rightIndexClose];


	// here we make the 2 lines longer, so we could detect intersection on image
	for (unsigned int i = 0; i < resLines.size(); ++i)
	{
		int length = 2000;
		float lenAB = sqrt(pow(resLines[i][0] - resLines[i][2], 2.0f) + pow(resLines[i][1] - resLines[i][3], 2));
		resLines[i][0] = resLines[i][2] - (resLines[i][2] - resLines[i][0]) / lenAB * length;
		resLines[i][2] = resLines[i][2] + (resLines[i][2] - resLines[i][0]) / lenAB * length;
		resLines[i][1] = resLines[i][3] - (resLines[i][3] - resLines[i][1]) / lenAB * length;
		resLines[i][3] = resLines[i][3] + (resLines[i][3] - resLines[i][1]) / lenAB * length;

		// here we want to make the lines to fit the size of the original image
		Mat matT = Mat::zeros(src.size(), src.type());
		cv::line(matT, cv::Point(resLines[i][0], resLines[i][1]), cv::Point(resLines[i][2], resLines[i][3]), cv::Scalar(255, 255, 255), 3);
		std::vector<cv::Vec4i> tempLines;
		cv::HoughLinesP(matT, tempLines, 1, CV_PI / 360, 100, 100, 1);
		//Mat matT1 = Mat::zeros(src.size(), src.type());
		//for (unsigned int j = 0; j < tempLines.size(); ++j)
		//{
		//    cv::line(matT1, cv::Point(tempLines[j][0], tempLines[j][1]), cv::Point(tempLines[j][2], tempLines[j][3]), cv::Scalar(255, 255, 255), 1);
		//}

		int maxLenIndex = 0;
		double max_dist = -1.0;
		for (size_t i = 0; i < tempLines.size(); i++)
		{
			float theta1, theta2, hyp;
			theta1 = (tempLines[maxLenIndex][3] - tempLines[maxLenIndex][1]);
			theta2 = (tempLines[maxLenIndex][2] - tempLines[maxLenIndex][0]);
			hyp = hypot(theta1, theta2);
			if (max_dist < hyp) {
				max_dist = hyp;
			}
		}

		resLines[i][0] = tempLines[maxLenIndex][0];
		resLines[i][1] = tempLines[maxLenIndex][1];
		resLines[i][2] = tempLines[maxLenIndex][2];
		resLines[i][3] = tempLines[maxLenIndex][3];
	}


	// we detect the intersection point (if exist) via image processing and findind the contour
	Mat mat = Mat::zeros(src.size(), src.type());
	for (unsigned int i = 0; i < resLines.size(); ++i)
	{
		cv::line(mat, cv::Point(resLines[i][0], resLines[i][1]), cv::Point(resLines[i][2], resLines[i][3]), cv::Scalar(255, 255, 255), 5);
	}

	cv::blur(mat, mat, Size(23, 23));
	cv::threshold(mat, mat, 140, 255, 0);
	cv::blur(mat, mat, Size(12, 12));
	cv::threshold(mat, mat, 70, 255, 0);

	vector<vector<Point>> contours;
	cv::findContours(mat, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

	auto leftLine = lines[leftIndexClose];
	auto rightLine = lines[rightIndexClose];

	if (!contours.empty() && !contours[0].empty())
	{
		// we define where is the area which we want to display, (the long lines created two areas,
		// one aside from the intersection point and the other from the other side)
		auto c = moments(contours[0]);
		auto centralPoint = cv::Point(0, 0);
		centralPoint.x = c.m10 / c.m00;
		centralPoint.y = c.m01 / c.m00;
		//cv::circle(mat, centralPoint, 3, cv::Scalar(0, 30, 255), 3);


		if ((centralPoint.y < leftLine[1] && centralPoint.y < leftLine[3])
			&& (centralPoint.y < rightLine[1] && centralPoint.y < rightLine[3]))
		{
			int indexLeft = leftLine[1] < leftLine[3] ? 1 : 3;
			int indexRight = rightLine[1] < rightLine[3] ? 1 : 3;
			resLines[0][indexLeft] = centralPoint.y;
			resLines[0][indexLeft - 1] = centralPoint.x;
			resLines[1][indexRight] = centralPoint.y;
			resLines[1][indexRight - 1] = centralPoint.x;

			resLines.push_back(Vec4f(resLines[0][(indexLeft + 1) % 4], resLines[0][(indexLeft + 2) % 4],
				resLines[1][(indexRight + 1) % 4], resLines[1][(indexRight + 2) % 4]));
		}
		else if ((centralPoint.y > leftLine[1] && centralPoint.y > leftLine[3])
			&& (centralPoint.y > rightLine[1] && centralPoint.y > rightLine[3]))
		{
			int indexLeft = leftLine[1] > leftLine[3] ? 1 : 3;
			int indexRight = rightLine[1] > rightLine[3] ? 1 : 3;
			resLines[0][indexLeft] = centralPoint.y;
			resLines[0][indexLeft - 1] = centralPoint.x;
			resLines[1][indexRight] = centralPoint.y;
			resLines[1][indexRight - 1] = centralPoint.x;

			resLines.push_back(Vec4f(resLines[0][(indexLeft + 1) % 4], resLines[0][(indexLeft + 2) % 4],
				resLines[1][(indexRight + 1) % 4], resLines[1][(indexRight + 2) % 4]));
		}
		else if ((centralPoint.x < leftLine[0] && centralPoint.x < leftLine[2])
			&& (centralPoint.x < rightLine[0] && centralPoint.x < rightLine[2]))
		{
			int indexLeft = leftLine[0] < leftLine[2] ? 0 : 2;
			int indexRight = rightLine[0] < rightLine[2] ? 0 : 2;
			resLines[0][indexLeft] = centralPoint.x;
			resLines[0][indexLeft + 1] = centralPoint.y;
			resLines[1][indexRight] = centralPoint.x;
			resLines[1][indexRight + 1] = centralPoint.y;

			resLines.push_back(Vec4f(resLines[0][(indexLeft + 2) % 4], resLines[0][(indexLeft + 3) % 4],
				resLines[1][(indexRight + 2) % 4], resLines[1][(indexRight + 3) % 4]));
		}
		else if ((centralPoint.x > leftLine[0] && centralPoint.x > leftLine[2])
			&& (centralPoint.x > rightLine[0] && centralPoint.x > rightLine[2]))
		{
			int indexLeft = leftLine[0] > leftLine[2] ? 0 : 2;
			int indexRight = rightLine[0] > rightLine[2] ? 0 : 2;
			resLines[0][indexLeft] = centralPoint.x;
			resLines[0][indexLeft + 1] = centralPoint.y;
			resLines[1][indexRight] = centralPoint.x;
			resLines[1][indexRight + 1] = centralPoint.y;


			resLines.push_back(Vec4f(resLines[0][(indexLeft + 2) % 4], resLines[0][(indexLeft + 3) % 4],
				resLines[1][(indexRight + 2) % 4], resLines[1][(indexRight + 3) % 4]));
		}

	}
	else
	{
		if (((resLines[0][1] < resLines[0][3]) && (resLines[1][1] < resLines[1][3])) ||
			((resLines[0][1] > resLines[0][3]) && (resLines[1][1] > resLines[1][3])))
		{
			resLines.push_back(Vec4f(resLines[0][0], resLines[0][1], resLines[1][0], resLines[1][1]));
			resLines.push_back(Vec4f(resLines[0][2], resLines[0][3], resLines[1][2], resLines[1][3]));
		}
		else
		{
			resLines.push_back(Vec4f(resLines[0][0], resLines[0][1], resLines[1][2], resLines[1][3]));
			resLines.push_back(Vec4f(resLines[0][2], resLines[0][3], resLines[1][0], resLines[1][1]));
		}
	}
	return resLines;
}


Mat drawPolygon(Mat src)
{
	//imshow("moto0", src);
	// now perform the line detection
	std::vector<cv::Vec4i> prevLines;
	cv::HoughLinesP(src, prevLines, 1, CV_PI / 360, 80, 40, 0);

	Mat drawing = Mat::zeros(src.size(), src.type());

	// draw the result as big green lines:
	for (unsigned int i = 0; i < prevLines.size(); ++i)
	{
		cv::line(drawing, cv::Point(prevLines[i][0], prevLines[i][1]), cv::Point(prevLines[i][2], prevLines[i][3]), cv::Scalar(255, 255, 255), 5);
	}
	//imshow("moto1", drawing);

	blur(drawing, drawing, Size(20, 20));
	cv::threshold(drawing, drawing, 130, 255, 0);
	morphologyEx(drawing, drawing, MORPH_GRADIENT, getStructuringElement(MORPH_RECT, Size(2, 2)));
	//imshow("moto2", drawing);



	// now perform the line detection
	std::vector<cv::Vec4i> lines;
	cv::HoughLinesP(drawing, lines, 1, CV_PI / 360, 20, 30, 3);


	Mat rDrawing = Mat::zeros(src.size(), src.type());

	if (!lines.empty())
	{
		auto rLines = createPolygon(lines, src);




		// draw the intersection point

		// draw the result as big green lines:
		for (unsigned int i = 0; i < rLines.size(); ++i)
		{
			cv::line(rDrawing, cv::Point(rLines[i][0], rLines[i][1]), cv::Point(rLines[i][2], rLines[i][3]), cv::Scalar(255, 255, 255), 5);
		}
	}
	return rDrawing;
}


std::string timeStampToString(const std::time_t & t)
{
	std::ostringstream oss;
	oss << t;
	return oss.str();
}


std::string objectNameFormat(std::string stationID, std::string cameraID, std::string timestamp, std::string format)
{
	return stationID + "/" + cameraID + "/" + timestamp + "." + format;
}


int main(int argc, char *argv[])
{
	string stationID = "80fad343-4025-41d4-aa32-b4033b417cc0";
	string cameraID = "2044f6a8-7dd8-4934-a2c7-d76e70ce4e6e";

	//auto s3Uploader = S3Uploader();
	auto httpAlert = HttpCreateAlert();

	string videoPath = "outcpp.mp4";
	string photoPath = "outcpp.png";
	string credentailsName = "credentials.txt";
	string timestamp;

	string path = "C:\\Users\\naorb\\Videos\\capture_3.mp4";
	string algo;
	Ptr<BackgroundSubtractor> pBackSub;
	pBackSub = createBackgroundSubtractorKNN();

	VideoWriter videoWriter;
	VideoCapture capture(path);
	if (!capture.isOpened()) {
		//error in opening the video input
		cerr << "Unable to open: " << path << endl;
		return 0;
	}
	Mat frame, fgMask, imS, imS2;

	int frameCounter = 0;
	int dangerFrameCounter = -1;
	int dangerStart = -1;


	vector<vector<Point>> polyContours;
	std::vector<Vec4i> rLines;


	while (true) {
		capture >> frame;
		if (frame.empty())
			break;
		//update the background model
		pBackSub->apply(frame, fgMask);
		Mat frameVid = frame.clone();
		//
		if (frameCounter % 30000 == 0)
		{
			Mat yellowOnly = yellowFilterSpots(frame);

			//Mat drawing = drawPolygon(yellowOnly);



			std::vector<cv::Vec4i> prevLines;
			cv::HoughLinesP(yellowOnly, prevLines, 1, CV_PI / 360, 80, 40, 0);

			Mat drawing = Mat::zeros(yellowOnly.size(), yellowOnly.type());

			// draw the result as big green lines:
			for (unsigned int i = 0; i < prevLines.size(); ++i)
			{
				cv::line(drawing, cv::Point(prevLines[i][0], prevLines[i][1]), cv::Point(prevLines[i][2], prevLines[i][3]), cv::Scalar(255, 255, 255), 5);
			}
			//imshow("moto1", drawing);

			blur(drawing, drawing, Size(20, 20));
			cv::threshold(drawing, drawing, 130, 255, 0);
			morphologyEx(drawing, drawing, MORPH_GRADIENT, getStructuringElement(MORPH_RECT, Size(2, 2)));
			//imshow("moto2", drawing);



			// now perform the line detection
			std::vector<cv::Vec4i> lines;
			cv::HoughLinesP(drawing, lines, 1, CV_PI / 360, 20, 30, 3);


			Mat rDrawing = Mat::zeros(yellowOnly.size(), yellowOnly.type());

			if (!lines.empty())
			{
				rLines = createPolygon(lines, yellowOnly);
				// draw the intersection point

				// draw the result as big green lines:
				for (unsigned int i = 0; i < rLines.size(); ++i)
				{
					//cv::line(frame, cv::Point(rLines[i][0], rLines[i][1]), cv::Point(rLines[i][2], rLines[i][3]), cv::Scalar(255, 255, 255), 5);
					cv::line(rDrawing, cv::Point(rLines[i][0], rLines[i][1]), cv::Point(rLines[i][2], rLines[i][3]), cv::Scalar(255, 255, 255), 5);
				}


			}


			//polyContours.clear();
			cv::findContours(rDrawing, polyContours, RETR_TREE, CHAIN_APPROX_SIMPLE);

			//cv::imshow("blabla", drawing);
			//waitKey();
		}

		if (!rLines.empty())
		{
			// draw the result as big green lines:
			for (unsigned int i = 0; i < rLines.size(); ++i)
			{
				cv::line(frame, cv::Point(rLines[i][0], rLines[i][1]), cv::Point(rLines[i][2], rLines[i][3]), cv::Scalar(255, 255, 255), 5);
			}
		}



		frameCounter++;
		//

		threshold(fgMask, fgMask, 10, 255, 0);
		blur(fgMask, fgMask, Size(35, 35));
		threshold(fgMask, fgMask, 40, 255, 0);

		vector<vector<Point> > contours;
		findContours(fgMask, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
		vector<vector<Point> > contours_poly(contours.size());
		vector<Rect> boundRect(contours.size());
		vector<Rect> fBoundRect(contours.size());

		bool safeState = true;
		int indexDangerBoundingBox = 0;


		for (size_t i = 0; i < contours.size(); i++)
		{
			approxPolyDP(contours[i], contours_poly[i], 3, true);
			boundRect[i] = boundingRect(contours_poly[i]);
			auto w = boundRect[i].width;
			auto h = boundRect[i].height;
			size_t size = boundRect[i].area();
			if (size > 2500 && boundRect[i].area() < fgMask.size().area() / 8 && w < fgMask.size().width / 2 && h < fgMask.size().height / 2)
			{
				auto pX = boundRect[i].x + 0.5 * w;
				auto pY = boundRect[i].y + 0.7 * h;

				fBoundRect.push_back(boundRect[i]);

				if (!polyContours.empty() && pointPolygonTest(polyContours[0], cv::Point(pX, pY), true) >= 0)
				{
					//cout << "ALERT!!\n";
					cv::circle(frame, cv::Point(pX, pY), 3, cv::Scalar(0, 30, 255), 3);
					safeState = false;
					indexDangerBoundingBox = i;
					break;
				}



			}

		}


		Mat drawing = Mat::zeros(fgMask.size(), CV_8UC3);
		for (size_t i = 0; i < fBoundRect.size(); i++)
		{
			Scalar color = Scalar(20, 255, 20);
			//drawContours(drawing, contours_poly, (int)i, color);
			rectangle(frame, fBoundRect[i].tl(), fBoundRect[i].br(), color, 2);
		}



		if (!safeState)
		{
			dangerFrameCounter++;
			dangerStart = frameCounter;

			cv::rectangle(frame,
				boundRect[indexDangerBoundingBox].tl(),
				boundRect[indexDangerBoundingBox].br(),
				Scalar(0, 0, 255), 2);
			cv::rectangle(frameVid,
				boundRect[indexDangerBoundingBox].tl(),
				boundRect[indexDangerBoundingBox].br(),
				Scalar(0, 0, 255), 2);

			if (dangerFrameCounter == 0) {
				timestamp = timeStampToString(std::time(nullptr));
				videoWriter = VideoWriter(videoPath, cv::VideoWriter::fourcc('P', 'y', 'A', 'V'), 10, frame.size());

				std::vector<int> params;
				params.push_back(16); // CV_IMWRITE_PNG_COMPRESSION == 16
				params.push_back(9);   // that's compression level, 9 == full , 0 == none
				cv::imwrite(photoPath, frameVid, params);
				/* alert via http */
				//threadHttp = std::thread(httpAlert, NGROK_URL, "AlertWrapper", stationID, cameraID, timestamp);
				httpAlert.run(NGROK_URL, "AlertWrapper", stationID, cameraID, timestamp);
			}


		}
		if (dangerFrameCounter >= 0)
		{
			videoWriter.write(frameVid);
		}
		if (dangerFrameCounter > 0 && frameCounter - dangerStart > 20)
		{
			videoWriter.release();
			//upload via s3 bucket
			auto photo = objectNameFormat(stationID, cameraID, timestamp, "png");
			auto vid = objectNameFormat(stationID, cameraID, timestamp, "mp4");



			//s3Uploader.uploadToS3(photo, photoPath);
			//s3Uploader.uploadToS3(vid, videoPath);

			std::string rootDir = "C:\\Users\\naorb\\source\\repos\\CrossRailsDetect\\ConsoleAppCrossRailsDetection\\";
			std::string command = rootDir + "S3Upload.exe";
			auto commandPhoto = command + " " + photo + " " + rootDir + photoPath + " " + rootDir + credentailsName;
			auto commandVid = command + " " + vid + " " + videoPath + " " + rootDir + credentailsName;

			system(commandPhoto.c_str());
			system(commandVid.c_str());

			// alert via http
			httpAlert.run(NGROK_URL, "S3Wrapper", stationID, cameraID, timestamp);

			dangerStart = -1;
			dangerFrameCounter = -1;
		}




		//RNG rng(12345);



		//get the frame number and write it on the current frame
		rectangle(frame, cv::Point(10, 2), cv::Point(100, 20),
			cv::Scalar(255, 255, 255), -1);
		stringstream ss;
		ss << capture.get(CAP_PROP_POS_FRAMES);
		string frameNumberString = ss.str();
		putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
			FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
		//show the current frame and the fg masks

		resize(fgMask, imS, Size(960, 540));
		resize(frame, imS2, Size(960, 540));
		imshow("FG Mask", fgMask);
		imshow("Frame", imS2);
		//get the input from the keyboard
		int keyboard = waitKey(30);
		//cout << keyboard;
		if (keyboard == 'q' || keyboard == 27)
			break;
	}



	if (dangerFrameCounter >= 0)
	{
		videoWriter.release();
		//upload via s3 bucket
		auto photo = objectNameFormat(stationID, cameraID, timestamp, "png");
		auto vid = objectNameFormat(stationID, cameraID, timestamp, "mp4");
		//s3Uploader.uploadToS3(photo, photoPath);
		//s3Uploader.uploadToS3(vid, videoPath);



		std::string rootDir = "C:\\Users\\naorb\\source\\repos\\CrossRailsDetect\\ConsoleAppCrossRailsDetection\\";
		std::string command = rootDir + "S3Upload.exe";
		auto commandPhoto = command + " " + photo + " " + rootDir + photoPath + " " + rootDir + credentailsName;
		auto commandVid = command + " " + vid + " " + videoPath + " " + rootDir + credentailsName;

		system(commandPhoto.c_str());
		system(commandVid.c_str());

		// alert via http

		httpAlert.run(NGROK_URL, "S3Wrapper", stationID, cameraID, timestamp);
	}

	return 0;
}