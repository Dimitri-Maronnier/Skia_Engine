#ifndef IMGLOADER_H
#define IMGLOADER_H
#include <GL/glew.h>
#include <stdlib.h>
#include <stdio.h>
#include <png.h>
#include <iostream>
#include <memory.h>

class ImgLoader
{
public:

    static bool import(std::string sFilePath, std::string pathToData, bool isHDR);
};

#endif // IMGLOADER_H
