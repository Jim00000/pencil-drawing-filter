#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int
main(int argc, char** argv)
{
    Mat src = imread("/home/jim/Github/pencil-drawing-filter/build/sample3.png",
                     CV_LOAD_IMAGE_GRAYSCALE);
    Mat padded;

    int m = getOptimalDFTSize(src.rows);
    int n = getOptimalDFTSize(src.cols);
    copyMakeBorder(src, padded, 0, m - src.rows, 0, n - src.cols, BORDER_CONSTANT,
                   Scalar::all(0));

    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexImg;
    merge(planes, 2, complexImg);
    dft(complexImg, complexImg);
    split(complexImg, planes);

    Mat magn, angl;
    Mat output(src.size(), src.type());
    cartToPolar(planes[0], planes[1], magn, angl, true);

    // vector<float> angles(360);
    // std::fill(angles.begin(), angles.end(), 0);
    // for(int i = 0 ; i < magn.rows; i++) {
    //     for(int j = 0 ; j < magn.cols; j++) {
    //         int angle = round(angl.at<float>(i, j));
    //         float mag  = round(magn.at<float>(i, j));
    //         angles[angle] += mag;
    //     }
    // }

    // for(int i = 0 ; i < angles.size(); i++)
    //     cout << i << ":" << angles[i] << endl;

    magnitude(planes[0], planes[1],
              planes[0]); //planes[0] = sqrt((planes[0])^2 + (planes[1])^2
    Mat magI = planes[0];
    magI += Scalar::all(1); //magI = log(1+planes[0])
    log(magI, magI);

    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));  //令邊長為偶數

    //將區塊重排，讓原點在影像的中央
    int cx = magI.cols / 2;
    int cy = magI.rows / 2;

    Mat q0(magI, Rect(0, 0, cx, cy));
    Mat q1(magI, Rect(cx, 0, cx, cy));
    Mat q2(magI, Rect(0, cy, cx, cy));
    Mat q3(magI, Rect(cx, cy, cx, cy));

    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

    normalize(magI, magI, 0, 1, CV_MINMAX);

    // imshow("source", src);
    // imshow("mag", magI);
    // waitKey();

    Mat output1;
    cv::logPolar(magI, output1, Point2f(magI.cols / 2, magI.rows / 2), 50.0, CV_WARP_FILL_OUTLIERS);
    Mat vec;
    cv::reduce(output1, vec, 1, CV_REDUCE_SUM);
    cv::Point posi, dummy;
    double min, max;
    cv::minMaxLoc(vec, &min, &max, &dummy, &posi);
    cout << posi << endl;
    cout << cx << endl;
    cout << cy << endl;
    // imshow("contours", output1);
    // waitKey();


    imshow("contours", magI);
    waitKey();

    return 0;
}
