#pragma once
#include <cstdio>
#include <stdlib.h>

#pragma once

#define SIZE_FILENAME 30
#define SIZE_BUFFER 500
#define ALPHABET_CHARS 26
#define TIME_TO_WAIT_PROCESSING 500
#define TIMES_ITERATION_PRINT_DOT_IN_CONSOLE 3

FILE* inFile;
FILE* outFile;

struct Pair
{
    char key;
    int index;
};

const struct Pair alphabet[ALPHABET_CHARS] = {
    {'A', 0}, {'B', 1}, {'C', 2}, {'D', 3}, {'E', 4}, {'F', 5}, {'G', 6}, {'H', 7}, {'I', 8}, {'J', 9}, {'K', 10}, {'L', 11},
    {'M', 12}, {'N', 13}, {'O', 14}, {'P', 15}, {'Q', 16}, {'R', 17}, {'S', 18}, {'T', 19}, {'U', 20}, {'V', 21}, {'W', 22}, {'X', 23},
    {'Y', 24}, {'Z', 25}
};

const char* outFilename = "output.txt";
const char* inFilename = "input.txt";


void Decrypt(char* ciphered, char* key, char* plaintext);
void Encrypt(char* plaintext, char* key, char* ciphered);
int GetCharIndex(char letter);
void PrintConsoleMessageWaiting(const char* message, unsigned int timeToWait);
void ReadTextStd(char* text, const char* message);
void ReadTextInputFile(char* text);
int MatchPlaintextLength(char* key, int* lenKey, int lenPlaintext);
void OpenFileToRead();
void OpenFileToWrite();
char LetterToUpper(char letter);
void ParseText(char* text);
