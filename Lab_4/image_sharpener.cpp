#include <iostream>
#include "libppm.h"
#include <cstdint>

using namespace std;

struct image_t* S1_smoothen(struct image_t *input_image)
{
	// TODO
	// remember to allocate space for smoothened_image. See read_ppm_file() in libppm.c for some help.
	struct image_t* image = new struct image_t;
	image->height=input_image->height;
	image->width=input_image->width;
	image->image_pixels = new uint8_t**[image->height];
	for(int i = 0; i < input_image->height; i++)
	{
		image->image_pixels[i] = new uint8_t*[image->width];
		for(int j = 0; j < input_image->width; j++)
		{
			image->image_pixels[i][j] = new uint8_t[3];
			for(int k = 0; k < 3; k++)
			{
				if(i==0 || j==0 || i==input_image->height-1 || j==input_image->width-1){
					image->image_pixels[i][j][k]=input_image->image_pixels[i][j][k];
				}
				else{
					int sum=0;
					for(int r=i-1;r<=i+1;r++){
						for(int c=j-1;c<=j+1;c++){
							sum+=input_image->image_pixels[r][c][k];
						}
					}
					image->image_pixels[i][j][k]=(uint8_t)(int)(sum/9);
					// cout<<image->image_pixels[i][j][k]<<endl;
				}
			}
		}
	}
	return image;
}

struct image_t* S2_find_details(struct image_t *input_image, struct image_t *smoothened_image)
{
	// TODO
	struct image_t* detail=new struct image_t;
	detail->width=input_image->width;
	detail->height=input_image->height;
	detail->image_pixels=new uint8_t**[input_image->height];
	for(int i = 0; i < input_image->height; i++)
	{
		detail->image_pixels[i]=new uint8_t*[input_image->width];
		for(int j = 0; j < input_image->width; j++)
		{
			detail->image_pixels[i][j]=new uint8_t[3];
			for(int k = 0; k < 3; k++)
			{
				detail->image_pixels[i][j][k]=abs(input_image->image_pixels[i][j][k]-smoothened_image->image_pixels[i][j][k]);
				// cout<<smoothened_image->image_pixels[i][j][k]<<endl;
			}
		}
	}
	return detail;
}

struct image_t* S3_sharpen(struct image_t *input_image, struct image_t *details_image)
{
	// TODO
	struct image_t* sharp=new struct image_t;
	sharp->height=input_image->height;
	sharp->width=input_image->width;
	sharp->image_pixels=new uint8_t**[input_image->height];
	for(int i = 0; i < input_image->height; i++)
	{
		sharp->image_pixels[i]=new uint8_t*[input_image->width];
		for(int j = 0; j < input_image->width; j++)
		{
			sharp->image_pixels[i][j]=new uint8_t[3];
			for(int k = 0; k < 3; k++)
			{
				sharp->image_pixels[i][j][k]=min(255, input_image->image_pixels[i][j][k]+details_image->image_pixels[i][j][k]);
			}
		}
	}
	return sharp;
}

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		cout << "usage: ./a.out <path-to-original-image> <path-to-transformed-image>\n\n";
		exit(0);
	}
	
	struct image_t *input_image = read_ppm_file(argv[1]);
	
	struct image_t *smoothened_image = S1_smoothen(input_image);
	
	struct image_t *details_image = S2_find_details(input_image, smoothened_image);
	
	struct image_t *sharpened_image = S3_sharpen(input_image, details_image);
	
	write_ppm_file(argv[2], sharpened_image);
	
	return 0;
}
