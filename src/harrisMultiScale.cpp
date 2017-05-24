#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

enum{
	PROGRAM_NAME=0,
	IM1,
	NARGS
};

struct {

	int x;
	int y;
	int l; //nivel da piramide
	//orientacao (eh um vetor)
	float harrisValue;//valor da resposta do detector


} corner;

/// Global variables
Mat src, src_gray;
int thresh = 20;
int max_thresh = 255;

const char* source_window = "Source image";
const char* corners_window = "Corners detected";



/// Function header
void showUsage( char** );
void cornerHarris( int, void* );

/**
 * @function main
 */
int main( int argc, char** argv )
{
  /// Load source image and convert it to gray
  if( argc != NARGS )
  {
    showUsage( argv );
  }



  src = imread( argv[IM1], IMREAD_COLOR );     //as imagens de panorama se encontram em tons de cinza. A linha abaixo pode não ser necessária
  cvtColor( src, src_gray, COLOR_BGR2GRAY );

  /// Create a window and a trackbar
  //namedWindow( source_window, WINDOW_AUTOSIZE );
  //createTrackbar( "Threshold: ", source_window, &thresh, max_thresh, cornerHarris_demo );
  //imshow( source_window, src );

  cornerHarris( 0, 0 );

  //waitKey(0);
  return(0);
}


void showUsage( char **argv )
{
	std::cout << "\n\n";
	std::cout << "Usage: " << argv[PROGRAM_NAME] << 
	             " Imagem1\n";
	exit( EXIT_FAILURE );
}


void cornerHarris( int, void* )
{

  Mat dst, dst_norm, dst_norm_scaled, tmp, dst_corner, dst_circle;
  dst_corner = Mat::zeros( src.size(), CV_32FC1 );

  /// Detector parameters
  int blockSize = 2;
  int apertureSize = 3;
  double k = 0.04;

  int i = 0;
  char str[15];

  tmp = src_gray;
  dst = tmp;
	

  /// Loop
  while( true )
  {

    if( tmp.rows == 64  || tmp.rows < 64)
      { break; }
      sprintf(str, "%d", i);
      strcat(str, ".jpg");
      pyrDown( tmp, dst, Size( tmp.cols/2, tmp.rows/2 ) );

  /// Detecting corners
     cornerHarris( dst, dst_corner, blockSize, apertureSize, k, BORDER_DEFAULT );

  /// Normalizing
    //normalize( dst_corner, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
    //convertScaleAbs( dst_norm, dst_norm_scaled );

    dst_circle = dst;

  /// Drawing a circle around corners
    for( int j = 0; j < dst_corner.rows ; j++ )
       { for( int i = 0; i < dst_corner.cols; i++ )
            {
              if( (int) dst_corner.at<float>(j,i) > thresh )
                {
                 circle( dst_circle, Point( i, j ), 5,  Scalar(0), 2, 8, 0 );
                }
            }
       }
     

    imwrite( str, dst_circle );
    tmp = dst;
    i++;
  }


  /// Showing the result
  //namedWindow( corners_window, WINDOW_AUTOSIZE );
  //imshow( corners_window, dst_norm_scaled );
}
