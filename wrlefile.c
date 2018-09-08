/* 
 * Compress data file from datasrc directory with rle algorithm
 * and send it to data directory
 * 
 * How to use:
 * ./wrlefile "PATH_0" "PATH_1" ... "PATH_n"
 * 
 * Example:
 * ./wrlefile "image/points"
 * > It will compress the file "image/points"
 * 
 * Return 0 on success else number of error(s)
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
		return 1;
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
	if (fclose(inputf) || fclose(outputf))
		return 1;
	return 0;
}

int main(int argc, char **argv)
{
	char in[64], out[64];
	int n;
	
	n = (argc > 1) ? 0 : 1;
	for (int i = 1; i < argc; i++)
	{
		if (sprintf(in, "datasrc/%s", argv[i]) < 0)
			return n + 1;
		if (sprintf(out, "data/%s", argv[i]) < 0)
			return n + 1;
		n += rlecompress(in, out);
	}
	printf("%d\n", n);
	return n;
}
