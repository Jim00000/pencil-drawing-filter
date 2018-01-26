#include <opencv2/opencv.hpp>
#include "combiner.hpp"

using namespace cv;
using namespace pencil;

int main()
{
    Mat paper = imread("/home/jim/Github/pencil-drawing-filter/resources/paper.png");
    cvtColor(paper, paper, CV_BGR2GRAY);
    Mat dst;
    combiner::extend_file(paper, dst, Size(600, 600));

    imshow("test", dst);
    waitKey();
    
    return 0;
}