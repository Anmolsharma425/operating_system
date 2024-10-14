#include <iostream>
#include <thread>
#include <semaphore.h>
#include "libppm.h"
#include <cstdint>
#include<chrono>

using namespace std;

// these global variables are used by all the threads and in each thread it overwrites to the previous memory only
sem_t sem_smoothened, sem_details, sem_sharpened;
struct image_t *smoothened_image = nullptr, *details_image = nullptr, *sharpened_image = nullptr;

struct image_t* S1_smoothen(struct image_t *input_image) {
    // no need to allocate this in each of the thread just overwrite the same memory as in the main
    // struct image_t* image = new image_t; 
    // image->width = input_image->width;
    // image->height = input_image->height;

    // image->image_pixels = new uint8_t**[image->height];
    // for (int i = 0; i < image->height; i++) {
    //     image->image_pixels[i] = new uint8_t*[image->width];
    //     for (int j = 0; j < image->width; j++) {
    //         image->image_pixels[i][j] = new uint8_t[3];
    //     }
    // }

    int height = input_image->height;
    int width = input_image->width;
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            smoothened_image->image_pixels[i][j][0] = (input_image->image_pixels[i][j][0] +input_image->image_pixels[i-1][j-1][0] +
                input_image->image_pixels[i-1][j][0] +input_image->image_pixels[i-1][j+1][0] +input_image->image_pixels[i][j-1][0] +input_image->image_pixels[i][j+1][0] +input_image->image_pixels[i+1][j-1][0]+input_image->image_pixels[i+1][j][0]+input_image->image_pixels[i+1][j+1][0]
            ) / 9;

            smoothened_image->image_pixels[i][j][1] = (
                input_image->image_pixels[i][j][1] +input_image->image_pixels[i-1][j-1][1] +input_image->image_pixels[i-1][j][1] +input_image->image_pixels[i-1][j+1][1] +input_image->image_pixels[i][j-1][1] +input_image->image_pixels[i][j+1][1] +input_image->image_pixels[i+1][j-1][1] +
                input_image->image_pixels[i+1][j][1] +input_image->image_pixels[i+1][j+1][1]
            ) / 9;

            smoothened_image->image_pixels[i][j][2] = (
                input_image->image_pixels[i][j][2] +input_image->image_pixels[i-1][j-1][2] +input_image->image_pixels[i-1][j][2] +input_image->image_pixels[i-1][j+1][2] +input_image->image_pixels[i][j-1][2] +input_image->image_pixels[i][j+1][2] +input_image->image_pixels[i+1][j-1][2] +input_image->image_pixels[i+1][j][2] +input_image->image_pixels[i+1][j+1][2]
            ) / 9;
        }
    }

    // Copy edge pixels from input image to the new image
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                for (int k = 0; k < 3; k++) {
                    smoothened_image->image_pixels[i][j][k] = input_image->image_pixels[i][j][k];
                }
            }
        }
    }

    return smoothened_image; // Return the smoothened image
}

struct image_t* S2_find_details(struct image_t *input_image, struct image_t *smoothened_image) {
    // struct image_t* image = new image_t;
    // image->width = input_image->width;
    // image->height = input_image->height;

    // image->image_pixels = new uint8_t**[image->height];
    // for (int i = 0; i < image->height; i++) {
    //     image->image_pixels[i] = new uint8_t*[image->width];
    //     for (int j = 0; j < image->width; j++) {
    //         image->image_pixels[i][j] = new uint8_t[3];
    //     }
    // }

    int height = input_image->height;
    int width = input_image->width;
    // uint8_t*** outputPixel = image->image_pixels;
    // uint8_t*** iPixel = input_image->image_pixels;
    // uint8_t*** sPixel = smoothened_image->image_pixels;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < 3; k++) {
                int detail_value = input_image->image_pixels[i][j][k] - smoothened_image->image_pixels[i][j][k];
                if (detail_value < 0) {
                    details_image->image_pixels[i][j][k] = 0;
                } else if (detail_value > 255) {
                    details_image->image_pixels[i][j][k] = 255;
                } else {
                    details_image->image_pixels[i][j][k] = detail_value;
                }
            }
        }
    }

    return details_image;
}

struct image_t* S3_sharpen(struct image_t *input_image, struct image_t *details_image) {
    // struct image_t* image = new image_t;
    // image->width = input_image->width;
    // image->height = input_image->height;

    // image->image_pixels = new uint8_t**[image->height];
    // for (int i = 0; i < image->height; i++) {
    //     image->image_pixels[i] = new uint8_t*[image->width];
    //     for (int j = 0; j < image->width; j++) {
    //         image->image_pixels[i][j] = new uint8_t[3];
    //     }
    // }

    int height = input_image->height;
    int width = input_image->width;
    // uint8_t*** outputPixel = image->image_pixels;
    // uint8_t*** iPixel = input_image->image_pixels;
    // uint8_t*** dPixel = details_image->image_pixels;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < 3; k++) {
                int detail_value = input_image->image_pixels[i][j][k] + details_image->image_pixels[i][j][k];
                if (detail_value < 0) {
                    sharpened_image->image_pixels[i][j][k] = 0;
                } 
                else if (detail_value > 255) {
                    sharpened_image->image_pixels[i][j][k] = 255;
                } 
                else {
                    sharpened_image->image_pixels[i][j][k] = detail_value;
                }
            }
        }
    }

    return sharpened_image;
}

void thread_S1(struct image_t *input_image, int iterations) {
    for (int i = 0; i < iterations; i++) {
        cout<<"thread1 "<<i<<endl;
        smoothened_image = S1_smoothen(input_image);
        sem_post(&sem_smoothened);  // unlocak sem_smoothened
    }
}

void thread_S2(struct image_t *input_image, int iterations) {
    for (int i = 0; i<iterations; i++) {
        sem_wait(&sem_smoothened); // lock the sem_smoothened so there thread 1 will wait
        cout<<"thread2 "<<i<<endl;
        details_image = S2_find_details(input_image, smoothened_image);
        sem_post(&sem_details);  // unlock sem_details
    }
}

void thread_S3(struct image_t *input_image, int iterations) {
    for (int i = 0; i < iterations; i++) {
        sem_wait(&sem_details);    // lock sem_details so that s2 is not executin but s1 is still working
        cout<<"thread3 "<<i<<endl;
        sharpened_image = S3_sharpen(input_image, details_image);
        if(i==iterations-1){
            sem_post(&sem_sharpened);
        }
    }
}

int main(int argc, char **argv) {

    // start of the execution of the program for time calculation using chrono
    auto start=chrono::high_resolution_clock::now();

    // ensure that it has valid arguments
    if (argc < 3) {
        cout<<"Enter valid arguments";
        return 1;
    }

    struct image_t *input_image = read_ppm_file(argv[1]);

    // ensure that images are read properly
    if (!input_image) {
        cout<<"There is error in reading input image"<<endl;
        return -1;
    }
    
    smoothened_image= new image_t;
    smoothened_image->height = input_image->height;
    smoothened_image->width = input_image->width;
    smoothened_image -> image_pixels = new uint8_t**[smoothened_image->height];
    for(int i=0;i<smoothened_image->height;i++){
        smoothened_image->image_pixels[i] = new uint8_t*[smoothened_image->width];
        for(int j=0;j<smoothened_image->width;j++){
                smoothened_image -> image_pixels [i][j] = new uint8_t[3];
        }
    }

    details_image= new image_t;
    details_image->height=input_image->height;
    details_image->width= input_image->width;
    details_image->image_pixels= new uint8_t**[details_image->height];
    for(int i=0;i<details_image->height;i++){
        details_image->image_pixels[i]= new uint8_t*[details_image->width];
        for(int j=0;j<details_image->width;j++){
            details_image->image_pixels[i][j]= new uint8_t[3];
        }
    }

<<<<<<< HEAD
    int iterations = 10;  // Perform the transformations multiple times
=======
    sharpened_image=new image_t;
    sharpened_image->height=input_image->height;
    sharpened_image->width=input_image->width;
    sharpened_image->image_pixels= new uint8_t**[sharpened_image->height];
    for(int i=0;i<sharpened_image->height;i++){
        sharpened_image->image_pixels[i]= new uint8_t*[sharpened_image->width];
        for(int j=0;j<sharpened_image->width;j++){
            sharpened_image->image_pixels[i][j]= new uint8_t[3];
        }
    }
>>>>>>> 361d2dc9d3dbeb19a5b7a45bd3ccbcac437df0f0

    // initiate the semaphore name,process/thread,initial value
    // for process it is 1 but for thread it is 0
    sem_init(&sem_smoothened,0,1);
    sem_init(&sem_details,0,1);
    sem_init(&sem_sharpened,0,1);

    int iterations = 1000; 

    // start the execution of each of the thread now first one is the function name then parameters to the fucntions
    // thread t1,t2,t3;
    // t1(thread_S1,input_image,iterations);
    // t2(thread_S2,smoothened_image,iterations);
    // t3(thread_S3,details_image,iterations);
    thread t1(thread_S1,input_image,iterations);
    thread t2(thread_S2,smoothened_image,iterations);
    thread t3(thread_S3,details_image,iterations);

    // Wait for the threads to finish
    t1.join(),t2.join(),t3.join();

     // Destroy semaphores
    sem_destroy(&sem_smoothened);
    sem_destroy(&sem_details);
    sem_destroy(&sem_sharpened);

    // write the sharpened image the ppm with name as argv[2]
    write_ppm_file(argv[2], sharpened_image);

    // delete the same way it was allocated
    for(int i=0;i<input_image->height;i++){
        for(int j=0;j<input_image->width;j++){
            delete[] input_image->image_pixels[i][j];
            delete[] smoothened_image->image_pixels[i][j];
            delete[] details_image->image_pixels[i][j];
            delete[] sharpened_image->image_pixels[i][j];
        }
        delete[] input_image->image_pixels[i];
        delete[] smoothened_image->image_pixels[i];
        delete[] details_image->image_pixels[i];
        delete[] sharpened_image->image_pixels[i];
    }

    delete[] input_image->image_pixels;
    delete[] smoothened_image->image_pixels;
    delete[] details_image->image_pixels;
    delete[] sharpened_image->image_pixels;

    delete[] input_image;
    delete[] smoothened_image;
    delete[] details_image;
    delete[] sharpened_image;

    // finding the time taken in all these processes
    auto end=chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_time = end-start;
    cout<<"Time to execute using threads"<<elapsed_time.count()<<endl;
    return 0;
}