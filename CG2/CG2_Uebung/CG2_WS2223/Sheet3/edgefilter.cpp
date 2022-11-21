#include "edgefilter.h"
#include "imageviewer-qt5.h"
#include <cmath>
#include<iostream>
#include <algorithm>

constexpr int new_clipped_value (int pixel_value){
    if(pixel_value > 255){
        pixel_value = 255;
    }
    else if(pixel_value < 0){
        pixel_value = 0;
    }
    return pixel_value;
}

constexpr QRgb RgbToYCbCr(QRgb pixel_Rgb) {
    int r = qRed(pixel_Rgb);
    int g = qGreen(pixel_Rgb);
    int b = qBlue(pixel_Rgb);
    int Y = round(0.299*(double)r + 0.587*(double)g + 0.114*(double)b);
    int Cb = round(-0.169*(double)r - 0.331*(double)g + 0.5*(double)b + 127.0);
    int Cr = round(0.5*(double)r - 0.419*(double)g - 0.081*(double)b + 127.0);

    Y = new_clipped_value(Y);
    Cb = new_clipped_value(Cb);
    Cr = new_clipped_value(Cr);

    QRgb pixel_YCbCr = qRgb(Y, Cb, Cr);

    return pixel_YCbCr;
}

constexpr QRgb YCbCrToRgb(QRgb pixel_YCbCr) {
    int Y = qRed(pixel_YCbCr);
    int Cb = qGreen(pixel_YCbCr);
    int Cr = qBlue(pixel_YCbCr);

    int r = round((double)Y + 1.4*((double)Cr-127.0));
    int g = round((double)Y - 0.343*((double)Cb-127.0) - 0.711*((double)Cr-127.0));
    int b = round((double)Y + 1.765*((double)Cb-127.0));

    r = new_clipped_value(r);
    g = new_clipped_value(g);
    b = new_clipped_value(b);

    QRgb pixel_Rgb = qRgb(r, g, b);

    return pixel_Rgb;
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

        //logFile << "---Show only X Gradient" << std::endl;
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

        //logFile << "---Show only Y Gradient" << std::endl;
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
                double I_x = qRed(x)-127.0;
                double I_y = qRed(y)-127.0; // Hier verschieben?
                int gradient = (int)round(sqrt( (I_x * I_x) + (I_y * I_y) ));
                gradient = new_clipped_value(gradient);
                image->setPixel(u, v, qRgb(gradient, gradient, gradient));
            }
        }

        //logFile << "---Show Gradient: |∇I|" << std::endl;
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
    image = filterImage(image, laplace_filter,3 ,3 , 2);

    logFile << "Do Laplace: " << std::endl;
    return image;
}

int getOrientationSector(double dx, double dy) {
    double temp = dx;
    dx = cos(M_PI/8)*dx - sin(M_PI/8)*dy;
    dy = sin(M_PI/8)*temp + cos(M_PI/8)*dy;

    if (dy < 0)
    {
        dx *= -1;
        dy *= -1;
    }

    int s;
    if (dx >= 0 && dx >= dy)
    {
        return 0;
    }
    if (dx >= 0 && dx < dy)
    {
        return 1;
    }
    if (dx < 0 && -dx < dy)
    {
        return 2;
    }
    if (dx < 0 && -dx >= dy)
    {
        return 3;
    }
}

bool isLocalMax(QImage* image_gradient, int u, int v, int s, int tLo) {
    int mC = qRed(image_gradient->pixel(u, v));
    if (mC < tLo)
    {
        return false;
    } else {
        int mL, mR;
        switch (s) {
        case 0:
            mL = qRed(image_gradient->pixel(u-1, v));
            mR = qRed(image_gradient->pixel(u+1, v));
            break;
        case 1:
            mL = qRed(image_gradient->pixel(u-1, v-1));
            mR = qRed(image_gradient->pixel(u+1, v+1));
            break;
        case 2:
            mL = qRed(image_gradient->pixel(u, v-1));
            mR = qRed(image_gradient->pixel(u, v+1));
            break;
        case 3:
            mL = qRed(image_gradient->pixel(u-1, v+1));
            mR = qRed(image_gradient->pixel(u+1, v-1));
            break;
        }
        return ((mL <= mR) && (mC >= mR));
    }
}

void traceAndThreshold(QImage* image_Gradient_Max, QImage*& image_bin, int u0, int v0, int tLo) {
    image_bin->setPixel(u0, v0, qRgb(255,255,255));

    int uL = std::max(u0-1, 0);
    int uR = std::min(u0+1, image_Gradient_Max->width()-1);
    int vT = std::max(v0-1, 0);
    int vB = std::min(v0+1, image_Gradient_Max->height()-1);

    for (int u = uL; u <= uR; u++)
    {
        for (int v = vT; v <= vB; v++)
        {
            if ((qRed(image_Gradient_Max->pixel(u,v)) >= tLo) && (qRed(image_bin->pixel(u,v)) == 0))
            {
                traceAndThreshold(image_Gradient_Max, image_bin, u, v, tLo);
            }
        }
    }
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
QImage* doCanny(QImage * image, double sigma, int tHi, int tLo){
    logFile << "-----------\nBeginne Canny Algorithmus:\nSigma: " + std::to_string(sigma) + "\ntHi: " + std::to_string(tHi) + "\ntLo: " + std::to_string(tLo) << std::endl;
    logFile << std::max(3,10) << std::endl;
    image = filterGauss2D(image, sigma, 2);

    QImage* image_x = new QImage(*image);
    QImage* image_y = new QImage(*image);
    QImage* image_Gradient = new QImage(*image);
    QImage* image_Gradient_max = new QImage(*image);
    QImage* image_bin = new QImage(*image);
    int* derivative_filter = new int[3];
    derivative_filter[0] = -1;
    derivative_filter[1] = 0;
    derivative_filter[2] = 1;
    int* smoothing_filter = new int[3];
    smoothing_filter[0] = 1;
    smoothing_filter[1] = 1;
    smoothing_filter[2] = 1;
    image_Gradient = doEdgeFilter(image_Gradient, derivative_filter, smoothing_filter, 0); // Schwarz/Weiß -> Alle 3 Kanäle gleich
    image_x = doEdgeFilter(image_x, derivative_filter, smoothing_filter, 1); // Schwarz/Weiß -> Alle 3 Kanäle gleich
    image_y = doEdgeFilter(image_y, derivative_filter, smoothing_filter, 2); // Schwarz/Weiß -> Alle 3 Kanäle gleich

    for (int u = 0; u < image->width(); u++)
    {
        for (int v = 0; v < image->height(); v++)
        {
            image_Gradient_max->setPixel(u, v, qRgb(0,0,0));
            image_bin->setPixel(u, v, qRgb(0,0,0));
        }
    }

    for (int u = 1; u < image->width()-1; u++)
    {
        for (int v = 1; v < image->height()-1; v++)
        {
            int dx = qRed(image_x->pixel(u,v));
            int dy = qRed(image_y->pixel(u,v));
            int s = getOrientationSector(dx, dy);
            if (isLocalMax(image_Gradient, u, v, s, tLo))
            {
                image_Gradient_max->setPixel(u,v, image_Gradient->pixel(u,v));
            }
        }
    }

    for (int u = 1; u < image->width()-1; u++)
    {
        for (int v = 1; v < image->height()-1; v++)
        {
            if (qRed(image_Gradient_max->pixel(u,v)) >= tHi && qRed(image_bin->pixel(u,v)) == 0)
            {
                traceAndThreshold(image_Gradient_max, image_bin, u, v, tLo);
            }
        }
    }

    return image_bin;
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
    int* derivative_filter = new int[3];
    derivative_filter[0] = -1;
    derivative_filter[1] = 0;
    derivative_filter[2] = 1;
    int* smoothing_filter = new int[3];
    smoothing_filter[0] = 1;
    smoothing_filter[1] = 2;
    smoothing_filter[2] = 1;
    image_gradient = doEdgeFilter(image_gradient, derivative_filter, smoothing_filter, 0); // Schwarz/Weiß Bild, alle drei Kanäle selber Wert

    QImage* image_gaus = new QImage(*image); //Gefiltertes Bild mit Gaus
    image_gaus = filterGauss2D(image_gaus, sigma, 2);

    for (int i = 0; i < image->width(); i++)
    {
        for (int j = 0; j < image->height(); j++)
        {
            QRgb pixel_gradient = image_gradient->pixel(i,j);
            if (qRed(pixel_gradient) >= tc)
            {
                QRgb pixel_original = RgbToYCbCr(image->pixel(i,j)); // Pixel aus unveränerten Bild
                int I_orig = qRed(pixel_original); // Intesität unverändert (original)
                int Cb = qGreen(pixel_original); // Farbkanal merken
                int Cr = qBlue(pixel_original); // Farbkanal merken
                QRgb pixel_gaus = RgbToYCbCr(image_gaus->pixel(i,j)); // Pixel aus geglätten Bild
                int m = I_orig - qRed(pixel_gaus); // Maske berechnen M = I - I*H

                int Y = (int)round((double)I_orig + ((double)sharpening_value * (double)m)); // Berechne neue Intensiät I' = I + aM
                Y = new_clipped_value(Y);
                image->setPixel(i, j, YCbCrToRgb(qRgb(Y, Cb, Cr)));
                //image->setPixel(i, j, qRgb(Y, Y, Y));
            }
        }
    }
    logFile << "Unsharp Masking ausgeführt mit Schärfungsgrad " << sharpening_value << ", Sigma " << sigma << " und tc " << tc << std::endl;
    return image;
}


}
