/* Dependencias del programa */
#include <iostream>
#include <vector>
#include <assert.h>
#include <cmath>
#include <png++/png.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

/* Constantes de utilidad */
#define RGB 3
#define PARAMS 4

static const std::string _dir = "img/";

typedef std::vector<double> Array;
typedef std::vector<Array> Matrix;
typedef std::vector<Matrix> Image;

int img_height, img_width;