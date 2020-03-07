#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

void colorToGrayscale(unsigned char *gray_img, unsigned char * color_img, int width, int height);

const int IS_PNG = 1;
const int IS_JPG = 2;
const int DESIRED_CHANNELS = 3;
const int MAX_NAME_LENGTH = 500;
int main(int argc, char *argv[]) {
    if (argc < 4){
        printf("Usage: color2Grayscale ${input color image file} ${output grayscale image file} ${image type}\n Image Types:\n\t1: PGN\n\t2: JPG");
	exit(-1);
    }

    int width, height, channels, type;
    char  in_name[MAX_NAME_LENGTH], out_name[MAX_NAME_LENGTH];
    strcpy(in_name, argv[1]);
    strcpy(out_name, argv[2]);
    type = atoi(argv[3]);

 char *dev_color_img, *dev_gray_img, *dev_height,*dev_width; 

    unsigned char *color_img = stbi_load(in_name, &width, &height, &channels, 0); // load and conver the image to 3 channels (ignore the transparancy channel)
    if(color_img == NULL) {
        printf("Error in loading the image\n");
        exit(-1);
    }
    printf("Loaded image %s with a width of %dpx, a height of %dpx and %d channels\n", in_name, width, height, channels);

    // Convert the input image to gray
    int gray_channels = channels == 4 ? 2 : 1;
    size_t gray_img_size = width * height * gray_channels;
size_t color_img_size = width * height * channels;
    
    unsigned char *gray_img = (unsigned char *)malloc(gray_img_size);
    if(gray_img == NULL) {
        printf("Unable to allocate memory for the gray image.\n");
        exit(1);
    }
    printf("Create a image array with a width of %dpx, a height of %dpx and %d channels\n", width, height, gray_channels);

	 cudaMalloc( (void**)&dev_color_img, (color_img_size* sizeof(unsigned char) );
     cudaMalloc( (void**)&dev_gray_img, gray_img_size * sizeof(unsigned char) );
     cudaMalloc( (void**)&dev_width, width* sizeof(int) );
	cudaMalloc( (void**)&dev_height, height * sizeof(int));
     cudaMemcpy( dev*color_img, color_img, color_img_size* sizeof(unsigned char), cudaMemcpyHostToDevice );
     

int threadsperblock = 256;
int blockspergrid = (N + threadsperblock - 1) /threadsperblock;

   colorToGrayscale<<<blockspergrid,threadsperblock>>>(dev_gray_img, dev_color_img,dev_width,dev_height);

cudaMemcpy( gray_img, dev_gray_img, gray_img_size * sizeof(unsigned 
char),cudaMemcpyDeviceToHost );

    if (type == IS_PNG)
    	stbi_write_png(out_name, width, height, gray_channels, gray_img, width * gray_channels);

    else
        if (type == IS_JPG)
            stbi_write_jpg(out_name, width, height, gray_channels, gray_img, 100); //The last parameter of the stbi_write_jpg function is a quality parameter that goes from 1 to 100. Since JPG is a lossy image format, you can chose how much data is dropped at save time. Lower quality means smaller image size on disk and lower visual image quality.
    printf("Wrote image %s with a width of %dpx, a height of %dpx and %d channels\n", out_name, width, height, channels);

    stbi_image_free(gray_img); 

	free(color_img);
    	free(gray_img);
    	cudaFree (dev_color_img);
    	cudaFree (dev_gray_img);
    	cudaFree (dev_height);
	cudaFree (dev_width);
	
	return 0;
}


__global__ void colorToGrayscale(unsigned char *gray_img, unsigned char * color_img, int width, int height){


    unsigned char pixel[DESIRED_CHANNELS];


	int x = threadIdx.x + blockIdx.x * blockDim.x;
        int y = threadIdx.y + blockIdx.y * blockDim.x;

     
           //If the input image has a transparency channel this will be simply copied to the second channel of the gray image, while the first channel of the gray image will contain the gray pixel values. If the input image has three channels, the output image will have only one channel with the gray data.
 		
if(x<width && y<height)
{
           // int greyOffset = row * width + col;

		int greyOffset = y * width + x;
            int rgbOffset = greyOffset * DESIRED_CHANNELS;
            pixel[0] = color_img[rgbOffset];
            pixel[1] = color_img[rgbOffset + 1];
            pixel[2] = color_img[rgbOffset + 2];

            gray_img[greyOffset] = pixel[0] * 0.3 + pixel[1] * 0.58 + pixel[2] * 0.11;
        }
    }

