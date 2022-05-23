#ifndef ERRORS_HEADER_FILE
#define ERRORS_HEADER_FILE
#include "gtopoImage.h" //include pgmImage file as Image struct it will
                      //need to be passed into some of the functions
                      //as arguments.

/* Constants */
#define MAGIC_NUMBER_RAW_PGM 0x3550
#define MAGIC_NUMBER_ASCII_PGM 0x3250
#define MIN_IMAGE_DIMENSION 1
#define MAX_IMAGE_DIMENSION 65536
#define MAX_COMMENT_LINE_LENGTH 128

/* Constants: Error Codes */
#define EXIT_NO_ERRORS 0
#define ERROR_BAD_ARGUMENT_COUNT 1
#define ERROR_BAD_FILE_NAME 2
#define ERROR_BAD_MAGIC_NUMBER 3
#define ERROR_BAD_COMMENT_LINE 4
#define ERROR_BAD_DIMENSIONS 5
#define ERROR_BAD_MAX_GRAY_VALUE 6
#define ERROR_IMAGE_MALLOC_FAILED 7
#define ERROR_BAD_DATA 8
#define ERROR_OUTPUT_FAILED 9
#define ERROR_MISCELLANEOUS 100

/* FUNC: checks the number of arguments supplied against the specified number of arguments */
int checkArgumentCount(int argc, int numOfArgs);

/* FUNC: checks the input file is valid */
int checkInputFile(FILE *filePointer);

/* FUNC: checks if the inputted width and height are both integers and greater than 0 */
int validateWidthAndHeight(char *width, char *height);

/* FUNC: checks if the image malloc is valid for the 2d array*/
int check2dImageDataMemoryAllocation(Image *imagePointer);

/* FUNC: checks if the image malloc is valid for each 1d array*/
int check1dImageDataMemoryAllocation(Image *imagePointer, int rowNum);

/* FUNC: checks if the image pixel is valid */
int checkPixelValue(Image *imagePointer, int pixelValue);

/* FUNC: checks if the output file is valid */
int checkOutputFile(FILE *filePointer);

/* FUNC: Check factor is an integer and is greater than 0 */
int validateFactorInput(char *charFactorInput);

/* FUNC: Check tiling output file template is correct */
int validateTileOutputTemplate(char *outputTemplateString);

/* FUNC: checks that both inputted row and column of the subimage are both integers and greater than or equal to 0. */
int validateRowAndColumnPosition(char *row, char *column);

/* FUNC: Handles the string outputMessage and closing variables and pointers after an error occurs */
int handleError(FILE *filePointer, char *filename, Image *imagePointer, int errorCode);

/* FUNC: Frees the image data 2d array */
void freeImageData (Image *imagePointer);

#endif