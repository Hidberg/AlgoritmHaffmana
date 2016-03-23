// AlgoritmHaffmana.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

struct codedChar
{
	int countChar;
	char symbol;
};

codedChar ValueChars[256];

codedChar fillChar(codedChar arrOfChar[], int lengthArr)
{
	for (int i = 0; i <= lengthArr; ++i)
	{
		arrOfChar[i].symbol = char(i);
	}
	return *arrOfChar;
}

void doCountChar(char* fnameOriginal)
{
	FILE* readingFile;
	readingFile = fopen(fnameOriginal, "r");
	int takenChar;
	while ((takenChar = fgetc(readingFile)) != EOF)
	{
		ValueChars[takenChar].countChar++;
	}
	fclose(readingFile);
}

void sortArrOfChar(codedChar arrOfChar[], int lengthArr)
{
	char flag = 0, temp_char;
	int temp_count;
	while (flag == 0)
	{
		flag = 1;
		for (int i = 0; i < lengthArr; ++i)
		{
			if (arrOfChar[i].countChar < arrOfChar[i + 1].countChar)
			{
				temp_count = arrOfChar[i].countChar;
				temp_char = arrOfChar[i].symbol;
				arrOfChar[i].countChar = arrOfChar[i + 1].countChar;
				arrOfChar[i].symbol = arrOfChar[i + 1].symbol;
				arrOfChar[i + 1].countChar = temp_count;
				arrOfChar[i + 1].symbol = temp_char;
				flag = 0;
			}
		}
	}
}

void showByte(char byte)
{
	int i;
	for (i = sizeof(byte)* 8 - 1; i >= 0; --i)
	{
		printf("%d", byte&(1 << i) ? 1 : 0);
	}
	printf(" ");
}

void writeCharsetInFile(char* fnameForCoding, codedChar sortedArr[], int lengthArr);

void encodeFile(char* fnameForCoding, char* fnameOriginal, codedChar sortedArr[], int lengthArr)
{
	writeCharsetInFile(fnameForCoding, sortedArr, 255);

	FILE *codingFile, *originalFile;
	codingFile = fopen(fnameForCoding, "a");
	originalFile = fopen(fnameOriginal, "r");
	char writedByte = 0, currentPosOfByte = 0;
	char takenChar;
	int checkingChar;
	while ((checkingChar = fgetc(originalFile)) != EOF)
	{
		takenChar = char(checkingChar);
		for (int i = 0; i <= lengthArr; ++i)
		{
			if (sortedArr[i].symbol == takenChar)
			{
				if (currentPosOfByte < 7)
				{
					++currentPosOfByte;
				}
				else
				{
					//showByte(writedByte);
					fputc(writedByte, codingFile);
					writedByte = 0;
					currentPosOfByte = 0;
				}
				break;
			}
			else
			{
				writedByte = (writedByte | char(pow(double(2), 7-currentPosOfByte)));
				if (currentPosOfByte < 7)
				{
					++currentPosOfByte;
				}
				else
				{
					//showByte(writedByte);
					fputc(writedByte, codingFile);
					writedByte = 0;
					currentPosOfByte = 0;
				}
			}
		}
	}
	if (currentPosOfByte > 0)
	{
		for (; currentPosOfByte <= 8; ++currentPosOfByte)	//Заполнение последнего байта в кодировке единицами.
		{
			writedByte = (writedByte | char(pow(double(2), 7 - currentPosOfByte)));
		}
		fputc(writedByte, codingFile); //Запись последнего байта.
	}
	//showByte(writedByte);
	fclose(codingFile);
	fclose(originalFile);
}

void writeCharsetInFile(char* fnameForCoding, codedChar sortedArr[], int lengthArr)
{
	FILE* codingFile;
	codingFile = fopen(fnameForCoding, "w");
	int i = 0;
	while (sortedArr[i].countChar != 0)
	{
		++i;
	}
	fputc(char(i), codingFile);
	for (i = 0; sortedArr[i].countChar != 0; ++i)
	{
		fputc(sortedArr[i].symbol, codingFile);
	}
	fclose(codingFile);
}

void decodeFile(char* fnameEncoded, char* fnameDecoded)
{
	FILE *encodedFile, *decodedFile;
	encodedFile = fopen(fnameEncoded, "r");
	decodedFile = fopen(fnameDecoded, "w");
	char countChar = fgetc(encodedFile);
	char *arrOfChar = (char*)calloc(countChar, sizeof(char));
	for (int i = 0; i < countChar; ++i)
	{
		arrOfChar[i] = fgetc(encodedFile);
	}
	*(arrOfChar + countChar) = 0;
	char takenChar;
	char temp_byte, currentChar = 0;
	while (feof(encodedFile) == 0)
	{
		takenChar = fgetc(encodedFile);
		temp_byte = takenChar;
		for (int i = 0; i < 8; ++i)
		{
			temp_byte = (temp_byte | char(pow(double(2), 7 - i)));
			if (takenChar != temp_byte)
			{
				fputc(*(arrOfChar+currentChar), decodedFile);
				temp_byte = takenChar;
				currentChar = 0;
			}
			else
			{
				++currentChar;
			}
		}
	}
	fclose(encodedFile);
	fclose(decodedFile);
}

void interfaceEncoding();
void interfaceDecoding();

void interfaceMain()
{
	printf("Введите '1' - encode или '2' - decode: ");
	char takenStr = 0;
	scanf("%c", &takenStr);
	while (takenStr != '1' && takenStr != '2')
	{
		scanf("%c", &takenStr);
	}
	if (takenStr == '1')
	{
		interfaceEncoding();
	}
	else
	{
		interfaceDecoding();
	}
	printf("Готово! Введите 1, чтобы попробовать снова: ");
	scanf("%c", &takenStr);
	while (takenStr != '1')
	{
		scanf("%c", &takenStr);
	}
	if (takenStr == '1')
	{
		interfaceMain();
	}
}

void scanf_str(char *str)
{
	scanf("%s", str);
	str += '\0';
}

void interfaceEncoding()
{
	printf("Введите путь файла который нужно архивировать: ");
	char *fnameOriginal = (char*)calloc(500, sizeof(char));
	scanf_str(fnameOriginal);
	while (_access(fnameOriginal, 0) != 0)
	{
		printf("Такого файла не существует! Попробуйте ещё: ");
		scanf_str(fnameOriginal);
	}
	printf("Теперь введите путь куда сохранить кодированный файл: ");
	char *fnameCoded = (char*)calloc(500, sizeof(char));
	scanf_str(fnameCoded);
	fillChar(ValueChars, 255);
	doCountChar(fnameOriginal);
	sortArrOfChar(ValueChars, 255);
	encodeFile(fnameCoded, fnameOriginal, ValueChars, 255);
	free(fnameOriginal); 
	free(fnameCoded);
}

void interfaceDecoding()
{
	printf("Введите путь файла, который нужно разархивировать: ");
	char *fnameCoded = (char*)calloc(500, sizeof(char));
	scanf_str(fnameCoded);
	while (_access(fnameCoded, 0) != 0)
	{
		printf("Такого файла не существует! Попробуйте ещё: ");
		scanf_str(fnameCoded);
	}
	printf("Теперь введите путь куда сохранить раскодированный файл: ");
	char *fnameDecoded = (char*)calloc(500, sizeof(char));
	scanf_str(fnameDecoded);
	decodeFile(fnameCoded, fnameDecoded);
	free(fnameCoded);
	free(fnameDecoded);
}

int main()
{
	setlocale(LC_ALL, "rus");
	interfaceMain();
	return 0;
}

