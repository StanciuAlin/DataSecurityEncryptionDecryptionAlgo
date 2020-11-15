#pragma once

#define SIZE_FILENAME 30
#define SIZE_BUFFER 3000
#define ALPHABET_CHARS 26
#define ASCII_UPPER_A 65
#define ASCII_LOWER_A 97
#define WHITESPACE ' '
#define N_WHITESPACES 100

FILE* inFile;
FILE* outFile;

#ifndef BRUTE_FORCE_ATTACK
#define BRUTE_FORCE_ATTACK 0
#endif // !BRUTE_FORCE_ATTACK

#ifndef KNOWM_SHIFT
#define KNOWN_SHIFT 0
#endif // !KNOWM_SHIFT

#ifndef ENCRYPT
#define ENCRYPT 0
#endif // !ENCRYPT



const char* outFilename = "outputT1.txt";
const char* inFilename = "inputT1.txt";

int shift = 4;

char whitespacesPos[N_WHITESPACES];
int nWhitespaces;

void OpenFileToRead();
void OpenFileToWrite();
int isEqual(char* text1, char* text2);
char* encrypt(char* text);
char* decrypt(char* text);
void MapWhitespacesPositionInputBuffer(char* text);
void UnmapWhitespacesPostionInputBuffer(char* text);

