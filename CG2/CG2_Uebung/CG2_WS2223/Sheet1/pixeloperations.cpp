#include "pixeloperations.h"
#include "imageviewer-qt5.h"


namespace cg2 {

constexpr int new_clippedRGB_value (int pixel_value){
    if(pixel_value > 255){
        pixel_value = 255;
    }
    else if(pixel_value < 0){
        pixel_value = 0;
    }
    return pixel_value;
}

/**
     * @brief calcImageCharacteristics
     *      calculation of the histogram, average value and variance of the image
     *      no return values, just set the references to the correct values
     * @param image
     *      working image
     * @param histogram_ref
     *      Reference to "inline double* histogramm = new double[256]"
     *      doubles allowed from [0-100]
     *      calculate the histogram and fill the histogramm:
     *      the histogramm[0] is corresponding to the luminance value 0 and so on
     *      NOTE!: the histogram must be scaled (linear or logarithmic) so that the
     *          highest value is 100!
     *      (reason: histogram image is 256x100 pixel)
     * @param variance_ref
     *      calculate and set the variance of the image to the variance_ref
     * @param average_ref
     *      calculate and set the average of the image to the average_ref
     * @param linear_scaling
     *      boolean, if true -> scale the histogram linear
     *               if false -> scale the histogram logarithmic
     */
void calcImageCharacteristics(QImage * image, double*& histogram_ref, int& variance_ref, int& average_ref, const bool linear_scaling){
    int pixel_count = (image->height()*image->width()); //Anzahl Pixel im Bild

    // Berechnung der Mittleren Helligkeit
    // In Schleife addiere Helligkeiten auf
    for (int i = 0; i < image->width(); i++) {
        for (int j = 0; j < image->height(); j++) {
            QRgb pixel = image->pixel(i, j);
            //image->setPixel(i,j,qRgb(qGray(pixel),qGray(pixel),qGray(pixel)));
            average_ref += qGray(pixel); //qGray wandelt in Graustufe um / Intensität
        }
    }
    average_ref /= pixel_count; //Durchschnitt ziehen

    //Berechne Varianz der Helligkeit/Intensität
    for (int i = 0; i < image->width(); i++) {
        for (int j = 0; j < image->height(); j++) {
            QRgb pixel = image->pixel(i, j);

            variance_ref += (qGray(pixel)-average_ref) * (qGray(pixel)-average_ref);
            histogram_ref[qGray(pixel)] ++;
        }
    }
    variance_ref /= pixel_count;

    //Skalierung linear oder logarithmisch
    if (linear_scaling) {
        double linear_scale = 0;
        for (int i = 0; i < 256; i++) {
            if (linear_scale < histogram_ref[i]) {
                linear_scale = histogram_ref[i];
            }
        }
        linear_scale = 100/linear_scale;
        for (int i = 0; i < 256; i++) {
            histogram_ref[i] *= linear_scale;
        }
    } else {
        double log_scale = 0;
        for (int i = 0; i < 256; i++) {
            histogram_ref[i] = log(histogram_ref[i]);
            if (log_scale < histogram_ref[i]) {
                log_scale = histogram_ref[i];
            }
        }
        log_scale = 100/log_scale;
        for (int i = 0; i < 256; i++) {
            histogram_ref[i] *= log_scale;
        }
    }
    logFile << "Image characteristics calculated:" << std::endl << "--- Average: " << average_ref << " ; Variance: " << variance_ref << std::endl << "--- Histogram calculated: " << "linear scaling = " << linear_scaling << std::endl;
}

/**
     * @brief changeImageDynamic
     *      calculate an image with the desired resolution (given bit depth -> dynamic value)
     *      and return the image pointer
     * @param image
     *      input image
     * @param newDynamicValue
     *      bit depth value for resolution values from 8 to 1
     * @return new Image to show in GUI
     */
QImage* changeImageDynamic(QImage * image, int newDynamicValue) {
    workingImage = new QImage(*backupImage);
    //image = workingImage;

    //    int s = pow(2, newDynamicValue);
    //    double intervall = 255.0/(s-1);

    //    double* values = new double[s];
    //    values[0] = 0;
    //    for(int i = 1; i < s; i++) {
    //        values[i] = values[i-1] + intervall;
    //    }
    //    //    for(int i = 0; i < s; i++) {
    //    //        logFile << values[i] << std::endl;
    //    //    }
    //    //    logFile << "Anzahl möglicher Werte: " << s << "   Intervall: " << intervall << std::endl;


    //    double prot;
    //    double pgruen;
    //    double pblau;

    //    for (int i = 0; i < workingImage->width(); i++) {
    //        for (int j = 0; j < workingImage->height(); j++) {
    //            for (int l = 0; l < s; l++) {

    //                QRgb pixel = workingImage->pixel(i,j);
    //                prot = qRed(pixel);
    //                pgruen = qGreen(pixel);
    //                pblau = qBlue(pixel);

    //                for(int k = 0; k < s; i++) {

    //                    if (prot >= values[k] - (intervall/2) && prot <= values[k] + (intervall/2)) {
    //                        prot = values[k];
    //                    }
    //                    if (pgruen >= values[k] - (intervall/2) && pgruen <= values[k] + (intervall/2)) {
    //                        pgruen = values[k];
    //                    }
    //                    if (pblau >= values[k] - (intervall/2) && pblau <= values[k] + (intervall/2)) {
    //                        pblau = values[k];
    //                    }

    //                }

    //                workingImage->setPixel(i, j, qRgb(prot, pgruen, pblau));
    //            }

    //        }
    //    }

    int max_r = 0;
    int min_r = 256;
    int max_g = 0;
    int min_g = 256;
    int max_b = 0;
    int min_b = 256;
    for(int i=0;i<image->width();i++)
    {
        for(int j=0;j<image->height();j++)
        {
            QRgb pixel = workingImage->pixel(i, j);
            int rot = qRed(pixel);
            int gruen = qGreen(pixel);
            int blau = qBlue(pixel);

            if (rot > max_r) {
                max_r = rot;
            }
            if (rot < min_r) {
                min_r = rot;
            }

            if (gruen > max_g) {
                max_g = gruen;
            }
            if (gruen < min_g) {
                min_g = gruen;
            }

            if (blau > max_b) {
                max_b = blau;
            }
            if (blau < min_b) {
                min_b = blau;
            }
        }
    }

    int mid_r = (int) ((double)(max_r - min_r) / 2 + min_r + 0.5);
    int mid_g = (int) ((double)(max_g - min_g) / 2 + min_g + 0.5);
    int mid_b = (int) ((double)(max_b -  min_b) / 2 + min_b + 0.5);



    double faktor = pow(2, newDynamicValue)/256;
    int faktorKehr = 1/faktor;
    int middle = pow(2, newDynamicValue)/2;
    for(int i=0;i<workingImage->width();i++)
    {
        for(int j=0;j<workingImage->height();j++)
        {
            QRgb pixel = workingImage->pixel(i,j);

            double prot = qRed(pixel);
            double pgruen = qGreen(pixel);
            double pblau = qBlue(pixel);
            double grau = qGray(pixel);

//            prot -= mid_r;
//            pgruen -= mid_g;
//            pblau -= mid_b;

            prot *= faktor;
            pgruen *= faktor;
            pblau *= faktor;

            if (prot >= middle) {
                prot = ceil(prot);
            } else if (prot <= middle) {
                prot = floor(prot);
            }
            if (pgruen >= middle) {
                pgruen = ceil(prot);
            } else if (pgruen <= middle) {
                pgruen = floor(prot);
            }
            if (pblau >= middle) {
                pblau = ceil(prot);
            } else if (pblau <= middle) {
                pblau = floor(prot);
            }

            prot *= faktorKehr;
            pgruen *= faktorKehr;
            pblau *= faktorKehr;

//            prot += mid_r;
//            pgruen += mid_g;
//            pblau += mid_b;

            prot = new_clippedRGB_value(prot);
            pgruen = new_clippedRGB_value(pgruen);
            pblau = new_clippedRGB_value(pblau);

            workingImage->setPixel(i, j, qRgb(prot, pgruen, pblau));

        }
    }

    //    int felder = 256/pow(2, newDynamicValue);
    //    std::cout << felder << std::endl;
    //    for (int i = 0; i < image->width(); i++) {
    //        for (int j = 0; j < image->height(); j++) {

    //            QRgb pixel = image->pixel(i, j);
    //            int prot = qRed(pixel);
    //            int pgruen = qGreen(pixel);
    //            int pblau = qBlue(pixel);
    //            int grau = qGray(pixel);
    //            int zahl = grau%felder;
    //            int faktor;

    //            if(zahl > (felder/2)){
    //                faktor = (felder - zahl);
    //            } else {
    //                faktor = zahl * (-1);
    //            }
    //            //std::cout <<"Felder: "<< felder << "Zahl: " << zahl << "Faktor:" << faktor << std::endl;
    //            int rot = prot + faktor*0.299;
    //            int gruen = pgruen + faktor*0.587;
    //            int blau = pblau  + faktor * 0.114;
    //            image->setPixel(i, j, qRgb(rot,gruen,blau));


    //            //std::cout << "Aus rot: " <<  prot  << " wurde " << rot << " " << gruen << " " << blau << " " << grau << std::endl;

    //            // pixel setter in image with qRgb
    //            // note that qRgb values must be in [0,255]
    //            //image->setPixel(i, j, qRgb(rot,gruen,blau));
    //        }
    //    }


    logFile << "Dynamik des Bildes geändert auf: " + std::to_string(newDynamicValue) + " Bit" << std::endl;
    return workingImage;

}

/**
 * @brief new_clippedRGB_value: Adds brightness adjust to pixel value. clipping the result to [0,255].
 * @param pixel_value
 * @param brightness_adjust_factor
 * @return result int [0,255]
 */
constexpr int new_clippedRGB_value_brightness(int pixel_value, int brightness_adjust_factor){
    int color = pixel_value + brightness_adjust_factor;
    if(color > 255){
        color = 255;
    }
    else if(color < 0){
        color = 0;
    }
    return color;
}

/**
     * @brief adjustBrightness
     *      Add brightness adjust on each pixel in the Image
     * @param image
     *      Input Image to work with
     * @param brightness_adjust_factor
     *      [-255,255]
     *      the brightness adjust shift, will be added on each pixel
     * @return result image, will be shown in the GUI
     */
QImage* adjustBrightness(QImage * image, int brightness_adjust_factor) {
    workingImage = new QImage(*backupImage);

    for(int i=0;i<image->width();i++)
    {
        for(int j=0;j<image->height();j++)
        {
            // Pixel object and pixel getter from image
            QRgb pixel = workingImage->pixel(i, j);
            int rot = new_clippedRGB_value_brightness(qRed(pixel), brightness_adjust_factor);
            int gruen = new_clippedRGB_value_brightness(qGreen(pixel), brightness_adjust_factor);
            int blau = new_clippedRGB_value_brightness(qBlue(pixel), brightness_adjust_factor);

            // pixel setter in image with qRgb
            // note that qRgb values must be in [0,255]
            workingImage->setPixel(i, j, qRgb(rot,gruen,blau));
        }
    }

    logFile << "Brightness adjust applied with factor = " <<brightness_adjust_factor << std::endl;
    return workingImage;

}

/**
     * @brief adjustContrast
     *      calculate an contrast adjustment on each pixel in the Image
     * @param image
     *      Input Image to work with
     * @param contrast_adjust_factor
     *      [0,3]
     *      the contrast adjust factor
     * @return result image, will be shown in the GUI
     */
QImage* adjustContrast(QImage * image, double contrast_adjust_factor){
    workingImage = new QImage(*backupImage);

    int max_r = 0;
    int min_r = 256;
    int max_g = 0;
    int min_g = 256;
    int max_b = 0;
    int min_b = 256;
    for(int i=0;i<image->width();i++)
    {
        for(int j=0;j<image->height();j++)
        {
            QRgb pixel = workingImage->pixel(i, j);
            int rot = qRed(pixel);
            int gruen = qGreen(pixel);
            int blau = qBlue(pixel);

            if (rot > max_r) {
                max_r = rot;
            }
            if (rot < min_r) {
                min_r = rot;
            }

            if (gruen > max_g) {
                max_g = gruen;
            }
            if (gruen < min_g) {
                min_g = gruen;
            }

            if (blau > max_b) {
                max_b = blau;
            }
            if (blau < min_b) {
                min_b = blau;
            }
        }
    }

    int mid_r = (int) ((double)(max_r - min_r) / 2 + min_r + 0.5);
    int mid_g = (int) ((double)(max_g - min_g) / 2 + min_g + 0.5);
    int mid_b = (int) ((double)(max_b -  min_b) / 2 + min_b + 0.5);

    logFile << min_r << ", " << max_r << ", " << mid_r << std::endl;
    logFile << min_g << ", " << max_g << ", " << mid_g << std::endl;
    logFile << min_b << ", " << max_b << ", " << mid_b << std::endl;

    for(int i=0;i<image->width();i++)
    {
        for(int j=0;j<image->height();j++)
        {
            QRgb pixel = workingImage->pixel(i, j);
            int rot = qRed(pixel);
            int gruen = qGreen(pixel);
            int blau = qBlue(pixel);

            rot -= mid_r;
            gruen -= mid_g;
            blau -= mid_b;

            rot = (int) (rot * contrast_adjust_factor + 0.5);
            gruen = (int) (gruen * contrast_adjust_factor + 0.5);
            blau = (int) (blau * contrast_adjust_factor + 0.5);

            rot += mid_r;
            gruen += mid_g;
            blau += mid_b;

            rot = new_clippedRGB_value(rot);
            gruen = new_clippedRGB_value(gruen);
            blau = new_clippedRGB_value(blau);

            workingImage->setPixel(i, j, qRgb(rot,gruen,blau));
        }
    }

    logFile << "Contrast calculation done with contrast factor: " << contrast_adjust_factor << std::endl;
    return workingImage;
}


/**
     * @brief doRobustAutomaticContrastAdjustment
     *      calculate the robust automatic contrast adjustment algorithm with the image as input
     * @param image
     *      Input Image to work with
     * @param plow
     *      [0%,5%]
     *      brightness saturation
     * @param phigh
     *      [0%,5%]
     *      dark saturation
     * @return result image, will be shown in the GUI
     */
QImage* doRobustAutomaticContrastAdjustment(QImage * image, double plow, double phigh){
    workingImage = new QImage(*backupImage);

    int high_r = 0;
    int low_r = 256;
    int high_g = 0;
    int low_g = 256;
    int high_b = 0;
    int low_b = 256;
    for(int i=0;i<image->width();i++)
    {
        for(int j=0;j<image->height();j++)
        {
            QRgb pixel = workingImage->pixel(i, j);
            int rot = qRed(pixel);
            int gruen = qGreen(pixel);
            int blau = qBlue(pixel);

            if (rot > high_r) {
                high_r = rot;
            }
            if (rot < low_r) {
                low_r = rot;
            }

            if (gruen > high_g) {
                high_g = gruen;
            }
            if (gruen < low_g) {
                low_g = gruen;
            }

            if (blau > high_b) {
                high_b = blau;
            }
            if (blau < low_b) {
                low_b = blau;
            }
        }
    }

    logFile << phigh << "   " << plow << std::endl;

    double anteilLow = workingImage->width() * workingImage->height() * plow;
    double anteilHigh = workingImage->width() * workingImage->height() * phigh;

    int count_low = 0;
    int i_low;
    for (i_low = 0; count_low <= anteilLow; i_low++) {
        count_low += histogramm[i_low];
    }

    int count_high = 0;
    int j_high;
    for(j_high = 255; count_high <= anteilHigh; j_high--) {
        count_high += histogramm[j_high];
    }

    logFile << i_low << "   " << j_high << std::endl;

    int aMin = 0;
    int aMax = 255;
    for(int i=0;i<workingImage->width();i++)
    {
        for(int j=0;j<workingImage->height();j++)
        {
            QRgb pixel = workingImage->pixel(i, j);
            int rot = qRed(pixel);
            int gruen = qGreen(pixel);
            int blau = qBlue(pixel);
            int grau = qGray(pixel);

            double Y = 0.299*rot + 0.587*gruen + 0.144*blau;

            if(grau <= i_low)
            {
                workingImage->setPixel(i, j, qRgb(0,0,0));
            }

            if(grau >= j_high)
            {
                workingImage->setPixel(i,j, qRgb(255,255,255));
            }

        }
    }

    logFile << "Robust automatic contrast adjustment applied with:"<< std::endl << "---plow = " << (plow*100) <<"%" << std::endl << "---phigh = " << (phigh*100)<<"%" << std::endl;

    return workingImage;
}

}


