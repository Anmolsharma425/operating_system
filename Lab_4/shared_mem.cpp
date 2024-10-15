#include <iostream>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include "libppm.h"
#include <cstdint>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

using namespace std;

// Declare semaphores and shared memory pointers for the images
// Create shared memory for images
uint8_t *create_shared_image(struct image_t *input_image, uint8_t *image)
{
	// struct image_t* image = new image_t;
	// cout<<"HELLO 4\n";
	// image->width = input_image->width;
	// image->height = input_image->height;

	image = new uint8_t[input_image->width * input_image->height * 3 * sizeof(uint8_t)];
	int index = 0;
	for (int i = 0; i < input_image->height; i++)
	{
		// image->image_pixels[i] = new uint8_t*[input_image->width];
		for (int j = 0; j < input_image->width; j++)
		{
			// image->image_pixels[i][j] = new uint8_t[3];
			for (int k = 0; k < 3; k++)
			{
				image[index] = input_image->image_pixels[i][j][k];
				// cout << (int)image[index] << " ";
				index++;
			}
			// cout << "\n";
		}
	}
	return image;
}

// S1_smoothen function
struct image_t *S1_smoothen(struct image_t *input_image)
{
	// TODO
	// remember to allocate space for smoothened_image. See read_ppm_file() in libppm.c for some help.
	struct image_t *image = new struct image_t;
	image->height = input_image->height;
	image->width = input_image->width;
	image->image_pixels = new uint8_t **[image->height];
	for (int i = 0; i < input_image->height; i++)
	{
		image->image_pixels[i] = new uint8_t *[image->width];
		for (int j = 0; j < input_image->width; j++)
		{
			image->image_pixels[i][j] = new uint8_t[3];
			for (int k = 0; k < 3; k++)
			{
				if (i == 0 || j == 0 || i == input_image->height - 1 || j == input_image->width - 1)
				{
					image->image_pixels[i][j][k] = input_image->image_pixels[i][j][k];
				}
				else
				{
					int sum = 0;
					for (int r = i - 1; r <= i + 1; r++)
					{
						for (int c = j - 1; c <= j + 1; c++)
						{
							sum += input_image->image_pixels[r][c][k];
						}
					}
					image->image_pixels[i][j][k] = (uint8_t)(int)(sum / 9);
				}
			}
		}
	}
	return image;
}

// S2_find_details function
struct image_t *S2_find_details(struct image_t *input_image, uint8_t *smoothened_image)
{
	// TODO
	struct image_t *detail = new struct image_t;
	detail->width = input_image->width;
	detail->height = input_image->height;
	detail->image_pixels = new uint8_t **[input_image->height];
	int index = 0;
	for (int i = 0; i < input_image->height; i++)
	{
		detail->image_pixels[i] = new uint8_t *[input_image->width];
		for (int j = 0; j < input_image->width; j++)
		{
			detail->image_pixels[i][j] = new uint8_t[3];
			for (int k = 0; k < 3; k++)
			{
				// cout<<(int)smoothened_image[index]<<" ";
				detail->image_pixels[i][j][k] = abs(input_image->image_pixels[i][j][k] - smoothened_image[index++]);
				// cout<<smoothened_image->image_pixels[i][j][k]<<endl;
			}
			// cout<<"\n";
		}
	}
	return detail;
}

// S3_sharpen function
struct image_t *S3_sharpen(struct image_t *input_image, uint8_t *details_image)
{
	// TODO
	struct image_t *sharp = new struct image_t;
	sharp->height = input_image->height;
	sharp->width = input_image->width;
	sharp->image_pixels = new uint8_t **[input_image->height];
	int index = 0;
	for (int i = 0; i < input_image->height; i++)
	{
		sharp->image_pixels[i] = new uint8_t *[input_image->width];
		for (int j = 0; j < input_image->width; j++)
		{
			sharp->image_pixels[i][j] = new uint8_t[3];
			for (int k = 0; k < 3; k++)
			{
				// cout<<(int)input_image->image_pixels[i][j][k]<<" ";
				sharp->image_pixels[i][j][k] = min(255, input_image->image_pixels[i][j][k] + details_image[index++]);
			}
			// cout<<"\n";
		}
	}
	return sharp;
}

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		cerr << "Usage: " << argv[0] << " <input_image.ppm> <output_image.ppm>" << endl;
		return 1;
	}

	struct image_t *input_image = read_ppm_file(argv[1]);
	if (!input_image)
	{
		cerr << "Error reading input image." << endl;
		return -1;
	}

	const char *name_1 = "/my_shared_memory_1";
	const char *name_2 = "/my_shared_memory_2";

	int shm_fd_1 = shm_open(name_1, O_CREAT | O_RDWR, 0666);
	if (shm_fd_1 == -1)
	{
		perror("shm_open");
		return 1;
	}

	int shm_fd_2 = shm_open(name_2, O_CREAT | O_RDWR, 0666);
	if (shm_fd_2 == -1)
	{
		perror("shm_open");
		return 1;
	}

	size_t size = input_image->width * input_image->height * 3 * sizeof(uint8_t)+1;

	if (ftruncate(shm_fd_1, size) == -1)
	{
		perror("ftruncate");
		return 1;
	}

	if (ftruncate(shm_fd_2, size) == -1)
	{
		perror("ftruncate");
		return 1;
	}

	uint8_t *smoothened_ptr = (uint8_t *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_1, 0);
	uint8_t *detail_ptr = (uint8_t *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_2, 0);

	// Create shared memory for images
	// smoothened_ptr = create_shared_image(input_image, smoothened_ptr);
	// detail_ptr = create_shared_image(input_image, detail_ptr);
	// sharpened_image = create_shared_image(input_image);

	int iterations = 500; // Number of iterations

	sem_t *sem_smoothened = sem_open("/my_semaphore_1", O_CREAT, 0666, 0);
	sem_t *sem_detail = sem_open("/my_semaphore_2", O_CREAT, 0666, 0);
	sem_init(sem_smoothened, 1, 0);
	sem_init(sem_detail, 1, 0);

	// Fork to create processes for S1, S2, and S3
	pid_t pid1 = fork();
	if (pid1 == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid1 == 0)
	{
		// process_S1(input_image, iterations, smoothened_image, sem_smoothened);  // Run S1 in child process
		sem_post(sem_smoothened);
		// sem_init(sem_smoothened, 1, 1);
		for (int i = 0; i < iterations; i++)
		{
			sem_wait(sem_smoothened);
			struct image_t *smoothened_image = S1_smoothen(input_image);
			memcpy(smoothened_ptr, create_shared_image(smoothened_image, smoothened_ptr), size);

			// cout << "S1\n";
			sem_post(sem_smoothened); // Signal completion of smoothening 0->1
									  // sem_post(sem_detail);
			while(smoothened_ptr[0]!=(uint8_t)(-1)){
				// cout<<"HELLO 1 ";
				if(i==iterations-1) break;
			}
		}
		exit(EXIT_SUCCESS);
	}

	pid_t pid2 = fork();
	// sleep(1);
	if (pid2 == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid2 == 0)
	{
		struct image_t *smoothened_image;
		sem_post(sem_detail);
		for (int i = 0; i < iterations; i++)
		{
			// cout << "S2\n";
			sem_wait(sem_detail);
			sem_wait(sem_smoothened); // Wait for S1 to complete
			// cout << "HELLO 3\n";
			// create_shared_image(smoothened_ptr, smoothened_image);
			struct image_t *details_image = S2_find_details(input_image, smoothened_ptr);
			memset(smoothened_ptr, (uint8_t)(-1), size);
			memcpy(detail_ptr, create_shared_image(details_image, detail_ptr), size);
			// cout<<"S2\n";
			sem_post(sem_detail); // Signal completion of finding details
			sem_post(sem_smoothened);
			while(detail_ptr[size/2]!=(uint8_t)(-1)){
				// cout<<"HELLO 2 ";
				if(i==iterations-1) break;
			}
		}
		exit(EXIT_SUCCESS);
	}

	struct image_t *sharpened_image;
	struct image_t *details_image;
	// sleep(1);
	for (int i = 0; i < iterations; i++)
	{
		// cout << "HELLO 4\n";
		sem_wait(sem_detail); // Wait for S2 to complete
		// create_shared_image(detail_ptr, details_image);
		// cout << "S3\n";
		sharpened_image = S3_sharpen(input_image, detail_ptr);
		memset(detail_ptr, (uint8_t)(-1), size);
		sem_post(sem_detail);
		cout<<i<<"\n";
		while(detail_ptr[0]==(uint8_t)(-1)){
			// cout<<"HELLO 3 ";
			if(i==iterations-1)
				break;
		}
	}

	// Wait for all child processes to complete
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);

	// Write the sharpened image to output file
	write_ppm_file(argv[2], sharpened_image);

	sem_close(sem_smoothened);
	sem_close(sem_detail);

	sem_unlink("/my_semaphore_1");
	sem_unlink("/my_semaphore_2");

	// Unmap the shared memory object
	if (munmap(smoothened_ptr, size) == -1)
	{
		perror("munmap");
		return 1;
	}

	if (munmap(detail_ptr, size) == -1)
	{
		perror("munmap");
		return 1;
	}

	// Close the shared memory file descriptor
	if (close(shm_fd_1) == -1)
	{
		perror("close");
		return 1;
	}
	if (close(shm_fd_2) == -1)
	{
		perror("close");
		return 1;
	}

	// Unlink the shared memory object
	if (shm_unlink(name_1) == -1)
	{
		perror("shm_unlink");
		return 1;
	}
	if (shm_unlink(name_2) == -1)
	{
		perror("shm_unlink");
		return 1;
	}

	// Cleanup shared memory and semaphores
	delete input_image;

	return 0;
}
