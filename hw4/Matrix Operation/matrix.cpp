#include <iostream>

using namespace std;
class Matrix {
public:
    Matrix();
    // Construct an identity matrix.
    Matrix(long long N) {
        this->n = N;
        this->data = new long long *[N];
        for (long long i = 0; i < N; i++) {
            this->data[i] = new long long[N];
            for (long long j = 0; j < N; j++) {
                if (i == j) {
                    this->data[i][j] = 1;
                } else {
                    this->data[i][j] = 0;
                }
            }
        }
    }
    // Construct a matrix with the given data.
    Matrix(long long N, long long **Data) {
        this->n = N;
        this->data = new long long *[N];
        for (long long i = 0; i < N; i++) {
            this->data[i] = new long long[N];
            for (long long j = 0; j < N; j++) {
                this->data[i][j] = Data[i][j];
            }
        }
    }
    ~Matrix() {
        for (long long i = 0; i < this->n; i++) {
            delete[] this->data[i];
        }
        delete[] this->data;
    };
    // Print out the matrix.
    void print() {
        for (long long i = 0; i < this->n; i++) {
            for (long long j = 0; j < this->n; j++) {
                cout << this->data[i][j] << " ";
            }
            cout << endl;
        }
    }

    // TODO:
    void add(const Matrix &rhs);
    void subtract(const Matrix &rhs);
    void multiply(const Matrix &rhs);
    void transpose();
    void power(long long x);

private:
    long long n;
    long long **data;
};

void Matrix::add(const Matrix &rhs){
    for(long long i = 0; i<n;i++){
        for(long long j = 0; j<n; j++){
            data[i][j] += rhs.data[i][j];
        }
    }
}

void Matrix::subtract(const Matrix &rhs){
    for(long long i = 0; i<n;i++){
        for(long long j = 0; j<n; j++){
            data[i][j] -= rhs.data[i][j];
        }
    }
}


void Matrix::multiply(const Matrix &rhs){
    long long **temp = new long long*[n]();
    for (long long i = 0; i < n; i++) {
        temp[i] = new long long[n]();
    }

    for (long long i = 0; i < n; i++) {
        for (long long k = 0; k < n; k++) {
            for (long long j = 0; j < n; j++) {
                temp[i][j] += data[i][k] * rhs.data[k][j];
            }
        }
    }

    for (long long i = 0; i < n; i++) {
        for (long long j = 0; j < n; j++) {
            data[i][j] = temp[i][j];
        }
        delete[] temp[i];
    }
    delete[] temp;
}

void Matrix::transpose(){
    for(long long i = 0; i<n;i++){
        for(long long j = i + 1; j<n; j++){
            swap(data[i][j], data[j][i]);
        }
    }
}

void Matrix::power(long long x) {

    Matrix result(n); // identity -> fot x = 1

    long long **temp = new long long*[n]();
    for (long long i = 0; i < n; i++) {
        temp[i] = new long long[n]();

        for(long long j = 0; j<n; j++){
            temp[i][j] = data[i][j];
        }
    }

    Matrix base(n, temp);
    for (long long i = 0; i < n; i++) delete[] temp[i];
    delete[] temp;

    // binary shift
    while (x > 0) {
        if (x & 1)
            result.multiply(base); 

        base.multiply(base);
        x >>= 1;
    }

    for (long long i = 0; i < n; i++)
        for (long long j = 0; j < n; j++)
            data[i][j] = result.data[i][j];
}