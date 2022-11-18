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
    QImage* image_copy = new QImage(*image);
    bool is_Ableitung = false;
    double s = 0;
    logFile << "Filter read:" << std::endl;
    for(int i = 0; i < filter_height; i++ ){
        for(int j = 0; j < filter_width; j++ ){
            logFile << filter[i][j];
            if(j < (filter_width-1)){
                logFile << " | ";
            }
            s += filter[i][j];
        }
        logFile << std::endl;
    }
    if (s == 0) {
        is_Ableitung = true;
        for(int i = 0; i < filter_height; i++ ){
            for(int j = 0; j < filter_width; j++ ){
                if(j < (filter_width-1)){
                }
                s += abs(filter[i][j]);
            }
        }
    }
    s = 1/s;


    int L = filter_width/2;
    int K = filter_height/2;
    for (int u = L; u < image->width()-L; u++)
    {
        for (int v = K; v < image->height()-K; v++)
        {
            int sum = 0;
            QRgb pixel = image_copy->pixel(u, v);
            pixel = RgbToYCbCr(pixel);
            int Cb = qGreen(pixel);
            int Cr = qBlue(pixel);
            for (int i = -L; i <= L; i++)
            {
                for (int j = -K; j <= K; j++)
                {
                    QRgb p = image_copy->pixel(u+i, v+j);
                    p = RgbToYCbCr(p);
                    int Y = qRed(p);
                    int c = filter[j+K][i+L];
                    sum += c * Y;
                }
            }
            int q = (int) round(s * (double)sum);
            q = new_clipped_value(q);
            if (is_Ableitung)
            {
                q += 127;
                q = new_clipped_value(q);
                image->setPixel(u, v, qRgb(q, q, q));
            }
            else
            {
                image->setPixel(u, v, YCbCrToRgb(qRgb(q, Cb, Cr)));
            }
        }
    }

    logFile << "filter applied:" << std::endl << "---border treatment: ";
    switch (border_treatment) {
    case 0:
        logFile << "Zentralbereich" << std::endl;
        break;
    case 1:
        for (int u = 0; u < L; u++)
        {
            for (int v = 0; v < image->height(); v++)
            {
                int sum = 0;
                QRgb pixel = image_copy->pixel(u, v);
                pixel = RgbToYCbCr(pixel);
                int Cb = qGreen(pixel);
                int Cr = qBlue(pixel);
                for (int i = -L; i <= L; i++)
                {
                    for (int j = -K; j <= K; j++)
                    {
                        int Y = 0;
                        if (image->valid(u+i, v+j))
                        {
                            QRgb p = image_copy->pixel(u+i, v+j);
                            p = RgbToYCbCr(p);
                            Y = qRed(p);
                        }
                        int c = filter[j+K][i+L];
                        sum += c * Y;
                    }
                }
                int q = (int) round(s * (double)sum);
                q = new_clipped_value(q);
                if (is_Ableitung)
                {
                    q += 127;
                q = new_clipped_value(q);
                    image->setPixel(u, v, qRgb(q, q, q));
                }
                else
                {
                    image->setPixel(u, v, YCbCrToRgb(qRgb(q, Cb, Cr)));
                }
            }
        }

        for (int u = L; u < image->width()-L; u++)
        {
            for (int v = 0; v < K; v++)
            {
                int sum = 0;
                QRgb pixel = image_copy->pixel(u, v);
                pixel = RgbToYCbCr(pixel);
                int Cb = qGreen(pixel);
                int Cr = qBlue(pixel);
                for (int i = -L; i <= L; i++)
                {
                    for (int j = -K; j <= K; j++)
                    {
                        int Y = 0;
                        if (image->valid(u+i, v+j))
                        {
                            QRgb p = image_copy->pixel(u+i, v+j);
                            p = RgbToYCbCr(p);
                            Y = qRed(p);
                        }
                        int c = filter[j+K][i+L];
                        sum += c * Y;
                    }
                }
                int q = (int) round(s * (double)sum);
                q = new_clipped_value(q);
                if (is_Ableitung)
                {
                    q += 127;
                q = new_clipped_value(q);
                    image->setPixel(u, v, qRgb(q, q, q));
                }
                else
                {
                    image->setPixel(u, v, YCbCrToRgb(qRgb(q, Cb, Cr)));
                }
            }
        }

        for (int u = image->width()-L; u < image->width(); u++)
        {
            for (int v = 0; v < image->height(); v++)
            {
                int sum = 0;
                QRgb pixel = image_copy->pixel(u, v);
                pixel = RgbToYCbCr(pixel);
                int Cb = qGreen(pixel);
                int Cr = qBlue(pixel);
                for (int i = -L; i <= L; i++)
                {
                    for (int j = -K; j <= K; j++)
                    {
                        int Y = 0;
                        if (image->valid(u+i, v+j))
                        {
                            QRgb p = image_copy->pixel(u+i, v+j);
                            p = RgbToYCbCr(p);
                            Y = qRed(p);
                        }
                        int c = filter[j+K][i+L];
                        sum += c * Y;
                    }
                }
                int q = (int) round(s * (double)sum);
                q = new_clipped_value(q);
                if (is_Ableitung)
                {
                    q += 127;
                q = new_clipped_value(q);
                    image->setPixel(u, v, qRgb(q, q, q));
                }
                else
                {
                    image->setPixel(u, v, YCbCrToRgb(qRgb(q, Cb, Cr)));
                }
            }
        }

        for (int u = L; u < image->width()-L; u++)
        {
            for (int v = image->height()-K; v < image->height(); v++)
            {
                int sum = 0;
                QRgb pixel = image_copy->pixel(u, v);
                pixel = RgbToYCbCr(pixel);
                int Cb = qGreen(pixel);
                int Cr = qBlue(pixel);
                for (int i = -L; i <= L; i++)
                {
                    for (int j = -K; j <= K; j++)
                    {
                        int Y = 0;
                        if (image->valid(u+i, v+j))
                        {
                            QRgb p = image_copy->pixel(u+i, v+j);
                            p = RgbToYCbCr(p);
                            Y = qRed(p);
                        }
                        int c = filter[j+K][i+L];
                        sum += c * Y;
                    }
                }
                int q = (int) round(s * (double)sum);
                q = new_clipped_value(q);
                if (is_Ableitung)
                {
                    q += 127;
                q = new_clipped_value(q);
                    image->setPixel(u, v, qRgb(q, q, q));
                }
                else
                {
                    image->setPixel(u, v, YCbCrToRgb(qRgb(q, Cb, Cr)));
                }
            }
        }

        logFile << "Zero Padding" << std::endl;
        break;
    case 2:
        logFile << image->height() << "   "  << image->width() << std::endl;
        for (int u = 0; u < L; u++)
        {
            for (int v = 0; v < image->height(); v++)
            {
                int sum = 0;
                QRgb pixel = image_copy->pixel(u, v);
                pixel = RgbToYCbCr(pixel);
                int Cb = qGreen(pixel);
                int Cr = qBlue(pixel);
                for (int i = -L; i <= L; i++)
                {
                    for (int j = -K; j <= K; j++)
                    {
                        int Y;
                        QRgb p;
                        if (image->valid(u+i, v+j))
                        {
                            p = image_copy->pixel(u+i, v+j);
                            p = RgbToYCbCr(p);
                            Y = qRed(p);
                        }
                        else
                        {
                            if (u+i < 0 && v+j < 0)
                            {
                                p = image_copy->pixel(0, 0);
                            }
                            else if (u+i >= 0 && v+j < 0)
                            {
                                p = image_copy->pixel(u+1, 0);
                            }
                            else if (u+i < 0 && v+j >= image->height())
                            {
                                p = image_copy->pixel(image->height()-1, image->height()-1);
                            }
                            else if (u+i >= 0 && v+j >= image->height())
                            {
                                p = image_copy->pixel(u+i, image->height()-1);
                            }
                            else
                            {
                                p = image_copy->pixel(0, v+j);
                            }
                            p = RgbToYCbCr(p);
                            Y = qRed(p);
                        }
                        int c = filter[j+K][i+L];
                        sum += c * Y;
                    }
                }
                int q = (int) round(s * (double)sum);
                q = new_clipped_value(q);
                if (is_Ableitung)
                {
                    q += 127;
                q = new_clipped_value(q);
                    image->setPixel(u, v, qRgb(q, q, q));
                }
                else
                {
                    image->setPixel(u, v, YCbCrToRgb(qRgb(q, Cb, Cr)));
                }
            }
        }

        for (int u = L; u < image->width()-L; u++)
        {
            for (int v = 0; v < K; v++)
            {
                int sum = 0;
                QRgb pixel = image_copy->pixel(u, v);
                pixel = RgbToYCbCr(pixel);
                int Cb = qGreen(pixel);
                int Cr = qBlue(pixel);
                for (int i = -L; i <= L; i++)
                {
                    for (int j = -K; j <= K; j++)
                    {
                        int Y;
                        QRgb p;
                        if (image->valid(u+i, v+j))
                        {
                            p = image_copy->pixel(u+i, v+j);
                            p = RgbToYCbCr(p);
                            Y = qRed(p);
                        }
                        else
                        {
                            p = image_copy->pixel(u+i, 0);
                            p = RgbToYCbCr(p);
                            Y = qRed(p);
                        }
                        int c = filter[j+K][i+L];
                        sum += c * Y;
                    }
                }
                int q = (int) round(s * (double)sum);
                q = new_clipped_value(q);
                if (is_Ableitung)
                {
                    q += 127;
                q = new_clipped_value(q);
                    image->setPixel(u, v, qRgb(q, q, q));
                }
                else
                {
                    image->setPixel(u, v, YCbCrToRgb(qRgb(q, Cb, Cr)));
                }
            }
        }

        for (int u = image->width()-L; u < image->width(); u++)
        {
            for (int v = 0; v < image->height(); v++)
            {
                int sum = 0;
                QRgb pixel = image_copy->pixel(u, v);
                pixel = RgbToYCbCr(pixel);
                int Cb = qGreen(pixel);
                int Cr = qBlue(pixel);
                for (int i = -L; i <= L; i++)
                {
                    for (int j = -K; j <= K; j++)
                    {
                        int Y;
                        QRgb p;
                        if (image->valid(u+i, v+j))
                        {
                            p = image_copy->pixel(u+i, v+j);
                            p = RgbToYCbCr(p);
                            Y = qRed(p);
                        }
                        else
                        {
                            if (u+i >= image->width() && v+j < 0)
                            {
                                p = image_copy->pixel(image->width()-1, 0);
                            }
                            else if (u+i < image->width() && v+j < 0)
                            {
                                p = image_copy->pixel(u+i, 0);
                            }
                            else if (u+i < image->width() && v+j >= image->height())
                            {
                                p = image_copy->pixel(u+i, image->height()-1);
                            }
                            else if (u+i >= image->width() && v+j >= image->height())
                            {
                                p = image_copy->pixel(image->width()-1, image->height()-1);
                            }
                            else
                            {
                                p = image_copy->pixel(image->width()-1, v+j);
                            }
                            p = RgbToYCbCr(p);
                            Y = qRed(p);
                        }
                        int c = filter[j+K][i+L];
                        sum += c * Y;
                    }
                }
                int q = (int) round(s * (double)sum);
                q = new_clipped_value(q);
                if (is_Ableitung)
                {
                    q += 127;
                q = new_clipped_value(q);
                    image->setPixel(u, v, qRgb(q, q, q));
                }
                else
                {
                    image->setPixel(u, v, YCbCrToRgb(qRgb(q, Cb, Cr)));
                }
            }
        }

        for (int u = L; u < image->width()-L; u++)
        {
            for (int v = image->height()-K; v < image->height(); v++)
            {
                int sum = 0;
                QRgb pixel = image_copy->pixel(u, v);
                pixel = RgbToYCbCr(pixel);
                int Cb = qGreen(pixel);
                int Cr = qBlue(pixel);
                for (int i = -L; i <= L; i++)
                {
                    for (int j = -K; j <= K; j++)
                    {
                        int Y;
                        QRgb p;
                        if (image->valid(u+i, v+j))
                        {
                            p = image_copy->pixel(u+i, v+j);
                            p = RgbToYCbCr(p);
                            Y = qRed(p);
                        }
                        else
                        {
                            p = image_copy->pixel(u+i, image->height()-1);
                            p = RgbToYCbCr(p);
                            Y = qRed(p);
                        }
                        int c = filter[j+K][i+L];
                        sum += c * Y;
                    }
                }
                int q = (int) round(s * (double)sum);
                q = new_clipped_value(q);
                if (is_Ableitung)
                {
                    q += 127;
                q = new_clipped_value(q);
                    image->setPixel(u, v, qRgb(q, q, q));
                }
                else
                {
                    image->setPixel(u, v, YCbCrToRgb(qRgb(q, Cb, Cr)));
                }
            }
        }

        logFile << "Konstante Randbedingung" << std::endl;
        break;
    case 3:
        for (int u = 0; u < L; u++)
        {
            for (int v = 0; v < image->height(); v++)
            {
                int sum = 0;
                QRgb pixel = image_copy->pixel(u, v);
                pixel = RgbToYCbCr(pixel);
                int Cb = qGreen(pixel);
                int Cr = qBlue(pixel);
                for (int i = -L; i <= L; i++)
                {
                    for (int j = -K; j <= K; j++)
                    {
                        int Y;
                        QRgb p;
                        if (image->valid(u+i, v+j))
                        {
                            p = image_copy->pixel(u+i, v+j);
                            p = RgbToYCbCr(p);
                            Y = qRed(p);
                        }
                        else
                        {
                            if (u+i < 0 && v+j < 0)
                            {
                                p = image_copy->pixel(-1*(u+i), -1*(v+j));
                            }
                            else if (u+i >= 0 && v+j < 0)
                            {
                                p = image_copy->pixel(u+i, -1*(v+j));
                            }
                            else if (u+i < 0 && v+j >= image->height())
                            {
                                p = image_copy->pixel(-1*(u+i), image->height()-(v+j-(image->height()-1)));
                            }
                            else if (u+i >= 0 && v+j >= image->height())
                            {
                                p = image_copy->pixel(u+i, image->height()-(v+j-(image->height()-1)));
                            }
                            else
                            {
                                p = image_copy->pixel(-1*(u+i), v+j);
                            }
                            p = RgbToYCbCr(p);
                            Y = qRed(p);
                        }
                        int c = filter[j+K][i+L];
                        sum += c * Y;
                    }
                }
                int q = (int) round(s * (double)sum);
                q = new_clipped_value(q);
                if (is_Ableitung)
                {
                    q += 127;
                q = new_clipped_value(q);
                    image->setPixel(u, v, qRgb(q, q, q));
                }
                else
                {
                    image->setPixel(u, v, YCbCrToRgb(qRgb(q, Cb, Cr)));
                }
            }
        }

        for (int u = L; u < image->width()-L; u++)
        {
            for (int v = 0; v < K; v++)
            {
                int sum = 0;
                QRgb pixel = image_copy->pixel(u, v);
                pixel = RgbToYCbCr(pixel);
                int Cb = qGreen(pixel);
                int Cr = qBlue(pixel);
                for (int i = -L; i <= L; i++)
                {
                    for (int j = -K; j <= K; j++)
                    {
                        int Y;
                        QRgb p;
                        if (image->valid(u+i, v+j))
                        {
                            p = image_copy->pixel(u+i, v+j);
                            p = RgbToYCbCr(p);
                            Y = qRed(p);
                        }
                        else
                        {
                            p = image_copy->pixel(u+i, -1*(v+j));
                            p = RgbToYCbCr(p);
                            Y = qRed(p);
                        }
                        int c = filter[j+K][i+L];
                        sum += c * Y;
                    }
                }
                int q = (int) round(s * (double)sum);
                q = new_clipped_value(q);
                if (is_Ableitung)
                {
                    q += 127;
                q = new_clipped_value(q);
                    image->setPixel(u, v, qRgb(q, q, q));
                }
                else
                {
                    image->setPixel(u, v, YCbCrToRgb(qRgb(q, Cb, Cr)));
                }
            }
        }

        for (int u = image->width()-L; u < image->width(); u++)
        {
            for (int v = 0; v < image->height(); v++)
            {
                int sum = 0;
                QRgb pixel = image_copy->pixel(u, v);
                pixel = RgbToYCbCr(pixel);
                int Cb = qGreen(pixel);
                int Cr = qBlue(pixel);
                for (int i = -L; i <= L; i++)
                {
                    for (int j = -K; j <= K; j++)
                    {
                        int Y;
                        QRgb p;
                        if (image->valid(u+i, v+j))
                        {
                            p = image_copy->pixel(u+i, v+j);
                            p = RgbToYCbCr(p);
                            Y = qRed(p);
                        }
                        else
                        {
                            if (u+i < image->width() && v+j < 0)
                            {
                                p = image_copy->pixel(u+i, -1*(v+j));
                            }
                            else if (u+i >= image->width() && v+j < 0)
                            {
                                p = image_copy->pixel(image->width()-(u+i-(image->width()-1)), -1*(v+j));
                            }
                            else if (u+i < image->width() && v+j >= image->height())
                            {
                                p = image_copy->pixel(u+i, image->height()-(v+j-(image->height()-1)));
                            }
                            else if (u+i >= image->width() && v+j >= image->height())
                            {
                                p = image_copy->pixel(image->width()-(u+i-(image->width()-1)), image->height()-(v+j-(image->height()-1)));
                            }
                            else
                            {
                                p = image_copy->pixel(image->width()-(u+i-(image->width()-1)), v+j);
                            }
                            p = RgbToYCbCr(p);
                            Y = qRed(p);
                        }
                        int c = filter[j+K][i+L];
                        sum += c * Y;
                    }
                }
                int q = (int) round(s * (double)sum);
                q = new_clipped_value(q);
                if (is_Ableitung)
                {
                    q += 127;
                q = new_clipped_value(q);
                    image->setPixel(u, v, qRgb(q, q, q));
                }
                else
                {
                    image->setPixel(u, v, YCbCrToRgb(qRgb(q, Cb, Cr)));
                }
            }
        }

        for (int u = L; u < image->width()-L; u++)
        {
            for (int v = image->height()-K; v < image->height(); v++)
            {
                int sum = 0;
                QRgb pixel = image_copy->pixel(u, v);
                pixel = RgbToYCbCr(pixel);
                int Cb = qGreen(pixel);
                int Cr = qBlue(pixel);
                for (int i = -L; i <= L; i++)
                {
                    for (int j = -K; j <= K; j++)
                    {
                        int Y;
                        QRgb p;
                        if (image->valid(u+i, v+j))
                        {
                            p = image_copy->pixel(u+i, v+j);
                            p = RgbToYCbCr(p);
                            Y = qRed(p);
                        }
                        else
                        {
                            p = image_copy->pixel(u+i, image->height()-(v+j-(image->height()-1)));
                            p = RgbToYCbCr(p);
                            Y = qRed(p);
                        }
                        int c = filter[j+K][i+L];
                        sum += c * Y;
                    }
                }
                int q = (int) round(s * (double)sum);
                q = new_clipped_value(q);
                if (is_Ableitung)
                {
                    q += 127;
                q = new_clipped_value(q);
                    image->setPixel(u, v, qRgb(q, q, q));
                }
                else
                {
                    image->setPixel(u, v, YCbCrToRgb(qRgb(q, Cb, Cr)));
                }
            }
        }

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
QImage* filterGauss2D(QImage * image, double gauss_sigma, int border_treatment) {
    int center = (int) (3.0 * gauss_sigma);
    int filter_Size = 2 * center + 1;
    double sigma2 = gauss_sigma * gauss_sigma;
    double h[filter_Size];

    double min = 9999999.0;
    for (int i = 0; i < filter_Size; i++)
    {
        double r = center - i;
        h[i] = (double) exp(-0.5 * (r * r) / sigma2);
        //s += h[i];
        if (h[i] < min)
        {
            min = h[i];
        }
    }
    double faktor = 1/min;

    int** filter_Spalte;
    filter_Spalte = new int*[filter_Size];
    for (int i = 0; i < filter_Size; i++)
    {
        filter_Spalte[i] = new int[1];
        filter_Spalte[i][0] = (int)round(h[i] * faktor);
        //logFile << filter_Spalte[i][0] << std::endl;
    }

    int** filter_Zeile;
    filter_Zeile = new int*[1];
    filter_Zeile[0] = new int[filter_Size];
    for (int i = 0; i < filter_Size; i++)
    {
        filter_Zeile[0][i] = (int)round(h[i] * faktor);
    }

    logFile << "2D Gauss-Filter angewendet mit σ: " << gauss_sigma;
    logFile <<  " ---border treatment: ";
    switch (border_treatment) {
    case 0:
        image = filterImage(image, filter_Spalte, 1, filter_Size, 0);
        image = filterImage(image, filter_Zeile, filter_Size, 1, 0);

        logFile << "Zentralbereich" << std::endl;
        break;
    case 1:
        image = filterImage(image, filter_Spalte, 1, filter_Size, 1);
        image = filterImage(image, filter_Zeile, filter_Size, 1, 1);

        logFile << "Zero Padding" << std::endl;
        break;
    case 2:
        image = filterImage(image, filter_Spalte, 1, filter_Size, 2);
        image = filterImage(image, filter_Zeile, filter_Size, 1, 2);

        logFile << "Konstante Randbedingung" << std::endl;
        break;
    case 3:
        image = filterImage(image, filter_Spalte, 1, filter_Size, 3);
        image = filterImage(image, filter_Zeile, filter_Size, 1, 3);

        logFile << "Gespiegelte Randbedingung" << std::endl;
        break;
    }
    return image;
}
}


