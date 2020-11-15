#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Utils.h"

void OpenFileToRead()
{
    /* open file in order to read info */
    inFile = fopen(inFilename, "r");

    if (inFile == NULL) {
        printf("Could not open file %s", inFilename);
        return 1;
    }
}


void OpenFileToWrite()
{
    /* open file in order to write info */
    outFile = fopen(outFilename, "w");

    if (outFile == NULL) {
        printf("Could not open file %s", outFilename);
        return 1;
    }
}

int isEqual(char* text1, char* text2)
{
    unsigned int charIdx = 0;
    if (strlen(text1) != strlen(text2))
    {
        return 0;
    }
    for (charIdx = 0; charIdx < strlen(text1); charIdx++)
    {
        if (*(text1 + charIdx) != *(text2 + charIdx))
        {
            return 0;
        }
        else
        {

        }
    }
    return 1;
}


void MapWhitespacesPositionInputBuffer(char* text)
{
    /* check every character from input text and keeps his position if it is a whitespace */
    for (int textCharId = 0; textCharId < strlen(text); textCharId++)
    {
        if (*(text + textCharId) == WHITESPACE)
        {
            whitespacesPos[nWhitespaces] = textCharId;
            nWhitespaces++;
        }
    }
}


void UnmapWhitespacesPostionInputBuffer(char* text)
{
    /* rebuild the input decrypted with corresponding whitespaces at their positions (positions keeps in whitespacesPos array) */
    for (int whitespacePosId = 0; whitespacePosId < strlen(whitespacesPos); whitespacePosId++)
    {
        /* text[whitespacesPos[whitespacePosId]] */
        *(text + whitespacesPos[whitespacePosId]) = WHITESPACE;
    }
}


char* encrypt(char* text)
{
    /*
    * This function receives text and shift and returns the encrypted text
    * Also, this function is used for decryption with shift = ALPHABET_CHARS - shift
    */
    char* result;
    int resultLength = 0;
    
    result = calloc(SIZE_BUFFER, sizeof(char*) * SIZE_BUFFER);

    /* traverse text */ 
    for (int i = 0; i < strlen(text); i++)
    {
        /* 
        * Apply transformation to each character
        * Encrypt Uppercase letters 
        */
        if (text[i] >= 'A' && text[i] <= 'Z')
        {
            if (result)
            {
                *(result + resultLength) = ((int)(text[i] + shift - ASCII_UPPER_A) % ALPHABET_CHARS + ASCII_UPPER_A);
            }
        }
        /* Encrypt Lowercase letters */ 
        else if (text[i] >= 'a' && text[i] <= 'z')
        {
            if (result)
            {
                *(result + resultLength) = ((int)(text[i] + shift - ASCII_LOWER_A) % ALPHABET_CHARS + ASCII_LOWER_A);
            }
        }
        else
        {
            if (result)
            {
                *(result + resultLength) = text[i];
            }
        }
        resultLength++;
    }

    /* Return the resulting string */ 
    return result;
}


char* decrypt(char* text)
{
    char* result;
    int resultLength;

    OpenFileToWrite();

    /* loop through every possible alphabet characters */
    for (int letterAlph = 0; letterAlph < ALPHABET_CHARS; letterAlph++)
    {
        /* It is important to set result to the blank string so that the previous iteration's value for result is cleared. */
        result = (char*)malloc(sizeof(char*) * strlen(text) + 1);
        resultLength = 0;
        /* run the encryption/decryption code on each letter in the input */
        for (int letterText = 0; letterText < strlen(text); letterText++)
        {
            if (text[letterText] >= 'A' && text[letterText] <= 'Z')
            {
                if ((text[letterText] - letterAlph - ASCII_UPPER_A) < 0)
                {
                    if (result)
                    {
                        *(result + resultLength) = (char)(91 + (text[letterText] - letterAlph - ASCII_UPPER_A));
                    }
                }
                else
                {
                    if (result)
                    {
                        *(result + resultLength) = (char)((text[letterText] - letterAlph - ASCII_UPPER_A) % ALPHABET_CHARS + ASCII_UPPER_A);
                    }
                }
            }
            else if (text[letterText] >= 'a' && text[letterText] <= 'z')
            {
                if ((text[letterText] - letterAlph - ASCII_LOWER_A) < 0)
                {
                    if (result)
                    {
                        *(result + resultLength) = (char)(123 + (text[letterText] - letterAlph - ASCII_LOWER_A));
                    }
                }

                else
                {
                    if (result)
                    {
                        *(result + resultLength) = (char)((text[letterText] - letterAlph - ASCII_LOWER_A) % ALPHABET_CHARS + ASCII_LOWER_A);
                    }
                }
            }
            else
            {
                if (result)
                {
                    *(result + resultLength) = (char)(text[letterText]);
                }
            }
            resultLength++;
        }
        if (result)
        {
            *(result + resultLength) = '\0';
        }

        //UnmapWhitespacesPostionInputBuffer(result);
        fprintf(outFile, "%s\n", result);
        free(result);
    }
    return result;
}


int main()
{
    char* buffer;
    char* resultEn;
    char* resultDec;

    buffer = calloc(SIZE_BUFFER, sizeof(char*) * SIZE_BUFFER);
    resultEn = calloc(SIZE_BUFFER, sizeof(char*) * SIZE_BUFFER);
    resultDec = calloc(SIZE_BUFFER, sizeof(char*) * SIZE_BUFFER);
    
    OpenFileToRead();
    OpenFileToWrite();

    while (fgets(buffer, SIZE_BUFFER, inFile) != NULL)
    {
        /* map whitespaces for input buffer */
        //MapWhitespacesPositionInputBuffer(buffer);
#if ENCRYPT
        /* encrypt the input */
        resultEn = encrypt(buffer);
        /* print in output file */
        fprintf(outFile, "The input encrypted with shift %d is %s\n\n", shift, resultEn);
#endif // 0

        
#if BRUTE_FORCE_ATTACK
#if ENCRYPT
        /* decrypt using brute force attack*/
        resultDec = decrypt(resultEn);
#else
        /* decrypt using brute force attack*/
        resultDec = decrypt(buffer);
#endif // 0
        
#else
#if KNOWN_SHIFT
        /* now prepare the shift value for decryption */
        printf("Give the shift to perform the decryption: ");
        scanf("%d", &shift);
        shift = ALPHABET_CHARS - shift;
#if ENCRYPT
        /* decrypt using encryption method with shift = 26 - shift*/
        resultDec = encrypt(resultEn);
#else
        /* decrypt using encryption method with shift = 26 - shift*/
        resultDec = encrypt(buffer);

#endif // 0

        /* restore shift value to encrypt next input */
        shift = ALPHABET_CHARS - shift;
        /* print the result in output file */
        fprintf(outFile, "The input decrypted is %s\n", resultDec);
#else 
        for (int alphCharId = ALPHABET_CHARS - 1; alphCharId >= 0; alphCharId--)
        {
            /* now prepare the shift value for decryption */
            //shift = ALPHABET_CHARS - shift;
            shift = alphCharId;
#if ENCRYPT
            /* decrypt using encryption method with shift = 26 - shift*/
            resultDec = encrypt(resultEn);
            /* print the input decrypted with corresponding shift */
            if (isEqual(buffer, resultDec))
            {
                fprintf(outFile, "The input decrypted with shift %d is %s\n", 26 - shift, resultDec);
            }
#else
            /* decrypt the input using encryption method with shift = 26 - shift */
            resultDec = encrypt(buffer);
            fprintf(outFile, "The input decrypted with shift %d is %s\n", 26 - shift, resultDec);
#endif // 0

            

        }
        /* print in output file */
        /*fprintf(outFile, "The input decrypted is %s\n", resultDec);*/
#endif // KNOWN_SHIFT
#endif // BRUTE_FORCE_ATTACK
        
        fprintf(outFile, "\n\n");
    }
    fclose(inFile);

    return 0;
}