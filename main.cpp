#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <cctype>

using namespace std;

// Node class represents a node in the sparse matrix
class Node {
public:
  int row, col;
  int value;
  Node* up;
  Node* down;
  Node* left;
  Node* right;
  Node(int row = -1, int col = -1) {
    this->row = row;
    this->col = col;
    up = down = left = right = nullptr;
  }
};

// Internal class represents an internal node in the sparse matrix
class Internal : public Node {
public:
  Internal() {
    this -> value = 0;
  }
  Internal(int row = -1, int col = -1, int value = 0) : Node(row, col) {
    this->value = value;
  }
};

// Header class represents a header node in the sparse matrix
class Header : public Node {
public:
  int row, col;
  Header() {
    this -> row = 0;
    this -> col = 0;
  }
  Header(int row = -1, int col = -1) : Node(row, col) {
    this->row = row;
    this->col = col;
  }
};

// SparseMatrix class represents the sparse matrix and its operations
class SparseMatrix {
private:
  int numRows, numCols;
  Header** rowHeaders;
  Header** colHeaders;
public:
  SparseMatrix();
  SparseMatrix(int rows, int cols);
  ~SparseMatrix();
  void insert(int row, int col, int value);
  void print() const;
  SparseMatrix* transpose();
  SparseMatrix* operator+(SparseMatrix& other);
  SparseMatrix* operator-(SparseMatrix& other);
  SparseMatrix* operator*(SparseMatrix& other);
};

// Default constructor initializes the sparse matrix with 0 rows, 0 columns, and NULL headers
SparseMatrix::SparseMatrix() {
  numRows = 0;
  numCols = 0;
  colHeaders = NULL;
  rowHeaders = NULL;
}

// Constructor with parameters initializes the sparse matrix with specified rows and columns
SparseMatrix::SparseMatrix(int rows, int cols) {
  numRows = rows;
  numCols = cols;
  colHeaders = new Header*[numCols];
  rowHeaders = new Header*[numRows];

  // Create column headers and set the column index
  for (int i = 0; i < numCols; i++) {
    colHeaders[i] = new Header(-1, i);
  }
  
  // Create row headers and set the row index
  for (int i = 0; i < numRows; i++) {
    rowHeaders[i] = new Header(i, -1);
  }
}

// Destructor for freeing the memory occupied by the sparse matrix
SparseMatrix::~SparseMatrix() {
  // Delete row headers and nodes in each row
  for (int i = 0; i < numRows; i++) {
    Node* curr = rowHeaders[i]->right;
    while (curr != rowHeaders[i]) {
      Node* temp = curr;
      if (curr->right != nullptr) {
        curr = curr->right;
      }
      else {
        break;
      }
    }
    delete rowHeaders[i]; // Delete the row header
  }
  delete[] rowHeaders; // Delete the array of row headers
  
  // Delete column headers and nodes in each column
  for (int i = 0; i < numCols; i++) {
    Node* curr = colHeaders[i]->down;
    while (curr != colHeaders[i]) {
      Node* temp = curr;
      if (curr->down != nullptr) {
        curr = curr->down;
      }
      else {
        break;
      }
    }
    if (colHeaders[i] != nullptr) {
      delete colHeaders[i]; // Delete the column header
    }
  }
  delete[] colHeaders; // Delete the array of column headers
  
}

// Function for inserting a new internal node with the given row, column, and value into the sparse matrix
void SparseMatrix::insert(int row, int col, int value) {
  // Create a new interrnal node with the given row, column, and value
  Internal* node = new Internal(row, col, value);

  Node* currRowHeader = rowHeaders[row]; // Get the current row header for the given row
  Node* currColHeader = colHeaders[col]; // Get the current column header for the given column

  
  // Find the correct position in the row to insert the new node
  while (currRowHeader->right != NULL && currRowHeader->right != currRowHeader && currRowHeader->right->col < col) {
    currRowHeader = currRowHeader->right;
  }
  
  // Find the correct position in the column to insert the new node
  while (currColHeader->down != NULL && currColHeader->down != currColHeader && currColHeader->down->row < row) {
    currColHeader = currColHeader->down;
  }

  // Insert the new node into the row
  node->right = currRowHeader->right;
  currRowHeader->right = node;
  node->left = currRowHeader;

  // Update the links of the neighboring nodes
  if (node != NULL && node->right != NULL) {
    node->right->left = node;
  }

  // Insert the new node into the column
  node->down = currColHeader->down;
  currColHeader->down = node;
  node->up = currColHeader;

  // Update the links of the neighboring nodes
  if (node->down != NULL) {
    node->down->up = node;
  }
}

// Function for transposing the current matrix, creating a new matrix with swapped dimensions
SparseMatrix* SparseMatrix::transpose() {
  // Create a new matrix with the swapped dimensions
  SparseMatrix* result = new SparseMatrix(numCols, numRows);

  for (int i = 0; i < numRows; i++) {
    Node* node = rowHeaders[i]->right;
    while (node != nullptr) {
      result->insert(node->col, node->row, node->value); // Insert the transposed values into the new matrix
      node = node->right;
    }
  }
  return result; // Return the transposed matrix
}

// Operator overloading for matrix addition: Adds the first matrix and second matrix
SparseMatrix* SparseMatrix::operator+(SparseMatrix& other) {
  // Create a new SparseMatrix object for storing the result
  SparseMatrix* result = new SparseMatrix(numRows, numCols);

  for (int i = 0; i < numRows; i++) {
    Node* node1 = rowHeaders[i]->right; // Get the first node in the current row of the first matrix
    Node* node2 = other.rowHeaders[i]->right; // Get the first node in the current row of the second matrix

    while (node1 != nullptr || node2 != nullptr) {
      // If node1 is null or node2 has a smaller column index, insert node2 into the result matrix
      if (node1 == nullptr || (node2 != nullptr && node2->col < node1->col)) {
        result->insert(i, node2->col, node2->value);
        node2 = node2->right;
      }
      // If node2 is null or node1 has a smaller column index, insert node1 into the result matrix
      else if (node2 == nullptr || (node1 != nullptr && node1->col < node2->col)) {
        result->insert(i, node1->col, node1->value);
        node1 = node1->right;
      }
      // If both nodes have the same column index, add their values and insert the sum into the result matrix
      else {
        result->insert(i, node1->col, node1->value + node2->value);
        node1 = node1->right;
        node2 = node2->right;
      }
    }
  }
  return result; // Return the result matrix
}

// Operator overloading for matrix subtraction: Subtracts the second matrix from the first matrix
SparseMatrix* SparseMatrix::operator-(SparseMatrix& other) {
  // Create a new SparseMatrix object for storing the result
  SparseMatrix* result = new SparseMatrix(numRows, numCols);

  for (int i = 0; i < numRows; i++) {
    Node* node1 = rowHeaders[i]->right; // Get the first node in the current row of the first matrix
    Node* node2 = other.rowHeaders[i]->right; // Get the first node in the current row of the second matrix

    while (node1 != nullptr || node2 != nullptr) {
      // If node1 is null or node2 has a smaller column index, insert the negation of node2 into the result matrix
      if (node1 == nullptr || (node2 != nullptr && node2->col < node1->col)) {
        result->insert(i, node2->col, -1 * node2->value);
        node2 = node2->right;
      }
      // If node2 is null or node1 has a smaller column index, insert node1 into the result matrix
      else if (node2 == nullptr || (node1 != nullptr && node1->col < node2->col)) {
        result->insert(i, node1->col, node1->value);
        node1 = node1->right;
      }
      // If both nodes have the same column index, subtract their values and insert the difference into the result matrix
      else {
        result->insert(i, node1->col, node1->value - node2->value);
        node1 = node1->right;
        node2 = node2->right;
      }
    }
  }
  return result; // Return the result matrix
}

// Operator overloading for matrix multiplication: Multiplies the first matrix with the second matrix
SparseMatrix* SparseMatrix::operator*(SparseMatrix& other) {
  // Create a new SparseMatrix object for storing the result
  SparseMatrix* result = new SparseMatrix(numRows, other.numCols);

  for (int i = 0; i < numRows; i++) {
    Node* node1 = rowHeaders[i]->right; // Get the first node in the current row of the first matrix
    while (node1 != nullptr) {
      Node* node2 = other.rowHeaders[node1->col]->right; // Get the first node in the current row of the second matrix
      while (node2 != nullptr) {
        double newValue = node1->value * node2->value;
        int col = node2->col;
        Node* resNode = result->rowHeaders[i];

        // Find the correct position to insert the new node in the result matrix
        while (resNode->right != nullptr && resNode->right->col < col) {
          resNode = resNode->right;
        }

        // If a node with the same column index already exists in the result matrix, update its value by adding the new value
        if (resNode->right != nullptr && resNode->right->col == col) {
          resNode->right->value += newValue;
        }
        // If the node with the same column index does not exist insert a new node in the result matrix
        else {
          result->insert(i, col, newValue);
        }
        node2 = node2->right;
      }
      node1 = node1->right;
    }
  }
  return result; // Return the result matrix
}

// Prints the sparse matrix in a readable format
void SparseMatrix::print() const {
  for (int i = 0; i < numRows; i++) {
    Node* node = rowHeaders[i]->right; // Get the first node in the current row of the sparse matrix
    
    for (int j = 0; j < numCols; j++) {
      // If there is a node and its column index matches the current column index, print its value
      if (node != nullptr && node != rowHeaders[i] && node->col == j) {
        cout << node->value << "\t"; // Print the value of the node
        node = node->right; // Move to the next node in the row
      }
      // If the current node does not exist or its column index does not match the current column, print 0
      else {
        cout << "0\t"; // Print 0 if there is no node at the current position
      }
    }
    cout << endl; // Move to the next row
  }
}

 // This function prompts the user to enter values for a matrix and stores the non-zero values in the provided SparseMatrix object
void enterMatrixValues(int row, int col, SparseMatrix* CreateMatrix) {  
  // Initialize the matrix array with zeros
  float matrixArray[row][col];
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      matrixArray[i][j] = 0;
    }
  }
      
  string enterRowString;
  int enterRow;
  int enterCol;
  float enterVal;
  bool finished = false;
  
  while(!finished) {
    cout << "Enter the row, column, and value for entries in the matrix\n";
 
    while (!finished) {
      // Enter row
      cout << "\nEnter 'F' for Row if finished\n";
      cout << "\nRow: ";

      // Get the row input
      while (true) {
        cin >> enterRowString;

        // Check if the input is a number
        bool isNumber = true;
        for (int i = 0; i < enterRowString.length(); i++) {
          // Allow a single negative sign at the beginning
          if (i == 0 && enterRowString[i] == '-') {
            isNumber = true;
          }
          else if (!isdigit(enterRowString[i])) {
            isNumber = false;
            break;
          }
        }

        if (isNumber) {
          enterRow = stoi(enterRowString);
          if (enterRow >= row) {
            cout << "\nRow must be less than " << row << endl;
            cout << "\nRow: ";
          }
          else if (enterRow <= 0) {
            cout << "\nRow must be greater than 0\n";
            cout << "\nRow: ";
          }
          else {
            break;
          }
        }
        else if (enterRowString == "F" || enterRowString == "f") {
          finished = true;
          break;
        }
        else {
          cout << "\nInvalid input\n";
          cout << "\nPlease enter a non-zero integer\n";
          cin.clear(); // Clear the error flags
          cin.ignore(numeric_limits<streamsize>::max(), '\n');
          cout << "\nRow: ";
        }
      }

      if (finished) {
        break; // Exit the inner loop and continue with the outer loop
      }
      
      // Enter column 
      cout << "\033[1A\033[2K";
      cout << "Row: " << enterRow << " Column: ";

      //Get the column input
      while (!(cin >> enterCol) || enterCol >= col || enterCol <= 0 || cin.peek() != '\n') {
        if (enterCol >= col && !cin.fail() && cin.peek() == '\n') {
          cout << "\nColumn must be less than " << col << endl;
        }
        else if (enterCol <= 0 && !cin.fail() && cin.peek() == '\n') {
          cout << "\nColumn must be greater than 0\n";
        }
        else {
          cout << "\nInvalid input\n";
          cout << "\nPlease enter a non-zero integer\n";
        }
        
        cin.clear(); // Clear the error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
        cout << "\nRow: " << enterRow << " Column: ";
      }

      // Enter value  
      cout << "\033[1A\033[2K";
      cout << "Row: " << enterRow << " Column: " << enterCol << " Value: ";
      
      // Get the value input
      while (!(cin >> enterVal) || cin.peek() != '\n') {
        cout << "\nInvalid input\n";
        cin.clear(); // Clear the error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
        cout << "\nRow: " << enterRow << " Column: " << enterCol << " Value: ";
      }

      // Add the entered value to the matrix array
      matrixArray[enterRow][enterCol] = enterVal;

      cout << "\033[2J\033[1;1H"; // Clear the console
      cout << "Enter the row, column, and value for entries in the matrix\n";    
    }
  }
  cout << "\033[2J\033[1;1H"; // Clear the console

  // Transfer the non-zero values from the matrix array to the SparseMatrix object
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      if (matrixArray[i][j] != 0) {
        CreateMatrix->insert(i-1, j-1, matrixArray[i][j]);
      }
    }
  }
}

int main() {

  char operation;
  int row;
  int col;
  int row2;
  int col2;
  bool validInput = true;
  
  cout << "Welcome to the Sparse Matrix Calculator\n" << endl;
  cout << "What operation would you like to use:\n" << endl;
  cout << "Enter 'T' for transpose" << endl;
  cout << "Enter '+' for addition" << endl;
  cout << "Enter '-' for subtraction" << endl;
  cout << "Enter '*' for multiplication" << endl;
  cout << endl;

  // Read the operation from the user
  cin >> operation;
  cout << endl;

  do {
    validInput = true; // Assume input is valid until proven otherwise
    if (operation == 'T' || operation == 't') {
      cout << "\033[2J\033[1;1H"; // Clear the console
      cout << "You've selected transpose\n" << endl;
    }
    else if (operation == '+') {
      cout << "\033[2J\033[1;1H"; // Clear the console
      cout << "You've selected addition\n" << endl;
    }
    else if (operation == '-') {
      cout << "\033[2J\033[1;1H"; // Clear the console
      cout << "You've selected subtraction\n" << endl;
    }
    else if (operation == '*') {
      cout << "\033[2J\033[1;1H"; // Clear the console 
      cout << "You've selected multiplication\n" << endl;
    }
    else {
      cout << "You've entered an invalid operation\n" << endl;
      validInput = false; // Input is invalid, prompt again
      cin.clear(); // Clear any error flags
      cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
      cout << "Please enter a valid operation: ";
      cin >> operation;
      cout << endl;
    }
  } while (!validInput); // Loop until valid input is provided

  if (operation == 'T' || operation == 't' || operation == '+' || operation == '-') {
    // Validate user input for the row size of the matrix
    cout << "Enter the row size of the matrix: ";
    while (!(cin >> row) || row <= 0 || cin.peek() != '\n') {
      if (cin.fail() || cin.peek() != '\n') {
        cout << "\nInvalid input\n";
        cout << "\nPlease enter a positive non-zero integer: ";
        cin.clear(); // Clear the error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
      }
      else if (row <= 0) {
        cout << "\nRow size must be greater than 0\n";
        cout << "\nPlease enter a positive non-zero integer: ";
      }
    }
    cout << "\033[2J\033[1;1H"; // Clear the console 

    // Validate user input for the column size of the matrix
    cout << "Enter the column size of the matrix: ";
    while (!(cin >> col) || col <= 0 || cin.peek() != '\n') {
      if (cin.fail() || cin.peek() != '\n') {
        cout << "\nInvalid input\n";
        cout << "\nPlease enter a positive non-zero integer: ";
        cin.clear(); // Clear the error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
      }
      else if (col <= 0) {
        cout << "\nColumn size must be greater than 0\n";
        cout << "\nPlease enter a positive non-zero integer: ";
      }
    }
    cout << "\033[2J\033[1;1H"; // Clear the console

    // Increment to matrices to include the last row/column
    row += 1;
    col += 1;
  }
  else {
    enter_matrix: // Jump back if col != row2
    
    // First matrix

    // Validate user input for the row size of the first matrix
    cout << "Enter the row size of the first matrix: ";
    while (!(cin >> row) || row <= 0 || cin.peek() != '\n') {
      if (cin.fail() || cin.peek() != '\n') {
        cout << "\nInvalid input\n";
        cout << "\nPlease enter a positive non-zero integer: ";
        cin.clear(); // Clear the error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
      }
      else if (row <= 0) {
        cout << "\nRow size must be greater than 0\n";
        cout << "\nPlease enter a positive non-zero integer: ";
      }
    }
    cout << "\033[2J\033[1;1H"; // Clear the console 

    // Validate user input for the column size of the first matrix
    cout << "Enter the column size of the first matrix: ";
    while (!(cin >> col) || col <= 0 || cin.peek() != '\n') {
      if (cin.fail() || cin.peek() != '\n') {
        cout << "\nInvalid input\n";
        cout << "\nPlease enter a positive non-zero integer: ";
        cin.clear(); // Clear the error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
      }
      else if (col <= 0) {
        cout << "\nColumn size must be greater than 0\n";
        cout << "\nPlease enter a positive non-zero integer: ";
      }
    }
    cout << "\033[2J\033[1;1H"; // Clear the console

    // Second matrix

    // Validate user input for the row size of the second matrix
    cout << "Enter the row size of the second matrix: ";
    while (!(cin >> row2) || row2 <= 0 || cin.peek() != '\n') {
      if (cin.fail() || cin.peek() != '\n') {
        cout << "\nInvalid input\n";
        cout << "\nPlease enter a positive non-zero integer: ";
        cin.clear(); // Clear the error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
      }
      else if (row2 <= 0) {
        cout << "\nRow size must be greater than 0\n";
        cout << "\nPlease enter a positive non-zero integer: ";
      }
    }
    cout << "\033[2J\033[1;1H"; // Clear the console 

    // Validate user input for the column size of the second matrix
    cout << "Enter the column size of the second matrix: ";
    while (!(cin >> col2) || col2 <= 0 || cin.peek() != '\n') {
      if (cin.fail() || cin.peek() != '\n') {
        cout << "\nInvalid input\n";
        cout << "\nPlease enter a positive non-zero integer: ";
        cin.clear(); // Clear the error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
      }
      else if (col2 <= 0) {
        cout << "\nColumn size must be greater than 0\n";
        cout << "\nPlease enter a positive non-zero integer: ";
      }
    }
    cout << "\033[2J\033[1;1H";// Clear the console
    
    // Increment to matrices to include the last row/column
    row += 1;
    col += 1;
    row2 += 1;
    col2 += 1;
  }

  // Handle the case where matrix multiplication is not possible and provide options to reenter matrices or exit
  if (operation == '*' && col != row2) {
    cout << "Unable to multiply a " << row-1 << "x" << col-1 << " matrix by a " << row2-1 << "x" << col2-1 << " matrix"<< endl; 
    while (true) {
      int reenter;
      cout << "\nEnter 1 to reenter the matrices or 0 to exit: ";
      if (!(cin >> reenter)) {
        cout << "\nInvalid input\n";
        cin.clear(); // Clear the error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
      }
      else if (reenter == 1) {
        cout << "\033[2J\033[1;1H"; // Clear the console
        goto enter_matrix;
      }
      else if (reenter == 0) {
        cout << "\033[2J\033[1;1H"; // Clear the console
        return 0;
      }
      else {
        cout << "\nInvalid input\n";
      }
    }
  }
  
  // Create new SparseMatrix objects for the matrices
  SparseMatrix* FirstMatrix = new SparseMatrix(row-1, col-1); 
  SparseMatrix* SecondMatrix = new SparseMatrix(row-1, col-1);
  SparseMatrix* MultMatrix = new SparseMatrix(row2-1, col2-1);
  SparseMatrix* ResultMatrix = new SparseMatrix(row-1, col-1);

  // Prompt the user to enter matrix values and print the matrices based on the selected operation
  if (operation == 'T' || operation == 't') {
    enterMatrixValues(row, col, FirstMatrix);
    cout << "Matrix: \n" << endl;
    FirstMatrix->print();
  }
  else if (operation == '*') {
    enterMatrixValues(row, col, FirstMatrix);
    cout << "First Matrix: \n" << endl;
    FirstMatrix->print();
    cout << "\nSecond Matrix: \n" << endl;
    enterMatrixValues(row2, col2, MultMatrix);
    cout << "First Matrix: \n" << endl;
    FirstMatrix->print();
    cout << "\nSecond Matrix: \n" << endl;
    MultMatrix->print();
  }
  else if (operation == '+' || operation == '-') {
    enterMatrixValues(row, col, FirstMatrix);
    cout << "First Matrix: \n" << endl;
    FirstMatrix->print();
    cout << "\nSecond Matrix: \n" << endl;
    enterMatrixValues(row, col, SecondMatrix);
    cout << "First Matrix: \n" << endl;
    FirstMatrix->print();
    cout << "\nSecond Matrix: \n" << endl;
    SecondMatrix->print();
  }
  
  cout << endl;

  // Perform matrix operations based on the user's input and print the corresponding results
  if (operation == 'T' || operation == 't') {
    cout << "Transposed Matrix: \n" << endl;
    ResultMatrix = FirstMatrix->transpose();
    ResultMatrix->print();
  }
  else if (operation == '*') {
    cout << "First Matrix * Second Matrix: \n" << endl;
    ResultMatrix = *FirstMatrix * *MultMatrix;
    ResultMatrix->print();
  }
  else if (operation == '+') {
    cout << "First Matrix + Second Matrix: \n" << endl;
    ResultMatrix = *FirstMatrix + *SecondMatrix;
    ResultMatrix->print();
  }
  else if (operation == '-') {
    cout << "First Matrix - Second Matrix: \n" << endl;
    ResultMatrix = *FirstMatrix - *SecondMatrix;
    ResultMatrix->print();
  }
  
  // Deallocate memory for the matrices
  delete FirstMatrix;
  delete SecondMatrix;
  delete MultMatrix;
  delete ResultMatrix;

  return 0;
}
