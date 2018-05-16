#ifndef _IMAGE_UTILS_HPP
#define _IMAGE_UTILS_HPP

/************************************************************************************/

Image load_image(const char* file ) {
    png::image<png::gray_pixel> img( _dir + file);
    Image img_mtx(RGB, Matrix(img.get_height(), Array(img.get_width())));

    for (unsigned int i=0 ; i<img.get_height() ; ++i) {
        for (unsigned int j=0 ; j<img.get_width() ; ++j) {
            img_mtx[0][i][j] = img[i][j];
        }
    }
    return img_mtx;
}

/************************************************************************/

void save_image(Image& img, const char* file )  {
    assert(img.size() == RGB);

    unsigned int _height = img[0].size();
    unsigned int _width = img[0][0].size();

    png::image<png::gray_pixel> img_file(_width, _height);

    for (unsigned int i=0 ; i<_height ; ++i) {
        for (unsigned int j=0 ; j<_width ; ++j) {
            img_file[i][j] = img[0][i][j];
        }
    }
    img_file.write( _dir + file );
}


/************************************************************************/

//Creates an Image out of a Matrix
void image_from_matrix(unsigned char *_mtx, Image &img , int cols, int rows ) {
    int index = 0;
    for (int i=0 ; i<rows ; ++i) {
        for (int j=0 ; j<cols ; ++j) {
          img[0][i][j] = _mtx[index];
          index++;
        }
    }
}

//Creates a matrix out of an Image
void matrix_from_image (Image &img, unsigned char *_mtx, int cols, int rows) {

    for (unsigned int i=0 ; i<rows ; ++i) {
        for (unsigned int j=0 ; j<cols ; ++j) {
            _mtx[(i*cols +j)] = (unsigned char)(img[0][i][j]);
        }
    }
}




 //matrix_from_image(_img, a, img_width, img_height);
//image_from_matrix(b, gauss_img, BLOCKCOLS, BLOCKROWS);












#endif
