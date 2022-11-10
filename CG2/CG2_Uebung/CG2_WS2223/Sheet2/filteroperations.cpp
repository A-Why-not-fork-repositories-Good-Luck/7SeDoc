#include "filteroperations.h"
#include "imageviewer-qt5.h"

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
     * @brief filterImage
     *      calculate the 2D filter over the image
     *      handle border treatment as desired
     * @param image
     *      input image
     * @param filter
     *      filter matrix with filter coefficients
     *      can reach up to 15x15
     * @param filter_width
     *      filter matrix width in range [0,15]
     * @param filter_height
     *      filter matrix height in range [0,15]
     * @param border_treatment
     *      0: Zentralbereich
     *      1: Zero Padding
     *      2: Konstante Randbedingung
     *      3: Gespiegelte Randbedingung
     * @return new Image to show in GUI
     */
    QImage* filterImage(QImage * image, int**& filter, int filter_width, int filter_height, int border_treatment) {

        logFile << "Filter read:" << std::endl;
        for(int i = 0; i < filter_height; i++ ){
            for(int j = 0; j < filter_width; j++ ){
                logFile << filter[i][j];
                if(j < (filter_width-1)){
                    logFile << " | ";
                }
            }
            logFile << std::endl;
        }



        logFile << "filter applied:" << std::endl << "---border treatment: ";
        switch (border_treatment) {
            case 0:
                logFile << "Zentralbereich" << std::endl;
                break;
            case 1:
                logFile << "Zero Padding" << std::endl;
                break;
            case 2:
                logFile << "Konstante Randbedingung" << std::endl;
                break;
            case 3:
                logFile << "Gespiegelte Randbedingung" << std::endl;
                break;
        }
        logFile << "---filter width: " << filter_width << std::endl;
        logFile << "---filter height: " << filter_height << std::endl;
        return image;
    }

    /**
     * @brief filterGauss2D
     *      calculate the 2D Gauss filter algorithm via two separate 1D operations,
     *      handle border treatment as desired
     * @param image
     *      input image
     * @param gauss_sigma
     *      sigma for gauss kernel
     * @param border_treatment
     *      0: Zentralbereich
     *      1: Zero Padding
     *      2: Konstante Randbedingung
     *      3: Gespiegelte Randbedingung
     * @return new Image to show in GUI
     */
    QImage* filterGauss2D(QImage * image, double gauss_sigma, int border_treatment){


        logFile << "2D Gauss-Filter angewendet mit σ: " << gauss_sigma;
        logFile <<  " ---border treatment: ";
        switch (border_treatment) {
            case 0:
                logFile << "Zentralbereich" << std::endl;
                break;
            case 1:
                logFile << "Zero Padding" << std::endl;
                break;
            case 2:
                logFile << "Konstante Randbedingung" << std::endl;
                break;
            case 3:
                logFile << "Gespiegelte Randbedingung" << std::endl;
                break;
        }
        return image;
    }
}

