#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  FILE* sample = NULL;
  FILE* file = NULL;
  FILE* outputFile = NULL;
  char header[3];
  char fileName[200];
  char outputFileName[200];
  int photoNumber;
  int width;
  int height;
  long size;
  int pixelValue;
  int total = 0;
  int average = 0;

  printf("Welcome to the photo editor.\n");
  printf("How many photos will you be using? (type in an integer): \n");
  scanf("%d", &photoNumber);

  //Initialize 2D array that will contain .ppm file names
  char files[photoNumber][200];

  printf("Enter the names of the .ppm files: \n");
  //Store user input into char array, copy to element of 2D array
  for(int i = 0; i < photoNumber; i = i + 1) {
    scanf("%s", &fileName);
    strcpy(files[i], fileName);
  }

  //Retrieve width and height from one of the user entered files
  printf("Attempting to open sample file.\n");
  sample = fopen(files[0], "r");
  printf("sample file opened.\n");
  fscanf(sample, "%s", header);
  fscanf(sample, " %d %d", &width, &height);  //Number of pixels in image
  fscanf(sample, " %d", &pixelValue);
  size = width * height * 3;  //Number of integers in file
  fclose(sample);

  //Create integer array of pointers to integer arrays that contain pixel values (2D)
  int *values[photoNumber];
  for(int i = 0; i < photoNumber; i = i + 1) {
    values[i] = (int*)malloc(sizeof(int) * size);  //Allocate memory for inner arrays
  }

  printf("Reading file contents.\n");
  //Read pixel values from each .ppm file
  for(int i = 0; i < photoNumber; i = i + 1) {
    file = fopen(files[i], "r");
    char throwaway1[3];
    int throwaway2;
    int throwaway3;
    int throwaway4;
    fscanf(file, "%s", throwaway1);
    fscanf(file, " %d %d", &throwaway2, &throwaway3);
    fscanf(file, " %d", &throwaway4);

    //Put pixel values into 2D array
    for(int j = 0; j < size; j = j + 1) {
      fscanf(file, " %d", &(values[i][j]));
    }

    fclose(file);
  }
  printf("Files read.\n");

  //Dynamically allocate memory for integer array containing average values
  int *averages = (int*)malloc(sizeof(int) * size);

  //Calculate average of each pixel and put values in averages array
  printf("Fixing pixel values.\n");
  for(int j = 0; j < size; j = j + 1) {
    for(int i = 0; i < photoNumber; i = i + 1) {
      total = total + values[i][j];
    }

    average = (int) (total / photoNumber);
    *(averages + j) = average;  //Set elements of averages array with each average
    total = 0;
  }
  free(values);  //Free array memory from heap
  printf("Pixel values fixed.\n");

  //Create output file and write averages data to it
  printf("Enter your preferred name for your final picture .ppm file: ");
  scanf("%s", &outputFileName);
  outputFile = fopen(outputFileName, "w");

  fprintf(outputFile, "P3\n");
  fprintf(outputFile, "%d %d\n", width, height);
  fprintf(outputFile, "255\n");
  for(int i = 0; i < size; i = i + 1) {
    fprintf(outputFile, "%d ", *(averages + i));
  }

  //Free array memory from heap and close output file
  free(averages);
  fclose(outputFile);
  printf("%s created and saved in current directory.", outputFileName);
  return 1;
}
