import matplotlib.pyplot as plt
import numpy as np

x1=0
y1=0
x2=2
y2=4
xp=[x1,x2]
yp=[y1,y2]
plt.scatter(xp, yp, marker = 'x',color = 'red', s = 40 ,label = 'point')
plt.legend(loc = 'best')
a=y1-y2
b=x2-x1
c=x1*y2-x2*y1
x=np.linspace(x1,x2,100)
y=(-a*x-c)/b
plt.plot(x,y)
plt.show()
