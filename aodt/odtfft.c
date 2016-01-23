/* include file containing all AODT library global variables */
#include "../inc/odtlib.h"
#include "../inc/odtlibfuncs.h"

#ifndef PI
#define PI  3.14159265358979323846
#endif

int aodtv64_dfft(double [],double [],int);
double aodtv64_cmplx_abs( double, double );


int aodtv64_fft(float *cbd,float *dx2,int *idxcnt)
{
    int ixx,idxc,iok;
    double xr[64],xi[64],magn[64];
    double dx,a,x;

    /* initialize arrays */
    for (ixx = 1; ixx <= 64; ++ixx) {
	xr[ixx - 1] = cbd[ixx-1];
	xi[ixx - 1] = (float)0.;
	magn[ixx - 1] = (float)0.;
    }
    /* call FFT routine */
    iok=aodtv64_dfft(xr,xi,64);
    if(iok<=0) return -1;
    for (ixx = 1; ixx <= 64; ++ixx) {
      magn[ixx-1]=aodtv64_cmplx_abs(xr[ixx-1],xi[ixx-1]);
    }
    /* compute number of harmonics and magnitude of main harmonic */
    dx = (float)0.;
    idxc = 0;
    for (ixx = 2; ixx <= 31; ++ixx) {
        a = magn[ixx - 2];
        x = magn[ixx - 1];
        dx += (x + a) / (float)2.;
        if (magn[ixx - 1] > magn[ixx - 2] && magn[ixx - 1] > magn[ixx]) {
            ++idxc;
        }
    }
    *dx2 = dx / magn[0];
    *idxcnt=idxc;
    return 0;

} /* MAIN__ */

double aodtv64_cmplx_abs(double real, double imag)
{
double temp;
if(real < 0)
        real = -real;
if(imag < 0)
        imag = -imag;
if(imag > real){
        temp = real;
        real = imag;
        imag = temp;
}
if((real+imag) == real)
        return(real);

temp = imag/real;
temp = real*sqrt(1.0 + temp*temp);  /*overflow!!*/
return(temp);

}

/*
       A Duhamel-Hollman split-radix dif fft
       Ref: Electronics Letters, Jan. 5, 1984
       Complex input and output data in arrays x and y
       Length is n.
*/

int aodtv64_dfft( double x[2], double y[2], int np )
{
double *px,*py;
int i,j,k,m,n,i0,i1,i2,i3,is,id,n1,n2,n4 ;
double  a,e,a3,cc1,ss1,cc3,ss3,r1,r2,s1,s2,s3,xt ;
  px = x - 1; py = y - 1;
  i = 2; m = 1; while (i < np) { i = i+i; m = m+1; };
  n = i; if (n != np) {
  for (i = np+1; i <= n; i++)  { *(px + i) = 0.0; *(py + i) = 0.0; };
  /*printf("\nuse %d point fft",n);*/ }

  n2 = n+n;
  for (k = 1;  k <= m-1; k++ ) {
    n2 = n2 / 2; n4 = n2 / 4; e = 2.0 * PI / n2; a = 0.0;
    for (j = 1; j<= n4 ; j++) {
      a3 = 3.0*a; cc1 = cos(a); ss1 = sin(a);
      cc3 = cos(a3); ss3 = sin(a3); a = j*e; is = j; id = 2*n2;
   while ( is < n ) {
   for (i0 = is; i0 <= n-1; i0 = i0 + id) {
      i1 = i0 + n4; i2 = i1 + n4; i3 = i2 + n4;
      r1 = *(px+i0) - *(px+i2);
      *(px+i0) = *(px+i0) + *(px+i2);
      r2 = *(px+i1) - *(px+i3);
      *(px+i1) = *(px+i1) + *(px+i3);
      s1 = *(py+i0) - *(py+i2);
      *(py+i0) = *(py+i0) + *(py+i2);
      s2 = *(py+i1) - *(py+i3);
      *(py+i1) = *(py+i1) + *(py+i3);
      s3 = r1 - s2; r1 = r1 + s2; s2 = r2 - s1; r2 = r2 + s1;
      *(px+i2) = r1*cc1 - s2*ss1; 
   *(py+i2) = -s2*cc1 - r1*ss1;
      *(px+i3) = s3*cc3 + r2*ss3;
       *(py+i3) = r2*cc3 - s3*ss3;
  }
       is = 2*id - n2 + j; id = 4*id;
 }
    }
  }

/*
---------------------Last stage, length=2 butterfly---------------------
*/
  is = 1; id = 4;
  while ( is < n) {
  for (i0 = is; i0 <= n; i0 = i0 + id) {
      i1 = i0 + 1; r1 = *(px+i0);
      *(px+i0) = r1 + *(px+i1);
      *(px+i1) = r1 - *(px+i1);
      r1 = *(py+i0);
      *(py+i0) = r1 + *(py+i1);
      *(py+i1) = r1 - *(py+i1);
    }
  is = 2*id - 1; id = 4 * id; }

/*
c--------------------------Bit reverse counter
*/
  j = 1; n1 = n - 1;
  for (i = 1; i <= n1; i++) {
    if (i < j) {
   xt = *(px+j);
      *(px+j) = *(px+i); *(px+i) = xt;
      xt = *(py+j); *(py+j) = *(py+i);
      *(py+i) = xt;
    }
    k = n / 2; while (k < j) { j = j - k; k = k / 2; }
    j = j + k;
  }

/*
  for (i = 1; i<=16; i++) printf("%d  %g   %gn",i,*(px+i),(py+i));
*/
  
  return(n);
}


