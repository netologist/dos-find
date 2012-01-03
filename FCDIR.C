/*------------------------------------------------------------------------
						FCDIR DOSYA ARAMA PROGRAMI v.1.0
Program Yazarç : Hasan ôzgan                      Bitiü Tarihi: 20/09/1999
------------------------------------------------------------------------*/
#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <dir.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>

/* Fonksiyon Prototipleri */
int arguments(const char *argv);
void walk_tree1(void);
void walk_tree2(void);
void disp_dir(const char *str);
void next_path(const char *str);
void prev_path(void);
int kontrol(void);

/* Sembolik Sabitler */
#define MAX_PATH 80
#define A_DRIVE 0
#define B_DRIVE 1
#define C_DRIVE 2
#define D_DRIVE 3
#define E_DRIVE 4
#define F_DRIVE 5

/* Global Deßiükenler */
	int n_sayac = 0;   /* Bu sayac programda dosyançn kac kez aranacagçnç belirtir */
	int n_tekrar = 0;
	char *aranan_dosya;
	char pth[MAX_PATH] = "C:/*.*";

void next_path(const char *str)
{
	*(pth + strlen(pth) - 3) = '\0';
	strcat(pth, str);
	strcat(pth, "/*.*");
}

void prev_path(void)
{
	int n;
	char *str = pth;

	n = strlen(pth);
	str += n - 5;
	while (*str != '/')
		--str;
	*str = '\0';
	strcat(str, "/*.*");
}

void disp_dir(const char *str)
{
	int i;

	for (i = 0; i < (strlen(str) - 4); ++i)
		putchar(str[i]);
	putchar('\n');
}

void walk_tree1(void)
{
	struct ffblk finfo;
	int result;

	result = findfirst(pth, &finfo, FA_DIREC | FA_SYSTEM | FA_HIDDEN);
	while (!result) {
		if (!strcmp(finfo.ff_name, ".")) {
			result = findnext(&finfo);
			result = findnext(&finfo);
			continue;
		}
		if (finfo.ff_attrib & FA_DIREC) {
			next_path(finfo.ff_name);
			walk_tree1();
		}
		else {
			if (!stricmp(aranan_dosya, finfo.ff_name)) {
				++n_sayac;
				if (n_sayac <= n_tekrar) {
                    if (n_sayac == n_tekrar) {
						pth[strlen(pth) - 3] = NULL;
						chdir(pth);
						exit(1);
					}
					next_path(finfo.ff_name);
					walk_tree1();

				}
			}
		}
		result = findnext(&finfo);
	}
	prev_path();
}

void walk_tree2(void)
{
	struct ffblk finfo;
	int result;

	result = findfirst(pth, &finfo, FA_DIREC | FA_SYSTEM | FA_HIDDEN);
	while (!result) {
		if (!strcmp(finfo.ff_name, ".")) {
			result = findnext(&finfo);
			result = findnext(&finfo);
			continue;
		}
		if (finfo.ff_attrib & FA_DIREC) {
			next_path(finfo.ff_name);
			walk_tree2();
		}
		else {
			if (!stricmp(aranan_dosya, finfo.ff_name)) {
				++n_sayac;
				switch (kontrol()) {
					case 1 : pth[strlen(pth) - 3] = NULL;
							 chdir(pth);
							 exit(1);

					case 2 : next_path(finfo.ff_name);
							 walk_tree2();
							 break;
				}
			}
		}
		result = findnext(&finfo);
	}
	prev_path();
}

int kontrol(void)
{
	char ch;
    int i;

	for (i = 0; i < (strlen(pth) - 3); ++i)
		putchar(pth[i]);
	printf(" (E/H) :");
	ch = getch();
	putchar('\n');

	switch (tolower(ch)) {
		case 'e': return 1;
		case 'h': return 2;
	}
}

int arguments(const char *argv)
{	/* Programçn ilk argÅmançn test eder */
	int i;
    char temp[80];
	char *parameter[6] = { "/h", "-h", "/n:", "-n:", "/s", "-s" };

    strcpy(temp, argv);

	if (temp[0] == '/' || temp[0] == '-')
		temp[3] = NULL;
	else
		return 0;

	for (i = 0; i < 6; i++)
		if (!stricmp(temp, parameter[i]))
			return (i + 1);
	return 0;
}

void main(int argc, char *argv[])
{
	int i;

	if (argc == 3)
		aranan_dosya = argv[2];

	if (argc == 1) {
YARDIM:
		printf("Kullançmç: fcdir <seáenek> [aranacak_dosya]\n"
			"<seáenek> = [/|-]h, [/|-]n:<sayç>, [/|-]s olarak kullanmalçsçnçz\n"
			"h : yardçm almançzç saßlar                        ôr: fcdir -h\n"
			"n : arançlan dosyançn n. bulundußu yere gider     ôr: fcdir -n:3 kernel32.dll\n"
			"s : tÅm bulundußu yerleri sorarak gîsterir        ôr: fcdir -s kernel32.dll\n"
			"\t\t\t\tProgramçn yazarç : Hasan ôZGAN (c) 1999\n");
		exit(1);
	}
	i = arguments(argv[1]);
	switch (i) {
		case 0 :
			if (argv[1][0] == '\\' || argv[1][0] == '/' || argv[1][0] == '-')
				printf("Yanlçü <seáenek> giriüi\n");

			else {
				aranan_dosya = argv[1];
				n_tekrar = 1;
				walk_tree1();
				printf("Dosya bulunamadç\n");
			} break;
		case 1 :
		case 2 :
			goto YARDIM;
		case 3 :
		case 4 :
			if (argv[1][3] < '0' || argv[1][3] > '9')
				printf("LÅtfen seáeneßin numarasçnçda girin\n");
			else {
				n_tekrar = atoi(&argv[1][3]);
				walk_tree1();
				printf("Dosya bulunamadç\n");
			} break;
		case 5 :
		case 6 :
			n_tekrar = 0;
			walk_tree2();
			printf("Dosya bulunamadç\n");
	}
}


