#include "constants.h"
#include "image_utils.hpp"
#include "gaussian_blur.hpp"

#define TMP 425  /* variable temporal */

int main(int argc, char** argv )  {

    /* Se checkean los parametros de entrada */
    if (argc != PARAMS) {
    	printf("Inserte los parametros necesarios...\n");
	    exit(-1);
    }

    int kernel_size = atoi(argv[1]);
    Matrix _kernel = calc_kernel(kernel_size, kernel_size, 10.0);
    std::cout << "Cargando imagen..." << std::endl;
    Image _img = load_image(argv[2]);
    img_height = _img[0].size();
    img_width = _img[0][0].size();
  //  printf("IMAGEN: \n");
    //print_img(_img);

   /* std::cout << "Aplicando filtro Gaussian Blur..." << std::endl;
    Image _new_img = apply_gaussian_filter(_img, _kernel);
    std::cout << "Guardando imagen..." << std::endl;
    save_image(_new_img, argv[3]);
    std::cout << "Listo!" << std::endl;  */

    /*********************************************************************/

    MPI_Init(&argc, &argv);
    int p, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    unsigned char a[img_height*img_width];
    const int NPROWS = 2;  /* number of rows in _decomposition_ */
    const int NPCOLS = 3;  /* number of cols in _decomposition_ */
    const int BLOCKROWS = img_height/NPROWS;  /* number of rows in _block_ */
    const int BLOCKCOLS = img_width/NPCOLS; /* number of cols in _block_ */

    // Fill the matrix with the image pixels
    if (rank == 0) {
        int index = 0;
        for (unsigned int i=0 ; i<img_height ; ++i) {
            for (unsigned int j=0 ; j<img_width ; ++j) {
                a[index] = (unsigned char)(_img[0][i][j]);
                index++;
            }
        }
    }

    if (p != NPROWS*NPCOLS) {
        fprintf(stderr,"Error: number of processors %d != %d x %d\n", p, NPROWS, NPCOLS);
        MPI_Finalize();
        exit(-1);
    }

    unsigned char b[BLOCKROWS*BLOCKCOLS];
    for (int ii=0; ii<BLOCKROWS*BLOCKCOLS; ii++) b[ii] = 0;

    MPI_Datatype blocktype;
    MPI_Datatype blocktype2;

    MPI_Type_vector(BLOCKROWS, BLOCKCOLS, img_width, MPI_CHAR, &blocktype2);
    MPI_Type_create_resized(blocktype2, 0, sizeof(char), &blocktype);
    MPI_Type_commit(&blocktype);

    int disps[NPROWS*NPCOLS];
    int counts[NPROWS*NPCOLS];
    for (int ii=0; ii<NPROWS; ii++) {
        for (int jj=0; jj<NPCOLS; jj++) {
            disps[ii*NPCOLS+jj] = ii*img_width*BLOCKROWS+jj*BLOCKCOLS;
            counts [ii*NPCOLS+jj] = 1;
        }
    }

    MPI_Scatterv(a, counts, disps, blocktype, b, BLOCKROWS*BLOCKCOLS, MPI_CHAR, 0, MPI_COMM_WORLD);

    Image gauss_img(RGB, Matrix(BLOCKROWS, Array(BLOCKCOLS)));
    image_from_matrix(b, gauss_img, BLOCKCOLS, BLOCKROWS);

    Image res (RGB, Matrix(BLOCKROWS, Array(BLOCKCOLS)));
    /* each proc prints it's "b" out, in order */
    for (int proc = 0; proc < p; proc++) {
        if (proc == rank) {
            if (rank == 0) {
                printf("Rank = %d\n", rank);
                res = apply_gaussian_filter(gauss_img, _kernel);
                //save_image(res, argv[3]);
            }
            else {
                res = apply_gaussian_filter(gauss_img, _kernel);
                //save_image(res, argv[3]);
                printf("Rank = %d\n", rank);
            }

            printf("\n");
        }
        matrix_from_image(res, b, BLOCKCOLS, BLOCKROWS);
        MPI_Barrier(MPI_COMM_WORLD);
    }

    //MPI_Gather
    MPI_Gatherv (b, BLOCKROWS*BLOCKCOLS, MPI_CHAR, a, counts, disps, blocktype, 0, MPI_COMM_WORLD);


    Image result_image(RGB, Matrix(img_height, Array(img_width)));
    image_from_matrix (a, result_image, img_width, img_height);
    save_image(result_image, argv[3]);

    MPI_Finalize();
    return 0;
}
