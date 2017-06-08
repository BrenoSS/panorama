#include "FeatureExtractor.h"

void FeatureExtractor::Compute(cv::Mat& image, std::vector<cv::KeyPoint>& keypoints,
                  cv::Mat& descriptors) {
    
    cv::Mat temp;
    int width = 5, height = 5, pixelValue, tamanho,  m = 0;
    std::vector<std::vector<int> > descritor;
    /*
     * Percorrendo o vector de Keypoints, para gerar um vector que armazenará os pixels ao redor do ponto
     */
    for (int l = 0; l < keypoints.size(); l++) {
        cv::Point2f p = keypoints[l].pt;
        std::cout << "Ponto - x: " << p.x << " y:" << p.y << std::endl;
        if ((p.x > 5) && (p.y > 5) && (p.y < image.rows - 6) && (p.x < image.cols - 6)) {
            //Opencv tem o sistema de coordenadas invertido
            std::cout << "Ponto Incluído - x: " << p.x << " y:" << p.y << std::endl;
            temp = image(cv::Rect(p.x - 2, p.y - 2, width, height));

            std::cout << "Tipo do temp: " << temp.type() << std::endl;
            std::vector<int> patchTolinha;

            std::cout << "Tamanho do vetor após a declaração: " << patchTolinha.size() << "\n" << std::endl;

            for (int i = 0; i < temp.rows; i++) {
                for (int j = 0; j < temp.cols; j++) {
                    pixelValue = (int) temp.at<uchar>(i, j);
                    std::cout << "Pixel Value: " << pixelValue << std::endl;
                    patchTolinha.push_back(pixelValue);
                    std::cout << "Nivel de cinza na imagem: " << (int) temp.at<uchar>(i, j) << std::endl;
                    std::cout << "Nivel de cinza no vector: " << patchTolinha[m] << std::endl;
                    m++;


                }
            }

            std::cout << "Tamanho do vector após ser preenchido " << patchTolinha.size() << "\n" << std::endl;
            m = 0;
            descritor.push_back(patchTolinha);

            //patchTolinha.swap(vector<float>());


        }
    }
    tamanho = descritor.size();
    std::cout << "\nNúmero de linhas da matriz de descritores " << tamanho << std::endl;
    //cv::Mat descritores1(tamanho, 25, src_gray1.type(), Scalar(0) );
    descriptors = cv::Mat(tamanho, 25, CV_32F, cv::Scalar(0) );
    
    for(int i= 0; i< tamanho; i++)
    {
        for(int j = 0; j < 25; j++){
            descriptors.at<float>(i,j) = (float)descritor[i][j];
        }
    }
}
  