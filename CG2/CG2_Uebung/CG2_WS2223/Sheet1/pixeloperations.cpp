#include "pixeloperations.h"
#include "imageviewer-qt5.h"


namespace cg2 {

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
            int r = qRed(pixel);
            int g = qGreen(pixel);
            int b = qBlue(pixel);
            int Y = round(0.299*(double)r + 0.587*(double)g + 0.114*(double)b);
            average_ref += Y;
        }
    }
    average_ref /= pixel_count; //Durchschnitt ziehen

    //Berechne Varianz der Helligkeit/Intensität
    for (int i = 0; i < image->width(); i++) {
        for (int j = 0; j < image->height(); j++) {
            QRgb pixel = image->pixel(i, j);

            int r = qRed(pixel);
            int g = qGreen(pixel);
            int b = qBlue(pixel);
            int Y = round(0.299*(double)r + 0.587*(double)g + 0.114*(double)b);
            variance_ref += (Y-average_ref) * (Y-average_ref);
            histogram_ref[Y] ++;
        }
    }
    variance_ref /= pixel_count;

    for (int i = 0; i < 256; i++)
    {
        histogramm_unscaled[i] = histogram_ref[i];
    }

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

    double faktor = pow(2, newDynamicValue)/256;
    int intervall = (int)(255.0/(pow(2, (double)newDynamicValue)-1));
    for(int i=0;i<workingImage->width();i++)
    {
        for(int j=0;j<workingImage->height();j++)
        {
            QRgb pixel = workingImage->pixel(i,j);
            pixel = RgbToYCbCr(pixel);
            int Y = qRed(pixel);
            int Cb = qGreen(pixel);
            int Cr = qBlue(pixel);

            Y += intervall/2;
            Y /= intervall;
            Y *= intervall;
            Y = new_clipped_value(Y);

            pixel = qRgb(Y, Cb, Cr);

            pixel = YCbCrToRgb(pixel);
            workingImage->setPixel(i, j, pixel);
        }
    }

    logFile << "Dynamik des Bildes geändert auf: " + std::to_string(newDynamicValue) + " Bit" << std::endl;
    return workingImage;

}

/**
 * @brief new_clipped_value: Adds brightness adjust to pixel value. clipping the result to [0,255].
 * @param pixel_value
 * @param brightness_adjust_factor
 * @return result int [0,255]
 */
constexpr int new_clipped_value_brightness(int pixel_value, int brightness_adjust_factor){
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
            QRgb pixel = workingImage->pixel(i, j);
            //            int rot = new_clipped_value_brightness(qRed(pixel), brightness_adjust_factor);
            //            int gruen = new_clipped_value_brightness(qGreen(pixel), brightness_adjust_factor);
            //            int blau = new_clipped_value_brightness(qBlue(pixel), brightness_adjust_factor);
            pixel = RgbToYCbCr(pixel);
            int Y = qRed(pixel);
            int Cb = qGreen(pixel);
            int Cr = qBlue(pixel);

            Y = new_clipped_value_brightness(Y, brightness_adjust_factor);

            pixel = qRgb(Y, Cb, Cr);

            pixel = YCbCrToRgb(pixel);
            workingImage->setPixel(i, j, pixel);
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
    /*
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
*/
    int min;
    int max;
    for (int i = 0; i < 256; i++) {
        if(histogramm[i] != NULL) {
            min = i;
            break;
        }
    }
    for (int i = 255; i >= 0; i--) {
        if(histogramm[i] != NULL) {
            max = i;
            break;
        }
    }
    //logFile << min << "   " << max << std::endl;
    int mid = (int)((double)(max - min)/2 + min + 0.5);

    for(int i=0;i<image->width();i++)
    {
        for(int j=0;j<image->height();j++)
        {
            QRgb pixel = workingImage->pixel(i, j);
            pixel = RgbToYCbCr(pixel);
            int Y = qRed(pixel);
            int Cb = qGreen(pixel);
            int Cr = qBlue(pixel);

            Y -= mid;
            Y = (int)(((double)Y * (double)contrast_adjust_factor) + 0.5);
            Y += mid;
            Y = new_clipped_value(Y);

            pixel = qRgb(Y, Cb, Cr);
            pixel = YCbCrToRgb(pixel);
            workingImage->setPixel(i, j, pixel);
            //            rot -= mid_r;
            //            gruen -= mid_g;
            //            blau -= mid_b;

            //            rot = (int) (rot * contrast_adjust_factor + 0.5);
            //            gruen = (int) (gruen * contrast_adjust_factor + 0.5);
            //            blau = (int) (blau * contrast_adjust_factor + 0.5);

            //            rot += mid_r;
            //            gruen += mid_g;
            //            blau += mid_b;

            //            rot = new_clipped_value(rot);
            //            gruen = new_clipped_value(gruen);
            //            blau = new_clipped_value(blau);

            //            workingImage->setPixel(i, j, qRgb(rot,gruen,blau));
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

    double anteilLow = workingImage->width() * workingImage->height() * plow;
    double anteilHigh = workingImage->width() * workingImage->height() * phigh;

    int count_low = 0;
    int low;
    for (low = 0; count_low < anteilLow; low++) {
        count_low += histogramm_unscaled[low];
    }

    int count_high = 0;
    int high;
    for(high = 255; count_high < anteilHigh; high--) {
        count_high += histogramm_unscaled[high];
    }

    logFile << low << "   " << high << std::endl;

    int min = 0;
    int max = 255;
    for(int i=0;i<workingImage->width();i++)
    {
        for(int j=0;j<workingImage->height();j++)
        {
            QRgb pixel = workingImage->pixel(i, j);
            pixel = RgbToYCbCr(pixel);
            int Y = qRed(pixel);
            int Cb = qGreen(pixel);
            int Cr = qBlue(pixel);

            if (Y <= low)
            {
                Y = min;
            }
            else if (Y >= high)
            {
                Y = max;
            }
            else if (Y > low && Y < high)
            {
                Y = (int) (min + (double)((double)(Y - low) * (double)((double)(max-min)/(double)(high-low))) + 0.5);
            }

            pixel = qRgb(Y, Cb, Cr);
            pixel = YCbCrToRgb(pixel);
            workingImage->setPixel(i, j, pixel);
        }
    }

    logFile << "Robust automatic contrast adjustment applied with:"<< std::endl << "---plow = " << (plow*100) <<"%" << std::endl << "---phigh = " << (phigh*100)<<"%" << std::endl;

    return workingImage;
}

}


