/*
Copyright 2018 Kacper Gawroński.
This file, and all files created by Kacper Gawroński in project
algebra-gauss, are licensed under GNU General Public License.
algebra-gauss is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
algebra-gauss is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with algebra-gauss.  If not, see <http://www.gnu.org/licenses/>.
*/
 

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h>
#include <numpy/arrayobject.h>
#include "eliminate.h"

extern PyObject* eliminate_cfunc (PyObject *dummy, PyObject *args);

static PyMethodDef eliminationmethods[] = {
    {
	    "eliminate",eliminate_cfunc,
        METH_VARARGS,
        "Implementation of Gaussian elimination algorithm."//Doc string
    },
    {NULL, NULL, 0, NULL} /* Sentinel */
};



static struct PyModuleDef gauss =
{
	PyModuleDef_HEAD_INIT,
	"gauss", /* name of module */
	"Module contains eliminate function, which implements Gaussian elimination algorithm.",          /* module documentation, may be NULL */
	-1,          /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
	eliminationmethods
};


PyMODINIT_FUNC PyInit_gauss(void)
{
	import_array();
	return PyModule_Create(&gauss);
}
