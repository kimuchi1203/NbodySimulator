#include "com_NbodySimulator_SimDataModel.h"
#include <math.h>

double range2(int i, int j, jdouble *x3) {
    double dx0, dx1, dx2;
    dx0 = x3[i*3+0]-x3[j*3+0];
    dx1 = x3[i*3+1]-x3[j*3+1];
    dx2 = x3[i*3+2]-x3[j*3+2];
    return dx0*dx0+dx1*dx1+dx2*dx2;
}

void sort(jdouble *tmp_a3, jint *indexes, jint num_p){
    double tmp;
    int i,j,k;
    
    for(i=0;i<num_p*3;++i){
        indexes[i] = num_p-1;
    }
    for(i=0;i<num_p;++i){
        for(j=0;j<num_p-1-1;++j){
            for(k=num_p-1-1;k>j;--k){
                if(tmp_a3[i*(num_p-1)*3 + k*3]<tmp_a3[i*(num_p-1)*3 + (k-1)*3]){
                    tmp = tmp_a3[i*(num_p-1)*3 + k*3];
                    tmp_a3[i*(num_p-1)*3 + k*3] = tmp_a3[i*(num_p-1)*3 + (k-1)*3];
                    tmp_a3[i*(num_p-1)*3 + (k-1)*3] = tmp;
                }
                if(tmp_a3[i*(num_p-1)*3 + k*3 +1]<tmp_a3[i*(num_p-1)*3 + (k-1)*3 +1]){
                    tmp = tmp_a3[i*(num_p-1)*3 + k*3 +1];
                    tmp_a3[i*(num_p-1)*3 + k*3 +1] = tmp_a3[i*(num_p-1)*3 + (k-1)*3 +1];
                    tmp_a3[i*(num_p-1)*3 + (k-1)*3 +1] = tmp;
                }
                if(tmp_a3[i*(num_p-1)*3 + k*3 +2]<tmp_a3[i*(num_p-1)*3 + (k-1)*3 +2]){
                    tmp = tmp_a3[i*(num_p-1)*3 + k*3 +2];
                    tmp_a3[i*(num_p-1)*3 + k*3 +2] = tmp_a3[i*(num_p-1)*3 + (k-1)*3 +2];
                    tmp_a3[i*(num_p-1)*3 + (k-1)*3 +2] = tmp;
                }
            }
        }
        for(j=0;j<num_p-1;++j){//length of tmp_a3[i][]
            if(indexes[i*3]==num_p-1){
                if(tmp_a3[i*(num_p-1)*3 + j*3]>0.0){
                    indexes[i*3] = j;
                }
            }
            if(indexes[i*3+1]==num_p-1){
                if(tmp_a3[i*(num_p-1)*3 + j*3 +1]>0.0){
                    indexes[i*3+1] = j;
                }
            }
            if(indexes[i*3+2]==num_p-1){
                if(tmp_a3[i*(num_p-1)*3 + j*3 +2]>0.0){
                    indexes[i*3+2] = j;
                }
            }
        }
    }
    return;
}
void Java_com_NbodySimulator_SimDataModel_ndk_1calcForce(JNIEnv *env, jobject this, jdoubleArray x3a, jdoubleArray a3a, jdoubleArray tmp_a3a, jdoubleArray massa, jintArray indexesa, jint num_p, jdouble k)
{
    jdouble* x3=(*env)->GetDoubleArrayElements(env,x3a,0); 
    jdouble* a3=(*env)->GetDoubleArrayElements(env,a3a,0); 
    jdouble* tmp_a3=(*env)->GetDoubleArrayElements(env,tmp_a3a,0); 
    jdouble* mass=(*env)->GetDoubleArrayElements(env,massa,0); 
    jint* indexes=(*env)->GetIntArrayElements(env,indexesa,0);
    double tmpF,r2,r;
    double tmpFx0, tmpFx1, tmpFx2;
    double negax=0.0, negay=0.0, negaz=0.0;
    double posix=0.0, posiy=0.0, posiz=0.0;
    int i,j;

    for(i=0;i<num_p;++i){
        for(j=i+1;j<num_p;++j){
            r2=range2(i,j,x3);
            r=sqrt(r2);
            tmpF=k/r2;
            tmpFx0=tmpF*(x3[i*3+0]-x3[j*3+0])/r;
            tmpFx1=tmpF*(x3[i*3+1]-x3[j*3+1])/r;
            tmpFx2=tmpF*(x3[i*3+2]-x3[j*3+2])/r;
            tmp_a3[(num_p-1)*3*i + (j-1)*3 + 0] = -1*mass[j]*tmpFx0;
            tmp_a3[(num_p-1)*3*i + (j-1)*3 + 1] = -1*mass[j]*tmpFx1;
            tmp_a3[(num_p-1)*3*i + (j-1)*3 + 2] = -1*mass[j]*tmpFx2;
            tmp_a3[(num_p-1)*3*j + i*3 + 0] = mass[i]*tmpFx0;
            tmp_a3[(num_p-1)*3*j + i*3 + 1] = mass[i]*tmpFx1;
            tmp_a3[(num_p-1)*3*j + i*3 + 2] = mass[i]*tmpFx2;
        }
    }
    
    sort(tmp_a3,indexes,num_p);
    
    for(i=0;i<num_p*3;++i){
        indexes[i] = num_p-1;
    }
    for(i=0;i<num_p;++i){
        for(j=0;j<indexes[i*3];++j){
            negax += tmp_a3[i*(num_p-1)*3 + j*3];
	    }
        for(j=indexes[i*3];j<num_p-1;++j){
	        posix += tmp_a3[i*(num_p-1)*3 + j*3];
        }
        for(j=0;j<indexes[i*3+1];++j){
            negay += tmp_a3[i*(num_p-1)*3 + j*3 + 1];
        }
        for(j=indexes[i*3+1];j<num_p-1;++j){
            posiy += tmp_a3[i*(num_p-1)*3 + j*3 + 1];
        }
        for(j=0;j<indexes[i*3+2];++j){
            negaz += tmp_a3[i*(num_p-1)*3 + j*3 + 2];
        }
        for(j=indexes[i*3+2];j<num_p-1;++j){
            posiz += tmp_a3[i*(num_p-1)*3 + j*3 + 2];
        }
        a3[i*3+0] = posix + negax;
        a3[i*3+1] = posiy + negay;
        a3[i*3+2] = posiz + negaz;
        negax = 0.0; posix = 0.0;
        negay = 0.0; posiy = 0.0;
        negaz = 0.0; posiz = 0.0;
    }
    
    (*env)->ReleaseDoubleArrayElements(env, x3a, x3, 0);
    (*env)->ReleaseDoubleArrayElements(env, a3a, a3, 0);
    (*env)->ReleaseDoubleArrayElements(env, tmp_a3a, tmp_a3, 0);
    (*env)->ReleaseDoubleArrayElements(env, massa, mass, 0);
    (*env)->ReleaseIntArrayElements(env,indexesa,indexes,0);
}
void Java_com_NbodySimulator_SimDataModel_ndk_1integrate(JNIEnv *env, jobject this, jdoubleArray x3a, jdoubleArray v3a, jdoubleArray a3a, jint num_p, jdouble dt)
{
	int i;
	jdouble* x3=(*env)->GetDoubleArrayElements(env,x3a,0); 
	jdouble* v3=(*env)->GetDoubleArrayElements(env,v3a,0); 
	jdouble* a3=(*env)->GetDoubleArrayElements(env,a3a,0); 
	for(i=0;i<num_p;++i){
		x3[i*3+0] += v3[i*3+0]*dt;
		x3[i*3+1] += v3[i*3+1]*dt;
		x3[i*3+2] += v3[i*3+2]*dt;
		v3[i*3+0] += a3[i*3+0]*dt;
		v3[i*3+1] += a3[i*3+1]*dt;
		v3[i*3+2] += a3[i*3+2]*dt;
		a3[i*3+0] = 0.0;
		a3[i*3+1] = 0.0;
		a3[i*3+2] = 0.0;
	}
	(*env)->ReleaseDoubleArrayElements(env, x3a, x3, 0);
	(*env)->ReleaseDoubleArrayElements(env, v3a, v3, 0);
	(*env)->ReleaseDoubleArrayElements(env, a3a, a3, 0);
}  
