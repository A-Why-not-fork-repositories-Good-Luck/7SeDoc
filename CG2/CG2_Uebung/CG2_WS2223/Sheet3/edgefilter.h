#ifndef EDGEFILTER_H
#define EDGEFILTER_H

#include <qimage.h>




namespace cg2 {
    QImage* doEdgeFilter(QImage * image, int*& derivative_filter, int*& smoothing_filter, int desired_image);
    QImage* doLaplaceFilter(QImage * image, int**& laplace_filter);
    QImage* doCanny(QImage * image, double sigma, int tHi, int tLo);
    inline bool imageGotChangedFlag = true;
    QImage* doUSM(QImage * image, double sharpening_value, double sigma, int tc);
    int getOrientationSector(double dx, double dy);
    bool isLocalMax(QImage* image_gradient, int u, int v, int s, int tLo);
    void traceAndThreshold(QImage* image_Gradient_Max, QImage*& image_bin, int u0, int v0, int tLo);
}

#endif // EDGEFILTER_H
