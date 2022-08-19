# pragma once
# include <iostream>
# include <cmath>
# include <string>
# include "AD.h"
# include "Matrix.h"
# include "LinearSolver.h"
# include "Discretizer.h"
# define MAX 1000
using namespace std;



class strList {
private:
    string elements[MAX];
public:
    strList();
    strList(int);
    strList infix_to_postfix( string Expression);
    AD Evaluate( double x, double y, int num_of_ind_var, AD u);
    void display();
    void addElement(string);
    void setElement(int, string);
    string getElement(int);
    int operatorPriority(char);
    // add member functions if you need.
};

template <class M>
class stackk{
   private:
      M elements[MAX];
      int size;
   public:
      stackk();
      bool isEmpty();
      M getTopElement();
      void push(M value);
      M pop();
      int getSize();
};


//---------------------------------------<< charList >>-------------------------------------------------------------------------------------------

strList::strList() {
    for (int i = 0; i < MAX; i++) {
        elements[i] = "";
    }
}

void strList::display() {
    for (int i = 0; elements[i] != ""; i++) {
         if (elements[i] != "$") {
            cout << elements[i]<<"  ";
        }

    }
    cout << endl;
}

void strList::addElement(string j) {
    int i = 0;
    while (elements[i] != "") {
        i++;
    }
    elements[i] = j;
}

void strList::setElement(int i, string j) {
    this->elements[i] = j;
}

string strList::getElement(int i) {
    return elements[i];
}


int strList::operatorPriority(char i) {
    if (i == '+' || i == '-') {
        return 1;
    }
    else if (i == '*' || i == '/') {
        return 2;
    }
    else if (i == '^') {
        return 3;
    }
    else if (i == '(') {
        return 4;
    }
    else if (i == ')') {
        return 5;
    }
    else if( (i >= 'a' && i <='z') || (i>= 'A' && i<='Z') || i=='_'){
       return 6;
    }
    else if( (i >= '0' && i <='9') || i== '.'){
       return 7;
    }
    else{
       return 0;
    }
}

strList strList ::infix_to_postfix( string Expression){

    string sym_ind_var[] = {"x","y","u"};

    int len = Expression.size();

    for( int i=0; i<len; i++){
       if( operatorPriority(Expression[i]) < 6 && operatorPriority(Expression[i]) != 0){
          string s = "";
          s.push_back(Expression[i]);
          addElement(s);
       }

       else if( operatorPriority(Expression[i]) == 7){
          string num = "";
          while( operatorPriority(Expression[i]) == 7 && i<len){
             num.push_back(Expression[i]);
             i++;
          }
          i--;
          addElement(num);
       }

       else if( operatorPriority(Expression[i]) == 6){
          string word = "";
          while( operatorPriority(Expression[i]) >= 6 && Expression[i] != '.' && i<len){
             word.push_back(Expression[i]);
             i++;
          }
          i--;
          addElement(word);
       }

   }


// ----Infix to Postfix---
    stackk<string> temp;
    strList postfix;

    for (int i = 0; elements[i] != ""; i++) {

       if ( operatorPriority(elements[i][0]) == 7) {    // Constant Numbers directly added to postfix
          postfix.addElement(elements[i]);
       }

       else if( operatorPriority(elements[i][0]) == 6){  // var identifier or unary operator
          int k = 0;
          for( int j=0; j<3; j++){
             if( elements[i] == sym_ind_var[j]){        // var identifier directly added to postfix
                postfix.addElement(elements[i]);
                k++;
             }
          }
          if (k == 0){
             temp.push(elements[i]);                  // unary operator pushed into stackk
          }
       }
       else if ( elements[i][0] == '(') {            // '(' pushed into stackk
          temp.push(elements[i]);
       }
       else if ( elements[i][0] == ')') {
             while ((temp.getTopElement()[0] != '(') && (!temp.isEmpty())) {
                string x = temp.pop();
                postfix.addElement(x);
             }
             if (temp.getTopElement()[0] == '(') {
                   string x = temp.pop();
                   if( operatorPriority(temp.getTopElement()[0]) == 6 ){
                      string y = temp.pop();
                     //  cout<<y<<endl;
                      postfix.addElement(y);
                   }
             }

        }
       else if (operatorPriority(elements[i][0]) < 3) {

          if (  !temp.isEmpty() && operatorPriority(elements[i][0]) <= operatorPriority(temp.getTopElement()[0]) ) {

             while ( !temp.isEmpty() && operatorPriority(elements[i][0]) <= operatorPriority(temp.getTopElement()[0]) && temp.getTopElement()[0] != '(') {
                   string x = temp.pop();
                  // cout<<x<<endl;
                   postfix.addElement(x);
             }
          }
            temp.push(elements[i]);

       }
       else if (operatorPriority(elements[i][0])==3){

                if (operatorPriority(elements[i][0]) < operatorPriority(temp.getTopElement()[0]) && !temp.isEmpty() ) {
                  //  cout<<"Inside"<<temp.getTopElement();
                   while ( !temp.isEmpty() && operatorPriority(elements[i][0]) <= operatorPriority(temp.getTopElement()[0]) && temp.getTopElement()[0] != '(') {
                      string x = temp.pop();
                      postfix.addElement(x);
                   }
             }

            temp.push(elements[i]);

         }
    }

    while (!temp.isEmpty()) {
        string x = temp.pop();
        postfix.addElement(x);
    }

    return postfix;


 }


AD strList::Evaluate( double x, double y, int num_of_ind_var, AD u) {

    AD answer;
    stackk<AD> temp;;

    string sym_ind_var[] = {"x","y","u"};
    AD x_cons(x, num_of_ind_var);
    AD y_cons(y, num_of_ind_var);
    AD ind_var[] = {x_cons, y_cons, u};

    for (int i = 0; elements[i] != ""; i++) {

   // cout<<" elements[i] : "<< elements[i]<<endl;

       if (operatorPriority(elements[i][0]) == 7) {
            double number = 0;
            int a = 0;
            int size = elements[i].size();

            for( int j=0; j<size; j++){
                if( elements[i][j] != '.'){
                      if( a == 0){
                         number = number * 10 + ( elements[i][j] - '0');
                      }
                      else{
                         number = number + (elements[i][j] - '0') * pow(10,a);
                      }
                }
                else if( elements[i][j] == '.'){
                       a--;
                }
            }
            AD cons(number, num_of_ind_var);
            temp.push(cons);
      }

       else if( operatorPriority(elements[i][0]) == 6){
          int a = 0;

          for( int j = 0; j<num_of_ind_var; j++){
             if( elements[i] == sym_ind_var[j]){
                 temp.push(ind_var[j]);
                 a++;
             }
          }

          if( a == 0){
             AD y = temp.pop();
             AD z;

             if( elements[i] == "sin"){
                z = sin(y);
                a++;
             }
             else if( elements[i] == "cos"){
                z = cos(y);
                a++;
             }
             else if( elements[i] == "tan"){
                z = tan(y);
                a++;
             }
             else if( elements[i] == "cosec"){
                z = cosec(y);
                a++;
             }
              else if( elements[i] == "sec"){
                z = sec(y);
                a++;
             }
             else if( elements[i] == "cot"){
                z = cot(y);
                a++;
             }
             else if( elements[i] == "arcsin"){
               z = arcsin(y);
               a++;
             }
             else if( elements[i] == "arccos"){
                z = arccos(y);
                a++;
             }
             else if( elements[i] == "arctan"){
                z = arctan(y);
                a++;
             }
             else if( elements[i] == "sinh"){
                z = sinh(y);
                a++;
             }
             else if( elements[i] == "cosh"){
                z = cosh(y);
                a++;
             }
             else if( elements[i] == "tanh"){
                z = tanh(y);
                a++;
             }
             else if( elements[i] == "log"){
                z = log(y);
                a++;
             }
             else if( elements[i] == "exp"){
                z = exp(y);
                a++;
             }
             else if( elements[i] == "abs"){
                z = abs(y);
                a++;
             }

             temp.push(z);
          }
          if ( a == 0){
             cout<<"Invalid";
             abort();
          }

      }

      else if( operatorPriority(elements[i][0]) <= 3) {
            AD y = temp.pop();
            AD x;
            if( !temp.isEmpty()){
               AD m = temp.pop();
               x=m;
            }
            else{
               AD m(0,num_of_ind_var);
               x=m;
            }
            AD z;

            if (elements[i][0] == '+') {
                z = x + y;
            }
            else if (elements[i][0] == '-') {
                z = x - y;
            }
            else if (elements[i][0] == '*') {
                z = x * y;
            }
            else if (elements[i][0] == '/') {
                z = x / y;
            }
            else if (elements[i][0] == '^') {
                z = x ^ y;
            }
            temp.push(z);
      }
    }
    answer = temp.getTopElement();
    return answer;
}



//----------------------------------------<< stackk >>--------------------------------------------------------------------

template <class M>
stackk<M> :: stackk(){
   size = 0;
}

template <class M>
bool stackk<M> ::isEmpty(){
   if(size==0){
      return true;
   }
   return false;
}

template <class M>
int stackk<M> ::getSize(){
   return size;
}

template <class M>
M stackk<M> ::getTopElement(){
   return elements[size-1];
}

template <class M>
void stackk<M> ::push(M value){
   size++;
   elements[size-1] = value;
}

template <class M>
M stackk<M> ::pop(){
   size--;
   return elements[size];
}






//-----------------------------------------<< Cost Function >>---------------------------------------------------------------

double Cost_Function(vector<double> x0, vector<double> x1){
      double temp = 0;
      int length = x0.size();
      for( int i = 0; i<length; i++){
        temp += pow(x0[i]-x1[i],2);
      }
      temp = sqrt(temp);
      return temp;
}





//----------------------------- EVALUATING POSTFIX FOR EXRESSIONS A,B,C,D,E,f1,f2,g1,g2 ----------------------------------------

class Postfix_All_Expressions{
   public:
        strList A_postfix;
        strList B_postfix;
        strList C_postfix;
        strList D_postfix;
        strList E_postfix;
        strList f1_postfix;
        strList f2_postfix;
        strList g1_postfix;
        strList g2_postfix;

        Postfix_All_Expressions( string A, string B, string C, string D, string E, string f1, string f2, string g1, string g2 );
};

Postfix_All_Expressions :: Postfix_All_Expressions(string A, string B, string C, string D, string E, string f1, string f2, string g1, string g2  ){
       A_postfix = A_postfix.infix_to_postfix(A);
       B_postfix = B_postfix.infix_to_postfix(B);
       C_postfix = C_postfix.infix_to_postfix(C);
       D_postfix = D_postfix.infix_to_postfix(D);
       E_postfix = E_postfix.infix_to_postfix(E);

       f1_postfix = f1_postfix.infix_to_postfix(f1);
       f2_postfix = f2_postfix.infix_to_postfix(f2);
       g1_postfix = g1_postfix.infix_to_postfix(g1);
       g2_postfix = g2_postfix.infix_to_postfix(g2);
}





//-----------------------------<< Non-Linear-Solver >>--------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------


//-----------------------------<< NEWTON-JACOBIAN >>--------------------------------------------------------------------------------

Discrete Newton_Jacobian( Postfix_All_Expressions Postfix, Discrete discretized_points, int linear_option, int iteration_count){

   cout<<"Newton_Jacobian Iteration count "<<iteration_count<<" : "<<endl;
   int num_of_ind_var = discretized_points.Points.size();

   //Setting up Boundary Points
   // At x = a or i == 0, u(a,y) = f1(y)
   for( int j=0; j<= discretized_points.n; j++){
       double x_value = discretized_points.getPoint(0,j).x_abscissa;
       double y_value = discretized_points.getPoint(0,j).y_ordinate;
       AD non_use(0,num_of_ind_var);
       AD f1_value = Postfix.f1_postfix.Evaluate(x_value, y_value, num_of_ind_var, non_use);
       discretized_points.Boundary_Points_a[j].u = f1_value;
   }

   //At x = b or i == m, u(b,y) = f2(y)
   for( int j=0; j<= discretized_points.n; j++){
       double x_value = discretized_points.getPoint(discretized_points.m,j).x_abscissa;
       double y_value = discretized_points.getPoint(discretized_points.m,j).y_ordinate;
       AD non_use(0,num_of_ind_var);
       AD f2_value = Postfix.f2_postfix.Evaluate(x_value, y_value, num_of_ind_var, non_use);
       discretized_points.Boundary_Points_b[j].u = f2_value;
   }

   // At y = c or j == 0, u(x,c) = g1(x)
   for( int i=0; i< discretized_points.m - 1; i++){
       double x_value = discretized_points.getPoint(i+1,0).x_abscissa;
       double y_value = discretized_points.getPoint(i+1,0).y_ordinate;
       AD non_use(0,num_of_ind_var);
       AD g1_value = Postfix.g1_postfix.Evaluate(x_value, y_value, num_of_ind_var, non_use);
       discretized_points.Boundary_Points_c[i].u = g1_value;
   }

   // At y = d or j == n, u(x,d) = g2(x)
   for( int i=0; i< discretized_points.m -1 ; i++){
       double x_value = discretized_points.getPoint(i+1,discretized_points.n).x_abscissa;
       double y_value = discretized_points.getPoint(i+1,discretized_points.n).y_ordinate;
       AD non_use(0,num_of_ind_var);
       AD g2_value = Postfix.g2_postfix.Evaluate(x_value, y_value, num_of_ind_var, non_use);
       discretized_points.Boundary_Points_d[i].u = g2_value;
   }

   // Set of Non-Linear Equations stored as functions array
   AD *functions;
   functions = new AD[num_of_ind_var];
   vector <double> Functions;

   // Initial x0
   vector <double> x0;
   //cout<<endl<<"x0 : "<<endl;
   for( int i = 0; i< (discretized_points.m - 1)*(discretized_points.n - 1); i++){
     x0.push_back(discretized_points.Points[i].u.getf()) ;
     //cout<<x0[i]<<"  ";
    }
    //cout<<endl;


    // Finding the Non-Linear Equations
   for( int k=0; k < num_of_ind_var; k++){

       double x_value = discretized_points.Points[k].x_abscissa;
       double y_value = discretized_points.Points[k].y_ordinate;

       AD coeff_A = Postfix.A_postfix.Evaluate(x_value, y_value, num_of_ind_var, discretized_points.Points[k].u);
       AD coeff_B = Postfix.B_postfix.Evaluate(x_value, y_value, num_of_ind_var, discretized_points.Points[k].u);
       AD coeff_C = Postfix.C_postfix.Evaluate(x_value, y_value, num_of_ind_var, discretized_points.Points[k].u);
       AD coeff_D = Postfix.D_postfix.Evaluate(x_value, y_value, num_of_ind_var, discretized_points.Points[k].u);
       AD coeff_E = Postfix.E_postfix.Evaluate(x_value, y_value, num_of_ind_var, discretized_points.Points[k].u);




       int i = discretized_points.Points[k].i;
       int j = discretized_points.Points[k].j;


       AD del_sqr_u_by_del_x_sqr = ( discretized_points.getPoint(i+1,j).u - 2 * discretized_points.getPoint(i,j).u + discretized_points.getPoint(i-1,j).u ) / ( discretized_points.delta_x * discretized_points.delta_x ) ;
       AD del_sqr_u_by_del_y_sqr = ( discretized_points.getPoint(i,j+1).u - 2 * discretized_points.getPoint(i,j).u + discretized_points.getPoint(i,j-1).u ) / ( discretized_points.delta_y * discretized_points.delta_y ) ;

       AD del_u_by_del_x = ( discretized_points.getPoint(i+1,j).u - discretized_points.getPoint(i-1,j).u ) / ( 2 * discretized_points.delta_x ) ;
       AD del_u_by_del_y = ( discretized_points.getPoint(i,j+1).u - discretized_points.getPoint(i,j-1).u ) / ( 2 * discretized_points.delta_y ) ;

       functions[k] = coeff_A * del_sqr_u_by_del_x_sqr + coeff_B * del_sqr_u_by_del_y_sqr + coeff_C * del_u_by_del_x + coeff_D * del_u_by_del_y + coeff_E;


       Functions.push_back(-1 * functions[k].getf());
   }

   //Finding Jacobian
   Matrix Jacobian = getJacobian(functions);
   //cout<<"Jacobian Matrix : "<<endl;
   //cout<<Jacobian<<endl;

   // Solving Linear Equation
    vector<double> y ;

    if( linear_option == 1){
        y = GaussElimination_solve(Jacobian,Functions);
    }
    else if( linear_option == 2){
        y = LU_Decomposition_solve(Jacobian,Functions);
    }
    else if( linear_option == 3){
        y = TriDiagonal_solve(Jacobian,Functions);
    }
    else if( linear_option == 4){
        y = Gauss_Jacobi_solve(Jacobian,Functions);
    }
    else if( linear_option == 5){
        y = Gauss_Seidal_solve(Jacobian,Functions);
    }
    else if( linear_option == 6){
        y = SOR_solve(Jacobian,Functions);
    }

   vector<double> x1(x0.size());

   for( int i = 0; i<x0.size(); i++){
      x1[i] = x0[i] + y[i];
   }

    //cout<<"x1 : "<<endl;
    //for( int i = 0; i< (discretized_points.m - 1)*(discretized_points.n - 1); i++){
     //cout<<x1[i]<<"  ";
    //}
    //cout<<endl<<endl;

    discretized_points.setValues(x1);

    cout<<"Cost Function = "<<Cost_Function(x0,x1)<<endl;
    cout<<endl<<endl;
   if( Cost_Function(x0,x1) < 0.01){
        cout<<"End of Iterations"<<endl;
       return discretized_points;
   }
   else{
    return Newton_Jacobian( Postfix, discretized_points, linear_option, iteration_count+1);
   }


}






//----------------------------------<< BROYDEN'S METHOD >>----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



Discrete Broyden_Method( Postfix_All_Expressions Postfix, Discrete discretized_points, int linear_option)
{
 int iteration_count;
 int num_of_ind_var = discretized_points.Points.size();

 vector<double> x___1(num_of_ind_var);   //previous iteration
 vector <double> x_0(num_of_ind_var);    // current iteration
 vector<double> x_1(num_of_ind_var);     // value to be found in current iteration
 

 // Set of Non-Linear Equations stored as functions array of class AD
 AD *functions;
 functions = new AD[num_of_ind_var];

 // x0 == xi
 // x1 == xi_plus_one
 // x1 of previous iteration = x_i_minus_one

 Matrix mat_Functions_0(num_of_ind_var,1);   // Col vector F(xi)
 Matrix mat_Functions__1(num_of_ind_var,1);

 Matrix mat_x_0(num_of_ind_var,1);           // Col vector xi
 Matrix mat_x__1(num_of_ind_var,1);
 Matrix mat_x_1(num_of_ind_var,1);

 Matrix mat_A_inverse_0(num_of_ind_var,num_of_ind_var);
 Matrix mat_A_inverse__1(num_of_ind_var,num_of_ind_var);


 for( iteration_count = 0; iteration_count == 0 || Cost_Function(x_0,x_1) > 0.001; iteration_count++){

   cout<<"Broyden Iteration count "<<iteration_count<<" : "<<endl;

   


   //Setting up Boundary Points
   // At x = a or i == 0, u(a,y) = f1(y)
   for( int j=0; j<= discretized_points.n; j++){
       double x_value = discretized_points.getPoint(0,j).x_abscissa;
       double y_value = discretized_points.getPoint(0,j).y_ordinate;
       AD non_use(0,num_of_ind_var);
       AD f1_value = Postfix.f1_postfix.Evaluate(x_value, y_value, num_of_ind_var, non_use);
       discretized_points.Boundary_Points_a[j].u = f1_value;
   }

   //At x = b or i == m, u(b,y) = f2(y)
   for( int j=0; j<= discretized_points.n; j++){
       double x_value = discretized_points.getPoint(discretized_points.m,j).x_abscissa;
       double y_value = discretized_points.getPoint(discretized_points.m,j).y_ordinate;
       AD non_use(0,num_of_ind_var);
       AD f2_value = Postfix.f2_postfix.Evaluate(x_value, y_value, num_of_ind_var, non_use);
       discretized_points.Boundary_Points_b[j].u = f2_value;
   }

   // At y = c or j == 0, u(x,c) = g1(x)
   for( int i=0; i< discretized_points.m - 1; i++){
       double x_value = discretized_points.getPoint(i+1,0).x_abscissa;
       double y_value = discretized_points.getPoint(i+1,0).y_ordinate;
       AD non_use(0,num_of_ind_var);
       AD g1_value = Postfix.g1_postfix.Evaluate(x_value, y_value, num_of_ind_var, non_use);
       discretized_points.Boundary_Points_c[i].u = g1_value;
   }

    // At y = d or j == n, u(x,d) = g2(x)
    for( int i=0; i< discretized_points.m -1 ; i++){
       double x_value = discretized_points.getPoint(i+1,discretized_points.n).x_abscissa;
       double y_value = discretized_points.getPoint(i+1,discretized_points.n).y_ordinate;
       AD non_use(0,num_of_ind_var);
       AD g2_value = Postfix.g2_postfix.Evaluate(x_value, y_value, num_of_ind_var, non_use);
       discretized_points.Boundary_Points_d[i].u = g2_value;
   }

   
    // Initial x0 for this iteration   ( x0 = xi)
    for( int i = 0; i< (discretized_points.m - 1)*(discretized_points.n - 1); i++){
       x_0[i] =  discretized_points.Points[i].u.getf() ;
       mat_x_0.setElement(i,0, x_0[i]);
    }

    // Finding the Non-Linear Equations
    for( int k=0; k < num_of_ind_var; k++){

       double x_value = discretized_points.Points[k].x_abscissa;
       double y_value = discretized_points.Points[k].y_ordinate;

       AD coeff_A = Postfix.A_postfix.Evaluate(x_value, y_value, num_of_ind_var, discretized_points.Points[k].u);
       AD coeff_B = Postfix.B_postfix.Evaluate(x_value, y_value, num_of_ind_var, discretized_points.Points[k].u);
       AD coeff_C = Postfix.C_postfix.Evaluate(x_value, y_value, num_of_ind_var, discretized_points.Points[k].u);
       AD coeff_D = Postfix.D_postfix.Evaluate(x_value, y_value, num_of_ind_var, discretized_points.Points[k].u);
       AD coeff_E = Postfix.E_postfix.Evaluate(x_value, y_value, num_of_ind_var, discretized_points.Points[k].u);

       int i = discretized_points.Points[k].i;
       int j = discretized_points.Points[k].j;


       AD del_sqr_u_by_del_x_sqr = ( discretized_points.getPoint(i+1,j).u - 2 * discretized_points.getPoint(i,j).u + discretized_points.getPoint(i-1,j).u ) / ( discretized_points.delta_x * discretized_points.delta_x ) ;
       AD del_sqr_u_by_del_y_sqr = ( discretized_points.getPoint(i,j+1).u - 2 * discretized_points.getPoint(i,j).u + discretized_points.getPoint(i,j-1).u ) / ( discretized_points.delta_y * discretized_points.delta_y ) ;

       AD del_u_by_del_x = ( discretized_points.getPoint(i+1,j).u - discretized_points.getPoint(i-1,j).u ) / ( 2 * discretized_points.delta_x ) ;
       AD del_u_by_del_y = ( discretized_points.getPoint(i,j+1).u - discretized_points.getPoint(i,j-1).u ) / ( 2 * discretized_points.delta_y ) ;

       functions[k] = coeff_A * del_sqr_u_by_del_x_sqr + coeff_B * del_sqr_u_by_del_y_sqr + coeff_C * del_u_by_del_x + coeff_D * del_u_by_del_y + coeff_E;

       mat_Functions_0.setElement(k,0,functions[k].getf());
   }

    // Finding A_inverse

    if ( iteration_count == 0){


       // Finding Inverse of Jacobian

       for( int i = 0; i<num_of_ind_var; i++){
          
          Matrix Jacobian = getJacobian(functions);
          vector<double> col(num_of_ind_var);   // ith column of A_inverse
          
          // creating a column vector such that only the ith entry is 1 and all other entries are 0
          vector<double> col_zeroes_one(num_of_ind_var,0);
          col_zeroes_one[i] = 1;
          

          if( linear_option == 1){
              col = GaussElimination_solve(Jacobian, col_zeroes_one);
          }
          else if( linear_option == 2){
              col = LU_Decomposition_solve(Jacobian, col_zeroes_one);
          }
           else if( linear_option == 3){
              col = TriDiagonal_solve(Jacobian, col_zeroes_one);
          }
          else if( linear_option == 4){
             col = Gauss_Jacobi_solve(Jacobian, col_zeroes_one);
          }
          else if( linear_option == 5){
             col = Gauss_Seidal_solve(Jacobian, col_zeroes_one);
          }
          else if( linear_option == 6){
             col = SOR_solve(Jacobian, col_zeroes_one);
          }



          // So the Linear Solver will give the ith col of Inverse Jacobian Matrix
          for(int j = 0; j<num_of_ind_var; j++){
             mat_A_inverse_0.M[j][i] = col[j]; 
          }
       }
    }
   
    else{
       //Finding A_inverse using -> A_inverse_i_minus_1 ,  s_i , y_i
       Matrix mat_s_0 = mat_x_0 - mat_x__1;
       Matrix mat_y_0 = mat_Functions_0 - mat_Functions__1;

       Matrix mat_s_0_transpose = Transpose(mat_s_0);
       Matrix mat_Term = mat_s_0_transpose * mat_A_inverse__1 * mat_y_0;
       
       double term = mat_Term.getElement(0,0);

       mat_A_inverse_0 = mat_A_inverse__1 + ( mat_s_0 - mat_A_inverse__1 * mat_y_0 ) * (mat_s_0_transpose * mat_A_inverse__1) / (term) ;    
      
    }


    // x1 = x0 - A_inverse * F(x0)

    mat_x_1 = mat_x_0 -  mat_A_inverse_0 * mat_Functions_0;

    
    for( int i = 0; i<num_of_ind_var; i++){
       x_1[i] = mat_x_1.getElement(i,0);
    }


    //Setting up values for next iteration
    discretized_points.setValues(x_1);

    for( int i = 0; i<num_of_ind_var; i++){
          mat_x__1.M[i][0] = x_0[i]; 
    }
    
    for( int i = 0; i<num_of_ind_var; i++){
          mat_Functions__1.M[i][0] = mat_Functions_0.M[i][0]; 
    } 

    for( int i = 0; i<num_of_ind_var; i++){
       for( int j = 0; j<num_of_ind_var; j++){
         mat_A_inverse__1.M[i][j] = mat_A_inverse_0.M[i][j];
       }
    }

    

    cout<<"Cost Function = "<<Cost_Function(x_0,x_1)<<endl;
    cout<<endl<<endl;

 }

 return discretized_points;



 }

































