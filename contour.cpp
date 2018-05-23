  #include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat src; Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

/// Function header
void thresh_callback(int, void* );

/** @function main */
int main( int argc, char** argv )
{
  /// Load source image and convert it to gray
  src = imread( argv[1], 1 );

  /// Convert image to gray and blur it
  cvtColor( src, src_gray, CV_BGR2GRAY );
  blur( src_gray, src_gray, Size(3,3) );

  /// Create Window
  char* source_window = "Source";
  namedWindow( source_window, CV_WINDOW_NORMAL );
  imshow( source_window, src );

  createTrackbar( " Canny thresh:", "Source", &thresh, max_thresh, thresh_callback );
  thresh_callback( 0, 0 );

  waitKey(0);

}

/** @function thresh_callback */
void thresh_callback(int, void* )
{
  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using canny
  Canny( src_gray, canny_output, thresh, thresh*2, 3 );
  /// Find contours
  findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Draw contours
  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  int a[50][2];
  int ct=0;
  for(int i=0;i<contours.size();i++)
  {
    int sumx=0;
    int sumy=0;
    if(contours[i].size()>5)
    {
      for(int j=0;j<contours[i].size();j++)
      {
        sumx+=contours[i][j].x;
        sumy+=contours[i][j].y;
      }
      int b=sumx/contours[i].size();
      int c=sumy/contours[i].size();
      int k;
      for(k=0;k<contours.size();k++)
      {
        if(abs(b-a[k][0])+abs(c-a[k][1])<10)
          break;
      }
      if(k==contours.size())
      {
        a[ct][0]=b;
        a[ct++][1]=c;
      }
    }
  }
  for(int i=0;i<ct;i++)
    cout<<a[i][0]<<","<<a[i][1]<<endl;

  for( int i = 0; i< contours.size(); i++)
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
     }

  /// Show in a window
  namedWindow( "Contours", CV_WINDOW_NORMAL );
  imshow( "Contours", drawing );
}