#ifndef ELIMINATE_COMPLEX_H
#define ELIMINATE_COMPLEX_H
#include <Python.h>
#include <numpy/arrayobject.h>
#include <float.h>
#include <numpy/npy_math.h> 


#define MINUS_MULTIPLY_COMPLEX_REAL(a,b) (-a.real*b.real+a.imag*b.imag)
#define MINUS_MULTIPLY_COMPLEX_IMAG(a,b) (-a.real*b.imag-a.imag*b.real)
#define MULTIPLY_COMPLEX_REAL(a,b) (a.real*b.real-a.imag*b.imag)
#define MULTIPLY_COMPLEX_IMAG(a,b) (a.real*b.imag+a.imag*b.real)



#define GENERATE_COMPLEX_ELIMINATION(DATA_TYPE,DATA_TYPE_DEFINE,DATA_TYPE_EPSILON,SUPPORT_DATA_TYPE) \
void DATA_TYPE##_multiply_matrixes(DATA_TYPE *A,int **indexes,DATA_TYPE *data, DATA_TYPE *result,npy_intp *dims,int start){ \
 	int i,j,l,index; \
 	if(start>0){ \
		for(i=start-1;i<dims[1];++i){ \
			if(i<dims[0])result[i*dims[1]+start-1]=data[i*dims[1]+start-1]; \
			result[(start-1)*dims[1]+i]=data[(start-1)*dims[1]+i]; \
		} \
	} \
	\
	for(i=start;i<dims[1];++i){ \
		l=indexes[0][start]*dims[1]+i; \
		result[start*dims[1]+i].real=MULTIPLY_COMPLEX_REAL(A[start],data[l]); \
		result[start*dims[1]+i].imag=MULTIPLY_COMPLEX_IMAG(A[start],data[l]); \
	} \
	for(j=start+1;j<dims[0];++j){ \
		for(i=start;i<dims[1];++i){ \
			l=indexes[1][j]*dims[1]+i; \
			result[j*dims[1]+i].real=data[indexes[0][j]*dims[1]+i].real+MULTIPLY_COMPLEX_REAL(A[j],data[l]); \
			result[j*dims[1]+i].imag=data[indexes[0][j]*dims[1]+i].imag+MULTIPLY_COMPLEX_IMAG(A[j],data[l]); \
			 \
		} \
	} \
} \
 \
 \
PyObject* DATA_TYPE##_eliminate(PyObject *raw_arr,PyObject *raw_vals){ \
	int i,j,k,l,current_row,index; \
	npy_intp dims[2],*result_dims; \
	DATA_TYPE *data1,*data2,*tmp_data; \
	DATA_TYPE *A; \
	DATA_TYPE *swapper; \
	SUPPORT_DATA_TYPE tmp; \
	PyObject * result; \
	int *indexes[2]; \
	dims[0]=PyArray_DIMS(raw_arr)[0]; \
	dims[1]=PyArray_DIMS(raw_arr)[1]+1; \
	result_dims=PyDimMem_NEW(1); \
	result_dims[0]=dims[0]; \
	result=PyArray_SimpleNew(1,result_dims,DATA_TYPE_DEFINE); \
 \
	 \
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
			data1[i*dims[1]+j].real=tmp_data[i*dims[0]+j].real; \
			data1[i*dims[1]+j].imag=tmp_data[i*dims[0]+j].imag; \
		} \
	} \
	tmp_data=PyArray_DATA(raw_vals); \
	for(i=0;i<dims[0];++i){ \
		data1[i*dims[1]+dims[1]-1].real=tmp_data[i].real; \
		data1[i*dims[1]+dims[1]-1].imag=tmp_data[i].imag; \
	} \
	 \
	k=0; \
	for(i=0;i<dims[0];++i){ \
		current_row=i; \
		while(current_row<dims[0]&& \
			data1[current_row*dims[1]+k].real<=DATA_TYPE_EPSILON&&data1[current_row*dims[1]+k].real>=-DATA_TYPE_EPSILON&& \
			data1[current_row*dims[1]+k].imag<=DATA_TYPE_EPSILON&&data1[current_row*dims[1]+k].imag>=-DATA_TYPE_EPSILON \
		){ \
			++current_row; \
		} \
		if(current_row<dims[0]){ \
		 \
			index=current_row*dims[1]+k; \
			if(current_row!=i){ \
				tmp=data1[index].real*data1[index].real+data1[index].imag*data1[index].imag; \
				A[i].real=data1[index].real/tmp; \
				A[i].imag=-data1[index].imag/tmp; \
				indexes[0][i]=current_row; \
				indexes[0][current_row]=i; \
				l=i*dims[1]+k; \
				A[current_row].real=MINUS_MULTIPLY_COMPLEX_REAL(A[i],data1[l]); \
				A[current_row].imag=MINUS_MULTIPLY_COMPLEX_IMAG(A[i],data1[l]); \
				indexes[1][current_row]=k; \
			}else{ \
				tmp=data1[index].real*data1[index].real+data1[index].imag*data1[index].imag; \
				A[i].real=data1[index].real/tmp; \
				A[i].imag=-data1[index].imag/tmp; \
				indexes[0][i]=i; \
			} \
			for(j=i+1;j<dims[0];++j){ \
				if(j!=current_row){ \
					indexes[0][j]=j; \
					l=j*dims[1]+k; \
					A[j].real=MINUS_MULTIPLY_COMPLEX_REAL(A[i],data1[l]); \
					A[j].imag=MINUS_MULTIPLY_COMPLEX_IMAG(A[i],data1[l]); \
					indexes[1][j]=current_row; \
				} \
			} \
			DATA_TYPE##_multiply_matrixes(A,indexes,data1,data2,dims,i); \
			swapper=data1; \
			data1=data2; \
			data2=swapper; \
		}else{ \
			for(j=0;j<i;++j){ \
				l=j*dims[1]+k; \
				if( \
				(data1[l].real>DATA_TYPE_EPSILON||data1[l].real<-DATA_TYPE_EPSILON)|| \
				(data1[l].imag>DATA_TYPE_EPSILON||data1[l].imag<-DATA_TYPE_EPSILON) \
				)return PyFloat_FromDouble(NPY_NAN); \
			} \
			--i; \
		} \
		k++; \	
	} \
	tmp_data=PyArray_DATA(result); \
	for(i=dims[0]-1;i>=0;--i){ \
		tmp_data[i]=data1[(i+1)*dims[1]-1]; \
		for(j=i-1;j>=0;--j){ \
			l=j*dims[1]+i; \
			data1[(j+1)*dims[1]-1].real-=MULTIPLY_COMPLEX_REAL(tmp_data[i],data1[l]); \
			data1[(j+1)*dims[1]-1].imag-=MULTIPLY_COMPLEX_IMAG(tmp_data[i],data1[l]); \
		} \
	} \
	 \
	free(data1); \
	free(data2); \
	free(A); \
	free(indexes[0]); \
	free(indexes[1]); \
	return result; \
	 \
}



#endif
