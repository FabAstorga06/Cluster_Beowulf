/* Dependencias del programa */
#include <iostream>
#include <vector>
#include <assert.h>
#include <cmath>
#include <png++/png.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <ctime>
#include <string>

/* Constantes de utilidad */
#define RGB 3
#define PARAMS 6
#define NS 100000000
#define REFLECT "-r"
#define CIRCULAR "-c"

static const std::string _dir = "img/";

typedef std::vector<double> Array;
typedef std::vector<Array> Matrix;
typedef std::vector<Matrix> Image;

int img_height, img_width;