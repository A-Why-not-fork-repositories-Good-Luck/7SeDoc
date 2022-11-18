#include "edgefilter.h"
#include "imageviewer-qt5.h"
#include <cmath>
#include<iostream>

constexpr int new_clipped_value (int pixel_value){
    if(pixel_value > 255){
        pixel_value = 255;
    }
    else if(pixel_value < 0){
        pixel_value = 0;
    }
    return pixel_value;
}

namespace cg2 {

/**
     * @brief doEdgeFilter
     *      calculate edge filter like sobel or prewitt with the help of separability.
     *      returns either the X, the Y or the entire Gradient.
     * @param image
     *      input image
     * @param derivative_filter
     *      1x3 1D filter array for derivative part
     * @param smoothing_filter
     *      1x3 1D filter array for the smoothing part
     * @param desired_image
     *      code for the desired image, that has to be shown:
     *      - desired_image = 1 -> show only X Gradient
     *      - desired_image = 2 -> show only Y Gradient
     *      - desired_image = 0 -> show Gradient: |∇I|
     * @return new Image to show in GUI
     */
QImage* doEdgeFilter(QImage * image, int*& derivative_filter, int*& smoothing_filter, int desired_image){
    logFile << "EdgeFilter applied:" << std::endl;
    logFile << "---derivative_filter: " << derivative_filter[0] << "|"<< derivative_filter[1] << "|" << derivative_filter[2]  << std::endl;
    logFile << "---smoothing_filter: " << smoothing_filter[0] << "|"<< smoothing_filter[1] << "|" << smoothing_filter[2]  << std::endl;
    logFile << "---desired_image: " << desired_image << std::endl;

    switch (desired_image) {
    case 1:
        int** smoothing_X;
        smoothing_X = new int*[3];
        smoothing_X[0] = new int[1];
        smoothing_X[1] = new int[1];
        smoothing_X[2] = new int[1];
        smoothing_X[0][0] = smoothing_filter[0];
        smoothing_X[1][0] = smoothing_filter[1];
        smoothing_X[2][0] = smoothing_filter[2];
        image = filterImage(image, smoothing_X, 1, 3, 2);

        int** derivative_X;
        derivative_X = new int*[1];
        derivative_X[0] = new int[3];
        derivative_X[0][0] = derivative_filter[0];
        derivative_X[0][1] = derivative_filter[1];
        derivative_X[0][2] = derivative_filter[2];
        image = filterImage(image, derivative_X, 3, 1, 2);

        logFile << "---Show only X Gradient" << std::endl;
        break;
    case 2:
        int** smoothing_Y;
        smoothing_Y = new int*[1];
        smoothing_Y[0] = new int[3];
        smoothing_Y[0][0] = smoothing_filter[0];
        smoothing_Y[0][1] = smoothing_filter[1];
        smoothing_Y[0][2] = smoothing_filter[2];
        image = filterImage(image, smoothing_Y, 3, 1, 2);

        int** derivative_Y;
        derivative_Y = new int*[3];
        derivative_Y[0] = new int[1];
        derivative_Y[1] = new int[1];
        derivative_Y[2] = new int[1];
        derivative_Y[0][0] = derivative_filter[0];
        derivative_Y[1][0] = derivative_filter[1];
        derivative_Y[2][0] = derivative_filter[2];
        image = filterImage(image, derivative_Y, 1, 3, 2);

        logFile << "---Show only Y Gradient" << std::endl;
        break;
    case 0:
        QImage* image_X = new QImage(*image);
        QImage* image_Y = new QImage(*image);

        image_X = doEdgeFilter(image_X, derivative_filter, smoothing_filter, 1);
        image_Y = doEdgeFilter(image_Y, derivative_filter, smoothing_filter, 2);

        for (int u = 0; u < image->width(); u++)
        {
            for (int v = 0; v < image->height(); v++)
            {
                QRgb x = image_X->pixel(u, v);
                QRgb y = image_Y->pixel(u, v);
                double I_x = qRed(x)-127;
                double I_y = qRed(y)-127; // Hier berschieben?
                int gradient = (int)round(sqrt( (I_x * I_x) + (I_y * I_y) ));
                gradient = new_clipped_value(gradient+127); // Und hier auch wieder zurück?
                image->setPixel(u, v, qRgb(gradient, gradient, gradient));
            }
        }

        logFile << "---Show Gradient: |∇I|" << std::endl;
        break;
    }
    return image;
}


/**
     * @brief doLaplaceFilter
     *      calculate the laplace edge filter
     * @param image
     *      input image
     * @param laplace_filter
     *      3x3 2D laplace filter matrix
     * @return new Image to show in GUI
     */
QImage* doLaplaceFilter(QImage * image, int**& laplace_filter){
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            logFile <<  + laplace_filter[i][j] << " ";
        }
        logFile << std::endl;
    }
    image = filterImage(image, laplace_filter,3 , 3, 0);

    logFile << "Do Laplace: " << std::endl;
    return image;
}

/**
     * @brief doCanny
     *      calculate the Canny Edge Detector
     * @param img
     *      input image
     * @param sigma
     *      Canny Edge Detector parameter
     *      sigma for gauss smoothing
     * @param tHi
     *      Canny Edge Detector parameter
     *      Threshhold tHi
     * @param tLo
     *      Canny Edge Detector parameter
     *      Threshhold tLo
     * @return new Image to show in GUI
     */
QImage* doCanny(QImage * img, double sigma, int tHi, int tLo){
    logFile << "-----------\nBeginne Canny Algorithmus:\nSigma: " + std::to_string(sigma) + "\ntHi: " + std::to_string(tHi) + "\ntLo: " + std::to_string(tLo) << std::endl;

    return img;

}


/**
     * @brief doUSM
     *      calculate the Unsharp Masking
     * @param image
     *      input image
     * @param sharpening_value
     *      Unsharp Masking parameter
     *      sharpening parameter (a)
     * @param sigma
     *      Unsharp Masking parameter
     *      sigma for gauss smoothing
     * @param tc
     *      Unsharp Masking parameter
     *      |∇I| must greater then threshhold tc
     * @return new Image to show in GUI
     */
QImage* doUSM(QImage * image, double sharpening_value, double sigma, int tc){
    QImage* image_gradient = new QImage(*image);
    //image_gradient = doEdgeFilter(image_gradient, )

    logFile << "Unsharp Masking ausgeführt mit Schärfungsgrad " << sharpening_value << ", Sigma " << sigma << " und tc " << tc << std::endl;
    return image;
}


}
