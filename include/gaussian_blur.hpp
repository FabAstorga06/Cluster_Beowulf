#ifndef _GAUSSIAN_BLUR_HPP
#define _GAUSSIAN_BLUR_HPP

/************************************************************************************/

int circular(int M, int x) {
    if (x<0) return x+M;
    if(x >= M) return x-M;
    return x;
}
int reflect(int M, int x) {
    if(x < 0) return -x-1;
    if(x >= M) return 2*M-x-1;
    return x;
}

/* Kernel para filtro */

Matrix calc_kernel(int rows, int cols, double sigma )  {
    Matrix kernel(rows, Array(cols));
    double res = 0.0;

    for (int i=0; i<rows; ++i) {
        for (int j=0; j<cols; ++j) {
            kernel[i][j] = exp(-(i*i+j*j)/(2*sigma*sigma))/(2*M_PI*sigma*sigma);
            res += kernel[i][j];
        }
    }
    for (int x=0; x<rows; ++x) {
        for (int y=0; y<cols; ++y) {
            kernel[x][y] /= res;
        }
    }
    return kernel;
}

/************************************************************************************/

/* Filtro Gauss Blur */

Image apply_gaussian_filter(Image &img, Matrix &filter, int (*f)(int, int) )  {
    assert(img.size()==RGB && filter.size()!=0);

    int rows = img[0].size();
    int cols = img[0][0].size();
    int filter_rows = filter.size();
    int filter_cols = filter[0].size();

    Image new_img(RGB, Matrix(rows, Array(cols)));

    double sum;
    int i_img,j_img;
    int y = (filter_rows/2);
    int x = (filter_cols/2);
    for(int i=0; i<rows; ++i) {
        for (int j=0; j<cols; ++j) {
            sum = 0.0;
            for(int h=-y; h<=y; ++h){
                for(int w=-x; w<=x; ++w){
                    j_img = (*f)(cols, j-w);
                    i_img = (*f)(rows, i-h);
                    
                    sum += (filter[w+x][h+y] * img[0][i_img][j_img]);
                }
            }
            new_img[0][i][j] = sum;
        }
    }
    return new_img;
}


#endif
