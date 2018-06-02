import numpy as np
from gauss import *

h=3
a=(np.random.random(h*h)*10)//1
b=(np.random.random(h)*10)//1

print(eliminate(a,b))
