import numpy as np
from gauss import *
from time import time
'''
h=1500
a=((np.random.random(h*h))).reshape(h,h)
b=(np.random.random(h))
start=time()
tmp=np.linalg.solve(a,b)
print(time()-start)
start=time()
tmp=eliminate(a,b)
print(time()-start)
'''
h=2600
a1=np.random.random(h*h)
a2=np.random.random(h*h)
b1=np.random.random(h)
b2=np.random.random(h)
a=np.array(list(map(lambda x:complex(*x),zip(a1,a2))),dtype="cdouble").reshape(h,h)
b=np.array(list(map(lambda x:complex(*x),zip(b1,b2))),dtype="cdouble").reshape(h)

start=time()
eliminate(a,b)
print(time()-start)
start=time()
np.linalg.solve(a,b)
print(time()-start)
