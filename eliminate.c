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

 

PyObject* eliminate_cfunc (PyObject *dummy, PyObject *args){
	PyObject *raw_arr,*raw_vals;
	import_array();
	
	if (!PyArg_ParseTuple(args,"OO",&raw_arr,&raw_vals)) return NULL;

	switch(PyArray_DESCR((PyArrayObject*)raw_arr)->type_num){
		/*case NPY_FLOAT:
			return npy_float_eliminate(raw_arr,raw_vals);
		*/case NPY_DOUBLE:
			return npy_double_eliminate(raw_arr,raw_vals);
		/*case NPY_LONGDOUBLE:
			return npy_longdouble_eliminate(raw_arr,raw_vals);
		*/
		//case NPY_CFLOAT:
		//	return npy_Cfloat_eliminate(raw_arr,raw_vals);
		case NPY_CDOUBLE:
			return npy_cdouble_eliminate(raw_arr,raw_vals);
		//case NPY_CLONGDOUBLE:
		//	return npy_clongdouble_eliminate(raw_arr,raw_vals);
		
		default:
			return NULL;
	}
	
}

