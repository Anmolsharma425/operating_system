#include <iostream>
#include "libppm.h"
#include <cstdint>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <chrono>

using namespace std;

void free_image(struct image_t *image) {
    if (!image) return;
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            delete[] image->image_pixels[i][j];
        }
        delete[] image->image_pixels[i];
    }
    delete[] image->image_pixels;
    delete image;
}

void S1_smoothen(struct image_t *input_image, int pipefd_s1_w)
{
	// TODO
	// remember to allocate space for smoothened_image. See read_ppm_file() in libppm.c for some help.
	for(int i = 0; i < input_image->height; i++)
	{
		uint8_t* flatten=new uint8_t[input_image->width*3];
		int index=0;
		for(int j = 0; j < input_image->width; j++)
		{
			for(int k = 0; k < 3; k++)
			{
				if(i==0 || j==0 || i==input_image->height-1 || j==input_image->width-1){
					flatten[index++]=input_image->image_pixels[i][j][k];
				}
				else{
					int sum=0;
					for(int r=i-1;r<=i+1;r++){
						for(int c=j-1;c<=j+1;c++){
							sum+=input_image->image_pixels[r][c][k];
						}
					}
					flatten[index++]=(uint8_t)(int)(sum/9);
				}
			}
		}
		write(pipefd_s1_w, flatten, index);
		delete flatten; 
	}
}

void S2_find_details(struct image_t *input_image, int pipefd_s2_w, int pipefd_s1_r)
{
	// TODO
	for(int i = 0; i < input_image->height; i++)
	{
		uint8_t* flatten= new uint8_t[input_image->width*3];
		int index=0;
		read(pipefd_s1_r, flatten, input_image->width*3*sizeof(uint8_t));
		for(int j = 0; j < input_image->width; j++)
		{
			for(int k = 0; k < 3; k++)
			{
				flatten[index]=abs(input_image->image_pixels[i][j][k]-flatten[index]);
				index++;
			}
		}
		write(pipefd_s2_w, flatten, index);
		delete flatten; 
	}
}

struct image_t* S3_sharpen(struct image_t *input_image, int pipefd_s3_r)
{
	// TODO
	struct image_t* sharp=new struct image_t;
	sharp->height=input_image->height;
	sharp->width=input_image->width;
	sharp->image_pixels=new uint8_t**[input_image->height];
	for(int i = 0; i < input_image->height; i++)
	{
		uint8_t* flatten= new uint8_t[input_image->width*3];
		int index=0;
		sharp->image_pixels[i]=new uint8_t*[input_image->width];
		read(pipefd_s3_r, flatten, input_image->width*3*sizeof(uint8_t));
		for(int j = 0; j < input_image->width; j++)
		{
			sharp->image_pixels[i][j]=new uint8_t[3];
			for(int k = 0; k < 3; k++)
			{
				sharp->image_pixels[i][j][k]=min(255, input_image->image_pixels[i][j][k]+flatten[index++]);
			}
		}
		delete flatten; 
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
	int pipe_s2_s3[2];
	
    if(pipe(pipe_s2_s3)<0){
        perror("pipe");
        exit(EXIT_FAILURE);
    }
	struct image_t *input_image = read_ppm_file(argv[1]);

	auto start = chrono::high_resolution_clock::now();
	
    pid_t cpid1=fork();
    if(cpid1==-1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if(cpid1==0){
		int pipe_s1_s2[2];
    	if(pipe(pipe_s1_s2)==-1){
    	    perror("pipe");
    	    exit(EXIT_FAILURE);
    	}
		pid_t cpid2=fork();
		if(cpid2==-1){
        	perror("fork");
        	exit(EXIT_FAILURE);
    	}
		if(cpid2==0){
        	close(pipe_s1_s2[0]);
        	for(int i=0;i<1000;i++){
	    	    S1_smoothen(input_image, pipe_s1_s2[1]);
        	}
        	close(pipe_s1_s2[1]);
        	exit(EXIT_SUCCESS);
		}
		else{
        	close(pipe_s2_s3[0]);
        	close(pipe_s1_s2[1]);
        	for(int i=0;i<1000;i++){
	    	    S2_find_details(input_image, pipe_s2_s3[1], pipe_s1_s2[0]);
        	}
        	close(pipe_s1_s2[0]);
        	close(pipe_s2_s3[1]);
			wait(NULL);
        	exit(EXIT_SUCCESS);
		}
    }

    close(pipe_s2_s3[1]);
    for(int i=0;i<999;i++){
		struct image_t *sharpened_image;
		// cout<<i<<'\n';
		sharpened_image = S3_sharpen(input_image, pipe_s2_s3[0]);
		free_image(sharpened_image);
    }
	struct image_t *sharpened_image;
	sharpened_image = S3_sharpen(input_image, pipe_s2_s3[0]);
	write_ppm_file(argv[2], sharpened_image);
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed_time = end - start;
	cout << "Time to execute using pipe IPC " << elapsed_time.count() << endl;
	free_image(input_image);
	free_image(sharpened_image);
	close(pipe_s2_s3[0]);
	wait(NULL);
	return 0;
}
