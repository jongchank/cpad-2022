#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <string>
#include <deque>
#include <iostream>
#include <cmath>
#include <fstream>
#include <time.h>

using namespace std;
using namespace cv;

Mat warped_img(Mat _frame);
int arrMaxIdx(int hist[], int start, int end, int Max);
Mat polyfit(vector<int> x_val, vector<int> y_val);
Mat detect_lines_sliding_window(Mat _frame, bool option);
Mat draw_lane(Mat _sliding_frame, Mat _frame);
void clear_release();
void display_lane(Mat _frame, string path, bool option);

/*lc: 1st = 15m, 2nd = 12.5m, 3rd =  12.5m*/
/*ci: 1st = 15m, 2nd = 15m, 3rd = 12.5m*/
float distance_ = 0.0f;
int width_ = 640;
int height_ = 480;
bool option_ = false;
int threshold_ = 128;	//sd-12.5, 17.5 | lc-1st, 2nd, 3rd | ci-1st, 2nd, 3rd
//int threshold_ = 215;	//ld-17.5
//int threshold_ = 220;	//sd-15, 20 | ld-12.5, 15, 20 | 
int prev_Llane_current_ = 160;
int prev_Rlane_current_ = 480;
int N_ = 6;
vector<Point2f> corners_;
vector<Point2f> warpCorners_;

vector<int> left_x_;
vector<int> left_y_;
vector<int> right_x_;
vector<int> right_y_;
vector<int> center_x_;
vector<int> center_y_;
vector<float> weights_ = {0, 0, 0, 0, 0, 0};	//before
//vector<float> weights_ = {0.05, 0.05, 0.1, 0.1, 0.3, 0.4};	//after

vector<Point2i> left_;
vector<Point2i> right_;

deque<vector<Point2i>> left_deque_;
deque<vector<Point2i>> right_deque_;

Mat left_coef_ = Mat::zeros(3, 1, CV_32F);
Mat right_coef_ = Mat::zeros(3, 1, CV_32F);
Mat center_coef_ = Mat::zeros(3, 1, CV_32F);

