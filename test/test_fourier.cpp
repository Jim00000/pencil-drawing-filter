#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int
main(int argc, char** argv)
{
    Mat src = imread("/home/jim/Github/pencil-drawing-filter/sample.jpg",
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

    Mat magn,angl;
    Mat output(src.size(), src.type());
    cartToPolar(planes[0],planes[1],magn,angl,true);

    for(int i = 0; i < src.rows; i++) {
        for(int j = 0; j < src.cols; j++) {
            cout << angl.at<float>(i, j) << endl;
        }
    }

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
    imshow("mag", magI);
    waitKey();

    return 0;
}