#include <opencv2/opencv.hpp>
#include "font24x40_lsb.h"
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <thread>
#include <unistd.h>

#define FONT_SY 40
#define FONT_SX 24

using namespace cv;
using namespace std;
using namespace chrono;

void draw_char(Mat &buffer, unsigned char ascii, Vec3b &color, int x0, int y0) {

    for (int y = 0; y < FONT_SY; ++y) {
        for (int x = 0; x < FONT_SX; ++x) {
            if (font[ascii][y] & (1 << x)) {
                buffer.at<cv::Vec3b>(y0 + y, x0 + x) = color;
            }
        }
    }
}

void draw_column(Mat &mat, int x, string line, int colorDiff) {
    Vec3b green(0, (255 - 30) / colorDiff, 0);

    int y = 0;
    for (auto c : line) {
        if(y >= colorDiff) {
            return;
        }
        draw_char(mat, c, green, x * FONT_SX, y++ * FONT_SY);

        green[1] += colorDiff;
    }
}


int main(int argc, char **argv) {
    int width =  atoi(argv[1]);
    int height = atoi(argv[2]);
    string fileName = argv[3];

    cv::Mat image = cv::Mat::zeros(height, width, CV_8UC3);

    ifstream lines(fileName);

    vector<thread> threads;

    string line;
    int x = 0;
    while (getline(lines, line)) {
        if(x >= width / FONT_SX) {
            break;
        }

        threads.emplace_back([line, x, &image, height]() {
            draw_column(image, x, line, height / FONT_SY);
        });

        x++;
    }

    for (auto &t : threads) {
        if(t.joinable()) {
            t.join();
        }
    }

    if (isatty(0)) {
        imshow("image", image);
        waitKey();
    } else {
        imwrite("matrix.png", image);
    }

}