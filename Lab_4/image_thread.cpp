#include <iostream>
#include "libppm.h"
#include <cstdint>
#include <chrono>
#include <thread>
#include <semaphore.h>

using namespace std;
struct image_t* smoothened_image, *details_image, *sharpened_image;
sem_t sem_sharpened, sem_details, sem_smoothened;
void S1_smoothen(struct image_t *input_image)
{
	// TODO
	struct image_t* image = smoothened_image;

	int height = image->height;
	int width = image->width;

	for(int i = 1; i < height - 1; i++){
		for(int j = 1; j < width - 1; j++){
			image->image_pixels[i][j][0] = (input_image->image_pixels[i][j][0] + input_image->image_pixels[i - 1][j - 1][0] + input_image->image_pixels[i - 1][j][0]    +input_image->image_pixels[i - 1][j + 1][0] +input_image->image_pixels[i][j - 1][0] + input_image->image_pixels[i][j + 1][0] + input_image->image_pixels[i + 1][j - 1][0] + input_image->image_pixels[i + 1][j][0] + input_image->image_pixels[i + 1][j + 1][0])/9;
			image->image_pixels[i][j][1] = (input_image->image_pixels[i][j][1] + input_image->image_pixels[i - 1][j - 1][1] + input_image->image_pixels[i - 1][j][1] + input_image->image_pixels[i - 1][j + 1][1] + input_image->image_pixels[i][j - 1][1] + input_image->image_pixels[i][j + 1][1] + input_image->image_pixels[i + 1][j - 1][1] + input_image->image_pixels[i + 1][j][1] + input_image->image_pixels[i + 1][j + 1][1])/9;
			image->image_pixels[i][j][2] = (input_image->image_pixels[i][j][2] + input_image->image_pixels[i - 1][j - 1][2] + input_image->image_pixels[i - 1][j][2] + input_image->image_pixels[i - 1][j + 1][2] +input_image->image_pixels[i][j - 1][2] + input_image->image_pixels[i][j + 1][2] + input_image->image_pixels[i + 1][j - 1][2] + input_image->image_pixels[i + 1][j][2] + input_image->image_pixels[i + 1][j + 1][2])/9;
		}	
	}

	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			if(i == 0 || i == height - 1 || j == 0 || j == width - 1){
				for(int k = 0; k < 3; k++){
					image->image_pixels[i][j][k] = input_image->image_pixels[i][j][k];
				}
			}
		}
	}
}

void S2_find_details(struct image_t *input_image, struct image_t *smoothened_image)
{
	// TODO
	struct image_t* image = details_image;
	int height = image->height;
	int width = image-> width;
	uint8_t*** outputPixel = image->image_pixels;
	uint8_t*** iPixel = input_image->image_pixels;
	uint8_t*** sPixel = smoothened_image->image_pixels;
	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			for(int k = 0; k < 3; k++){
				int detail_value = iPixel[i][j][k] - sPixel[i][j][k];
				if(detail_value < 0){
					outputPixel[i][j][k] = 0;
				}
				else if(detail_value > 255){
					outputPixel[i][j][k] = 255;
				}
				else{
					outputPixel[i][j][k] = detail_value;
				}
			}
		}
	}
}

void S3_sharpen(struct image_t *input_image, struct image_t *details_image)
{
	struct image_t* image = sharpened_image;
	int height = image->height;
	int width = image-> width;
	uint8_t*** outputPixel = image->image_pixels;
	uint8_t*** iPixel = input_image->image_pixels;
	uint8_t*** dPixel = details_image->image_pixels;
	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			for(int k = 0; k < 3; k++){
				int detail_value = iPixel[i][j][k] + dPixel[i][j][k];
				if(detail_value < 0){
					outputPixel[i][j][k] = 0;
				}
				else if(detail_value > 255){
					outputPixel[i][j][k] = 255;
				}
				else{
					outputPixel[i][j][k] = detail_value;
				}
			}
		}
	}
}

void t1_S1(struct image_t *input_image){
    for(int i=0;i<1000;i++){
        cout<<"hello am in thread 1"<<" "<<i<<endl;
        S1_smoothen(input_image);
        sem_post(&sem_smoothened);
	}
}

void t2_S2(struct image_t *input_image){
    for(int i=0;i<1000;i++){
        sem_wait(&sem_smoothened);
        cout<<"hello in thread 2 "<<" "<<i<<endl;
        S2_find_details(input_image,smoothened_image);
        sem_post(&sem_details);
    }
}

void t3_S3(struct image_t *input_image){
    for(int i=0;i<1000;i++){
        sem_wait(&sem_details);
        cout<<"hello am in thread 3"<<" "<<i<<endl;
        S3_sharpen(input_image,details_image);
        if(i==999){
            sem_post(&sem_sharpened);
        }
    }
}

int main(int argc, char **argv)
{
    auto startTime= chrono::high_resolution_clock::now();
	if(argc != 3)
	{
		cout << "usage: ./a.out <path-to-original-image> <path-to-transformed-image>\n\n";
		exit(0);
	}
	struct image_t *input_image = read_ppm_file(argv[1]);

	smoothened_image = new image_t;
	details_image = new image_t;
	sharpened_image = new image_t;

	smoothened_image->width = input_image->width;
	smoothened_image->height = input_image->height;
	details_image->width = input_image->width;
	details_image->height = input_image->height;
	sharpened_image->width = input_image->width;
	sharpened_image->height = input_image->height;

	smoothened_image->image_pixels = new uint8_t**[input_image->height];
	details_image->image_pixels = new uint8_t**[input_image->height];
	sharpened_image->image_pixels = new uint8_t**[input_image->height];

	for(int i=0; i<input_image->height; i++){
		smoothened_image->image_pixels[i] = new uint8_t*[input_image->width];
		details_image->image_pixels[i] = new uint8_t*[input_image->width];
		sharpened_image->image_pixels[i] = new uint8_t*[input_image->width];
		for(int j=0; j<input_image->width; j++){
			smoothened_image->image_pixels[i][j] = new uint8_t[3];
			details_image->image_pixels[i][j] = new uint8_t[3];
			sharpened_image->image_pixels[i][j] = new uint8_t[3];
		}
	}
    sem_init(&sem_sharpened,0,0);
    sem_init(&sem_details,0,0);
    sem_init(&sem_smoothened,0,0);

    thread t1(t1_S1,input_image);
    thread t2(t2_S2,input_image);
    thread t3(t3_S3,input_image);
    t1.join(),t2.join(),t3.join();

    sem_destroy(&sem_sharpened);
    sem_destroy(&sem_smoothened);
    sem_destroy(&sem_details);

	write_ppm_file(argv[2], sharpened_image);

	for(int i=0; i<input_image->height; i++){
		for(int j=0; j<input_image->width; j++){
			delete[] smoothened_image->image_pixels[i][j];
			delete[] details_image->image_pixels[i][j];
			delete[] sharpened_image->image_pixels[i][j];
		}
		delete[] smoothened_image->image_pixels[i];
		delete[] details_image->image_pixels[i];
		delete[] sharpened_image->image_pixels[i];
	}
	
	delete[] smoothened_image->image_pixels;
	delete[] details_image->image_pixels;
	delete[] sharpened_image->image_pixels;

	delete[] smoothened_image;
	delete[] details_image;
	delete[] sharpened_image;

    auto endTime=chrono::high_resolution_clock::now();
    chrono::duration<double> timeTaken = endTime - startTime;
    cout<<"Time take for the execution of the Program is: "<<timeTaken.count()<<endl;
	return 0;
}
