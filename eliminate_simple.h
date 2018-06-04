#ifndef ELIMINATE_SIMPLE_H
#define ELIMINATE_SIMPLE_H
#include <Python.h>
#include <numpy/arrayobject.h>
#include <float.h>
#include <numpy/npy_math.h> 


#define GENERATE_ELIMINATION(DATA_TYPE,DATA_TYPE_DEFINE,DATA_TYPE_EPSILON) \
void DATA_TYPE##_multiply_matrixes(DATA_TYPE *A,int **indexes,DATA_TYPE *data, DATA_TYPE *result,npy_intp *dims,int start){ \
 	int i,j,index; \
 	if(start>0){ \
		for(i=start-1;i<dims[1];++i){ \
			if(i<dims[0])result[i*dims[1]+start-1]=data[i*dims[1]+start-1]; \
			result[(start-1)*dims[1]+i]=data[(start-1)*dims[1]+i]; \
		} \
	} \
	\
	for(i=start;i<dims[1];++i){ \
		result[start*dims[1]+i]=A[start]*data[indexes[0][start]*dims[1]+i]; \
	} \
	for(j=start+1;j<dims[0];++j){ \
		for(i=start;i<dims[1];++i){ \
			result[j*dims[1]+i]=data[indexes[0][j]*dims[1]+i]+A[j]*data[indexes[1][j]*dims[1]+i]; \
		} \
	} \
} \
\
PyObject* DATA_TYPE##_eliminate(PyObject *raw_arr,PyObject *raw_vals){ \
	int i,j,k,current_row,index; \
	npy_intp dims[2],*result_dims; \
	DATA_TYPE *data1,*data2,*tmp_data; \
	DATA_TYPE *A; \
	DATA_TYPE *swapper; \
	PyObject * result; \
	int *indexes[2]; \
	dims[0]=PyArray_DIMS(raw_arr)[0]; \
	dims[1]=PyArray_DIMS(raw_arr)[1]+1; \
	result_dims=PyDimMem_NEW(1); \
	result_dims[0]=dims[0]; \
	result=PyArray_SimpleNew(1,result_dims,DATA_TYPE_DEFINE); \
\
	data1=malloc(sizeof(DATA_TYPE)*dims[0]*dims[1]); \
	data2=malloc(sizeof(DATA_TYPE)*dims[0]*dims[1]); \
	tmp_data=PyArray_DATA(raw_arr); \
	A=malloc(sizeof(*A)*dims[0]); \
	indexes[0]=malloc(sizeof(**indexes)*dims[0]); \
	indexes[1]=malloc(sizeof(**indexes)*dims[0]); \
	\
	for(i=0;i<dims[0];++i){ \
		for(j=0;j<dims[0];++j){ \
			data1[i*dims[1]+j]=tmp_data[i*dims[0]+j]; \
		} \
	} \
	tmp_data=PyArray_DATA(raw_vals); \
	for(i=0;i<dims[0];++i){ \
		data1[i*dims[1]+dims[1]-1]=tmp_data[i]; \
	} \
	\
	k=0; \
	for(i=0;i<dims[0];++i){ \
		current_row=i; \
		while(current_row<dims[0]&&data1[current_row*dims[1]+k]<=DATA_TYPE_EPSILON&&data1[current_row*dims[1]+k]>=-DATA_TYPE_EPSILON){ \
			++current_row; \
		} \
		if(current_row<dims[0]){ \
		\
			index=current_row*dims[1]+k; \
			if(current_row!=i){ \
				A[i]=1/data1[index]; \
				indexes[0][i]=current_row; \
				indexes[0][current_row]=i; \
				A[current_row]=-A[i]*data1[i*dims[1]+k]; \
				indexes[1][current_row]=k; \
			}else{ \
				A[i]=1/data1[index]; \
				indexes[0][i]=i; \
			} \
			for(j=i+1;j<dims[0];++j){ \
				if(j!=current_row){ \
					indexes[0][j]=j; \
					A[j]=-A[i]*data1[j*dims[1]+k]; \
					indexes[1][j]=current_row; \
				} \
			} \
			DATA_TYPE##_multiply_matrixes(A,indexes,data1,data2,dims,i); \
			swapper=data1; \
			data1=data2; \
			data2=swapper; \
		}else{ \
			for(j=0;j<i;++j){ \
				if(data1[j*dims[1]+k]>DATA_TYPE_EPSILON||data1[j*dims[1]+k]<-DATA_TYPE_EPSILON)return PyFloat_FromDouble(NPY_NAN); \
			} \
			--i; \
		} \
		k++; \
	} \
	tmp_data=PyArray_DATA(result); \
	for(i=dims[0]-1;i>=0;--i){ \
		tmp_data[i]=data1[(i+1)*dims[1]-1]; \
		for(j=i-1;j>=0;--j){ \
			data1[(j+1)*dims[1]-1]-=tmp_data[i]*data1[j*dims[1]+i]; \
		} \
	} \
	\
	free(data1); \
	free(data2); \
	free(A); \
	free(indexes[0]); \
	free(indexes[1]); \
	return result; \
}



#endif
