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

bool cmpfun(cv::DMatch a, cv::DMatch b) { return a.distance < b.distance; }
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
  double min_dist = 100, max_dist = 0;
  FeatureExtractor *descritor = new FeatureExtractor();
          
  //src1 = cv::imread( "/home/breno/VC/goldengate-00.png", cv::IMREAD_COLOR );
  src1 = cv::imread( "/home/breno/VC/Image1.jpg", cv::IMREAD_COLOR );
  cv::cvtColor( src1, src_gray1, cv::COLOR_BGR2GRAY );

  std::cout << "Tipo da imagem em tons de cinza: " << src_gray1.type() << std::endl;

  //src2 = cv::imread( "/home/breno/VC/goldengate-01.png", cv::IMREAD_COLOR );
  src2 = cv::imread( "/home/breno/VC/Image2.jpg", cv::IMREAD_COLOR );
  cv::cvtColor( src2, src_gray2, cv::COLOR_BGR2GRAY );
  
  cv::Mat src_gray1_expanded(src_gray1.rows, (src_gray1.cols+src_gray2.cols), src_gray1.type());
  cv::Mat dst_roi = src_gray1_expanded(cv::Rect(0, 0, src_gray1.cols, src_gray1.rows));
  src_gray1.copyTo(dst_roi);
  //src_gray1.copyTo(src_gray1_expanded(Rect(0, 0, src_gray1.rows, src_gray1.cols)));
  
  imwrite("src_gray1_expanded.jpg",src_gray1_expanded);
  
  cv::Mat src_gray2_expanded(src_gray2.rows, (src_gray1.cols+src_gray2.cols), src_gray2.type());
  cv::Mat dst_roi2 = src_gray2_expanded(cv::Rect(src_gray1.cols, 0, src_gray2.cols, src_gray2.rows));
  src_gray2.copyTo(dst_roi2);
  //src_gray1.copyTo(src_gray1_expanded(Rect(0, 0, src_gray1.rows, src_gray1.cols)));
  
  cv::imwrite("src_gray2_expanded.jpg",src_gray2_expanded);

  cornerHarris( src_gray1_expanded, keypoints_1 );
  cornerHarris( src_gray2_expanded, keypoints_2 );
  
  descritor->Compute(src_gray1_expanded, keypoints_1, IM1_descriptors);
  descritor->Compute(src_gray2_expanded, keypoints_2, IM2_descriptors);
  
  std::cout << "Descritores 1 = " << std::endl << " " << IM1_descriptors << std::endl << std::endl;
  std::cout << "\n Tipo da Matriz: "<< IM1_descriptors.type() << "\n\n"<< std::endl;
  std::cout << "Descritores 2 = " << std::endl << " " << IM2_descriptors << std::endl << std::endl;
  
  cv::FlannBasedMatcher matcher;
  std::vector< cv::DMatch > matches;
  matcher.match( IM1_descriptors, IM2_descriptors, matches );
  
   for( int i = 0; i < matches.size(); i++ )
   { 
    double dist = matches[i].distance;
    if( dist < min_dist ) min_dist = dist;
    if( dist > max_dist ) max_dist = dist;
   }
  
  std::cout << "Min Distance: " << min_dist << std::endl;
  std::cout << "Max Distance: " << max_dist << std::endl;
  
  std::vector< cv::DMatch > good_matches, vector_matches;

//  for( int i = 0; i < IM1_descriptors.rows; i++ )
//  { if( matches[i].distance <= cv::max(2*min_dist, 0.02) )
//    { vector_matches.push_back( matches[i]); }
//  }
  
  std::sort(matches.begin(), matches.end(), cmpfun);
  
  std::vector<cv::Point2f> match1, match2;
  for (int i = 0; i < 3; ++i)
  {
    cv::Point2f pt1 = keypoints_1[matches[i].queryIdx].pt;
    cv::Point2f pt2 = keypoints_2[matches[i].trainIdx].pt;
    match1.push_back(pt1);
    match2.push_back(pt2);
    good_matches.push_back(matches[i]);
    std::cout << i << " pt1: " << pt1.x << ", " << pt1.y << " - pt2: " << pt2.x << ", " << pt2.y << std::endl;
    //printf("%3d pt1: (%.2f, %.2f) pt2: (%.2f, %.2f)\n", i, pt1.x, pt1.y, pt2.x, pt2.y);
  }
  
  std::cout<< "Número de Good_matches: " << good_matches.size() <<std::endl;
    cv::Mat fun = cv::getAffineTransform(match1, match2);
    
    std::cout << "Matriz de transformacao = " << std::endl << " " << fun << std::endl << std::endl;
    
//    
//    for (int i = 0; i < fun.rows; ++i)
//    {
//        for (int j = 0; j < fun.cols; j++)
//        {
//            std::cout << fun.at<double>(i,j) << std::endl;
//            //printf("%.2f ", fun.at<float>(i,j));
//        }
//        //printf("\n");
//    }

  
  
  
 
  
  std::cout << "Número de Good_Matches " << (int)good_matches.size() << std::endl;

  for( int i = 0; i < (int)good_matches.size(); i++ )
  { 
    std::cout << "Good Match " << i << ": " << "Keypoint1: " << good_matches[i].queryIdx << " -- Keypoint2: " << good_matches[i].trainIdx << std::endl;
          
  }
  
     cv::Mat img_matches;
  cv::drawMatches( src_gray1_expanded, keypoints_1, src_gray2_expanded, keypoints_2,
               matches, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1),
               std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

  //-- Show detected matches
  cv::imwrite("GoodMatches.jpg", img_matches);
  
  //cv::Mat warp_dst = cv::Mat::zeros( (src_gray1.rows + src_gray2.rows), (src_gray1.cols + src_gray2.cols), src_gray1.type() );
  
  cv::Mat warp_dst = cv::Mat::zeros( src_gray1.rows, (src_gray1.cols+src_gray2.cols), src_gray1.type() );
  
  cv::warpAffine( src_gray1_expanded, warp_dst, fun, warp_dst.size() );
  
  cv::imwrite("Src1_warped.jpg", warp_dst);
  
  for(int i = 0; i < warp_dst.rows; i++)
  {
      for(int j = 0; j < warp_dst.cols; j++)
      {
          if((int)warp_dst.at<uchar>(i,j) !=0)
          {
              src_gray2_expanded.at<uchar>(i,j) = warp_dst.at<uchar>(i,j);
          }
      }
  }
  
  cv::imwrite("panorama.jpg", src_gray2_expanded);
  
//  Mat roiImgResult_Left = warp_dst(Rect(0,0,src_gray1.cols,src_gray1.rows));
//  
//  src_gray1.copyTo(roiImgResult_Left);
  
  std::cout << "Linhas da Imagem 1: " << src_gray1.rows << std::endl << "Colunas Imagem 1: " << src_gray1.cols <<std::endl;
  std::cout << "Linhas da warp_dst: " << warp_dst.rows << std::endl << "Colunas warp_dst : " << warp_dst.cols <<std::endl;
  
    cv::Size sz1 = src_gray1.size();
    cv::Size sz2 = warp_dst.size();
    
    std::cout << "height da Imagem 1: " << sz1.height << std::endl << "width Imagem 1: " << sz1.width <<std::endl;
    std::cout << "height da Imagem 2: " << sz2.height << std::endl << "width Imagem 2: " << sz2.width <<std::endl;
//    Mat im3(sz1.height, (sz1.width+sz2.width), src_gray1.type());
//    std::cout << "Linhas da im3: " << im3.rows << std::endl << "Colunas im3: " << im3.cols <<std::endl;
//    src_gray1.copyTo(im3(Rect(0, 0, sz1.height, sz1.width)));
//    warp_dst.copyTo(im3(Rect(sz1.width, 0, sz2.width, sz2.height)));
    
//    Mat newImage;
//hconcat(src_gray1, warp_dst, newImage);
    
//    Mat left(im3, Rect(0, 0, src_gray1.rows, src_gray1.cols));
//    src_gray1.copyTo(left);
//    Mat right(im3, Rect(src_gray1.rows, 0, warp_dst.rows, warp_dst.cols));
//    warp_dst.copyTo(right);
  

  
//  cv::imwrite("ImagensJuntas.jpg", newImage);
  

  
  
  

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
               //cv::circle( src, cv::Point( i, j ), 5,  cv::Scalar(0), 2, 8, 0 );
              }
          }
     }
  
  imwrite("corners.png", src); 

  
}
