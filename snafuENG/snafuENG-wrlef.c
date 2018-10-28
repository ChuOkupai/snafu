/* 
 * Compress data file with RLE algorithm
 * 
 * How to use:
 * ./wrlefile "SNF_PATH_0" "SNF_PATH_1" ... "SNF_PATH_n"
 * 
 * Example:
 * $> ./wrlefile "foo"
 * It will compress the file "foo" and save it as "foo.rle"
 */
#include <stdio.h>

int rlecompress(const char *inputfile, const char *outputfile)
{
	FILE *inputf = 0, *outputf = 0;
	unsigned char n;
	char neof, i, j;
	
	inputf = fopen(inputfile, "r");
	if (! inputf)
		return 1;
	outputf = fopen(outputfile, "wb");
	if (! outputf)
	{
		fclose(inputf);
		return 2;
	}
	i = fgetc(inputf);
	j = fgetc(inputf);
	while (i != EOF)
	{
		n = 0;
		if (i == j)
		{
			while (i == j && n < 127)
			{
				j = fgetc(inputf);
				n++;
			}
			n += 128;
			fwrite(&n, 1, sizeof(n), outputf);
			fwrite(&i, 1, sizeof(i), outputf);
			if (j == EOF)
				break;
		}
		else
		{
			while (j != EOF && i != j && n < 127)
			{
				i = j;
				j = fgetc(inputf);
				n++;
			}
			neof = (j == EOF) ? 0 : 2;
			if (! neof)
				n++;
			fseek(inputf, -n - neof, SEEK_CUR);
			n--;
			fwrite(&n, 1, sizeof(n), outputf);
			n++;
			for (i = 0; i < n; i++)
			{
				j = fgetc(inputf);
				fwrite(&j, 1, sizeof(j), outputf);
			}
			if (neof)
				j = fgetc(inputf);
			else
				break;
		}
		i = j;
		j = fgetc(inputf);
	}
	if (fclose(inputf))
		return 3;
	if (fclose(outputf))
		return 4;
	return 0;
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("%s: missing file\n", argv[0]);
		return 1;
	}
	char in[250], out[250];
	int e;
	
	for (int i = 1; i < argc; i++)
	{
		if (sprintf(in, "%s", argv[i]) < 0 || sprintf(out, "%s.rle", argv[i]) < 0)
		{
			printf("%s: %s: sprintf error\n", argv[0], argv[i]);
			return 2;
		}
		e = rlecompress(in, out);
		if (e)
		{
			printf("%s: ", argv[0]);
			if (e == 1)
				printf("%s: No such file or directory", in);
			if (e == 2)
				printf("%s: Couldn't create file", out);
			else
				printf("%s: Couldn't close file", (e == 3) ? in : out);
			putchar('\n');
			return 3;
		}
	}
	return 0;
}
