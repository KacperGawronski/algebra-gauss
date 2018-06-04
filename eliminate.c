#include <Python.h>
#include <numpy/arrayobject.h>
#include "eliminate.h"

void swap_pointers(void **a, void **b){
	void **tmp;
	tmp=a;
	a=b;
	b=tmp;
}

#include "eliminate_simple.h"
#include "eliminate_complex.h"

GENERATE_ELIMINATION(npy_float,NPY_FLOAT,FLT_EPSILON) 
GENERATE_ELIMINATION(npy_double,NPY_DOUBLE,FLT_EPSILON) 
GENERATE_ELIMINATION(npy_longdouble,NPY_LONGDOUBLE,DBL_EPSILON) 

GENERATE_COMPLEX_ELIMINATION(npy_cfloat,NPY_CFLOAT,FLT_EPSILON,npy_float)
GENERATE_COMPLEX_ELIMINATION(npy_cdouble,NPY_CDOUBLE,FLT_EPSILON,npy_double)
GENERATE_COMPLEX_ELIMINATION(npy_clongdouble,NPY_CLONGDOUBLE,DBL_EPSILON,npy_longdouble)


PyObject* eliminate_cfunc (PyObject *dummy, PyObject *args){
	PyObject *raw_arr,*raw_vals;
	import_array();
	
	if (!PyArg_ParseTuple(args,"OO",&raw_arr,&raw_vals)) return NULL;

	switch(PyArray_DESCR((PyArrayObject*)raw_arr)->type_num){
		case NPY_FLOAT:
			return npy_float_eliminate(raw_arr,raw_vals);
		case NPY_DOUBLE:
			return npy_double_eliminate(raw_arr,raw_vals);
		case NPY_LONGDOUBLE:
			return npy_longdouble_eliminate(raw_arr,raw_vals);
		
		case NPY_CFLOAT:
			return npy_cfloat_eliminate(raw_arr,raw_vals);
		case NPY_CDOUBLE:
			return npy_cdouble_eliminate(raw_arr,raw_vals);
		case NPY_CLONGDOUBLE:
			return npy_clongdouble_eliminate(raw_arr,raw_vals);
		
		default:
			return NULL;
	}
	
}

