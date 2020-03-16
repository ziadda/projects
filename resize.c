// resizes a BMP file by a factor of f

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: resize f infile outfile\n");

        return 1;
    }

    // remember arguments
    float f = atof(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    // ensure f is within limited range
    if (!(f > 0.0 && f <= 100.0))
    {
        printf("The resize factor, must satisfy 0 < n <= 100.\n");

        return 2;
    }

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);

        return 3;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);

        return 4;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");

        return 5;
    }

    // update outfile's BITMAPINFOHEADER
    BITMAPINFOHEADER obi = bi;
    obi.biWidth *= f;
    int opadding = (4 - (obi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    obi.biHeight *= f;
    obi.biSizeImage = ((sizeof(RGBTRIPLE) * obi.biWidth) + opadding) * abs(obi.biHeight);


    // update outfile's BITMAPFILEHEADER
    BITMAPFILEHEADER obf = bf;
    obf.bfSize = obi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER and BITMAPINFOHEADER
    fwrite(&obf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&obi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine infile's padding
    int ipadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over the input image scanlines
    for (int in_scanline = 0, out_scanline = 0, biHeight = abs(bi.biHeight); in_scanline < biHeight; in_scanline++)
    {
        // create an array to hold resized scanlines
        RGBTRIPLE new_scanline[obi.biWidth];

        // iterate over the input image pixels
        for (int in_pixel = 0, out_pixel = 0; in_pixel < bi.biWidth; in_pixel++)
        {
            // read and store the RGBTRIPLE for the current pixel
            RGBTRIPLE triple;
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // add the pixel to "new_scanline" an appropriate number of times
            while (out_pixel + 1 <= (in_pixel + 1) * f)
            {
                new_scanline[out_pixel] = triple;
                out_pixel++;
            }
        }

        // skip over any padding in the input file
        fseek(inptr, ipadding, SEEK_CUR);

        // write the resized scanline to the output file an appropriate number of times
        while (out_scanline + 1 <= (in_scanline + 1) * f)
        {
            fwrite(new_scanline, sizeof(RGBTRIPLE), obi.biWidth, outptr);

            // write any padding for the scanline to the output file
            for (int i = 0; i < opadding; i++)
            {
                fputc(0x00, outptr);
            }
            out_scanline++;
        }
    }
    // close infile
    fclose(inptr);

    // close outfiles
    fclose(outptr);

    // success
    return 0;
}