#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#include "playfair.h"

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


void CountWhitespaces(char* text)
{
    int i;
    int lengthText = strlen(text);

    for (i = 0; i < lengthText; i++)
    {
        if (text[i] == WHITESPACE)
        {
            *(whitespacesPos + counterWhiteSpaces) = i;
            counterWhiteSpaces++;
        }
    }
}


int RemoveWhitespaces(char* text)
{
    int i, count = 0;
    int lengthText = strlen(text);

    for (i = 0; i < lengthText; i++)
    {
        if (text[i] != WHITESPACE)
        {
            text[count++] = text[i];
        }
    }     
    text[count] = END_STRING;
    
    return count;
}


void ToLowerCase(char* text)
{
    int i;
    int lengthText = strlen(text);

    for (i = 0; i < lengthText; i++) 
    {
        if (text[i] > ASCII_UPPER_A - 1 && text[i] < ASCII_UPPER_Z + 1)
        {
            text[i] += UPPER_LOWER_CHAR_DIFF;
        }
    }
}


void ParseText(char* text)
{
    int lengthText = strlen(text);

    int pos = 0;
    for (int i = 0; i < lengthText - 1; i++)
    {     
        if (text[i] == text[i + 1])
        {
            pos = i + 1;
            lengthText++;
            for (int j = lengthText; j >= pos; j--)
            {
                text[j] = text[j - 1];
            }

            text[pos] = 'x';
            i++;
        }
    }

    /* padding with X char if the length of the input text is an odd number */
    if (lengthText % 2)
    {
        *(text + lengthText++) = 'x';
        *(text + lengthText) = END_STRING;
    }

}


void GeneratePolybiusSquare(char* key, char polySquare[POLYBIUS_TABLE_SIZE][POLYBIUS_TABLE_SIZE])
{
    int lengthKey = strlen(key);
    int line, column, keyCharId;
    int *dictCharAppear;

    /* a hashmap to store count of the alphabet chars*/
    dictCharAppear = (int*) calloc(ALPHABET_CHARS, sizeof(int));
    
    
    for (line = 0; line < lengthKey; line++) 
    {
        if (key[line] != 'j')
        {
            dictCharAppear[key[line] - ASCII_LOWER_A] = KEY_CHAR_APPEAR;
        }
    }

    dictCharAppear['j' - ASCII_LOWER_A] = 1;

    line = 0;
    column = 0;

    for (keyCharId = 0; keyCharId < lengthKey; keyCharId++) 
    {
        if (dictCharAppear[key[keyCharId] - ASCII_LOWER_A] == 2)
        {
            dictCharAppear[key[keyCharId] - ASCII_LOWER_A] -= 1;
            polySquare[line][column] = key[keyCharId];
            column++;
            if (column == POLYBIUS_TABLE_SIZE)
            {
                line++;
                column = 0;
            }
        }
    }

    for (keyCharId = 0; keyCharId < 26; keyCharId++) 
    {
        if (dictCharAppear[keyCharId] == 0) 
        {
            polySquare[line][column] = (char)(keyCharId + ASCII_LOWER_A);
            column++;
            if (column == POLYBIUS_TABLE_SIZE) 
            {
                line++;
                column = 0;
            }
        }
    }
}


void Search(char polySquare[POLYBIUS_TABLE_SIZE][POLYBIUS_TABLE_SIZE], char ch1, char ch2, int* arrPosChars)
{
    int line, column;

    if (ch1 == 'j')
    {
        ch1 = 'i';
    }    
    else if (ch2 == 'j')
    {
        ch2 = 'i';
    }

    for (line = 0; line < POLYBIUS_TABLE_SIZE; line++) {

        for (column = 0; column < POLYBIUS_TABLE_SIZE; column++) {

            if (polySquare[line][column] == ch1) {
                arrPosChars[LINE_CHAR_POS_CH1] = line;
                arrPosChars[COLUMN_CHAR_POS_CH1] = column;
            }
            else if (polySquare[line][column] == ch2) {
                arrPosChars[LINE_CHAR_POS_CH2] = line;
                arrPosChars[COLUMN_CHAR_POS_CH2] = column;
            }
        }
    }
}


int Modulo5(int x)
{
    int res;
    res = x % 5;
    return (x < 0) ? res + 5 : res;
}

void Encrypt(char* text, char polySquare[POLYBIUS_TABLE_SIZE][POLYBIUS_TABLE_SIZE])
{
    int i, arrPosChars[4];
    int lengthText = strlen(text);

    for (i = 0; i < lengthText; i += 2) 
    {

        Search(polySquare, text[i], text[i + 1], arrPosChars);

        if (arrPosChars[LINE_CHAR_POS_CH1] == arrPosChars[LINE_CHAR_POS_CH2])
        {
            text[i] = polySquare[arrPosChars[LINE_CHAR_POS_CH1]][Modulo5(arrPosChars[COLUMN_CHAR_POS_CH1] + 1)];
            text[i + 1] = polySquare[arrPosChars[LINE_CHAR_POS_CH1]][Modulo5(arrPosChars[COLUMN_CHAR_POS_CH2] + 1)];
        }
        else if (arrPosChars[COLUMN_CHAR_POS_CH1] == arrPosChars[COLUMN_CHAR_POS_CH2])
        {
            text[i] = polySquare[Modulo5(arrPosChars[LINE_CHAR_POS_CH1] + 1)][arrPosChars[COLUMN_CHAR_POS_CH1]];
            text[i + 1] = polySquare[Modulo5(arrPosChars[LINE_CHAR_POS_CH2] + 1)][arrPosChars[COLUMN_CHAR_POS_CH1]];
        }
        else 
        {
            text[i] = polySquare[arrPosChars[LINE_CHAR_POS_CH1]][arrPosChars[COLUMN_CHAR_POS_CH2]];
            text[i + 1] = polySquare[arrPosChars[LINE_CHAR_POS_CH2]][arrPosChars[COLUMN_CHAR_POS_CH1]];
        }
    }
}


void Decrypt(char* text, char polySquare[POLYBIUS_TABLE_SIZE][POLYBIUS_TABLE_SIZE])
{
    int i, arrPosChars[4];
    int lengthText = strlen(text);

    for (i = 0; i < lengthText; i += 2)
    {
        Search(polySquare, text[i], text[i + 1], arrPosChars);
        if (arrPosChars[LINE_CHAR_POS_CH1] == arrPosChars[LINE_CHAR_POS_CH2])
        {
            text[i] = polySquare[arrPosChars[LINE_CHAR_POS_CH1]][Modulo5(arrPosChars[COLUMN_CHAR_POS_CH1] - 1)];
            text[i + 1] = polySquare[arrPosChars[LINE_CHAR_POS_CH1]][Modulo5(arrPosChars[COLUMN_CHAR_POS_CH2] - 1)];
        }
        else if (arrPosChars[COLUMN_CHAR_POS_CH1] == arrPosChars[COLUMN_CHAR_POS_CH2])
        {
            text[i] = polySquare[Modulo5(arrPosChars[LINE_CHAR_POS_CH1] - 1)][arrPosChars[COLUMN_CHAR_POS_CH1]];
            text[i + 1] = polySquare[Modulo5(arrPosChars[LINE_CHAR_POS_CH2] - 1)][arrPosChars[COLUMN_CHAR_POS_CH1]];
        }
        else
        {
            text[i] = polySquare[arrPosChars[LINE_CHAR_POS_CH1]][arrPosChars[COLUMN_CHAR_POS_CH2]];
            text[i + 1] = polySquare[arrPosChars[LINE_CHAR_POS_CH2]][arrPosChars[COLUMN_CHAR_POS_CH1]];
        }
    }
}


void EncryptByPlayfairCipher(char* text, char* key)
{
    char lengthPlainText, lengthKey;
    char polySquare[POLYBIUS_TABLE_SIZE][POLYBIUS_TABLE_SIZE];

    lengthKey = strlen(key);
    lengthKey = RemoveWhitespaces(key);
    ToLowerCase(key);
 
    lengthPlainText = strlen(text);
    ToLowerCase(text);
    lengthPlainText = RemoveWhitespaces(text);

    ParseText(text);

    GeneratePolybiusSquare(key, polySquare);

    Encrypt(text, polySquare);
}


void DecryptByPlayfairCipher(char* text, char* key)
{
    char lengthPlainText, lengthKey;
    char polySquare[POLYBIUS_TABLE_SIZE][POLYBIUS_TABLE_SIZE];

    lengthKey = strlen(key);
    lengthKey = RemoveWhitespaces(key);
    ToLowerCase(key);

    lengthPlainText = strlen(text);
    ToLowerCase(text);
    lengthPlainText = RemoveWhitespaces(text);

    GeneratePolybiusSquare(key, polySquare);

    Decrypt(text, polySquare);
}


void ReadTextStd(char* text, const char* message)
{
    printf("%s", message);
    gets(text);
}


void PrintConsoleMessageWaiting(const char* message, unsigned int timeToWait)
{
    printf("%s", message);
    for (unsigned int i = 0; i < TIMES_ITERATION_PRINT_DOT_IN_CONSOLE; i++)
    {
        printf(".");
        Sleep(timeToWait);
    }
    printf("\n\nThe output is printed in file!\n\n\n\n");
}


void UndoDoubleChars(char* text)
{
    int lengthText = strlen(text);

    int pos = 0;
    for (int i = 1; i < lengthText - 1; i++)
    {
        if (text[i] == 'x')
        {
            if (text[i - 1] == text[i + 1])
            {
                pos = i;
                for (int j = i; j < lengthText; j++)
                {
                    text[j] = text[j + 1];
                }
                lengthText--;
                //i++;
            }
        }
    }
}


void UndoWhitespaces(char* text)
{
    int lengthText = strlen(text);
    int pos = 0;
    int whitespacesId = 0;

    for (int i = 0; i < lengthText; i++)
    {
        if (i == *(whitespacesPos + whitespacesId))
        {
            pos = i;
            lengthText++;
            for (int j = lengthText; j >= pos; j--)
            {
                text[j] = text[j - 1];
            }

            text[pos] = WHITESPACE;
            i++;
            whitespacesId++;
        }
    }
}


void PostProcessText(char* text)
{
    UndoDoubleChars(text);
    UndoWhitespaces(text);
}


int main()
{
    /* declarations */
    char* key;
    char* plaintext;
    int lenKey;
    int lenChiphered;

    /* memory allocations */
    plaintext = (char*)calloc(SIZE_BUFFER, sizeof(char*) * SIZE_BUFFER);
    key = (char*)calloc(SIZE_BUFFER, sizeof(char*) * SIZE_BUFFER);
    whitespacesPos = (int*)calloc(SIZE_BUFFER/10, sizeof(int*) * SIZE_BUFFER);

    /* open input.txt file to read the plaintext */
    OpenFileToRead();
    /* open output.txt file in order to write the encrypted text */
    OpenFileToWrite();

    /* read the plain text which should be encrypted */
    if (fgets(plaintext, SIZE_BUFFER, inFile) == NULL)
    {
        printf("Error!");
    }
    CountWhitespaces(plaintext);

#if ENCRYPT
    /* read the key for Vigenere algorithm */
    printf("-------------------Encryption with Playfair cipher------------------------------\n\n");
    ReadTextStd(key, "Insert the key for encrypt operation: ");
    PrintConsoleMessageWaiting("Encrypt ", TIME_TO_WAIT_PROCESSING);
    /* encrypt the input text using the Playfair cipher algorithm */
    EncryptByPlayfairCipher(plaintext, key);
    /* print to output file the ciphered text */
    fprintf(outFile, "\n\t\t\t\t\t Encryption with Playfair cipher\n\n");
    fprintf(outFile, "The ciphered text is \"%s\"", plaintext);
#endif // ENCRYPT

#if DECRYPT
    /* read the key for Vigenere algorithm */
    printf("-------------------Decryption with Playfair cipher------------------------------\n\n");
    ReadTextStd(key, "Insert the key for decrypt operation: ");
    PrintConsoleMessageWaiting("Decrypt ", TIME_TO_WAIT_PROCESSING);
    /* decrypt the input text using the Playfair cipher algorithm */
    DecryptByPlayfairCipher(plaintext, key);
    /* postprocess to delete 'x' char for double presence and to put again the whitespaces as original text */
    PostProcessText(plaintext);
    /* print to output file the deciphered text */
    fprintf(outFile, "\n\n\t\t\t\t\t Decryption with Playfair cipher\n\n");
    fprintf(outFile, "The deciphered text is \"%s\"", plaintext);
#endif // DECRYPT

    return 0;
}