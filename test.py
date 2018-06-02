import numpy as np
from gauss import *
from time import time
h=2000
a=((np.random.random(h*h)*10)//1).reshape(h,h)
b=(np.random.random(h)*10)//1
start=time()
tmp=np.linalg.solve(a,b)
print(time()-start)
start=time()
tmp=eliminate(a,b)
print(time()-start)
