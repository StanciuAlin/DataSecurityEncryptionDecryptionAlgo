#pragma once

#define SIZE_FILENAME                 30
#define SIZE_BUFFER                   3000
#define ALPHABET_CHARS                26
#define ASCII_UPPER_A                 65
#define ASCII_UPPER_Z                 90

#define UPPER_LOWER_CHAR_DIFF         32 

#define ASCII_LOWER_A                 97
#define WHITESPACE                    ' '
#define END_STRING                    '\0'

#define POLYBIUS_TABLE_SIZE           5
#define KEY_CHAR_APPEAR               2
#define ALPHABET_CHAR_APPEAR          1

#define LINE_CHAR_POS_CH1             0 
#define	COLUMN_CHAR_POS_CH1           1
#define LINE_CHAR_POS_CH2             2
#define COLUMN_CHAR_POS_CH2           3

#define TIME_TO_WAIT_PROCESSING 0
#define TIMES_ITERATION_PRINT_DOT_IN_CONSOLE 3
	
#ifndef ENCRYPT
#define ENCRYPT 1
#endif // !ENCRYPT

#ifndef DECRYPT
#define DECRYPT 1
#endif // !DECRYPT


FILE* inFile;
FILE* outFile;

const char* outFilename = "output.txt";
const char* inFilename = "input.txt";

int* whitespacesPos;
int counterWhiteSpaces = 0;

void OpenFileToRead();
void OpenFileToWrite();
int RemoveWhitespaces(char* text);
void ToLowerCase(char* text);
void ParseText(char* text);
void GeneratePolybiusSquare(char* key, char polySquare[POLYBIUS_TABLE_SIZE][POLYBIUS_TABLE_SIZE]);
void Search(char polySquare[POLYBIUS_TABLE_SIZE][POLYBIUS_TABLE_SIZE], char ch1, char ch2, int* arrPosChars);
int Modulo5(int x);
void Encrypt(char* text, char polySquare[POLYBIUS_TABLE_SIZE][POLYBIUS_TABLE_SIZE]);
void Decrypt(char* text, char polySquare[POLYBIUS_TABLE_SIZE][POLYBIUS_TABLE_SIZE]);
void EncryptByPlayfairCipher(char* text, char* key);
void DecryptByPlayfairCipher(char* text, char* key);

void ReadTextStd(char* text, const char* message);
void CountWhitespaces(char* text);
void UndoDoubleChars(char* text);
void UndoWhitespaces(char* text);
void PostProcessText(char* text);

