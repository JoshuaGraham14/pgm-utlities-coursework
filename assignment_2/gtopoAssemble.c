/***********************************/
/* COMP 1921  Programming Project  */
/* Assignment_2                    */
/*                                 */
/* Joshua Graham                   */
/* ID: 201496708                   */
/* sc21jg@leeds.ac.uk              */
/*                                 */
/* Date commenced: 22/05/22        */
/***********************************/

/***********************************/
/* Reads a gtopo file and splits it  */
/***********************************/

/***********************************/
/* Main Routine                    */
/***********************************/

/* Libraries */
#include <stdio.h> //library for I/O routines
#include <stdlib.h> //library for memory routines
#include <string.h> //library for string manipulation

/* Header files */
#include "gtopoImage.h"
#include "gtopoErrors.h"

/* FUNC: checks the number of arguments is correct */
int checkArgumentCountAssemble(int argc);

// /* FUNC: checks if the inputted width and height are both integers and greater than 0 */
// int validateWidthAndHeight(char *width, char *height);

/* FUNC: checks that both inputted row and column of the subimage are both integers and greater than or equal to 0 */
int validateRowAndColumnPosition(char *row, char *column);

/* FUNC: splits the input */
int writeAssembled(char *filename, Image *imagePointer, int reductionFactor);

/***********************************/
/* main routine                    */
/*                                 */
/* 4+3i CLI parameters:            */
/* argv[0]: executable name        */
/* argv[1]: output file name       */
/* argv[2]: width of output image  */
/* argv[3]: height of output image */
/* Remainder: quintuplets of:      */
/* argv[3i+1]: start row position  */
/* argv[3i+2]: start column position */
/* argv[3i+3]: subimage to insert  */
/* argv[3i+4]: width of subimage   */
/* argv[3i+5]: height of subimage  */
/*                                 */
/* returns 0 on success            */
/* non-zero error code on fail     */
/***********************************/
int main(int argc, char **argv)
{ /* main() */

    /* check for correct number of arguments */
    int returnVal; //return value variable
    /* check if there were correct CLI arguments   */
	if((returnVal = checkArgumentCountAssemble(argc)) != 0)
    {
        /* if there weren't correct CLI arguments:   */
        if (returnVal == -1)
        {
            /* if there were no CLI arguments    */
            /* output usage message and return 0 */
            printf("Usage: %s outputImage.gtopo width height (row column inputImage.gtopo)+\n", argv[0]);
            return EXIT_NO_ERRORS;
        }
        /* else return the return value of the checkArgumentCount() method */
        return returnVal;
    }
	
	/* create an imagePtr to store the main gtopo image data as an Image struct */
    Image *mainImage = malloc(sizeof(Image)); // dynamically allocate memory for mainImage

    /* first validate the width and height of the mainImage, returning if they are not valid */
	if ((returnVal = validateWidthAndHeight(argv[2], argv[3])) != 0) return returnVal;

    /* now that the inputted width and height have been validated, we can initialise the width and height and allocate memory for the Image's imageData */
    initialiseImage(mainImage, argv[2], argv[3]); //initialise the fields of the Image
    mallocImageDataArray(mainImage); //allocate memory for the Image's imageData

    /* first validate the width and height of the mainImage, returning if they are not valid */
	if ((returnVal = validateWidthAndHeight(argv[2], argv[3])) != 0) return returnVal;

    /* create another Image as a temporary store of the image to insert's data */
    Image *subImage = malloc(sizeof(Image)); // dynamically allocate memory for subImage

    for (int quintupletIndex = 4; quintupletIndex<argc-4; quintupletIndex+=5)
    {
        /* NOTE: */
        /* argv[quintupletIndex]:  start row position     */
        /* argv[quintupletIndex+1]: start column position */
        /* argv[quintupletIndex+2]: subimage to insert    */
        /* argv[quintupletIndex+3]: width of subimage     */
        /* argv[quintupletIndex+4]: height of subimage    */

        //validateRowAndColumnPosition(argv[10], argv[11]);
        // printf("%d, %s, %d\n", tripletIndex, argv[tripletIndex], validateRowAndColumnPosition(argv[tripletIndex], argv[tripletIndex+1]));
        // printf("%d, %s, %d\n", tripletIndex+1, argv[tripletIndex+1], validateRowAndColumnPosition(argv[tripletIndex], argv[tripletIndex+1]));

        /* first validate the row and column position of the subImage, returning if they are not valid */
	    if ((returnVal = validateRowAndColumnPosition(argv[quintupletIndex], argv[quintupletIndex+1])) != 0) return returnVal;

        /* Read data from input subImage file, store data in subImage */
	    if ((returnVal = readGtopoFile(argv[quintupletIndex+2], subImage, argv[quintupletIndex+3], argv[quintupletIndex+4])) != 0) return returnVal;

        /* define for-loop variable counters: */
        int rowIndex;
        int columnIndex;
        /* nested iteratation through each element/pixelValue in the imageData array,   */
        /* BUT each loop increments by the reductionFactor in order to reduce the image */
        for (rowIndex = 0; rowIndex < subImage->height; rowIndex++)
        { /*per row of pixels*/
            for (columnIndex = 0; columnIndex < subImage->width; columnIndex++)
            { /*per pixel*/
                /* replace pixel of mainImage with the corresponding pixel of the subImage */
                
                mainImage->imageData[atoi(argv[quintupletIndex])+rowIndex][atoi(argv[quintupletIndex+1])+columnIndex] = subImage->imageData[rowIndex][columnIndex];

            } /*per pixel*/
        }
    }

    /* Write mainImage data to filename and only return returnVal if it wasn't successful */
    if ((returnVal = writeGtopoFile(argv[1], mainImage, 1)) != 0) return returnVal;

	/* at this point, we are done and can exit with a success code */
    printf("ASSEMBLED\n");
	return EXIT_NO_ERRORS;
} /* main() */

/******************************************/
/* FUNC: checkArgumentCountAssemble       */
/* -> checks the number of arguments      */
/* supplied against the specified number  */
/* of arguments.                          */
/*                                        */
/* Parameters:                            */
/* - argc: integer num of arguments       */
/* Returns: - 0 on success                */
/*          - non-zero error code on fail */
/******************************************/
int checkArgumentCountAssemble(int argc)
{
    if (argc <= 9 || (argc-4)%5 != 0)
    { /* wrong arg count */
        /* IF there were no arguments */
        if (argc == 1) return -1;

        /* ELSE output error message and return with error code */ 
        printf("ERROR: Bad Argument Count\n");
        return ERROR_BAD_ARGUMENT_COUNT;
    } /* wrong arg count */

    /* ELSE return with success code */
    return EXIT_NO_ERRORS;
}

/******************************************/
/* FUNC: validateRowAndColumnPosition     */
/* ->  checks that inputted row and       */
/* column positions are both integers and */
/* greater than zero.                     */
/*                                        */
/* Parameters:                            */
/* - row: char pointer to CL argument   */
/* - column: char pointer to CL argument  */
/* Returns: - 0 on success                */
/*          - ERROR_MISCELLANEOUS on fail */
/******************************************/
int validateRowAndColumnPosition(char *row, char *column)
{
    /* if row and column are both integers (including 0)*/
    if ((atoi(row) && atoi(column)) || 
    (strcmp(row, "0") == 0 || atoi(column)) ||
    (strcmp(column, "0") == 0 || atoi(row)) ||
    (strcmp(row, "0") == 0 || strcmp(column, "0") == 0)
    )
    {
        int rowInt = atoi(row);
        int columnInt = atoi(column);
        /* if reduction factor is greater than or equal to 0 */
        if (rowInt >= 0 || columnInt >= 0)
        {
            /* return with success code */
            return EXIT_NO_ERRORS;
        }
    }
    /* print an error message        */
    printf("ERROR: Miscellaneous (row or column parameter invalid)\n");
    /* return an error code          */
    return ERROR_MISCELLANEOUS;
}

/******************************************/
/* FUNC: writeAssembled                       */
/* -> splits the input image into         */
/* tilingFactor * tilingFactor smaller    */
/* images corresponding to parts of the   */
/* image.                                 */
/*                                        */
/* Parameters:                            */
/* - filename: filename string            */
/* - imagePointer: Image pointer          */
/* - tilingFactor: Tiling Factor          */
/* Returns: - 0 on success                */
/*          - non-zero error code on fail */
/******************************************/
int writeAssembled(char *filename, Image *imagePointer, int tilingFactor)
{
    return 0;
}