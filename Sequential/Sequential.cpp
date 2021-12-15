#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

// a class for storing values
class Matrix {
public:
    int row;
    int column;
    double* elements;
};

Matrix readFile(const char* filename);
Matrix multiplySequential(Matrix A, Matrix x);
void writeFile(Matrix y, string filename);

int main(int argc, char* argv[])
{
    auto t_start = std::chrono::high_resolution_clock::now();

    Matrix A = readFile("BigA.txt");
    Matrix x = readFile("BigX.txt");
    Matrix y = multiplySequential(A, x);

    writeFile(y, "y.txt");
    
    auto t_end = std::chrono::high_resolution_clock::now();
    double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();

    cout << "elapsed time: " << elapsed_time_ms << endl;

}

Matrix multiplySequential(Matrix A, Matrix x) {

    Matrix y;
    y.column = A.column;
    y.row = x.row;
    int dim = y.column;

    double* result = new double[y.column * y.row];

    for (int i = 0; i < A.column; i++) {

        double localresult = 0;
        for (int j = 0; j < A.row; j++) {
            localresult += x.elements[j] * A.elements[i * dim + j];
        }
        result[i] = localresult;
    }
    
    y.elements = result;
    return y;
}

Matrix readFile(const char* filename) {

    Matrix m;

    std::ifstream fin(filename);
    std::string line;
    double data_in;

    // open the input stream
    if (!fin) {
        cout << filename << " not found!" << endl;
        exit(0);
    }

    std::getline(fin, line);
    std::stringstream stream(line);

    stream >> data_in;
    int col = (int)data_in;

    stream >> data_in;
    int row = (int)data_in;

    double* in_matrix = (double*)malloc(col * row * sizeof(double));

    cout << filename << " col: " << col << " row: " << row << endl;

    for (int i = 0; i < col; i++) {
        std::getline(fin, line);
        std::stringstream stream(line);
        for (int j = 0; j < row; j++) {
            stream >> data_in;
            *(in_matrix + (i * row) + j) = data_in;
        }
    }

    m.column = col;
    m.row = row;
    m.elements = in_matrix;
    return m;
}

void writeFile(Matrix y, string filename) {

    string content;
    for (int i = 0; i < y.column; i++) {
        content.append("   " + std::to_string(y.elements[i]) + "\n");
    }

    ofstream result(filename);
    result << content;
    result.close();
}
