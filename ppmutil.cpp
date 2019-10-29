#ifndef __ppmutil__
#define __ppmutil__

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "pixelinfo.h"

using namespace std;

void saveppm(const char *filename, int w, int h, PixelInfo *data){
    size_t kWidth = w;
    size_t kHeight = h;
    size_t area = kWidth * kHeight;
    std::ostringstream headerStream;
    headerStream << "P6" << endl;
    headerStream << kWidth << ' ' << kHeight << endl;
    headerStream << 255 << endl;
    std::ofstream fileStream(filename, std::ios::out | std::ios::binary);

    fileStream << headerStream.str();

    for(int i=0; i < area; i++){
        unsigned char r, g, b;
        PixelInfo pixel = data[i];
        pixel.clamp();
        r = static_cast<unsigned char>(pixel.r);
        g = static_cast<unsigned char>(pixel.g);
        b = static_cast<unsigned char>(pixel.b);
        fileStream << r << g << b;
        // if (i == 0) std::cout << pixel.r << ' ' << pixel.g << ' ' << pixel.b << endl;
    }

    fileStream.flush();
    fileStream.close();

}

#endif //