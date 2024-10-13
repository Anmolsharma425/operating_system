#include <iostream>
#include <thread>
#include <semaphore.h>
#include "libppm.h"
#include <cstdint>
<<<<<<< HEAD
#include<chrono>

using namespace std;

// these global variables are used by all the threads and in each thread it overwrites to the previous memory only
=======

using namespace std;

>>>>>>> 6160ca6421c7da131c022b76d14ddfcd148ef824
sem_t sem_smoothened, sem_details, sem_sharpened;
struct image_t *smoothened_image = nullptr, *details_image = nullptr, *sharpened_image = nullptr;

struct image_t* S1_smoothen(struct image_t *input_image) {
<<<<<<< HEAD
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
=======
    struct image_t* image = new image_t;
    image->width = input_image->width;
    image->height = input_image->height;

    image->image_pixels = new uint8_t**[image->height];
    for (int i = 0; i < image->height; i++) {
        image->image_pixels[i] = new uint8_t*[image->width];
        for (int j = 0; j < image->width; j++) {
            image->image_pixels[i][j] = new uint8_t[3];
        }
    }

    int height = image->height;
    int width = image->width;
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            image->image_pixels[i][j][0] = (
                input_image->image_pixels[i][j][0] +
                input_image->image_pixels[i - 1][j - 1][0] +
                input_image->image_pixels[i - 1][j][0] +
                input_image->image_pixels[i - 1][j + 1][0] +
                input_image->image_pixels[i][j - 1][0] +
                input_image->image_pixels[i][j + 1][0] +
                input_image->image_pixels[i + 1][j - 1][0] +
                input_image->image_pixels[i + 1][j][0] +
                input_image->image_pixels[i + 1][j + 1][0]
            ) / 9;

            image->image_pixels[i][j][1] = (
                input_image->image_pixels[i][j][1] +
                input_image->image_pixels[i - 1][j - 1][1] +
                input_image->image_pixels[i - 1][j][1] +
                input_image->image_pixels[i - 1][j + 1][1] +
                input_image->image_pixels[i][j - 1][1] +
                input_image->image_pixels[i][j + 1][1] +
                input_image->image_pixels[i + 1][j - 1][1] +
                input_image->image_pixels[i + 1][j][1] +
                input_image->image_pixels[i + 1][j + 1][1]
            ) / 9;

            image->image_pixels[i][j][2] = (
                input_image->image_pixels[i][j][2] +
                input_image->image_pixels[i - 1][j - 1][2] +
                input_image->image_pixels[i - 1][j][2] +
                input_image->image_pixels[i - 1][j + 1][2] +
                input_image->image_pixels[i][j - 1][2] +
                input_image->image_pixels[i][j + 1][2] +
                input_image->image_pixels[i + 1][j - 1][2] +
                input_image->image_pixels[i + 1][j][2] +
                input_image->image_pixels[i + 1][j + 1][2]
>>>>>>> 6160ca6421c7da131c022b76d14ddfcd148ef824
            ) / 9;
        }
    }

    // Copy edge pixels from input image to the new image
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                for (int k = 0; k < 3; k++) {
<<<<<<< HEAD
                    smoothened_image->image_pixels[i][j][k] = input_image->image_pixels[i][j][k];
=======
                    image->image_pixels[i][j][k] = input_image->image_pixels[i][j][k];
>>>>>>> 6160ca6421c7da131c022b76d14ddfcd148ef824
                }
            }
        }
    }

<<<<<<< HEAD
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
=======
    return image;  // Return the smoothened image
}

struct image_t* S2_find_details(struct image_t *input_image, struct image_t *smoothened_image) {
    struct image_t* image = new image_t;
    image->width = input_image->width;
    image->height = input_image->height;

    image->image_pixels = new uint8_t**[image->height];
    for (int i = 0; i < image->height; i++) {
        image->image_pixels[i] = new uint8_t*[image->width];
        for (int j = 0; j < image->width; j++) {
            image->image_pixels[i][j] = new uint8_t[3];
        }
    }

    int height = image->height;
    int width = image->width;
    uint8_t*** outputPixel = image->image_pixels;
    uint8_t*** iPixel = input_image->image_pixels;
    uint8_t*** sPixel = smoothened_image->image_pixels;
>>>>>>> 6160ca6421c7da131c022b76d14ddfcd148ef824

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < 3; k++) {
<<<<<<< HEAD
                int detail_value = input_image->image_pixels[i][j][k] - smoothened_image->image_pixels[i][j][k];
                if (detail_value < 0) {
                    details_image->image_pixels[i][j][k] = 0;
                } else if (detail_value > 255) {
                    details_image->image_pixels[i][j][k] = 255;
                } else {
                    details_image->image_pixels[i][j][k] = detail_value;
=======
                int detail_value = iPixel[i][j][k] - sPixel[i][j][k];
                if (detail_value < 0) {
                    outputPixel[i][j][k] = 0;
                } else if (detail_value > 255) {
                    outputPixel[i][j][k] = 255;
                } else {
                    outputPixel[i][j][k] = detail_value;
>>>>>>> 6160ca6421c7da131c022b76d14ddfcd148ef824
                }
            }
        }
    }

<<<<<<< HEAD
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
=======
    return image;  // Return the details image
}

struct image_t* S3_sharpen(struct image_t *input_image, struct image_t *details_image) {
    struct image_t* image = new image_t;
    image->width = input_image->width;
    image->height = input_image->height;

    image->image_pixels = new uint8_t**[image->height];
    for (int i = 0; i < image->height; i++) {
        image->image_pixels[i] = new uint8_t*[image->width];
        for (int j = 0; j < image->width; j++) {
            image->image_pixels[i][j] = new uint8_t[3];
        }
    }

    int height = image->height;
    int width = image->width;
    uint8_t*** outputPixel = image->image_pixels;
    uint8_t*** iPixel = input_image->image_pixels;
    uint8_t*** dPixel = details_image->image_pixels;
>>>>>>> 6160ca6421c7da131c022b76d14ddfcd148ef824

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < 3; k++) {
<<<<<<< HEAD
                int detail_value = input_image->image_pixels[i][j][k] + details_image->image_pixels[i][j][k];
                if (detail_value < 0) {
                    sharpened_image->image_pixels[i][j][k] = 0;
                } 
                else if (detail_value > 255) {
                    sharpened_image->image_pixels[i][j][k] = 255;
                } 
                else {
                    sharpened_image->image_pixels[i][j][k] = detail_value;
=======
                int detail_value = iPixel[i][j][k] + dPixel[i][j][k];
                if (detail_value < 0) {
                    outputPixel[i][j][k] = 0;
                } else if (detail_value > 255) {
                    outputPixel[i][j][k] = 255;
                } else {
                    outputPixel[i][j][k] = detail_value;
>>>>>>> 6160ca6421c7da131c022b76d14ddfcd148ef824
                }
            }
        }
    }

<<<<<<< HEAD
    return sharpened_image;
=======
    return image;  // Return the sharpened image
>>>>>>> 6160ca6421c7da131c022b76d14ddfcd148ef824
}

void thread_S1(struct image_t *input_image, int iterations) {
    for (int i = 0; i < iterations; i++) {
        cout<<"thread1 "<<i<<endl;
        smoothened_image = S1_smoothen(input_image);
<<<<<<< HEAD
        sem_post(&sem_smoothened);  // unlocak sem_smoothened
=======
        sem_post(&sem_smoothened);
>>>>>>> 6160ca6421c7da131c022b76d14ddfcd148ef824
    }
}

void thread_S2(struct image_t *input_image, int iterations) {
<<<<<<< HEAD
    for (int i = 0; i<iterations; i++) {
        sem_wait(&sem_smoothened); // lock the sem_smoothened so there thread 1 will wait
        cout<<"thread2 "<<i<<endl;
        details_image = S2_find_details(input_image, smoothened_image);
        sem_post(&sem_details);  // unlock sem_details
=======
    for (int i = 0; i < iterations; i++) {
        sem_wait(&sem_smoothened);
        cout<<"thread2 "<<i<<endl;
        details_image = S2_find_details(input_image, smoothened_image);
        sem_post(&sem_details);  // Signal S2 completion
>>>>>>> 6160ca6421c7da131c022b76d14ddfcd148ef824
    }
}

void thread_S3(struct image_t *input_image, int iterations) {
    for (int i = 0; i < iterations; i++) {
<<<<<<< HEAD
        sem_wait(&sem_details);    // lock sem_details so that s2 is not executin but s1 is still working
        cout<<"thread3 "<<i<<endl;
        sharpened_image = S3_sharpen(input_image, details_image);
        if(i==iterations-1){
            sem_post(&sem_sharpened);
=======
        sem_wait(&sem_details);  // Wait for S2 to complete
        cout<<"thread3 "<<i<<endl;
        sharpened_image = S3_sharpen(input_image, details_image);
        if(i==iterations-1){
            sem_post(&sem_sharpened);  // Signal S3 completion
>>>>>>> 6160ca6421c7da131c022b76d14ddfcd148ef824
        }
    }
}

int main(int argc, char **argv) {
<<<<<<< HEAD

    // start of the execution of the program for time calculation using chrono
    auto start=chrono::high_resolution_clock::now();

    // ensure that it has valid arguments
    if (argc < 3) {
        cout<<"Enter valid arguments";
=======
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <input_image.ppm> <output_image.ppm>" << endl;
>>>>>>> 6160ca6421c7da131c022b76d14ddfcd148ef824
        return 1;
    }

    struct image_t *input_image = read_ppm_file(argv[1]);
<<<<<<< HEAD

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
=======
    if (!input_image) {
        cerr << "Error reading input image." << endl;
        return -1;
    }

    // Initialize semaphores
    sem_init(&sem_smoothened, 0, 0);
    sem_init(&sem_details, 0, 0);
    sem_init(&sem_sharpened, 0, 0);

    int iterations = 1000;  // Perform the transformations multiple times

    // Create threads for S1, S2, and S3
    thread t1(thread_S1, input_image, iterations);
    thread t2(thread_S2, input_image, iterations);
    thread t3(thread_S3, input_image, iterations);

    // Wait for the threads to finish
    t1.join();
    t2.join();
    t3.join();

    // Clean up images and write output
    // if (sharpened_image) {
        write_ppm_file(argv[2], sharpened_image);
    // }

    // Cleanup allocated memory
    delete input_image;
    delete smoothened_image;
    delete details_image;
    delete sharpened_image;

    // Destroy semaphores
>>>>>>> 6160ca6421c7da131c022b76d14ddfcd148ef824
    sem_destroy(&sem_smoothened);
    sem_destroy(&sem_details);
    sem_destroy(&sem_sharpened);

<<<<<<< HEAD
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
=======
>>>>>>> 6160ca6421c7da131c022b76d14ddfcd148ef824
    return 0;
}
