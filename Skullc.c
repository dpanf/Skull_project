#include "lodepng.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int i,j;

char *loadPng(const char *filename, int *w, int *h){ 
  unsigned char* image=NULL;
  int error=lodepng_decode32_file(&image,w,h,filename); 
  if(error)
    printf("error %u: %s\n", error,lodepng_error_text(error)); 
  return image; 
}

void preparation(unsigned char *image, int h, int w){ 
  for(i=2;i<h-1;i++) 
    for(j=2;j<w-1;j++){ 
      if(image[w*i+j]<100) 
        image[w*i+j]=0; 
      if(image[w*i+j]>160) 
        image[w*i+j]=255; 
    }
}

void Gauss(unsigned char* p, unsigned char* l, int h, int w){
  int a[3][3],x,y;
  for(i=1;i<h-1;i++) 
    for(j=1;j<w-1;j++){
      for(x=0;x<3;x++)
        for(y=0;y<3;y++)
          a[x][y]=0;
      for(x=0;x<3;x++)
        for(y=0;y<3;y++){
          if(x==0) a[x][y]+=4*(i-1)*w;
          else if(x==1) a[x][y]+=4*i*w;
               else a[x][y]+=4*(i+1)*w;
          if(y==0) a[x][y]+=4*(j-1);
          else if(y==1) a[x][y]+=4*j;
               else a[x][y]+=4*(j+1);
        }
      l[a[1][1]+0]=0.09*p[a[0][0]+0]+0.12*p[a[0][1]+0]+0.09*p[a[0][2]+0]+0.12*p[a[1][0]+0]+0.15*p[a[1][1]+0]+0.12*p[a[1][2]+0]+0.09*p[a[2][0]+0]+0.12*p[a[2][1]+0]+0.09*p[a[2][2]+0]; 
      l[a[1][1]+1]=0.09*p[a[0][0]+1]+0.12*p[a[0][1]+1]+0.09*p[a[0][2]+1]+0.12*p[a[1][0]+1]+0.15*p[a[1][1]+1]+0.12*p[a[1][2]+1]+0.09*p[a[2][0]+1]+0.12*p[a[2][1]+1]+0.09*p[a[2][2]+1];
      l[a[1][1]+2]=0.09*p[a[0][0]+2]+0.12*p[a[0][1]+2]+0.09*p[a[0][2]+2]+0.12*p[a[1][0]+2]+0.15*p[a[1][1]+2]+0.12*p[a[1][2]+2]+0.09*p[a[2][0]+2]+0.12*p[a[2][1]+2]+0.09*p[a[2][2]+2];
      l[a[1][1]+3]=0.09*p[a[0][0]+3]+0.12*p[a[0][1]+3]+0.09*p[a[0][2]+3]+0.12*p[a[1][0]+3]+0.15*p[a[1][1]+3]+0.12*p[a[1][2]+3]+0.09*p[a[2][0]+3]+0.12*p[a[2][1]+3]+0.09*p[a[2][2]+3];
        } 
    return; 
}

void colour(unsigned char* pic, unsigned char* mcod, int h, int w, int n){ 
  for(i=0;i<w*h;i++){
    mcod[i*n+0]=100+pic[i];
    mcod[i*n+1]=2+pic[i];
    mcod[i*n+2]=1.5+pic[i];
    mcod[i*n+3]=150;
  }
  return; 
}

void writePng(const char* filename, const unsigned char* image, unsigned width, unsigned height){
  unsigned char* png;
  size_t pngsize;
  LodePNGState state;
  lodepng_state_init(&state);
  int error=lodepng_encode(&png, &pngsize, image, width, height, &state);
  if(!error) lodepng_save_file(png, pngsize, filename);
  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
  lodepng_state_cleanup(&state);
  free(png);
}

int main(){
  int w,h,n=4;
  char *file1="skull.png";
  char *file2="skull3.png";
  char *image=loadPng(file1, &w, &h);
  if(image==NULL){
    printf("ERROR: can't read file %s\n", file1); 
    return -1;
  }
  unsigned char *opic=(unsigned char*)malloc(w*h*sizeof(unsigned char)); 
  unsigned char *piccopy=(unsigned char*)malloc(w*h*sizeof(unsigned char)); 
  unsigned char *mcopic=(unsigned char*)malloc(w*h*n*sizeof(unsigned char));  
  for(i=0;i<h*w;i++){
    opic[i]=0.299*image[4*i+0]+0.587*image[4*i+1]+0.114*image[4*i+2];
  }
  preparation(opic,h,w);
  Gauss(opic,piccopy,h,w);
  colour(piccopy,mcopic,h,w,n);
  writePng(file2,mcopic,w,h);
  free(opic);
  free(piccopy);
  free(mcopic);
  free(image);
}