# Sparse Matrix Calculator

This C++ code implements a sparse matrix class that leverages linked lists and depth-first search (DFS) to support various operations. The class allows for seamless insertion, transposition, addition, subtraction, and multiplication of sparse matrices. By utilizing linked lists, the code optimizes memory usage and speeds up computations for matrices with a large number of zero elements. The inclusion of DFS ensures efficient traversal and manipulation of the matrix structure. With this implementation, you can perform various matrix operations on sparse matrices with improved performance and reduced memory overhead.

## Table of Contents

- [Introduction](#introduction)
- [Usage](#usage)
- [SparseMatrix Class](#sparsematrix-class)
- [Example Usage](#example-usage)
- [Contributing](#contributing)
- [License](#license)

## Introduction

Sparse matrices, which contain a significant number of zero values, pose unique challenges in terms of storage and manipulation. Efficiently handling these matrices is essential for numerous applications, ranging from scientific computing to machine learning and data analysis. This code presents a streamlined implementation of a sparse matrix class that prioritizes memory optimization and offers essential operations tailored for sparse matrices. By addressing the specific requirements of sparse matrices, this code enables efficient storage and manipulation of these matrices, unlocking their full potential in diverse domains.

## Usage

To use this code, you will need a C++ compiler and an environment to run C++ programs. Follow these steps to get started:

1. Clone the repository or download the source code file

2. Compile the code using a C++ compiler

3. Run the compiled program

4. Follow the prompts to enter the matrix values and perform various operations

By following these steps, you'll be able to compile and run the code, input matrix values, and execute a variety of operations as instructed by the program

## SparseMatrix Class

The code includes the following classes:

- `Node`: Represents a node in the sparse matrix, which can be an internal node or a header node.

- `Internal`: Represents an internal node in the sparse matrix.

- `Header`: Represents a header node in the sparse matrix.

- `SparseMatrix`: Represents the sparse matrix and provides operations such as insertion, transposition, addition, subtraction, and multiplication.

The `SparseMatrix` class provides the following public methods:

- `SparseMatrix()`: Default constructor that initializes the sparse matrix with 0 rows, 0 columns, and NULL headers.

- `SparseMatrix(int rows, int cols)`: Constructor with parameters that initializes the sparse matrix with the specified number of rows and columns.

- `~SparseMatrix()`: Destructor for freeing the memory occupied by the sparse matrix.

- `void insert(int row, int col, int value)`: Inserts a new internal node with the given row, column, and value into the sparse matrix.

- `void print() const`: Prints the sparse matrix in a readable format.

- `SparseMatrix* transpose()`: Returns a new matrix that is the transpose of the current matrix.

- `SparseMatrix* operator+(SparseMatrix& other)`: Returns a new matrix that is the sum of the current matrix and another matrix.

- `SparseMatrix* operator-(SparseMatrix& other)`: Returns a new matrix that is the difference between the current matrix and another matrix.

- `SparseMatrix* operator*(SparseMatrix& other)`: Returns a new matrix that is the product of the current matrix and another matrix.

## Example Usage

Here's an example of how to use the `SparseMatrix` class:

```cpp
#include <iostream>
#include "SparseMatrix.h"

int main() {
  
// Create a sparse matrix with 3 rows and 3 columns
SparseMatrix matrix1(3, 3);

// Insert values into the matrix
matrix1.insert(0, 0, 1);
matrix1.insert(0, 1, 2);
matrix1.insert(1, 1, 3);
matrix1.insert(2, 0, 4);

// Print the matrix
cout << "Matrix:" << endl;
matrix1.print();

// Transpose the matrix
cout << "\nTransposed Matrix:" << endl;
SparseMatrix* transposeMatrix = matrix1.transpose();
transposeMatrix->print();

// Create a second sparse matrix with 3 rows and 3 columns
SparseMatrix matrix2(3, 3);
  
// Insert values into the second matrix
matrix2.insert(0, 0, 1);
matrix2.insert(0, 1, 2);
matrix2.insert(2, 2, 3);

// Print the second matrix
cout << "\nSecond Matrix:" << endl;
matrix2.print();

// Perform matrix addition
cout << "\nTransposed Matrix + Second Matrix:" << endl;
SparseMatrix* sumMatrix = *transposeMatrix + matrix2;
sumMatrix->print();

// Free the memory
delete transposeMatrix;
delete sumMatrix;

return 0;
}
```

## Contributing

Contributions are welcomed! If you find any issues or have suggestions for improvement, please open an issue on the repository or submit a pull request. Your feedback and contributions will be greatly appreciated. 

## License

This code is licensed under the MIT License.
