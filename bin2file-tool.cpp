#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define DefaultOutputFilePathPrefix "bin"
typedef unsigned char BYTE;

char DefaultOutputFilePath[256];
char* outfilepath = DefaultOutputFilePath;
int inType = 0;
bool finish = false;

void printhelp();
BYTE ReadOneByte();

int main(int argc, char* argv[])
{
	//获取时间
	struct tm tm_now;
	time_t starttime = time(NULL);
	localtime_s(&tm_now, &starttime);
	sprintf_s(DefaultOutputFilePath, "%s_%04d%02d%02d_%02d%02d%02d", DefaultOutputFilePathPrefix,
		tm_now.tm_year + 1900, tm_now.tm_mon + 1, tm_now.tm_mday, tm_now.tm_hour, tm_now.tm_min, tm_now.tm_sec);

	switch (argc) {
	case 1:printhelp(); exit(0);
	case 3:outfilepath = argv[2];//注意这里不要添加break;//DON'T break;
	case 2:
		if (sscanf_s(argv[1], "%d", &inType) && (inType == 2 || inType == 16)) {
			FILE* fp;
			fopen_s(&fp, outfilepath, "wb");
			//printf("inType:%d\n", inType);
			if (fp != NULL) {
				while (!finish) {
					BYTE byte = ReadOneByte();
					if (!finish)fwrite(&byte, 1, 1, fp);
				}
				fclose(fp);
			}
			else printf("Can't write to output file! Please check!");
		}
		else {
			printhelp();
			exit(0);
		}
		break;
	default:
		printhelp();
		exit(0);
	}

	printf("Done!(%.3lfs)", clock() / 1000.0);
	return 0;
}

void printhelp()
{
	printf(
		"Binary File tool.\n\n"
		"Wrong arguments!\n"
		"Usage:\n"
		"  bin2file InputType [OutputFile] < InputFilePath\n\n"
		"InputType\t2 or 16 (2 for Binary, 16 for Hex)\n"
		"OutputFile\tDefault is \"" DefaultOutputFilePathPrefix "_TIME\" such as \"%s\"\n"
		, DefaultOutputFilePath
	);
}

BYTE ReadOneByte()
{
	BYTE byte = 0x00;
	unsigned char a[8] = { '0','0','0','0','0','0','0','0' };
	if (inType == 2)
		for (int i = 0; i < 8; i++) {
			if (scanf_s("%c", &a[i]) != 1)finish = true;
			if (a[i] == '0' || a[i] == '1')a[i] -= '0';
			byte |= a[i] << (7 - i);
		}
	if (inType == 16)
		for (int i = 0; i < 2; i++) {
			if (scanf_s("%c", &a[i]) != 1)finish = true;
			if (a[i] >= '0' && a[i] <= '9')byte |= (a[i] - '0') << (i ? 0 : 4);
			else if (a[i] >= 'a' && a[i] <= 'f')byte |= (a[i] - 'a' + 10) << (i ? 0 : 4);
			else if (a[i] >= 'A' && a[i] <= 'F')byte |= (a[i] - 'A' + 10) << (i ? 0 : 4);
		}
	return byte;
}
