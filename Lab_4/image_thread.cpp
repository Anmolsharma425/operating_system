#include <iostream>
#include <thread>
#include <semaphore.h>
#include "libppm.h"
#include <cstdint>
#include <chrono>

using namespace std;

sem_t sem_smoothened, sem_details , sem_sharpened;
struct image_t *smoothened_image, *details_image, *sharpened_image;

struct image_t* S1_smoothen(struct image_t *input_image)
{
	// TODO
	struct image_t* image = new image_t;
	image->width = input_image->width;
	image->height = input_image->height;

	image->image_pixels = new uint8_t**[image->height];
	for(int i = 0; i < image->height; i++){
		image->image_pixels[i] = new uint8_t*[image->width];
		for(int j = 0; j < image->width; j++){
			image->image_pixels[i][j] = new uint8_t[3];
		}
	}

	int height = image->height;
	int width = image->width;
	// uint8_t*** pixel = input_image->image_pixels;
	// uint8_t*** outputPixel = image->image_pixels;
	for(int i = 1; i < height - 1; i++){
		for(int j = 1; j < width - 1; j++){
			image->image_pixels[i][j][0] = (input_image->image_pixels[i][j][0] + input_image->image_pixels[i - 1][j - 1][0] + input_image->image_pixels[i - 1][j][0]    +input_image->image_pixels[i - 1][j + 1][0] +input_image->image_pixels[i][j - 1][0] + input_image->image_pixels[i][j + 1][0] + input_image->image_pixels[i + 1][j - 1][0] + input_image->image_pixels[i + 1][j][0] + input_image->image_pixels[i + 1][j + 1][0])/9;
			
			image->image_pixels[i][j][1] = (input_image->image_pixels[i][j][1] + input_image->image_pixels[i - 1][j - 1][1] + input_image->image_pixels[i - 1][j][1] + input_image->image_pixels[i - 1][j + 1][1] + input_image->image_pixels[i][j - 1][1] + input_image->image_pixels[i][j + 1][1] + input_image->image_pixels[i + 1][j - 1][1] + input_image->image_pixels[i + 1][j][1] + input_image->image_pixels[i + 1][j + 1][1])/9;
			
			image->image_pixels[i][j][2] = (input_image->image_pixels[i][j][2] + input_image->image_pixels[i - 1][j - 1][2] + input_image->image_pixels[i - 1][j][2] + input_image->image_pixels[i - 1][j + 1][2] +input_image->image_pixels[i][j - 1][2] + input_image->image_pixels[i][j + 1][2] + input_image->image_pixels[i + 1][j - 1][2] + input_image->image_pixels[i + 1][j][2] + input_image->image_pixels[i + 1][j + 1][2])/9;
			// cout << i << " " << j << " " << width << " " << height << "\n";
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

	// remember to allocate space for smoothened_image. See read_ppm_file() in libppm.c for some help.
	return image;
}

struct image_t* S2_find_details(struct image_t *input_image, struct image_t *smoothened_image)
{
	// TODO
	struct image_t* image = new image_t;
	image->width = input_image->width;
	image->height = input_image->height;

	image->image_pixels = new uint8_t**[image->height];
	for(int i = 0; i < image->height; i++){
		image->image_pixels[i] = new uint8_t*[image->width];
		for(int j = 0; j < image->width; j++){
			image->image_pixels[i][j] = new uint8_t[3];
		}
	}
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

	return image;
}

struct image_t* S3_sharpen(struct image_t *input_image, struct image_t *details_image)
{
	struct image_t* image = new image_t;
	image->width = input_image->width;
	image->height = input_image->height;

	image->image_pixels = new uint8_t**[image->height];
	for(int i = 0; i < image->height; i++){
		image->image_pixels[i] = new uint8_t*[image->width];
		for(int j = 0; j < image->width; j++){
			image->image_pixels[i][j] = new uint8_t[3];
		}
	}
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

	return image;
}

void thread_S1(struct image_t *input_image, int iterations) {
    for (int i = 0; i < iterations; i++) {
        smoothened_image = S1_smoothen(input_image);
        sem_post(&sem_smoothened);
    }
}

void thread_S2(struct image_t *input_image, int iterations) {
    for (int i = 0; i < iterations; i++) {
        sem_wait(&sem_smoothened);
        details_image = S2_find_details(input_image, smoothened_image);
        sem_post(&sem_details);  // Signal S2 completion
    }
}

void thread_S3(struct image_t *input_image, int iterations) {
    for (int i = 0; i < iterations; i++) {
        sem_wait(&sem_details);  // Wait for S2 to complete
        sharpened_image = S3_sharpen(input_image, details_image);
        // if (i == iterations - 1) {
        	sem_post(&sem_sharpened);  // Signal final S3 completion
        // }
    }
}

int main(int argc, char **argv)
{
    if(argc != 3) {
        cout << "Usage: ./a.out <path-to-original-image> <path-to-transformed-image>\n\n";
        exit(0);
    }

    struct image_t *input_image = read_ppm_file(argv[1]);

    // Initialize semaphores
    sem_init(&sem_smoothened, 0, 0);
    sem_init(&sem_details, 0, 0);
    sem_init(&sem_sharpened, 0, 0);

    int iterations = 1000;  // Perform the transformations 1000 times

    // Create threads for S1, S2, and S3
    thread t1(thread_S1, input_image, iterations);
    thread t2(thread_S2, input_image, iterations);
    thread t3(thread_S3, input_image, iterations);

    // Wait for the final thread to complete after 1000 iterations
    sem_wait(&sem_sharpened);

    // Write the result of the final transformation (after 1000 iterations)
    write_ppm_file(argv[2], sharpened_image);

    // Clean up
    t1.join();
    t2.join();
    t3.join();
    sem_destroy(&sem_smoothened);

    sem_destroy(&sem_details);

    sem_destroy(&sem_sharpened);

    return 0;
}
