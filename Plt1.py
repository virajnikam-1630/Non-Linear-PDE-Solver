# -*- coding: utf-8 -*-
"""
Created on Thu Oct  7 12:17:11 2021

@author: hp
"""

import matplotlib.pyplot as plt
import numpy as np
from csv import reader


x_values = np.array([])
y_values = np.array([])
u_values = np.array([])



with open('Points_1.csv','r') as f:
    csv_reader = reader(f)
    
    for row in csv_reader:
        if row[0] != 'x':
         x_values = np.append(x_values,float(row[0]))
        if row[1] != 'y':
         y_values = np.append(y_values,float(row[1]))
        if row[2] != 'z':
         u_values = np.append(u_values,float(row[2]))   
         

X = x_values.reshape((21,21))
       
Y = y_values.reshape((21,21))

Z = u_values.reshape((21,21))

fig = plt.figure()
ax = fig.add_subplot(111,projection='3d')

#ax.plot_surface(X,Y,Z)
ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap= "plasma", linewidth=0, antialiased=False)
ax.set(xlabel = 'X', ylabel='Y',zlabel='U(x,y)', title = 'Q1 Surface of Function U(x,y) vs x & y')
plt.show()

         