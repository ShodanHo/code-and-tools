/******************************************************************************
 *
 * Copyright (c) 2016 ShodanHo. All rights reserved.
 *
 * Project: Deskew image investigations.
 *
 * Description: Deskew image investigations using the OpenCV package.
 *
 * Date: April 10, 2016.
 *
 * Reference: http://felix.abecassis.me/2011/10/opencv-rotation-deskewing/
 *
 *****************************************************************************/

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <vector>

#define STR(x) #x << '=' << x

static void deskew(const char* filename, double angle) {
  cv::Mat img = cv::imread(filename, 0);
  cv::Mat orig = img;
  cv::imshow("Original", orig);

  cv::bitwise_not(img, img);
  cv::imshow("bitwise_not", img);
  // sleep(5);

  std::vector<cv::Point> points;
  cv::Mat_<uchar>::iterator it = img.begin<uchar>();
  cv::Mat_<uchar>::iterator end = img.end<uchar>();
  for (; it != end; ++it)
    if (*it)
      points.push_back(it.pos());

  std::cout << STR(points.size()) << '\n';
  cv::RotatedRect box = cv::minAreaRect(cv::Mat(points));
  std::cout << STR(box.angle) << '\n';

  cv::Mat rot_mat = cv::getRotationMatrix2D(box.center, box.angle + 90, 1);
  cv::Mat rotated;
  cv::warpAffine(img, rotated, rot_mat, img.size(), cv::INTER_CUBIC);
  cv::Size box_size = box.size;
  if (box.angle < -45.)
    std::swap(box_size.width, box_size.height);
  cv::Mat cropped;
  cv::getRectSubPix(rotated, box_size, box.center, cropped);
  // cv::imshow("Original", img);
  cv::imshow("Rotated", rotated);
  cv::imshow("Cropped", cropped);
  cv::waitKey(0);
}

int main() {
  std::cout << "Hello world!\n";
  deskew("/home/stuartf/delete/Untitled.jpg", 0.0);
  return 0;
}
