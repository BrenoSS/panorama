#include "FeatureExtractor.h"

void FeatureExtractor::Compute(cv::Mat& image, std::vector<cv::KeyPoint>& keypoints,
                  cv::Mat& descriptors) {
    
    cv::Mat roi40X40, roi8X8, blurredPatch;
    int pixelValue, tamanho,  m = 0, indiceX, indiceY;
    std::vector<std::vector<int> > descritor;
    /*
     * Percorrendo o vector de Keypoints, para gerar um vector que armazenará os pixels ao redor do ponto
     */
    for (int l = 0; l < keypoints.size(); l++) {
        cv::Point2f p = keypoints[l].pt;
        std::cout << "Ponto - x: " << p.x << " y:" << p.y << std::endl;
        if ((p.x > 19) && (p.y > 19) && (p.y < image.rows - 20) && (p.x < image.cols - 20)) {
            //Opencv tem o sistema de coordenadas invertido
            std::cout << "Ponto Incluído - x: " << p.x << " y:" << p.y << std::endl;
            
            indiceX = p.x - 19;
            indiceY = p.y - 19;
            roi40X40 = getRoi40X40(image, indiceX, indiceY);
            
            cv::imwrite("roi40X40.jpg", roi40X40);
            
            blurredPatch = getBlurredRoi(roi40X40);
            
            cv::imwrite("blurredPatch.jpg", blurredPatch);
            
            roi8X8 = getRoi8X8(blurredPatch);
            
            cv::imwrite("roi8X8.jpg", roi8X8);
            
            std::vector<int> patchTolinha;

            //std::cout << "Tamanho do vetor após a declaração: " << patchTolinha.size() << "\n" << std::endl;

            for (int i = 0; i < roi8X8.rows; i++) {
                for (int j = 0; j < roi8X8.cols; j++) {
                    pixelValue = (int) roi8X8.at<uchar>(i, j);
                    //std::cout << "Pixel Value: " << pixelValue << std::endl;
                    patchTolinha.push_back(pixelValue);
                    std::cout << "Nivel de cinza na imagem: " << (int) roi8X8.at<uchar>(i, j) << std::endl;
                    //std::cout << "Nivel de cinza no vector: " << patchTolinha[m] << std::endl;
                    m++;


                }
            }

            //std::cout << "Tamanho do vector após ser preenchido " << patchTolinha.size() << "\n" << std::endl;
            m = 0;
            descritor.push_back(patchTolinha);

            //patchTolinha.swap(vector<float>());


        }
    }
    tamanho = descritor.size();
    //std::cout << "\nNúmero de linhas da matriz de descritores " << tamanho << std::endl;
    //cv::Mat descritores1(tamanho, 25, src_gray1.type(), Scalar(0) );
    descriptors = cv::Mat(tamanho, 64, CV_32F, cv::Scalar(0) );
    
    for(int i= 0; i< tamanho; i++)
    {
        for(int j = 0; j < 64; j++){
            descriptors.at<float>(i,j) = (float)descritor[i][j];
        }
    }
}

cv::Mat FeatureExtractor::getRoi8X8(cv::Mat& roi40X40)
{
             
    cv::Mat img8X8 = cv::Mat(8, 8, roi40X40.type(), cv::Scalar(0));
    int m1 = 0, l1 = 0;
      
    for(int i1 = 0; i1 < 8; i1++)
    {
        for(int j1 = 0; j1 < 8; j1++)
        {
            img8X8.at<uchar>(i1,j1) = roi40X40.at<uchar>(l1,m1);
            //std::cout << "Valor do pixel: " << (int)img8X8.at<uchar>(i1,j1) <<std::endl;
            m1 = m1 + 5;
        }
        l1 = l1 + 5;
        m1 = 0;
    }

    return img8X8;
}

cv::Mat FeatureExtractor::getBlurredRoi(cv::Mat& patch)
{
    cv::Mat blurredPatch;
    cv::GaussianBlur(patch, blurredPatch, cv::Size(7,7), 0);
    
    return blurredPatch;
}

cv::Mat FeatureExtractor::getRoi40X40(cv::Mat& image, int x, int y)
{
    cv::Mat temp;
    int width = 39, height = 39;
    temp = image(cv::Rect(x, y, width, height));
    
    cv::imwrite("temp.jpg", temp);
    
   // temp = image(cv::RotatedRect(cv::Point2f(x,y), cv::Size2f(width, height), 30));
    //http://docs.opencv.org/trunk/db/dd6/classcv_1_1RotatedRect.html
//    cv::RotatedRect rRect = cv::RotatedRect(cv::Point2f(x,y), cv::Size2f(width, height), 30);
//    cv::Point2f vertices[4];
//    rRect.points(vertices);
//    for (int i = 0; i < 4; i++)
//        line(image, vertices[i], vertices[(i+1)%4], cv::Scalar(0,255,0));
//    
//    cv::imwrite("rotatedRect.jpg", image);
    
    //std::cout << "Tipo do temp: " << temp.type() << std::endl;
    
    
    cv::RotatedRect rect;
        // matrices we'll use
        cv::Mat M, rotated, cropped;
        // get angle and size from the bounding box
        float angle = 30.0;
        //Size rect_size = rect.size;
        // thanks to http://felix.abecassis.me/2011/10/opencv-rotation-deskewing/
//        if (rect.angle < -45.) {
//            angle += 90.0;
//            swap(rect_size.width, rect_size.height);
//        }
        
        //solucao tirada de:
        //http://answers.opencv.org/question/497/extract-a-rotatedrect-area/
        // get the rotation matrix
        M = cv::getRotationMatrix2D(cv::Point2f(x+19,y+19), angle, 1.0);
        // perform the affine transformation
        cv::warpAffine(image, rotated, M, image.size(), cv::INTER_CUBIC);
        // crop the resulting image
        cv::getRectSubPix(rotated, cv::Size(39,39), cv::Point2f(x+19,y+19), cropped);
        
        cv::imwrite("cropped.jpg",cropped);

    return temp;
}
  