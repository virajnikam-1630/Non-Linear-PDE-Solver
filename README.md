# Non-Linear-PDE-Solver
This project helps to automatically solve a Partial Differential Equation with given boundary conditions using discretization


## Method
Solving a given Partial Differential Equation (PDE) involves the following steps -
  1. In put the PDE with given boundary conditions
  2. Discretization of PDE using finite difference method.<br>
     Generate the system of nonlinear algebraic equations.
  3. Solve the system of nonlinear algebraic equations using Newton's or Broyden's Method. <br>
     We let the user choose which method to use.
  4. Solve the system of linear equations.<br>
     We let the user to choose any method from -
        i. Gauss elimination
        ii. Gauss Jacobi
        iii. Gauss Seidal
        iv. LU Decomposition
        v. Tri Diagonal
        vi. SOR
  5. Plot the results as a surface plot (Using python)
        
     

![me18b082_assgn_8_Q3](https://user-images.githubusercontent.com/108379478/185564264-a50d2d90-6625-41d5-8fd9-5d5c9ec16ed8.png)

