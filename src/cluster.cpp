#include "constants.h"
#include "image_utils.hpp"
#include "gaussian_blur.hpp"

#define COLS  12
#define ROWS  8

int main(int argc, char **argv )  {

    /* Se checkean los parametros de entrada */
    if (argc != PARAMS) {
    	printf("Inserte los parametros necesarios...\n");
	    exit(-1);
    }

    int kernel_size = atoi(argv[1]);
    Matrix _kernel = calc_kernel(kernel_size, kernel_size, 100.0);
    std::cout << "Cargando imagen..." << std::endl;
    Image _img = load_image(argv[2]);
    img_height = _img[0].size();
    img_width = _img[0][0].size();
    print_RGBimg(_img);

    std::cout << "Aplicando filtro Gaussian Blur..." << std::endl;
    Image _new_img = apply_gaussian_filter(_img, _kernel);
    std::cout << "Guardando imagen..." << std::endl;
    save_image(_new_img, argv[3]);
    std::cout << "Listo!" << std::endl;
    
    /*********************************************************************/

    MPI_Init(&argc, &argv);
    int p, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    char i;

    char a[ROWS*COLS];
    const int NPROWS=1;  /* number of rows in _decomposition_ */
    const int NPCOLS=2;  /* number of cols in _decomposition_ */
    const int BLOCKROWS = ROWS/NPROWS;  /* number of rows in _block_ */
    const int BLOCKCOLS = COLS/NPCOLS; /* number of cols in _block_ */

    // Fill the matrix with the image pixels
    if (rank == 0) {
        for (int ii=0; ii<ROWS*COLS; ii++) {
            a[ii] = (char)ii;
        }
    }

    if (p != NPROWS*NPCOLS) {
        fprintf(stderr,"Error: number of PEs %d != %d x %d\n", p, NPROWS, NPCOLS);
        MPI_Finalize();
        exit(-1);
    }
    char b[BLOCKROWS*BLOCKCOLS];
    for (int ii=0; ii<BLOCKROWS*BLOCKCOLS; ii++) b[ii] = 0;

    MPI_Datatype blocktype;
    MPI_Datatype blocktype2;

    MPI_Type_vector(BLOCKROWS, BLOCKCOLS, COLS, MPI_CHAR, &blocktype2);
    MPI_Type_create_resized( blocktype2, 0, sizeof(char), &blocktype);
    MPI_Type_commit(&blocktype);

    int disps[NPROWS*NPCOLS];
    int counts[NPROWS*NPCOLS];
    for (int ii=0; ii<NPROWS; ii++) {
        for (int jj=0; jj<NPCOLS; jj++) {
            disps[ii*NPCOLS+jj] = ii*COLS*BLOCKROWS+jj*BLOCKCOLS;
            counts [ii*NPCOLS+jj] = 1;
        }
    }
    //printf("a -> %d\n", );
    MPI_Scatterv(a, counts, disps, blocktype, b, BLOCKROWS*BLOCKCOLS, MPI_CHAR, 0, MPI_COMM_WORLD);

    /* each proc prints it's "b" out, in order */
    for (int proc=0; proc<p; proc++) {
        if (proc == rank) {
            printf("Rank = %d\n", rank);
            if (rank == 0) {
                printf("Global matrix: \n");
                for (int ii=0; ii<ROWS; ii++) {
                    for (int jj=0; jj<COLS; jj++) {
                        printf("%3d ",a[ii*COLS+jj]);
                    }
                    printf("\n");
                }
            }
            //else {
              printf("Local Matrix:\n");
              for (int ii=0; ii<BLOCKROWS; ii++) {
                  for (int jj=0; jj<BLOCKCOLS; jj++) {
                      printf("%3d ",(int)b[ii*BLOCKCOLS+jj]);
                  }
                  printf("\n");
              }
            //}

            printf("\n");
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
