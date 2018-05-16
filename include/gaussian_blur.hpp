#ifndef _GAUSSIAN_BLUR_HPP
#define _GAUSSIAN_BLUR_HPP

/************************************************************************************/

Matrix calc_kernel(int height, int width, double sigma )  {
    Matrix kernel(height, Array(width));
    double res = 0.0;

    for (int i=0; i<height; ++i) {
        for (int j=0; j<width; ++j) {
            kernel[i][j] = exp(-(i*i+j*j)/(2*sigma*sigma))/(2*M_PI*sigma*sigma);
            res += kernel[i][j];
        }
    }
    for (int x=0; x<height; ++x) {
        for (int y=0; y<width; ++y) {
            kernel[x][y] /= res;
        }
    }
    return kernel;
}

/************************************************************************************/

Image apply_gaussian_filter(Image &img, Matrix &filter)  {
    assert(img.size()==RGB && filter.size()!=0);

    int height = img[0].size();
    int width = img[0][0].size();
    int filter_height = filter.size();
    int filter_width = filter[0].size();

    Image new_img(RGB, Matrix(height, Array(width)));

    for (int d=0; d<RGB; ++d) {
        for (int i=0; i<height; ++i) {
            for (int j=0; j<width; ++j) {
                for (int h=i; h<(i+filter_height); ++h) {
                    for (int w=j; w<(j+filter_width); ++w) {
                        if(h>=height || w>= width){
                            break;
                        }
                        else{
                            new_img[d][i][j] += (filter[h-i][w-j] * img[d][h][w] );
                        }
                    }
                }
            }
        }
    }
    return new_img;
}

void print_img (Image &mtx)  {
  for(int i = 0; i < 300; ++i) {
    for(int j = 0; j < 300; ++j) {
       // printf("(");
            std::cout << " " << (unsigned int)mtx[0][i][j];
       // printf(")");
    }
    printf("\n");
  }
}

#endif
