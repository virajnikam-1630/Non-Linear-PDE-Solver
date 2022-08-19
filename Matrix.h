# pragma once
# include <iostream>
using namespace std;



class Vector{
    public:
         double *A;
         int size;
         Vector();
         Vector(int);

};

 Vector::Vector(){
    size = 0;
 }

 Vector::Vector(int s){
    size = s;
    A = new double[size];
 }



class Matrix {
public:
    double** M;
    int num_rows, num_cols;

    Matrix();
    Matrix(int, int);
    void input();
    void display();
    void setElement( int, int, double);
    double getElement( int, int);
    Matrix operator+(Matrix);
    Matrix operator-(Matrix);
    Matrix operator*(Matrix);
    Matrix operator+(double);
    Matrix operator-(double);
    Matrix operator*(double);
    Matrix operator/(double);

    friend ostream& operator<<(ostream&, Matrix&);
    friend istream& operator>>(istream&, Matrix&);

    friend Matrix operator+( double&, Matrix&);
    friend Matrix operator-( double&, Matrix&);
    friend Matrix operator*( double&, Matrix&);
    friend Matrix operator/( double&, Matrix&);

    // Define other operators as per requirement.

    friend Matrix Transpose(Matrix&);
};



Matrix ::Matrix(){
 num_rows = 0;
 num_cols = 0;
 }

Matrix :: Matrix( int r, int c){
num_rows = r;
num_cols = c;
M = new double*[num_rows];
for( int i = 0; i<num_rows; i++){
    M[i] = new double[num_cols];
}
}

void Matrix :: input(){
    cout<<"Enter the elements row-wise : "<<endl;
    for( int i = 0; i<num_rows; i++){
        for( int j = 0; j<num_cols; j++){
            cin>>M[i][j];
        }
    }
}

void Matrix ::display(){
    for( int i = 0; i<num_rows; i++){
        for( int j = 0; j<num_cols; j++){
             cout.precision(4);
             cout<<fixed;
             cout<<this->M[i][j]<<" ";
        }
        cout<<endl;
    }
}

void Matrix ::setElement( int i, int j, double x){
   M[i][j] = x;
}

double Matrix ::getElement( int i, int j){
 return M[i][j];
 }

Matrix Matrix ::operator+( Matrix B){

if( num_rows != B.num_rows || num_cols != B.num_cols ){
    cout<<"Invalid";
    abort();
}

else{
    Matrix Sum( num_rows, num_cols);

    for( int i=0; i<num_rows; i++){
        for( int j=0; j<num_cols; j++){
            Sum.M[i][j] = M[i][j] + B.M[i][j];
        }
    }
    return Sum;
}
}

Matrix Matrix ::operator-( Matrix B){
if( num_rows != B.num_rows || num_cols != B.num_cols ){
    cout<<"Invalid";
    abort();
}

else{
    Matrix Sub( num_rows, num_cols);

    for( int i=0; i<num_rows; i++){
        for( int j=0; j<num_cols; j++){
            Sub.M[i][j] = M[i][j] - B.M[i][j];
        }
    }
    return Sub;
}

}

Matrix Matrix ::operator*( Matrix B){
if( num_cols != B.num_rows){
    cout<<"Invalid";
    abort();
}

else{
    Matrix Mul( num_rows, B.num_cols);

    for( int i=0; i<num_rows; i++){
        for( int j=0; j<B.num_cols; j++){
                double x = 0;
                Mul.M[i][j] = x;
                for( int k = 0; k < num_cols; k++){
                  Mul.M[i][j] = Mul.M[i][j]+M[i][k]*B.M[k][j];
                }
        }
    }
    return Mul;
}

}

Matrix Matrix::operator+( double a){
Matrix ans(num_rows,num_cols);
for( int i = 0 ; i < num_rows; i++){
    for( int j = 0; j < num_cols; j++){
        ans.M[i][j] = M[i][j] + a;
    }
}
return ans;

}

Matrix Matrix::operator-( double a){
Matrix ans(num_rows,num_cols);
for( int i = 0 ; i < num_rows; i++){
    for( int j = 0; j < num_cols; j++){
        ans.M[i][j] = M[i][j] - a;
    }
}
return ans;

}

Matrix Matrix::operator*( double a){
Matrix ans(num_rows,num_cols);
for( int i = 0 ; i < num_rows; i++){
    for( int j = 0; j < num_cols; j++){
        ans.M[i][j] = M[i][j] * a;
    }
}
return ans;

}

Matrix Matrix::operator/( double a){
Matrix ans(num_rows,num_cols);
for( int i = 0 ; i < num_rows; i++){
    for( int j = 0; j < num_cols; j++){
        ans.M[i][j] = M[i][j] / a;
    }
}
return ans;

}

ostream& operator<<(ostream& out, Matrix& A){
for( int i = 0; i<A.num_rows; i++){
        for( int j = 0; j<A.num_cols; j++){
            out<< A.M[i][j]<<" ";
        }
        out<<endl;
    }
return out;
}

istream& operator>>(istream& in, Matrix& A){
//cout<<"Enter the elements row-wise : "<<endl;
    for( int i = 0; i<A.num_rows; i++){
        for( int j = 0; j<A.num_cols; j++){
            double number;
            in>>number;
            A.M[i][j] = number;
        }
    }
return in;
}

Matrix operator+( double& a, Matrix& A){
Matrix ans;
ans = A + a;
return ans;
}

Matrix operator-( double& a, Matrix& A){
Matrix ans;
ans = A - a;
double minus_one = -1;
ans = ans * minus_one;
return ans;
}

Matrix operator*( double& a, Matrix& A){
Matrix ans;
ans = A * a;
return ans;
}

Matrix operator/( double& a, Matrix& A){
Matrix ans(A.num_rows,A.num_cols);
for( int i = 0 ; i < A.num_rows; i++){
    for( int j = 0; j < A.num_cols; j++){
        ans.M[i][j] = A.M[i][j] / a;
    }
}
return ans;

}

Matrix Transpose(Matrix& A){
    Matrix Transpose(A.num_cols,A.num_rows);
    for( int j=0; j<A.num_cols; j++){
        for( int i=0; i<A.num_rows; i++){
            Transpose.M[j][i] = A.M[i][j];
        }
    }
    return Transpose;
}
