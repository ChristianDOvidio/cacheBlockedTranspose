#include <iostream>
#include <chrono>

// input matrix A (row-major order) of dimensions m x n
// output matrix AT (row-major order) of dimensions n x m
void transpose(const int *A, unsigned int m, unsigned int n, int *AT)
{
    unsigned int i, j;

    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
        {
            AT[i + j * m] = A[j + i * n];
            //std::cout << "New: " << (i + (j * m)) << " Old: " << (j + (i * n)) << "\n";
        }
    }
}

void transpose_block(const int *A, unsigned int m, unsigned int n, unsigned int blockSize, int *AT)
{
    if ((blockSize > n) || (blockSize > m))
        return; //blockSize can't exceed bounds of matrix
    if (n != m)
        return; //block has to be square

    unsigned int numBlocks = m / blockSize; //number of blocks along one dimension (height or width)
    unsigned int w, x, i, j, tempJ, tempI;

    for (w = 0; w < numBlocks; w++)
    { //iterate over all columns of original
        for (x = 0; x < numBlocks; x++)
        { //iterate over all rows of original
            for (i = (w * blockSize); i < ((w + 1) * blockSize); i++)
            { //iterate over block's columns of original
                for (j = (x * blockSize); j < ((x + 1) * blockSize); j++)
                { //iterate over block's rows of orginal
                    AT[i + (j * m)] = A[j + (i * n)];
                    if ((x == (numBlocks - 1)) && (j == (((x + 1) * blockSize) - 1)) && (j < n))
                    { //reached end of row without transposing all elements
                        for (tempJ = (j + 1); tempJ < n; tempJ++)
                        { //transposes remaining elements in row
                            //std::cout << "New: " << (i + (tempJ * m)) << " Old: " << (tempJ + (i * n)) << "\n";
                            AT[i + (tempJ * m)] = A[tempJ + (i * n)];
                        }
                        if ((w == (numBlocks - 1)) && (i == (((w + 1) * blockSize) - 1)) && (i < n))
                        { //transposes remaining corner elements
                            for (tempI = (i + 1); tempI < n; tempI++)
                            {
                                for (tempJ = (j + 1); tempJ < n; tempJ++)
                                {
                                    //std::cout << "New: " << (tempI + (tempJ * m)) << " Old: " << (tempJ + (tempI * n)) << "\n";
                                    AT[tempI + (tempJ * m)] = A[tempJ + (tempI * n)];
                                }
                            }
                        }
                    }
                    if ((w == (numBlocks - 1)) && (i == (((w + 1) * blockSize) - 1)) && (i < n))
                    { //reached end of column without transposing all elements
                        for (tempI = (i + 1); tempI < n; tempI++)
                        { //transposes remaining element in column
                            //std::cout << "New: " << (tempI + (j * m)) << " Old: " << (j + (tempI * n)) << "\n";
                            AT[tempI + (j * m)] = A[j + (tempI * n)];
                        }
                    }
                    //std::cout << " " << w << " " << x << " " << i << " " << j << "\n";
                    //std::cout << "New: " << (i + (j * m)) << " Old: " << (j + (i * n)) << "\n";
                }
            }
        }
    }
}

bool check_transpose(const int *A, const int *AT, unsigned int m, unsigned int n)
{
    unsigned int i, j;

    for (i = 0; i < m; i++)
    { //iterate over columns of orginal
        for (j = 0; j < n; j++)
        { //iterate rows of original
            if (AT[j + i * n] != A[i + j * m])
            {
                return false;
            }
        }
    }

    return true;
}

int main(int argc, char **argv)
{
    if (argc < 2)
        return 0;
    int blockSize = std::atoi(argv[1]);
    unsigned int n = 5000;

    int *A = new int[n * n];
    int *AT = new int[n * n];
    int *Ablock = new int[n * n];
    int *ATblock = new int[n * n];
    int randNum;

    std::srand(std::time(nullptr));
    for (unsigned int i = 0; i < n * n; i++)
    {
        randNum = std::rand();
        A[i] = randNum;
	Ablock[i] = randNum;
    }

    auto start = std::chrono::high_resolution_clock::now();
    transpose(A, n, n, AT);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Time of transpose: " << diff.count() << " sec\n";

    
    start = std::chrono::high_resolution_clock::now();
    transpose_block(Ablock, n, n, blockSize, ATblock);
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    std::cout << "Time of blocked transpose with " << blockSize << " blocks: "  << diff.count() << " sec\n";


    if (!check_transpose(A, AT, n, n))
    {
        std::cout << "Incorrect !";
    }

    delete[] A;
    delete[] AT;
}
