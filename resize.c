// Resizes a BMP file

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ����������� �����
    if (argc != 4)
    {
        fprintf(stderr, "Usage: copy infile outfile\n");
        return 1;
    }

    // ����� �����
    char *infile = argv[2];
    char *outfile = argv[3];

    float N = atof(argv[1]);
    
    if (N <= 0 || N > 100)
    {
        fprintf(stderr, "Usage: copy infile outfile\n");
        return 5;
    }
    
    float test = floor(N);
// CASE 1
    if ((N - test) == 0) 
	{
		 int n = (int)test; 
    // ��������� ���������� ����
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // ��������� ���� - ���������
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // ������  BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // ������ BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // �������� �� ���� � 24-����� BMP
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
     // ��������� �������
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int paddingZ = (4 - (bi.biWidth*n * sizeof(RGBTRIPLE)) % 4) % 4;
    // ������ ���������� ���
    bf.bfSize = bf.bfSize - padding*abs(bi.biHeight) - 54;
    bf.bfSize = bf.bfSize * n * n;
    bf.bfSize = bf.bfSize + 54 + paddingZ*abs(bi.biHeight)*n;
    bi.biWidth = bi.biWidth * n;
    bi.biHeight = bi.biHeight * n;
    bi.biSizeImage = bi.biSizeImage - padding*abs(bi.biHeight/n);
    bi.biSizeImage = bi.biSizeImage * n * n;
    bi.biSizeImage = bi.biSizeImage + paddingZ*abs(bi.biHeight);

    // �������� �������� BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // �������� �������� BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);


    // �������� �� ������
    for (int i = 0, biHeight = abs(bi.biHeight)/n; i < biHeight; i++)
    {
        for(int x = 0; x < n; x++)
        {
        // �������� �� ������� � �����
        for (int j = 0; j < (bi.biWidth/n); j++)
        {
            // �����
            RGBTRIPLE triple;

            // ���������� RGB � �������� �����
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // ����� RGB �� ��������� �����
            for(int a = 0; a < n; a++)
            {
               fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);

            }

        }
        // ������ ������� ����� �������
        fseek(inptr, padding, SEEK_CUR);

        // ������ ����� � ���� �����
        for (int k = 0; k < paddingZ; k++)
        {
            fputc(0x00, outptr);
        }
        if (x < (n-1))
        {
        fseek(inptr, -3*abs(bi.biWidth)/n - padding, SEEK_CUR);
        }
        }
        
            
    }
    

    // ��������� ������� ����
    fclose(inptr);

    // ��������� �������� ����
    fclose(outptr);

    return 0;
	}
// CASE2
    else if (N < 1)
	{
	    
    // �������� �������� �����
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // �������� ��������� �����
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // ������ BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // ������ BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ���������� �� ���� � 24 ����� BMP
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
     // ��������� ������
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int paddingZ = (4 - ((int)floor(bi.biWidth*N) * sizeof(RGBTRIPLE)) % 4) % 4;
    // ������ ���������� ���
    int copyBBW = bi.biWidth;
    bi.biWidth = (int)floor(bi.biWidth * N);
    bi.biHeight = (int)floor(bi.biHeight * N);
    bi.biSizeImage = (bi.biWidth*abs(bi.biHeight)*3) + (paddingZ*abs(bi.biHeight));
    bf.bfSize = (bi.biWidth*abs(bi.biHeight)*3) + (paddingZ*abs(bi.biHeight)) + 54;

    // ������� BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // ������� BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    int BitsPerBitsZ = floor(copyBBW/bi.biWidth);
    int rest = copyBBW - bi.biHeight*BitsPerBitsZ;
    // �������� �� ������
    for (int i = 0; i < abs(bi.biHeight); i++)
    {
        
        // i������� �� �������
        for (int j = 0; j < bi.biWidth; j++)
        {
            // �����
            RGBTRIPLE triple;

            // ������ RGB 
            for(int a = 0; a < BitsPerBitsZ; a++)
            {
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            }
            // �������� RGB � �������� ����
            
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);

        }
        // ������ ������� ����� �������
        fseek(inptr, padding, SEEK_CUR);
        
        // ������ ����� � ����� �����
        
        for (int k = 0; k < paddingZ; k++)
        {
            fputc(0x00, outptr);
        }
        fseek(inptr,copyBBW*3+padding,SEEK_CUR);
    }
    

    fclose(inptr);

    fclose(outptr);

    return 0;
	}
// CASE3
    else 
	{
	    
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    bf.bfSize = bf.bfSize - padding*abs(bi.biHeight) - 54;
    bf.bfSize = roundf(bf.bfSize * N) * roundf(bf.bfSize * N);
    bf.bfSize = bf.bfSize + 54;
    bi.biSizeImage = bi.biSizeImage - padding*bi.biHeight;
    int copyBBW = bi.biWidth;
    bi.biWidth = roundf(bi.biWidth * N);
    bi.biHeight = roundf(bi.biHeight * N);
    bi.biSizeImage = roundf(bi.biSizeImage * N) * roundf(bi.biSizeImage * N);
    int paddingZ = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    int BitsZPerBits = floor(bi.biWidth/copyBBW);
    int rest = bi.biWidth-(BitsZPerBits*copyBBW);
    for (int i = 0; i < copyBBW; i++)
    {
        for(int x = 0; x < BitsZPerBits; x++)
        {

        for (int j = 0; j < copyBBW; j++)
            {

                RGBTRIPLE triple;

                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
    
                for(int a = 0; a < BitsZPerBits; a++)
                    {
                       fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        
                    }
            
            }
                for(int b = 0; b < rest*3; b++)
                    {
                        fputc(0xff, outptr); 
                    }
                for(int c = 0; c < paddingZ; c++)
                    {
                        fputc(0x00, outptr);
                    }
            
                fseek(inptr, padding, SEEK_CUR);
                if (x < (BitsZPerBits- 1))
                    {
                        fseek(inptr, -3*copyBBW - padding, SEEK_CUR);
                    }
        }
            
    }
    
    fclose(inptr);

    fclose(outptr);

    return 0;
	}
   
}
