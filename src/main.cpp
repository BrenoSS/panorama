/**
 * @function cornerHarris_Demo.cpp
 * @brief Demo code for detecting corners using Harris-Stephens method
 * @author OpenCV team
 */
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <vector>
#include "FeatureExtractor.h"


enum{
  PROGRAM_NAME=0,
  IM1,
  IM2,
  NARGS
};


void showUsage( char** );
void cornerHarris( cv::Mat&, std::vector<cv::KeyPoint>& );

/**
 * @function main
 */
int main( int argc, char** argv )
{

//  if( argc != NARGS )
//  {
//    showUsage( argv );
//  }
  /// Load source image and convert it to gray
    

  cv::Mat src1, src2, src_gray1, src_gray2, IM1_descriptors, IM2_descriptors; 
  std::vector<cv::KeyPoint> keypoints_1, keypoints_2;
  FeatureExtractor *descritor = new FeatureExtractor();
          
  src1 = cv::imread( "/home/breno/VC/carmel-00.png", cv::IMREAD_COLOR );
  cv::cvtColor( src1, src_gray1, cv::COLOR_BGR2GRAY );

  std::cout << "Tipo da imagem em tons de cinza: " << src_gray1.type() << std::endl;

  src2 = cv::imread( "/home/breno/VC/carmel-01.png", cv::IMREAD_COLOR );
  cv::cvtColor( src2, src_gray2, cv::COLOR_BGR2GRAY );

  cornerHarris( src_gray1, keypoints_1 );
  cornerHarris( src_gray2, keypoints_2 );
  
  descritor->Compute(src_gray1, keypoints_1, IM1_descriptors);
  descritor->Compute(src_gray2, keypoints_2, IM2_descriptors);
  
  std::cout << "Descritores 1 = " << std::endl << " " << IM1_descriptors << std::endl << std::endl;
  std::cout << "\n Tipo da Matriz: "<< IM1_descriptors.type() << "\n\n"<< std::endl;
  std::cout << "Descritores 2 = " << std::endl << " " << IM2_descriptors << std::endl << std::endl;
  

  return(0);
}

void showUsage( char **argv )
{
  std::cout << "\n\n";
  std::cout << "Usage: " << argv[PROGRAM_NAME] << 
               " Imagem1 Imagem2" <<std::endl;
  exit( EXIT_FAILURE );
}

/**
 * @function cornerHarris_demo
 * @brief Executes the corner detection and draw a circle around the possible corners
 */
void cornerHarris( cv::Mat& src, std::vector<cv::KeyPoint>& keypoints )
{
  int thresh = 200;
  cv::Mat dst, dst_norm;
  dst = cv::Mat::zeros( src.size(), CV_32FC1 );

  /// Detector parameters
  int blockSize = 2;
  int apertureSize = 3;
  double k = 0.04;

  /// Detecting corners
  cv::cornerHarris( src, dst, blockSize, apertureSize, k, cv::BORDER_DEFAULT );

  /// Normalizing
  cv::normalize( dst, dst_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat() );

  /// Drawing a circle around corners
  for( int j = 0; j < dst_norm.rows ; j++ )
     { for( int i = 0; i < dst_norm.cols; i++ )
          {
            if( (int) dst_norm.at<float>(j,i) > thresh )
              {
                cv::Point2f pt(i, j);
               keypoints.push_back(cv::KeyPoint(pt,40.0,-1,0,0,-1));
               cv::circle( src, cv::Point( i, j ), 5,  cv::Scalar(0), 2, 8, 0 );
              }
          }
     }
  
  imwrite("corners.png", src); 

  
}
