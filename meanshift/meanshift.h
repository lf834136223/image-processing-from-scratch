//
// author: 会飞的吴克
//

#ifndef IMAGEPROCESSINGFROMSCRATCH_MEANSHIFT_H
#define IMAGEPROCESSINGFROMSCRATCH_MEANSHIFT_H
#include <iostream>
#include <vector>

namespace Segmentation{

    const double PI  = 3.14159265358979323846264338327950288;
    const int channels = 3;
    const double EQUAL_ERR = 1e-10;
    class Color{
    private:
        double vals[channels]{};
    public:
        double operator[](const int i) const
        {
            return vals[i];
        }

        double& operator[](const int i)
        {
            return vals[i];
        }
        Color()= default;
        Color(double a, double b,double c){
            vals[0] = a;
            vals[1] = b;
            vals[2] = c;
        }
    };

    typedef std::vector<std::vector<Color> > Image;
    typedef std::vector<Color> Row;

    void RGBtoLUV(const Color & color, Color & color_LUV)
    {
        Color WhiteReference_XYZ = {95.047,100.000,108.883};
        Color color_XYZ;
        double r = color[0] / 255.0, g = color[1] / 255.0, b = color[2] / 255.0;
        r = (r > 0.04045 ? pow((r + 0.055) / 1.055, 2.4) : r / 12.92) * 100.0;
        g = (g > 0.04045 ? pow((g + 0.055) / 1.055, 2.4) : g / 12.92) * 100.0;
        b = (b > 0.04045 ? pow((b + 0.055) / 1.055, 2.4) : b / 12.92) * 100.0;

        // Observer. = 2°, Illuminant = D65
        color_XYZ[0] = r * 0.4124 + g * 0.3576 + b * 0.1805;
        color_XYZ[1] = r * 0.2126 + g * 0.7152 + b * 0.0722;
        color_XYZ[2] = r * 0.0193 + g * 0.1192 + b * 0.9505;

        const double Epsilon = 0.008856; // Intent is 216/24389
        const double Kappa = 903.3; // Intent is 24389/27
        double y = color_XYZ[1] / WhiteReference_XYZ[1];
        color_LUV[0] = (y > Epsilon ? 116.0 * pow(y,1.0/3.0) - 16.0 : Kappa * y);

        double targetDenominator = color_XYZ[0] + color_XYZ[1]*15.0 + color_XYZ[2]*3.0;
        double referenceDenominator = WhiteReference_XYZ[0] + WhiteReference_XYZ[1]*15.0 + WhiteReference_XYZ[2]*3.0;
        // ReSharper disable CompareOfFloatsByEqualityOperator
        double xTarget = targetDenominator == 0 ? 0 : ((4.0 * color_XYZ[0] / targetDenominator) - (4.0 * WhiteReference_XYZ[0] / referenceDenominator));
        double yTarget = targetDenominator == 0 ? 0 : ((9.0 * color_XYZ[1] / targetDenominator) - (9.0 * WhiteReference_XYZ[1] / referenceDenominator));
        // ReSharper restore CompareOfFloatsByEqualityOperator

        color_LUV[1] = (13.0 * color_LUV[0] * xTarget);
        color_LUV[2] = (13.0 * color_LUV[0] * yTarget);
    }

    void RGBimage2LUVimage(const Image & RGB,Image & LUV)
    {
        assert(!RGB.empty());
        for(unsigned long i = 1;i < RGB.size();i++)
        {
            assert(RGB[i].size() == RGB[0].size());
        }

        LUV.clear();
        for(auto it = RGB.begin();it < RGB.end();it++)
        {
            Row tempRow;
            for(auto it2 = it->begin();it2 < it->end();it2++)
            {
                Color temp;
                RGBtoLUV((*it2),temp);
                tempRow.push_back(temp);
            }
            LUV.push_back(tempRow);
        }
    }

    void LUVtoRGB(const Color & luv, Color & rgb)
    {
        Color WhiteReference_XYZ = {95.047,100.000,108.883};
        const double Epsilon = 0.008856; // Intent is 216/24389
        const double Kappa = 903.3; // Intent is 24389/27

        const double c = -1.0 / 3.0;
        double Denominator = WhiteReference_XYZ[0] + 15.0 * WhiteReference_XYZ[1] + 3.0 * WhiteReference_XYZ[2];
        double uPrime = (4.0 * WhiteReference_XYZ[0]) / Denominator;
        double vPrime = (9.0 * WhiteReference_XYZ[1]) / Denominator;
        double a = (1.0 / 3.0) * ((52.0 * luv[0]) / (luv[1] + 13 * luv[0] * uPrime) - 1.0);
        double imteL_16_116 = (luv[0] + 16.0) / 116.0;
        double y = luv[0] > Kappa * Epsilon
                ? imteL_16_116 * imteL_16_116 * imteL_16_116
                : luv[0] / Kappa;
        double b = -5.0 * y;
        double d = y * ((39.0 * luv[0]) / (luv[2] + 13.0 * luv[0] * vPrime) - 5.0);
        double x = (d - b) / (a - c);
        double z = x * a + b;
        // Color color_XYZ(100 * x,100 * y,100 * z);

        double r = x * 3.2406 + y * -1.5372 + z * -0.4986;
        double g = x * -0.9689 + y * 1.8758 + z * 0.0415;
        b = x * 0.0557 + y * -0.2040 + z * 1.0570;

        r = r > 0.0031308 ? 1.055 * pow(r, 1 / 2.4) - 0.055 : 12.92 * r;
        g = g > 0.0031308 ? 1.055 * pow(g, 1 / 2.4) - 0.055 : 12.92 * g;
        b = b > 0.0031308 ? 1.055 * pow(b, 1 / 2.4) - 0.055 : 12.92 * b;

        r *= 255.0;
        g *= 255.0;
        b *= 255.0;

        rgb[0] = r < 0 ? 0 :(r > 255 ? 255 : r);
        rgb[1] = g < 0 ? 0 :(g > 255 ? 255 : g);
        rgb[2] = b < 0 ? 0 :(b > 255 ? 255 : b);
    }

    void LUVimage2RGBimage(const Image & LUV,Image & RGB)
    {
        assert(!LUV.empty());
        for(unsigned long i = 1;i < LUV.size();i++)
        {
            assert(LUV[i].size() == LUV[0].size());
        }

        RGB.clear();
        for(auto it = LUV.begin();it < LUV.end();it++)
        {
            Row tempRow;
            for(auto it2 = it->begin();it2 < it->end();it2++)
            {
                Color temp;
                LUVtoRGB((*it2),temp);
                tempRow.push_back(temp);
            }
            RGB.push_back(tempRow);
        }
    }

    double Guass(double x){
        double exponent = -x*x/2.0;
        return (1.0/(sqrt(2*PI)))*exp(exponent);
    }

    void meanshift(const Image & src,Image & dst){
        const double sr = 6;
        const double tr = 8;
        const unsigned long max_itr = 10;
        const double color_threshold = 0.05;

        assert(!src.empty());
        for(unsigned long i = 0;i < src.size();i++)
        {
            assert(src[i].size() == src[0].size());
        }

        dst.clear();

        for(unsigned long i = 0;i < src.size();i++)
        {
            Row dst_row;
            for(unsigned long j = 0;j < src[i].size();j++)
            {
                double cur_row = i;
                double cur_col = j;
                Color cur_color = src[i][j];
                for(unsigned long k = 0;k < max_itr;k++)
                {
                    double ltr = (cur_row - sr - 1);// left top row.
                    double ltc = (cur_col - sr - 1);// left top col.
                    double rbr = (cur_row + sr + 1);// right bottom row.
                    double rbc = (cur_col + sr + 1);// right bottom col.

                    ltr = ltr < 0 ? 0 : ltr;
                    ltc = ltc < 0 ? 0 : ltc;
                    rbr = rbr > src.size() ? src.size() : rbr;
                    rbc = rbc > src[i].size() ? src[i].size() : rbc;

                    double displacement_r = 0;
                    double displacement_c = 0;
                    double denominator = 0;
                    for(unsigned long l = ltr;l < rbr;l++)
                    {
                        for(unsigned long m = ltc;m < rbc;m++)
                        {
                            double temp = cur_row - l;
                            double distant_sq = 0;
                            distant_sq += (temp*temp);
                            temp = cur_col - m;
                            distant_sq += (temp*temp);

                            if(distant_sq <= sr*sr)
                            {
                                double color_distance = 0;
                                for(int c = 0;c < channels;c++)
                                {
                                    temp = src[l][m][c] - cur_color[c];
                                    color_distance += (temp*temp);
                                }
                                color_distance = sqrt(color_distance);

                                double weight = Guass(color_distance/tr);
                                displacement_r += (l - cur_row) * weight;
                                displacement_c += (m - cur_col) * weight;
                                denominator += weight;
                            }
                        }
                    }

                    double dr = displacement_r / denominator;
                    double dc = displacement_c / denominator;
                    cur_row += dr;
                    cur_col += dc;
                    Color pre_color = cur_color;

                    if(cur_row >= src.size() || cur_col >= src[i].size())
                    {
                        int cur_r = cur_row;
                        int cur_c = cur_col;
                        cur_color[0] = src[cur_r][cur_c][0];
                        cur_color[1] = src[cur_r][cur_c][1];
                        cur_color[2] = src[cur_r][cur_c][2];
                    }
                    else
                    {
                        for(int c = 0;c < channels;c++)
                        {
                            int lt_r = cur_row,lt_c = cur_col;
                            double temp1 = src[lt_r][lt_c][c] + (cur_col - lt_c)*(src[lt_r][lt_c+1][c] - src[lt_r][lt_c][c]);
                            double temp2 = src[lt_r+1][lt_c][c] + (cur_col - lt_c)*(src[lt_r+1][lt_c+1][c] - src[lt_r+1][lt_c][c]);
                            cur_color[c] = temp1 + (cur_row - lt_r)*(temp2-temp1);
                        }
                    }
                    // bilinear interpolation

                    double color_distance = 0;
                    for(int c = 0;c < channels;c++)
                    {
                        double temp = pre_color[c] - cur_color[c];
                        color_distance += (temp*temp);
                    }

                    if((abs(dr) < EQUAL_ERR && abs(dc) < EQUAL_ERR) || color_distance < color_threshold*color_threshold ||k == max_itr - 1)
                    {
                        dst_row.push_back(cur_color);
                        // you can record the converge point here (cur_row,cur_col)
                        break;
                    }
                }
            }
            dst.push_back(dst_row);
        }
    }

}
#endif //IMAGEPROCESSINGFROMSCRATCH_MEANSHIFT_H
