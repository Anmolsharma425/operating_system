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
#include <chrono>

using namespace std;

// S1_smoothen function
uint8_t *S1_smoothen(struct image_t *input_image)
{
	// TODO
	// remember to allocate space for smoothened_image. See read_ppm_file() in libppm.c for some help.
	uint8_t *image = new uint8_t[input_image->width * input_image->height * 3];
	int index = 0;
	for (int i = 0; i < input_image->height; i++)
	{
		for (int j = 0; j < input_image->width; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				if (i == 0 || j == 0 || i == input_image->height - 1 || j == input_image->width - 1)
				{
					image[index++] = input_image->image_pixels[i][j][k];
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
					image[index++] = (uint8_t)(int)(sum / 9);
				}
			}
		}
	}
	return image;
}

// S2_find_details function
uint8_t *S2_find_details(struct image_t *input_image, uint8_t *smoothened_image)
{
	// TODO
	int index = 0;
	uint8_t *detail = new uint8_t[input_image->height * input_image->width * 3];
	for (int i = 0; i < input_image->height; i++)
	{
		for (int j = 0; j < input_image->width; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				detail[index] = abs(input_image->image_pixels[i][j][k] - smoothened_image[index]);
				index++;
			}
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
				sharp->image_pixels[i][j][k] = min(255, input_image->image_pixels[i][j][k] + details_image[index++]);
			}
		}
	}
	return sharp;
}

void free_image(struct image_t *image)
{
    if (image)
    {
        // Free the image pixel data
        for (int i = 0; i < image->height; i++)
        {
            for (int j = 0; j < image->width; j++)
            {
                delete[] image->image_pixels[i][j];  // Free each RGB array
            }
            delete[] image->image_pixels[i];  // Free each row
        }
        delete[] image->image_pixels;  // Free the 2D array holding the rows

        // Finally, free the image struct itself
        delete image;
    }
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
	auto start = chrono::high_resolution_clock::now();
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

	size_t size = input_image->width * input_image->height * 3 * sizeof(uint8_t);

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

	int iterations = 1000; // Number of iterations

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
		for (int i = 0; i < iterations; i++)
		{
			uint8_t *smoothened_image;
			smoothened_image = S1_smoothen(input_image);

			if (i == 0)
				sem_post(sem_smoothened);

			sem_wait(sem_smoothened);
			memcpy(smoothened_ptr, smoothened_image, size);
			sem_post(sem_smoothened); // Signal completion of smoothening 0->1

			delete smoothened_image;

			while (smoothened_ptr[0] != (uint8_t)(-1))
			{
				if (i == iterations - 1)
					break;
			}
		}
		exit(EXIT_SUCCESS);
	}

	pid_t pid2 = fork();
	if (pid2 == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid2 == 0)
	{
		
		for (int i = 0; i < iterations; i++)
		{
			uint8_t *smoothened_image = new uint8_t[size];
			uint8_t *detail_image;

			sem_wait(sem_smoothened); // Wait for S1 to complete
			memcpy(smoothened_image, smoothened_ptr, size);
			memset(smoothened_ptr, (uint8_t)(-1), size);
			sem_post(sem_smoothened);
			detail_image = S2_find_details(input_image, smoothened_image);

			delete smoothened_image;

			if (i == 0)
				sem_post(sem_detail);

			sem_wait(sem_detail);
			memcpy(detail_ptr, detail_image, size);
			sem_post(sem_detail); // Signal completion of finding details

			delete detail_image;

			while (detail_ptr[size / 2] != (uint8_t)(-1))
			{
				if (i == iterations - 1)
					break;
			}
			while (smoothened_ptr[0] == (uint8_t)(-1))
			{
				if (i == iterations - 1)
					break;
			}
		}
		exit(EXIT_SUCCESS);
	}
	
	struct image_t *sharpened_image;
	for (int i = 0; i < iterations; i++)
	{
		if(i!=0) struct image_t *sharpened_image;
		uint8_t *detail_image = new uint8_t[size];
		sem_wait(sem_detail); // Wait for S2 to complete
		memcpy(detail_image, detail_ptr, size);
		memset(detail_ptr, (uint8_t)(-1), size);
		sem_post(sem_detail);

		sharpened_image = S3_sharpen(input_image, detail_image);

		if (i != 999)
			free_image(sharpened_image);
		
		delete detail_image;

		while (detail_ptr[0] == (uint8_t)(-1))
		{
			if (i == iterations - 1)
				break;
		}
	}

	// Wait for all child processes to complete
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);

	// Write the sharpened image to output file
	write_ppm_file(argv[2], sharpened_image);
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed_time = end - start;
	cout << "Time to execute using shared memory IPC " << elapsed_time.count() << endl;

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
