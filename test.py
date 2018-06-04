import numpy as np
from gauss import *
from time import time
<<<<<<< HEAD
'''
h=1500
a=((np.random.random(h*h))).reshape(h,h)
b=(np.random.random(h))
=======
h=2000
a=((np.random.random(h*h)*10)//1).reshape(h,h)
b=(np.random.random(h)*10)//1
>>>>>>> 5414dcd7ee34026e83a26dc57bd0ad241558fd6e
start=time()
tmp=np.linalg.solve(a,b)
print(time()-start)
start=time()
tmp=eliminate(a,b)
print(time()-start)
<<<<<<< HEAD
'''
h=3
a1=np.random.random(h*h)
a2=np.random.random(h*h)
b1=np.random.random(h)
b2=np.random.random(h)
a=np.array(list(map(lambda x:complex(*x),zip(a1,a2)))).reshape(h,h)
b=np.array(list(map(lambda x:complex(*x),zip(b1,b2)))).reshape(h)

print(np.linalg.solve(a,b))
print(eliminate(a,b))
=======
>>>>>>> 5414dcd7ee34026e83a26dc57bd0ad241558fd6e
