# pragma once
# include <cmath>
# include <iostream>
# include <vector>
# include "Matrix.h"
using namespace std;




 class AD{
    private:
         double f;
         Vector df;
         int id;

    public:
         AD();
         AD(double,int, int);
         AD(double, int);
         void change_value_of_ind_var(double value);

         double getf();
         double getDF(int);
         vector<double> getGradient();
         friend Matrix getJacobian(AD*);

         AD operator+(AD);
         AD operator-(AD);
         AD operator*(AD);
         AD operator/(AD);
         AD operator^(AD);

        // c+f, c-f,etc
	    friend AD operator+(double,AD);
	    friend AD operator*(double,AD);
	    friend AD operator*(double,AD);
	    friend AD operator/(double,AD);
	    friend AD operator^(double,AD);

	    // f+c, f-c, f*c, f/c.

	    AD operator+(double);
	    AD operator-(double);
	    AD operator*(double);
	    AD operator/(double);
	    AD operator^(double);


         friend AD sin(AD);
         friend AD cos(AD);
         friend AD tan(AD);
         friend AD cosec(AD);
         friend AD sec(AD);
         friend AD cot(AD);
         friend AD arcsin(AD);
         friend AD arccos(AD);
         friend AD arctan(AD);
         friend AD sinh(AD);
         friend AD cosh(AD);
         friend AD tanh(AD);
         friend AD log(AD);
         friend AD exp(AD);
         friend AD abs(AD);

};



 AD ::AD(){
     f = 0;
}

 AD ::AD(double value, int id, int size){
      this->f = value;
     this->id = id;
     this->df = Vector(size);
     for( int i = 0; i<size; i++){
         this->df.A[i] = 0;
     }
     this->df.A[id] = 1;
}

 AD ::AD(double value, int size){
     this->f = value;
     this->df = Vector(size);
     for( int i = 0; i<size; i++){
         this->df.A[i] = 0;
     }
 }

 void AD::change_value_of_ind_var(double value){
     this->f = value;
 }



 double AD::getf(){
     return this->f;
 }

 double AD::getDF(int index){
     return this->df.A[index];
 }

 vector<double> AD:: getGradient(){
     vector<double> gradient(this->df.size);
     for(int i = 0; i<this->df.size; i++){
         gradient.push_back(this->df.A[i]);
     }
     return gradient;
 }

 Matrix getJacobian(AD funList[]){
    //  cout<<"Inside JAcobian"<<endl;
     int n = funList[0].df.size;
     Matrix M(n,n);
     for(int i = 0; i<n; i++){
         for(int j = 0; j<n; j++){
             M.M[i][j] = funList[i].getDF(j);
         }
     }
     return M;
 }


 AD AD::operator+(AD g){
     AD h;
     h.f = this->f + g.f;
     h.df = Vector(this->df.size);
     for( int i=0; i<this->df.size; i++){
         h.df.A[i] = this->df.A[i] + g.df.A[i];
     }
     return h;
 }

 AD AD::operator-(AD g){
     AD h;
     h.f = this->f - g.f;
     h.df = Vector(this->df.size);
     for( int i=0; i<this->df.size; i++){
         h.df.A[i] = this->df.A[i] - g.df.A[i];
     }
     return h;
 }

 AD AD::operator*(AD g){
     AD h;
     h.f = this->f * g.f;
     h.df = Vector(this->df.size);
     for( int i=0; i<this->df.size; i++){
         h.df.A[i] = this->df.A[i] * g.f + this->f * g.df.A[i] ;
     }
     return h;
 }

 AD AD::operator/(AD g){
     AD h;
     h.f = this->f / g.f;
     h.df = Vector(this->df.size);
     for( int i=0; i<this->df.size; i++){
         h.df.A[i] = (this->df.A[i] * g.f - this->f * g.df.A[i]) / (g.f*g.f) ;
     }
     return h;
 }

 AD AD::operator^(AD g){
     AD h;
     h.f = pow(this->f,g.f);
     h.df = Vector(this->df.size);
     for( int i=0; i<this->df.size; i++){
         h.df.A[i] = h.f * ( g.f * this->df.A[i] / this->f + g.df.A[i] * log(abs(this->f)) ) ;
     }
     return h;
 }

 AD AD::operator+(double c){
     AD h;
     h.f = this->f + c;
     h.df = this->df;
     return h;
 }

 AD AD::operator-(double c){
     AD h;
     h.f = this->f - c;
     h.df = this->df;
     return h;
 }

 AD AD::operator*(double c){
     AD h;
     h.f = this->f * c;
     h.df = Vector(this->df.size);
     for( int i=0; i<this->df.size; i++){
         h.df.A[i] = this->df.A[i] * c ;
     }
     return h;
 }

  AD AD::operator/(double c){
     AD h;
     h.f = this->f / c;
     h.df = Vector(this->df.size);
     for( int i=0; i<this->df.size; i++){
         h.df.A[i] = this->df.A[i] / c ;
     }
     return h;
 }

   AD AD::operator^(double c){
     AD h;
     h.f = pow(this->f,c);
     h.df = Vector(this->df.size);
     for( int i=0; i<this->df.size; i++){
         h.df.A[i] = c * pow(this->f,c-1) * this->df.A[i] ;
     }
     return h;
 }

 AD operator+(double c, AD g){
    AD h;
    h = g + c;
    return h;
 }

 AD operator-(double c, AD g){
    AD h;
    h = g - c;
    h = h * (-1) ;
    return h;
 }

 AD operator*(double c, AD g){
    AD h;
    h = g * c;
    return h;
 }

 AD operator/(double c, AD g){
    AD h;
    h.f = c / g.f;
    h.df = Vector(g.df.size);
     for( int i=0; i<g.df.size; i++){
         h.df.A[i] = (-1) * c / pow(g.f,2) * g.df.A[i];
     }
     return h;
 }

  AD operator^(double c, AD g){
    AD h;
    h.f = pow(c,g.f);
    h.df = Vector(g.df.size);
     for( int i=0; i<g.df.size; i++){
         h.df.A[i] = log(abs(g.f)) * pow(c,g.f) * g.df.A[i];
     }
     return h;
 }





 AD sin(AD g){
     AD h;
     h.f = sin(g.f);
     h.df = Vector(g.df.size);
     for( int i = 0; i<g.df.size; i++){
         h.df.A[i] = cos(g.f) * g.df.A[i];
     }
     return h;
 }

 AD cos(AD g){
     AD h;
     h.f = cos(g.f);
     h.df = Vector(g.df.size);
     for( int i = 0; i<g.df.size; i++){
         h.df.A[i] = -1 * sin(g.f) * g.df.A[i];
     }
     return h;
 }

 AD tan(AD g){
     AD h;
     h.f = tan(g.f);
     h.df = Vector(g.df.size);
     for( int i = 0; i<g.df.size; i++){
         h.df.A[i] = (1/cos(g.f)) * (1/cos(g.f)) * g.df.A[i];
     }
     return h;
 }

 AD cosec(AD g){
     AD h;
     h.f = 1/sin(g.f);
     h.df = Vector(g.df.size);
     for( int i = 0; i<g.df.size; i++){
         h.df.A[i] =  -1 * (1/sin(g.f)) * (1/tan(g.f)) * g.df.A[i] ;
     }
     return h;
 }

 AD sec(AD g){
     AD h;
     h.f = 1/cos(g.f);
     h.df = Vector(g.df.size);
     for( int i = 0; i<g.df.size; i++){
         h.df.A[i] =  (1/sin(g.f)) * tan(g.f) * g.df.A[i] ;
     }
     return h;
 }

 AD cot(AD g){
     AD h;
     h.f = 1/tan(g.f);
     h.df = Vector(g.df.size);
     for( int i = 0; i<g.df.size; i++){
         h.df.A[i] =  -1 * (1/sin(g.f)) * (1/sin(g.f)) * g.df.A[i] ;
     }
     return h;
 }

 AD arcsin(AD g){
     AD h;
     h.f = asin(g.f);
     h.df = Vector(g.df.size);
     for( int i = 0; i<g.df.size; i++){
         h.df.A[i] =  1 / sqrt(1 - g.f * g.f) * g.df.A[i] ;
     }
     return h;
 }

 AD arccos(AD g){
     AD h;
     h.f = acos(g.f);
     h.df = Vector(g.df.size);
     for( int i = 0; i<g.df.size; i++){
         h.df.A[i] =  -1 / sqrt(1 - g.f * g.f) * g.df.A[i] ;
     }
     return h;
 }

 AD arctan(AD g){
     AD h;
     h.f = atan(g.f);
     h.df = Vector(g.df.size);
     for( int i = 0; i<g.df.size; i++){
         h.df.A[i] =  1 / (1 + g.f * g.f) * g.df.A[i] ;
     }
     return h;
 }

 AD sinh(AD g){
     AD h;
     h.f = sinh(g.f);
     h.df = Vector(g.df.size);
     for( int i = 0; i<g.df.size; i++){
         h.df.A[i] = cosh(g.f) * g.df.A[i];
     }
     return h;
 }

 AD cosh(AD g){
     AD h;
     h.f = cosh(g.f);
     h.df = Vector(g.df.size);
     for( int i = 0; i<g.df.size; i++){
         h.df.A[i] = sinh(g.f) * g.df.A[i];
     }
     return h;
 }

 AD tanh(AD g){
     AD h;
     h.f = tanh(g.f);
     h.df = Vector(g.df.size);
     for( int i = 0; i<g.df.size; i++){
         h.df.A[i] = -1 * (1/sinh(g.f)) * (1/sinh(g.f)) * g.df.A[i];
     }
     return h;
 }

 AD log(AD g){
     AD h;
     h.f = log(g.f);
     h.df = Vector(g.df.size);
     for( int i = 0; i<g.df.size; i++){
         h.df.A[i] = (1 / g.f) * g.df.A[i];
     }
     return h;
 }

 AD exp(AD g){
     AD h;
     h.f = exp(g.f);
     h.df = Vector(g.df.size);
     for( int i = 0; i<g.df.size; i++){
         h.df.A[i] = exp(g.f) * g.df.A[i];
     }
     return h;
 }

 AD abs(AD g){
     AD h;
     h.f = abs(g.f);
     h.df = Vector(g.df.size);
     for( int i = 0; i<g.df.size; i++){
         h.df.A[i] = (g.f / abs(g.f)) * g.df.A[i];
     }
     return h;
 }













