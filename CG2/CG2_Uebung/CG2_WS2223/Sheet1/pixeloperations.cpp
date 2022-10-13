#include "pixeloperations.h"
#include "imageviewer-qt5.h"


namespace cg2 {
    
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
        for (int i = 0; i < image->width(); i++) {
            for (int j = 0; j < image->height(); j++) {
            }
        }
        for (int i = 0; i < 256; i++) {
            histogram_ref[i] = 50;
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

        for (int i = 0; i < image->width(); i++) {
            for (int j = 0; j < image->height(); j++) {

            }
        }
        logFile << "Dynamik des Bildes geändert auf: " + std::to_string(newDynamicValue) + " Bit" << std::endl;
        return image;

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
    QImage* adjustBrightness(QImage * image, int brightness_adjust_factor){
        for(int i=0;i<image->width();i++)
        {
            for(int j=0;j<image->height();j++)
            {

            }
        }

        logFile << "Brightness adjust applied with factor = " <<brightness_adjust_factor << std::endl;
        return image;

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

        for(int i=0;i<image->width();i++)
        {
            for(int j=0;j<image->height();j++)
            {
            }
        }

        logFile << "Contrast calculation done with contrast factor: " << contrast_adjust_factor << std::endl;
        return image;
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

        for(int i=0;i<image->width();i++)
        {
            for(int j=0;j<image->height();j++)
            {
            }
        }

        logFile << "Robust automatic contrast adjustment applied with:"<< std::endl << "---plow = " << (plow*100) <<"%" << std::endl << "---phigh = " << (phigh*100)<<"%" << std::endl;

        return image;
    }

}


