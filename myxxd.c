#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define BAD_NUMBER_ARGS 1

/**
 * Parses the command line.
 *
 * argc: the number of items on the command line (and length of the
 *       argv array) including the executable
 * argv: the array of arguments as strings (char* array)
 * bits: the integer value is set to TRUE if bits output indicated
 *       outherwise FALSE for hex output
 *
 * returns the input file pointer (FILE*)
 **/
FILE *parseCommandLine(int argc, char **argv, int *bits) {
  if (argc > 2) {
    printf("Usage: %s [-b|-bits]\n", argv[0]);
    exit(BAD_NUMBER_ARGS);
  }

  if (argc == 2 &&
      (strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "-bits") == 0)) {
    *bits = TRUE;
  } else {
    *bits = FALSE;
  }

  return stdin;
}

/**
 * Writes data to stdout in hexadecimal.
 *
 * See myxxd.md for details.
 *
 * data: an array of no more than 16 characters
 * size: the size of the array
 **/
void printDataAsHex(unsigned char *data, size_t size)
{
    unsigned int q = 0;
    for (int i = 0; i < size; i++)
    {
        if (i % 2 == 0)
        {
            printf(" ");
        }
        printf("%02x", data[i]);
        q += 2;
    }
    for (int i = q; i < 32; i ++)
    {
        printf(" ");
        if (i % 4 == 0)
        {
            printf(" ");
        }
    }
}

/**
 * Writes data to stdout as characters.
 *
 * See myxxd.md for details.
 *
 * data: an array of no more than 16 characters
 * size: the size of the array
 **/
void printDataAsChars(unsigned char *data, size_t size) {
    for (unsigned int i = 0; i < size; i++)
    {
        int myDec = data[i];
        if (myDec == 10)
        {
            printf(".");
            break;
        }
        else if ((myDec < 32) || (myDec > 126))
        {
            printf(".");
        }
        else
        {
            printf("%c", data[i]);
        }
    }
}

void readAndPrintInputAsHex(FILE *input)
{
    unsigned char data[16];
    int numBytesRead = fread(data, 1, 16, input);
    unsigned int offset = 0;
    while (numBytesRead != 0)
    {
        printf("%08x:", offset);
        offset += numBytesRead;
        printDataAsHex(data, numBytesRead);
        printf("  ");
        printDataAsChars(data, numBytesRead);
        printf("\n");
        numBytesRead = fread(data, 1, 16, input);
    }
}

/**
 * Bits output for xxd.
 *
 * See myxxd.md for details.
 *
 * input: input stream
 **/
void readAndPrintInputAsBits(FILE *input)
{
    unsigned char data[6];
    int numBytesRead = fread(data, 1, 6, input);
    unsigned int offset = 0;

    // Creates a binary 1 or 0, adds them to an array, then prints the array in reverse to give the correct values
    while (numBytesRead != 0)
    {
        printf("%08x:", offset);
        offset += numBytesRead;
        unsigned int q = 0;
        int binChar[8];
        for (unsigned int i = 0; i < numBytesRead; i++)
        {
            printf(" ");
            int dec = data[i];
            for (unsigned int j = 0; j < 8; j++)
            {
                if (dec % 2 == 1)
                {
                    binChar[j] = 1;
                }
                else
                {
                    binChar[j] = 0;
                }
                dec = dec / 2;
                q += 1;
            }
            for (unsigned int k = 8; k > 0; k--)
            {
                printf("%d", binChar[k - 1]);
            }
        }
        for (int i = q; i < 48; i++)
        {
            printf(" ");
            if (i % 8 == 0)
            {
                printf(" ");
            }
        }

        // Prints the characters in order of how they were input, if character is out of range, outputs a "."
        printf("  ");
        for (unsigned int i = 0; i < 6; i++)
        {
            int myDec = data[i];
            if (myDec == 10)
            {
                printf(".");
                break;
            }
            else if ((myDec < 32) || (myDec > 126))
            {
                printf(".");
            }
            else
            {
                printf("%c", data[i]);
            }
        }
        printf("\n");
        numBytesRead = fread(data, 1, 6, input);
    }
}

int main(int argc, char **argv)
{
    int bits = FALSE;
    FILE *input = parseCommandLine(argc, argv, &bits);

    if (bits == FALSE)
    {
    readAndPrintInputAsHex(input);
    }
    else
    {
    readAndPrintInputAsBits(input);
    }
    return 0;
}
