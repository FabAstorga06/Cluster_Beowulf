#ifndef _IMAGE_UTILS_HPP
#define _IMAGE_UTILS_HPP

/************************************************************************************/

Image load_image(const char* file ) {
    png::image<png::rgb_pixel> img( _dir + file);
    Image img_mtx(3, Matrix(img.get_height(), Array(img.get_width())));

    for (unsigned int i=0 ; i<img.get_height() ; ++i) {
        for (unsigned int j=0 ; j<img.get_width() ; ++j) {
            img_mtx[0][i][j] = img[i][j].red;
            img_mtx[1][i][j] = img[i][j].green;
            img_mtx[2][i][j] = img[i][j].blue;
        }
    }
    return img_mtx;
}

/************************************************************************/

void save_image(Image& img, const char* file )  {
    assert(img.size() == RGB);

    unsigned int _height = img[0].size();
    unsigned int _width = img[0][0].size();

    png::image<png::rgb_pixel> img_file(_width, _height);

    for (unsigned int i=0 ; i<_height ; ++i) {
        for (unsigned int j=0 ; j<_width ; ++j) {
            img_file[i][j].red = img[0][i][j];
            img_file[i][j].green = img[1][i][j];
            img_file[i][j].blue = img[2][i][j];
        }
    }
    img_file.write( _dir + file );
}
 
#endif