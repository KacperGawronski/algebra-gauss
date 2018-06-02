#ifndef ELIMINATE_SIMPLE_H
#define ELIMINATE_SIMPLE_H
#include <Python.h>
#include <numpy/arrayobject.h>
#include <float.h>


void swap_pointers(void **a, void **b){
	void *tmp;
	tmp=a;
	a=b;
	b=tmp;
}


#define DATA_TYPE npy_double
#define DATA_TYPE_DEFINE NPY_DOUBLE
#define DATA_TYPE_EPSILON FLT_EPSILON


struct npy_double_pair{
	DATA_TYPE value;
	int index;
};


npy_double_multply_matrixes(struct npy_double_pair *A[2],DATA_TYPE *data1, DATA_TYPE *result,int start){
	
	
}


PyObject* npy_double_eliminate(PyObject *raw_arr,PyObject *raw_vals){
	int i,j,k,current_row,index;
	npy_intp dims[2];
	DATA_TYPE *data1,*data2,*tmp_data;
	struct npy_double_pair *A[2];
	
	
	dims[0]=PyArray_DIMS(raw_arr)[0];
	dims[1]=PyArray_DIMS(raw_arr)[1]+1;
	data1=malloc(sizeof(DATA_TYPE)*dims[0]*dims[1]);
	data2=malloc(sizeof(DATA_TYPE)*dims[0]*dims[1]);
	tmp_data=PyArray_DATA(raw_arr);
	A[0]=malloc(sizeof(*A)*dims[0]);
	A[1]=malloc(sizeof(*A)*dims[0]);
	
	for(i=0;i<dims[0];++i){
		for(j=0;j<dims[0];++j){
			data1[i*dims[1]+j]=tmp_data[i*dims[0]+j];
		}
	}
	tmp_data=PyArray_DATA(raw_vals);
	for(i=0;i<dims[0];++i){
		data1[i*dims[1]+dims[1]-1]=tmp_data[i];
	}
	
	k=0;
	for(i=0;i<dims[0];++i){
		current_row=i;
		while(current_row<dims[0]&&data1[current_row*dims[1]+k]<=DATA_TYPE_EPSILON&&data1[current_row*dims[1]+k]>=-DATA_TYPE_EPSILON){
			++current_row;
		}
		if(current_row<dims[0]){
		
			index=current_row*dims[1]+k;
			if(current_row!=i){
				A[0][i].value=1/data1[index];
				A[0][i].index=current_row;
				A[0][current_row].value=1;
				A[0][current_row].index=i;
				A[1][current_row].value=A[0][i].value*data1[i*dims[1]+k];
				A[1][current_row].index=k;
			}else{
			}
			for(j=i+1;j<dims[0];++j){
				if(j!=current_row){
					A[0][j].value=1;
					A[0][j].index=j;
					A[1][j].value=A[0][i].value*data1[j*dims[1]+k];
					A[1][j].index=current_row;
				}
			}
			npy_double_multiply_matrixes(A,data1,data2,i);
			
		}else{
			--i;
		}
		k++;	
	}
	
	
	return NULL;
	
}



#endif
