#include <stdio.h>
#include <string>
#include <windows.h>

#include "vigenere.h"

void OpenFileToRead()
{
    /* open file in order to read info */
    inFile = fopen(inFilename, "r");

    if (inFile == NULL) {
        printf("Could not open file %s", inFilename);
        exit(0);
    }
}


void OpenFileToWrite()
{
    /* open file in order to write info */
    outFile = fopen(outFilename, "w");

    if (outFile == NULL) {
        printf("Could not open file %s", outFilename);
        exit(0);
    }
}

char LetterToUpper(char letter)
{
    if ('a' <= letter && 'z' >= letter)
    {
        return (char)(letter - 32);
    }
    else
    {
        return letter;
    }
}

void ParseText(char* text)
{
    for (unsigned int i = 0; i < strlen(text); i++)
    {
        *(text + i) = LetterToUpper(*(text + i));
    }
}

int MatchPlaintextLength(char* key, int* lenKey, int lenPlaintext)
{
    int counter = 0;

    if (*lenKey > lenPlaintext)
    {
        return -1;
    }
    else if (*lenKey == lenPlaintext)
    {
        return 0;
    }
    else
    {
        while (*lenKey < lenPlaintext)
        {
            *(key + *lenKey) = *(key + counter);
            (*lenKey)++;
            counter++;
            if (counter == strlen(key) - 1)
            {
                counter = 0;
            }
        }
        return 0;
    }
}


void ReadTextInputFile(char* text)
{
    if (text)
    {
        if (fgets(text, SIZE_BUFFER, inFile) != NULL)
        {
            ParseText(text);
        }
        else
        {
            if (!strcmp(text, ""))
            {
                fprintf(outFile, "Not input text\n");
            }
            else
            {
                // do nothing
            }
        }
    }
}

void ReadTextStd(char* text, const char* message)
{
    printf("%s", message);
    scanf("%s", text);
    ParseText(text);
}

void PrintConsoleMessageWaiting(const char* message, unsigned int timeToWait)
{
    printf("%s", message);
    for (unsigned int i = 0; i < TIMES_ITERATION_PRINT_DOT_IN_CONSOLE; i++)
    {
        printf(".");
        Sleep(timeToWait);
    }
    printf("\nThe output is printed in file!\n");
}


int GetCharIndex(char letter)
{
    int letterPosition = -1;
    unsigned int i;
    for (i = 0; i < ALPHABET_CHARS; i++)
    {
        if (letter == alphabet[i].key)
        {
            //letterPosition = i;
            return i;
        }
    }
    return letterPosition;
}

void Encrypt(char* plaintext, char* key, char* ciphered)
{
    unsigned int i;
    int letterPosPlaintext;
    int letterPosKey;

    for (i = 0; i < strlen(plaintext); i++)
    {
        letterPosPlaintext = GetCharIndex(plaintext[i]);
        letterPosKey = GetCharIndex(key[i]);
        *(ciphered + i) = alphabet[(alphabet[letterPosKey].index + alphabet[letterPosPlaintext].index) % 26].key;
    }
}


void Decrypt(char* ciphered, char* key, char* plaintext)
{
    unsigned int i;
    int letterPosCiphered;
    int letterPosKey;

    for (i = 0; i < strlen(ciphered); i++)
    {
        letterPosCiphered = GetCharIndex(ciphered[i]);
        letterPosKey = GetCharIndex(key[i]);
        *(plaintext + i) = alphabet[((alphabet[letterPosCiphered].index - alphabet[letterPosKey].index) + 26) % 26].key;
    }
}


int main()
{
    /* declarations */
    char* key;
    char* plaintext;
    char* ciphertext;
    int lenKey;
    int lenChiphered{};

    /* memory allocations */
    plaintext = (char*) calloc(SIZE_BUFFER, sizeof(char*) * SIZE_BUFFER);
    key = (char*) calloc(SIZE_BUFFER, sizeof(char*) * SIZE_BUFFER);
    ciphertext = (char*) calloc(SIZE_BUFFER, sizeof(char*) * SIZE_BUFFER);

    
    /* open input.txt file to read the plaintext */
    OpenFileToRead();
    /* open output.txt file in order to write the encrypted text */
    OpenFileToWrite();
    /* read the plain text which should be encrypted */
    ReadTextInputFile(plaintext);
    /* read the key for Vigenere algorithm */
    ReadTextStd(key, "Insert the key for encrypt operation: ");

    if (plaintext)
    {
        /* set the length for plaintext */
        lenChiphered = strlen(plaintext);
    }
    if (key)
    {
        /* set the length of the key*/
        lenKey = strlen(key);
    }

    /* the key from input should have the same length as the input plaintext */
    if (key && plaintext)
    {
        /* match the length of the key with the length of the plaintext */
        MatchPlaintextLength(key, &lenKey, lenChiphered);
    }

    PrintConsoleMessageWaiting("Encrypt text ", TIME_TO_WAIT_PROCESSING);
    
    Encrypt(plaintext, key, ciphertext);
    
    fprintf(outFile, "The text encrypted is %s\n", ciphertext);
    /* clear the current plaintext in order to save the deciphered text */
    if (plaintext)
    {
        memset(plaintext, '\0', SIZE_BUFFER);
    }
    /* read the key for Vigenere algorithm */
    ReadTextStd(key, "\nInsert the key for decryption operation: ");
    
    PrintConsoleMessageWaiting("Decrypt text ", TIME_TO_WAIT_PROCESSING);
    
    if (key)
    {
        /* set the length of the key*/
        lenKey = strlen(key);
    }

    if (key)
    {
        MatchPlaintextLength(key, &lenKey, lenChiphered);
    }

    Decrypt(ciphertext, key, plaintext);
    
    fprintf(outFile, "The text decrypted is %s\n", plaintext);
    /* close the streams */
    fclose(inFile);
    fclose(outFile);

	return 0;
}