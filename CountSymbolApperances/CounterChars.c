#include <stdio.h>
#include <stdlib.h>
#include "CounterChars.h"

int main()
{
    char* buffer;
    buffer = calloc(SIZE_BUFFER, sizeof(char*) * SIZE_BUFFER);

    OpenFileToRead();

    while (fgets(buffer, SIZE_BUFFER, inFile) != NULL)
    {
        CountCharAppearance(buffer);
    }

    //free(buffer);
    fclose(inFile);

    /* print map appearances for python plotting */
    OpenFileToWrite(outFilenamePy);
    WriteFile();
    fclose(outFile);

    GetCharAppearanceNominalVal();
    GetCharAppearanceProcentualVal();
    
    fclose(outFile);
    
}

void GetCharAppearanceNominalVal()
{
    OpenFileToWrite(outFilename);
    fprintf(outFile, "\n\t\t\t Frequency in nominal value\n");
    for (int i = 0; i < NUM_CHARS_DIGITS; i++)
    {
#if ALL_CHARS
        fprintf(outFile, "\tFrequency of char \'%c\' is %d\n", map_appearances[i].key, map_appearances[i].value);
#else
        if (map_appearances[i].value)
        {
            fprintf(outFile, "\tFrequency of char \'%c\' is %d\n", map_appearances[i].key, map_appearances[i].value);
        }
#endif // ALL_CHARS
    }
    fclose(outFile);
}

void GetCharAppearanceProcentualVal()
{
    OpenFileToAppend();
    fprintf(outFile, "\n\n\t\t\t Frequency in percentage\n");
    for (int i = 0; i < NUM_CHARS_DIGITS; i++)
    {
        /*
        * counterChars...........................100%
        * map[i].value...........................x %
        * --------------------------------------------
        * => x = (map[i].value / counterChars) * 100
        */
        double percent = (double)(((double)map_appearances[i].value / counterChars) * 100);
#if ALL_CHARS
        fprintf(outFile, "\tFrequency of char \'%c\' in percentage is %0.4f%%\n", map_appearances[i].key, percent);
#else
        if (map_appearances[i].value)
        {
            fprintf(outFile, "\tFrequency of char \'%c\' in percentage is %0.4f%%\n", map_appearances[i].key, percent);
        }
#endif // ALL_CHARS
        
    }
    fclose(outFile);
}

void CountCharAppearance(char* buffer)
{
    int charIdx = 0;
    while (*(buffer + charIdx) != '\n')
    {
        /* search the *(buffer + charIdx) character to increase number of apperances */
        char currentChar = *(buffer + charIdx);
        for (unsigned int i = 0; i < NUM_CHARS_DIGITS; i++)
        {
            /* check if we find the current character in the map with all chars and digits */
            if (map_appearances[i].key == currentChar)  //use tolower() & toupper() for extended usecase
            {
                /* increase number of appearances */
                map_appearances[i].value++;
            }
        }
        /* go to next char in current buffer */
        charIdx++;
    }
    /* count characters from current buffer */
    counterChars += charIdx;

    /* print in output file */
    //fputs(buffer, outFile);

}

void OpenFileToRead()
{
    /* open file in order to read info */
    inFile = fopen(inFilename, "r");

    if (inFile == NULL) {
        printf("Could not open file %s", inFilename);
        return 1;
    }
}

void OpenFileToWrite(char* outFilename)
{
    /* open file in order to write info */
    outFile = fopen(outFilename, "w");

    if (outFile == NULL) {
        printf("Could not open file %s", outFilename);
        return 1;
    }
}

void OpenFileToAppend()
{
    /* open file in order to append info */
    outFile = fopen(outFilename, "a");

    if (outFile == NULL) {
        printf("Could not open file %s", outFilename);
        return 1;
    }
}

void WriteFile()
{
    unsigned int mapIdx = 0;
    for (mapIdx = 0; mapIdx < NUM_CHARS_DIGITS; mapIdx++)
    {
        fprintf(outFile, "%c %d\n", map_appearances[mapIdx].key, map_appearances[mapIdx].value);
    }
}
