# pragma once
# include <iostream>
# include <vector>
# include "AD.h"
# include "Matrix.h"
using namespace std;

class Discrete_Point{
     public:
         int i;
         int j;
         double x_abscissa;
         double y_ordinate;
         AD u;
         Discrete_Point();
         Discrete_Point( int, int, double, double);
};

Discrete_Point ::Discrete_Point(){
     i = 0;
     j = 0;
     x_abscissa = 0;
     y_ordinate = 0;
}

Discrete_Point :: Discrete_Point(int i1, int j1, double x, double y){
     i = i1;
     j = j1;
     x_abscissa = x;
     y_ordinate = y;
     AD u();
}

class Discrete{
     public:
         int m;    // i from 0 to m   total m rows
         int n;    // j from 0 to n   total n cols
         double a,b,c,d;
         double delta_x;
         double delta_y;

         vector <Discrete_Point> Points;
         vector <Discrete_Point> Boundary_Points_a;
         vector <Discrete_Point> Boundary_Points_b;
         vector <Discrete_Point> Boundary_Points_c;
         vector <Discrete_Point> Boundary_Points_d;
         Discrete( double, double, double, double, int, int);
         Discrete_Point getPoint(int, int);
         Discrete_Point givePoint(int);    // (m+1)(n+1) number of points  from 0 to [(m+1)(n+1) - 1]
         void setValues(vector<double> values);

};

Discrete :: Discrete(double a1, double b1, double c1, double d1, int m1, int n1){
     m = m1;
     n = n1;
     a = a1;
     b = b1;
     c = c1;
     d = d1;
     delta_x = (b-a)/m;
     delta_y = (d-c)/n;

     for(int i=0; i<=m; i++){
         for( int j=0; j<=n; j++){
             if( i == 0){
                Discrete_Point temp( i, j, a + delta_x * i, c + delta_y * j );
                AD temp2(0, (m-1)*(n-1));
                temp.u = temp2;
                Boundary_Points_a.push_back(temp);
             }
             else if( i == m){
                Discrete_Point temp( i, j, a + delta_x * i, c + delta_y * j );
                AD temp2(0, (m-1)*(n-1));
                temp.u = temp2;
                Boundary_Points_b.push_back(temp);

             }
             else if( j == 0){
                Discrete_Point temp( i, j, a + delta_x * i, c + delta_y * j );
                AD temp2(0, (m-1)*(n-1));
                temp.u = temp2;
                Boundary_Points_c.push_back(temp);

             }
             else if( j == n){
                Discrete_Point temp( i, j, a + delta_x * i, c + delta_y * j );
                AD temp2(0, (m-1)*(n-1));
                temp.u = temp2;
                Boundary_Points_d.push_back(temp);

             }
             else{
                Discrete_Point temp( i, j, a + delta_x * i, c + delta_y * j );
                AD temp2(0, (i-1)*(n-1) + j-1, (m-1)*(n-1));
                temp.u = temp2;
                Points.push_back(temp);
             }
         }
     }



}

Discrete_Point Discrete :: getPoint(int i1, int j1){

    if( i1 == 0){
         return Boundary_Points_a[j1];
    }
    else if( i1 == m){
         return Boundary_Points_b[j1];
    }
    else if( j1 == 0){
         return Boundary_Points_c[i1 - 1];
    }
    else if( j1 == n){
         return Boundary_Points_d[i1 - 1];
    }
    else{
      int k = (i1-1)*(n-1) + j1 - 1 ;
      return Points[k];
    }
}

Discrete_Point Discrete ::givePoint(int k){
      if( k < (m-1)*(n-1) ){
           return Points[k];
      }
      return Points[k];
}

void Discrete :: setValues( vector<double> values){
     for( int k = 0; k < Points.size(); k++){
        Points[k].u.change_value_of_ind_var(values[k]);
     }
}

