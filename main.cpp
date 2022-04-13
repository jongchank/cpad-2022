#include "lanedetection.h"

using namespace std;
using namespace cv;

void set_weights(int total_num, float cur, float last){
    float gap = (cur - last) / (total_num - 1);
    float sum = total_num * ((2 * last) + ((total_num - 1) * gap)) / 2;
    for (int i = 1; i <= total_num; i++){
        float val = (last + (i - 1) * gap) / sum;
        weights_.push_back(val);
    }
}

Mat warped_img(Mat _frame) {
    Mat result, trans;
    trans = getPerspectiveTransform(corners_, warpCorners_);
    warpPerspective(_frame, result, trans, Size(width_, height_));

    return result;
}

int arrMaxIdx(int hist[], int start, int end, int Max) {
    int max_index = -1;
    int max_val = 0;

    if (end > Max)
        end = Max;

    for (int i = start; i < end; i++) {
        if (max_val < hist[i]) {
            max_val = hist[i];
            max_index = i;
        }
    }
    if (max_index == -1) {
        return -1;
    }
    return max_index;
}

Mat polyfit(vector<int> x_val, vector<int> y_val) {
    Mat coef(3, 1, CV_32F);
    int i, j, k, n, N;
    N = (int)x_val.size();
    n = 2;
    double* x, * y;
    x = new double[N];
    y = new double[N];
    for (int q = 0; q < N; q++) {
        x[q] = (double)(x_val[q]);
        y[q] = (double)(y_val[q]);
    }
    double* X;
    X = new double[2 * n + 1];                        //Array that will store the values of sigma(xi),sigma(xi^2),sigma(xi^3)....sigma(xi^2n)
    for (i = 0; i < (2 * n + 1); i++)
    {
        X[i] = 0;
        for (j = 0; j < N; j++)
            X[i] = X[i] + pow(x[j], i);        //consecutive positions of the array will store N,sigma(xi),sigma(xi^2),sigma(xi^3)....sigma(xi^2n)
    }
    double** B, * a;
    B = new double* [n + 1];
    for (int i = 0; i < (n + 1); i++)
        B[i] = new double[n + 2];
    a = new double[n + 1];            //B is the Normal matrix(augmented) that will store the equations, 'a' is for value of the final coefficients
    for (i = 0; i <= n; i++)
        for (j = 0; j <= n; j++)
            B[i][j] = X[i + j];            //Build the Normal matrix by storing the corresponding coefficients at the right positions except the last column of the matrix
    double* Y;
    Y = new double[n + 1];                    //Array to store the values of sigma(yi),sigma(xi*yi),sigma(xi^2*yi)...sigma(xi^n*yi)
    for (i = 0; i < (n + 1); i++)
    {
        Y[i] = 0;
        for (j = 0; j < N; j++)
            Y[i] = Y[i] + pow(x[j], i) * y[j];        //consecutive positions will store sigma(yi),sigma(xi*yi),sigma(xi^2*yi)...sigma(xi^n*yi)
    }
    for (i = 0; i <= n; i++)
        B[i][n + 1] = Y[i];                //load the values of Y as the last column of B(Normal Matrix but augmented)
    n = n + 1;                //n is made n+1 because the Gaussian Elimination part below was for n equations, but here n is the degree of polynomial and for n degree we get n+1 equations

    for (i = 0; i < n; i++)                    //From now Gaussian Elimination starts(can be ignored) to solve the set of linear equations (Pivotisation)
        for (k = i + 1; k < n; k++)
            if (B[i][i] < B[k][i])
                for (j = 0; j <= n; j++)
                {
                    double temp = B[i][j];
                    B[i][j] = B[k][j];
                    B[k][j] = temp;
                }

    for (i = 0; i < (n - 1); i++)            //loop to perform the gauss elimination
        for (k = i + 1; k < n; k++)
        {
            double t = B[k][i] / B[i][i];
            for (j = 0; j <= n; j++)
                B[k][j] = B[k][j] - t * B[i][j];    //make the elements below the pivot elements equal to zero or elimnate the variables
        }
    for (i = n - 1; i >= 0; i--)                //back-substitution
    {                        //x is an array whose values correspond to the values of x,y,z..
        a[i] = B[i][n];                //make the variable to be calculated equal to the rhs of the last equation
        for (j = 0; j < n; j++)
            if (j != i)            //then subtract all the lhs values except the coefficient of the variable whose value                                   is being calculated
                a[i] = a[i] - B[i][j] * a[j];
        a[i] = a[i] / B[i][i];            //now finally divide the rhs by the coefficient of the variable to be calculated
        coef.at<float>(i, 0) = (float)a[i];
    }

    delete[] x;
    delete[] y;
    delete[] X;
    delete[] Y;
    delete[] B;
    delete[] a;

    return coef;
}

Mat detect_lines_sliding_window(Mat _frame, bool option) {
    Mat frame, result;
    int width = _frame.cols;
    int height = _frame.rows;

    _frame.copyTo(frame);
    Mat nonZero;
    findNonZero(frame, nonZero);

    vector<int> good_left_inds;
    vector<int> good_right_inds;
    int* hist = new int[width];

    for (int i = 0; i < width; i++) {
        hist[i] = 0;
    }

    for (int j = (height / 2); j < height; j++) {
        for (int i = 0; i < width; i++) {
            if (frame.at <uchar>(j, i) == 255) {
                hist[i] += 1;
            }
        }
    }

    cvtColor(frame, result, COLOR_GRAY2BGR);

    int mid_point = width / 2; // 320
    int quarter_point = mid_point / 2; // 160
    int n_windows = 9;
    int margin = 120 * width / 1280;
    int min_pix = 30 * width / 1280;

    int window_width = margin * 2;    // 120
    int window_height;
    int distance;
    if (option) {
        window_height = (height >= distance_) ? ((height - distance_) / n_windows) : (height / n_windows);    // defalut = 53
        distance = distance_;
    }
    else {
        distance = 0;
        window_height = height / n_windows;
    }
    int offset = margin;
    int range = 120 / 4;
    int Llane_base = arrMaxIdx(hist, 50, mid_point-100, width);
    int Rlane_base = arrMaxIdx(hist, mid_point+100, width-50, width);

    int Llane_current;
    int Rlane_current;

    if (Llane_base == -1 || Rlane_base == -1) { //Case: histogram array is empty 
        Llane_current = prev_Llane_current_;
        Rlane_current = prev_Rlane_current_;
    }
    else {
        Llane_current = Llane_base;
        Rlane_current = Rlane_base;
    }
    prev_Llane_current_ = Llane_current;
    prev_Rlane_current_ = Rlane_current;

    /* params are used to calc gap */
    int L_prev = Llane_current;
    int R_prev = Rlane_current;
    int L_gap = 0;
    int R_gap = 0;

    //unsigned int index;
    for (int window = 0; window < n_windows; window++) {
        int    Ly_pos = height - (window + 1) * window_height; // win_y_low , win_y_high = win_y_low - window_height
        int    Ry_pos = height - (window + 1) * window_height;
        int    Ly_top = height - window * window_height;
        int    Ry_top = height - window * window_height;

        int    Lx_pos = Llane_current - margin; // win_xleft_low, win_xleft_high = win_xleft_low + margin*2
        int    Rx_pos = Rlane_current - margin; // win_xrignt_low, win_xright_high = win_xright_low + margin*2
        rectangle(result, \
            Rect(Lx_pos, Ly_pos, window_width, window_height), \
            Scalar(255, 50, 100), 1);
        rectangle(result, \
            Rect(Rx_pos, Ry_pos, window_width, window_height), \
            Scalar(100, 50, 255), 1);

        int nZ_y, nZ_x;
        good_left_inds.clear();
        good_right_inds.clear();
        vector <int>().swap(good_left_inds);
        vector <int>().swap(good_right_inds);

        for (unsigned int index = (nonZero.total() - 1); index > 1; index--) {
            nZ_y = nonZero.at<Point>(index).y;
            nZ_x = nonZero.at<Point>(index).x;
            if ((nZ_y >= Ly_pos) && \
                (nZ_y > (distance)) && \
                (nZ_y < Ly_top) && \
                (nZ_x >= Lx_pos) && \
                (nZ_x < (Lx_pos + window_width))) {
                result.at<Vec3b>(nonZero.at<Point>(index))[0] = 255;
                result.at<Vec3b>(nonZero.at<Point>(index))[1] = 0;
                result.at<Vec3b>(nonZero.at<Point>(index))[2] = 0;
                good_left_inds.push_back(index);
            }

            if ((nZ_y >= (Ry_pos)) && \
                (nZ_y > (distance)) && \
                (nZ_y < Ry_top) && \
                (nZ_x >= Rx_pos) && \
                (nZ_x < (Rx_pos + window_width))) {
                result.at<Vec3b>(nonZero.at<Point>(index))[0] = 0;
                result.at<Vec3b>(nonZero.at<Point>(index))[1] = 0;
                result.at<Vec3b>(nonZero.at<Point>(index))[2] = 255;
                good_right_inds.push_back(index);
            }
        }

        int Lsum, Rsum;
        Lsum = Rsum = 0;
        unsigned int _size;
        vector<int> Llane_x;
        vector<int> Llane_y;
        vector<int> Rlane_x;
        vector<int> Rlane_y;

        if (good_left_inds.size() > (size_t)min_pix) {
            _size = (unsigned int)(good_left_inds.size());
            for (int i = Ly_top - 1; i >= Ly_pos; i--)
            {
                int Ly_sum = 0;
                int count = 0;
                for (unsigned int index = 0; index < _size; index++) {
                    int j = nonZero.at<Point>(good_left_inds.at(index)).y;
                    if (i == j)
                    {
                        Ly_sum += nonZero.at<Point>(good_left_inds.at(index)).x;
                        count++;
                        Lsum += nonZero.at<Point>(good_left_inds.at(index)).x;
                    }
                }
                if (count != 0)
                {
                    Llane_x.push_back(Ly_sum / count);
                    Llane_y.push_back(i);
                    if (left_deque_.size() != N_){
                        left_x_.push_back(Ly_sum / count);
                        left_y_.push_back(i);
                    }
                }
                else {
                    Llane_x.push_back(-1);    //No value in each y-axis of sliding-window
                    Llane_y.push_back(i);
                }
            }
            Llane_current = Lsum / _size;    // Average value all of points in sliding-window
        }
        else {
            for (int i = Ly_top - 1; i >= Ly_pos; i--)
            {
                Llane_x.push_back(-1);    //No value in each y-axis of sliding-window
                Llane_y.push_back(i);
            }
            Llane_current += (L_gap);
        }
        if (good_right_inds.size() > (size_t)min_pix) {
            _size = (unsigned int)(good_right_inds.size());
            for (int i = Ry_top - 1; i >= Ry_pos; i--)
            {
                int Ry_sum = 0;
                int count = 0;
                for (unsigned int index = 0; index < _size; index++) {
                    int j = nonZero.at<Point>(good_right_inds.at(index)).y;
                    if (i == j)
                    {
                        Ry_sum += nonZero.at<Point>(good_right_inds.at(index)).x;
                        count++;
                        Rsum += nonZero.at<Point>(good_right_inds.at(index)).x;
                    }
                }
                if (count != 0)
                {
                    Rlane_x.push_back(Ry_sum / count);
                    Rlane_y.push_back(i);
                    if (right_deque_.size() != N_){
                        right_x_.push_back(Ry_sum / count);
                        right_y_.push_back(i);
                    }

                }
                else {
                    Rlane_x.push_back(-1);
                    Rlane_y.push_back(i);
                }
            }
            Rlane_current = Rsum / _size;

        }
        else {
            for (int i = Ry_top - 1; i >= Ry_pos; i--)
            {
                Rlane_x.push_back(-1);
                Rlane_y.push_back(i);
            }
            Rlane_current += (R_gap);
        }
        if (window != 0) {    //Predict the next sliding-window x-pos
            if (Rlane_current != R_prev) {
                R_gap = (Rlane_current - R_prev);
            }
            if (Llane_current != L_prev) {
                L_gap = (Llane_current - L_prev);
            }
        }

        if (left_deque_.size() != N_){
            for (int i = 0; i < Llane_x.size(); i++)
            {
                if ((Llane_x.at(i) != -1) && (Rlane_x.at(i) != -1)) {
                    center_x_.push_back((Llane_x.at(i) + Rlane_x.at(i)) / 2);
                    center_y_.push_back(Llane_y.at(i));
                }
            }
        }
        else{
            for (int i = 0; i < left_.size(); i++)
            {
                if ((left_.at(i).x != -1) && (right_.at(i).x != -1)) {
                    center_x_.push_back((left_.at(i).x + right_.at(i).x) / 2);
                    center_y_.push_back(left_.at(i).y);
                }
            }
        }    

        L_prev = Llane_current;
        R_prev = Rlane_current;
        
    }

    if (left_x_.size() != 0) {
        left_coef_ = polyfit(left_y_, left_x_);
    }

    if (right_x_.size() != 0) {
        right_coef_ = polyfit(right_y_, right_x_);
    }

    if ((left_x_.size() != 0) && (right_x_.size() != 0)) {
        center_coef_ = polyfit(center_y_, center_x_);
    }
    

    delete[] hist;

    return result;
}

Mat draw_lane(Mat _sliding_frame, Mat _frame) {
    Mat new_frame, left_coef(left_coef_), right_coef(right_coef_), center_coef(center_coef_), trans;
    trans = getPerspectiveTransform(warpCorners_, corners_);
    _frame.copyTo(new_frame);

    vector<Point> left_point;
    vector<Point> right_point;
    vector<Point> center_point;

    vector<Point2f> left_point_f;
    vector<Point2f> right_point_f;
    vector<Point2f> center_point_f;

    vector<Point2f> warped_left_point;
    vector<Point2f> warped_right_point;
    vector<Point2f> warped_center_point;

    vector<Point> left_points;
    vector<Point> right_points;
    vector<Point> center_points;

    if ((!left_coef.empty()) && (!right_coef.empty())) {
        for (int i = 0; i <= height_; i++) {
            Point temp_left_point;
            Point temp_right_point;
            Point temp_center_point;

            temp_left_point.x = (int)((left_coef.at<float>(2, 0) * pow(i, 2)) + (left_coef.at<float>(1, 0) * i) + left_coef.at<float>(0, 0));
            temp_left_point.y = (int)i;
            temp_right_point.x = (int)((right_coef.at<float>(2, 0) * pow(i, 2)) + (right_coef.at<float>(1, 0) * i) + right_coef.at<float>(0, 0));
            temp_right_point.y = (int)i;
            temp_center_point.x = (int)((center_coef.at<float>(2, 0) * pow(i, 2)) + (center_coef.at<float>(1, 0) * i) + center_coef.at<float>(0, 0));
            temp_center_point.y = (int)i;

            left_point.push_back(temp_left_point);
            left_point_f.push_back(temp_left_point);
            right_point.push_back(temp_right_point);
            right_point_f.push_back(temp_right_point);
            center_point.push_back(temp_center_point);
            center_point_f.push_back(temp_center_point);
        }
        const Point* left_points_point_ = (const cv::Point*)Mat(left_point).data;
        int left_points_number_ = Mat(left_point).rows;
        const Point* right_points_point_ = (const cv::Point*)Mat(right_point).data;
        int right_points_number_ = Mat(right_point).rows;
        const Point* center_points_point_ = (const cv::Point*)Mat(center_point).data;
        int center_points_number_ = Mat(center_point).rows;


        polylines(_sliding_frame, &left_points_point_, &left_points_number_, 1, false, Scalar(255, 200, 200), 5);
        polylines(_sliding_frame, &right_points_point_, &right_points_number_, 1, false, Scalar(200, 200, 255), 5);
        polylines(_sliding_frame, &center_points_point_, &center_points_number_, 1, false, Scalar(200, 255, 200), 5);

        perspectiveTransform(left_point_f, warped_left_point, trans);
        perspectiveTransform(right_point_f, warped_right_point, trans);
        perspectiveTransform(center_point_f, warped_center_point, trans);

        for (int i = 0; i <= height_; i++) {
            Point temp_left_point;
            Point temp_right_point;
            Point temp_center_point;

            temp_left_point.x = (int)warped_left_point[i].x;
            temp_left_point.y = (int)warped_left_point[i].y;
            temp_right_point.x = (int)warped_right_point[i].x;
            temp_right_point.y = (int)warped_right_point[i].y;
            temp_center_point.x = (int)warped_center_point[i].x;
            temp_center_point.y = (int)warped_center_point[i].y;

            left_points.push_back(temp_left_point);
            right_points.push_back(temp_right_point);
            center_points.push_back(temp_center_point);
        }

        const Point* left_points_point = (const cv::Point*)Mat(left_points).data;
        int left_points_number = Mat(left_points).rows;
        const Point* right_points_point = (const cv::Point*)Mat(right_points).data;
        int right_points_number = Mat(right_points).rows;
        const Point* center_points_point = (const cv::Point*)Mat(center_points).data;
        int center_points_number = Mat(center_points).rows;


        polylines(new_frame, &left_points_point, &left_points_number, 1, false, Scalar(255, 100, 100), 5);
        polylines(new_frame, &right_points_point, &right_points_number, 1, false, Scalar(100, 100, 255), 5);
        polylines(new_frame, &center_points_point, &center_points_number, 1, false, Scalar(100, 255, 100), 5);

        left_point.clear();
        right_point.clear();
        center_point.clear();

        return new_frame;
    }
    return _frame;
}

void clear_release() {
    left_x_.clear();
    left_y_.clear();
    right_x_.clear();
    right_y_.clear();
    center_x_.clear();
    center_y_.clear();
    left_.clear();
    right_.clear();
    vector <Point2i>().swap(left_);
    vector <Point2i>().swap(right_);
    vector <int>().swap(left_x_);
    vector <int>().swap(left_y_);
    vector <int>().swap(right_x_);
    vector <int>().swap(right_y_);
    vector <int>().swap(center_x_);
    vector <int>().swap(center_y_);

    left_coef_ = Mat::zeros(3, 1, CV_32F);
    right_coef_ = Mat::zeros(3, 1, CV_32F);
    center_coef_ = Mat::zeros(3, 1, CV_32F);
}

void display_lane(Mat _frame, string path, bool option) {
    Mat new_frame, temp_frame, warped_frame, gray_frame, blur_frame, binary_frame, sliding_frame, resized_frame;

    resize(_frame, new_frame, Size(width_, height_));
    Mat trans = getPerspectiveTransform(corners_, warpCorners_);

    warpPerspective(new_frame, warped_frame, trans, Size(width_, height_));
    GaussianBlur(warped_frame, blur_frame, Size(5, 5), 0, 0, BORDER_DEFAULT);
    cvtColor(blur_frame, gray_frame, COLOR_BGR2GRAY);
    threshold(gray_frame, binary_frame, threshold_, 255, THRESH_BINARY);

    sliding_frame = detect_lines_sliding_window(binary_frame, option);
    resized_frame = draw_lane(sliding_frame, new_frame);

    clear_release();

    namedWindow("Window1");
    moveWindow("Window1", 0, 0);
    namedWindow("Window2");
    moveWindow("Window2", 640, 0);
    namedWindow("Window3");
    moveWindow("Window3", 1280, 0);

    if (!new_frame.empty()) {
        string name;
        name = path + "_1.png";
        resize(new_frame, new_frame, Size(640, 480));
        imshow("Window1", new_frame);
        imwrite(name, new_frame);
    }
    if (!sliding_frame.empty()) {
        string name;
        name = path + "_2.png";
        resize(sliding_frame, sliding_frame, Size(640, 480));
        imshow("Window2", sliding_frame);
        imwrite(name, sliding_frame);
    }
    if (!resized_frame.empty()) {
        string name;
        name = path + "_3.png";
        resize(resized_frame, resized_frame, Size(640, 480));
        imshow("Window3", resized_frame);
        imwrite(name, resized_frame);
    }
    waitKey(3);
}

int main() {
    vector<int> left_x_;
    vector<int> left_y_;
    vector<int> right_x_;
    vector<int> right_y_;
    vector<int> center_x_;
    vector<int> center_y_;

    /*lc: 1st = 15m, 2nd = 12.5m, 3rd =  12.5m*/
    /*ci: 1st = 15m, 2nd = 15m, 3rd = 12.5m*/
    float t_gap = f;
    float b_gap = f;
    float t_height = f;
    float b_height = f;
    int width_offset = 5;
    int extra_up = 160;
    int extra_down = 210;

    int top_gap = width_ * t_gap;
    int bot_gap = width_ * b_gap;
    int top_height = height_ * t_height;
    int bot_height = height_ * b_height;

    corners_.resize(4);
    corners_[0] = Point2f(top_gap + width_offset, bot_height);
    corners_[1] = Point2f((width_ - top_gap) + width_offset + 15, bot_height);
    corners_[2] = Point2f(bot_gap + width_offset, top_height);
    corners_[3] = Point2f((width_ - bot_gap) + width_offset, top_height);

    warpCorners_.resize(4);
    warpCorners_[0] = Point2f(extra_up, 0.0);
    warpCorners_[1] = Point2f(width_ - extra_up, 0.0);
    warpCorners_[2] = Point2f(extra_down, height_);
    warpCorners_[3] = Point2f(width_ - extra_down, height_);


    Mat img;
    string in_name;
    string out_name;
    char temp[128] = { 0, };
    for (int i = 0; i < 200; i++) {
        /* read input file */
        sprintf(temp, "/home/nvidia/lanedetection/image/frame%04d", i);
        in_name = temp;
        in_name = in_name + ".jpg";
        img = imread(in_name);

        /* Write output file in display_lane func */
        sprintf(temp, "/home/nvidia/lanedetection/image/frame%04d", i);
        out_name = temp;

        display_lane(img, out_name, option_);
    }
    destroyAllWindows();

    return 0;
}
