# include <iostream>
# include <fstream>
# include "AD.h"
# include "Discretizer.h"
# include "LinearSolver.h"
# include "Non_linear.h"
# include "Matrix.h"
# include <iomanip>
using namespace std;







// --------------------------<< main() >>---------------------------------------------------------------------------------------

int main(){


    cout<<"General form of the nonlinear PDE : "<<endl
        <<"    A(x,y,u)*d2u/dx2 + B(x,y,u)*d2u/dy2 + C(x,y,u)*du/dx + D(x,y,u)*du/dy + E(x,y,u) = 0 "<<endl<<endl;

    cout<<"Enter the functions A, B, C, D, E serially in terms of 'u', 'x', 'y' : "<<endl;
    string A,B,C,D,E;
    cin>>A>>B>>C>>D>>E;

    cout<<"Enter the boundary points a,b,c,d : "<<endl;
    double a,b,c,d;
    cin>>a>>b>>c>>d;

    cout<<"Enter the boundary conditions f1(x),f2(x),g1(y),g2(y) serially in terms of 'u', 'x', 'y' : "<<endl;
    string f1,f2,g1,g2;
    cin>>f1>>f2>>g1>>g2;

    int m1,n1;
    cout<<"Enter the number of points along x axis ,i.e., m : "<<endl;
    cin>>m1;
    cout<<"Enter the number of points along y axis ,i.e., n : "<<endl;
    cin>>n1;

    int non_linear_option;
    cout<<"Enter 1 for Newton's Method"<<endl<<"OR Enter 2 for Broyden's Method : "<<endl;
    cin>>non_linear_option;

    int linear_option;
    cout<<"Enter Linear_option"<<endl;
    cout<<"Enter 1 for Gauss Elimination"<<endl;
    cout<<"Enter 2 for LU Decomposition"<<endl;
    cout<<"Enter 3 for TriDiagonal Method"<<endl;
    cout<<"Enter 4 for Gauss Jacobi Method"<<endl;
    cout<<"Enter 5 for Gauss-Seidel Method"<<endl;
    cout<<"Enter 6 for Successive Over-Relaxation Method"<<endl;
    cin>>linear_option;

     //Evaluating Postfix for all Expressions
     Postfix_All_Expressions Postfix(A,B,C,D,E,f1,f2,g1,g2);
     
     // Discretization
     Discrete discretized_points(a, b, c, d, m1, n1);

     // Setting Initial Guess
     vector<double> initial_guess;
     for ( int k=0; k<(m1-1)*(n1-1); k++){
         initial_guess.push_back(0.1);       // Initial guess 0.01 for all entries
     }

     discretized_points.setValues(initial_guess);


     if( non_linear_option == 1){
         // NEWTON'S METHOD TO SOLVE NON-LINEAR SYSTEM

         discretized_points = Newton_Jacobian( Postfix, discretized_points, linear_option, 0);
         cout<<endl<<"Answer = "<<endl;
         cout<<setw(25)<< left
             <<"x"
             <<setw(25)<< left
             <<"y"
             <<setw(25)<< left
             <<"u"<<endl<<endl;
         for( int i = 0; i<= m1; i++ ){
           for( int j = 0; j<=n1; j++){
                 cout<<setw(25)<< left
                     <<discretized_points.getPoint(i,j).x_abscissa
                     <<setw(25)<< left
                     <<discretized_points.getPoint(i,j).y_ordinate
                     <<setw(25)<< left
                     <<discretized_points.getPoint(i,j).u.getf()
                     <<"\n";
           }
     }

     }

     if( non_linear_option == 2){
         // BROYDEN'S METHOD

        //  Matrix dummy_col_vector( (m1-1)*(n1-1) ,1);              // Used only as dummy input for first iteration
        //  Matrix dummy_A( (m1-1)*(n1-1), (m1-1)*(n1-1) );         // Used only as dummy input for first iteration

         discretized_points = Broyden_Method( Postfix, discretized_points,linear_option);
         cout<<endl<<"Answer = "<<endl;         
         cout<<setw(25)<< left
             <<"x"
             <<setw(25)<< left
             <<"y"
             <<setw(25)<< left
             <<"u"<<endl<<endl;
         for( int i = 0; i<= m1; i++ ){
           for( int j = 0; j<=n1; j++){
                 cout<<setw(25)<< left
                     <<discretized_points.getPoint(i,j).x_abscissa
                     <<setw(25)<< left
                     <<discretized_points.getPoint(i,j).y_ordinate
                     <<setw(25)<< left
                     <<discretized_points.getPoint(i,j).u.getf()
                     <<endl;
           }
     }

     }


     // Output to a CSV file
     fstream fout;
     fout.open("Points.csv", ios::out | ios::app);

     for( int i = 0; i<= m1; i++ ){
           for( int j = 0; j<=n1; j++){
                 fout<<discretized_points.getPoint(i,j).x_abscissa<<","
                     <<discretized_points.getPoint(i,j).y_ordinate<<","
                     <<discretized_points.getPoint(i,j).u.getf()<<","
                     <<"\n";
           }
     }


 return 0;
}
