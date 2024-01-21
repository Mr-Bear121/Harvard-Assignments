#include "helpers.h"
#include "math.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    double average;
    for (int column = 0; column < height; column++)
    {
        for (int row = 0; row < width; row++)
        {
            average = round((image[column][row].rgbtBlue + image[column][row].rgbtRed + image[column][row].rgbtGreen) / 3.0);
            image[column][row].rgbtRed = average;
            image[column][row].rgbtBlue = average;
            image[column][row].rgbtGreen = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    float red;
    float green;
    float blue;
    int sepRed;
    int sepGreen;
    int sepBlue;
    for (int column = 0; column < height; column++)
    {
        for (int row = 0; row < width; row++)
        {
            // record the original RGB values
            red = image[column][row].rgbtRed;
            blue = image[column][row].rgbtBlue;
            green = image[column][row].rgbtGreen;
            // sepia algorithm
            sepRed = round(0.393 * red + 0.769 * green + 0.189 * blue);
            sepGreen = round(0.349 * red + 0.686 * green + 0.168 * blue);
            sepBlue = round(0.272 * red + 0.534 * green + 0.131 * blue);
            // if the algorithm exceeds max rgb value then just set it to the max
            if (sepRed > 255)
            {
                sepRed = 255;
            }
            if (sepBlue > 255)
            {
                sepBlue = 255;
            }
            if (sepGreen > 255)
            {
                sepGreen = 255;
            }
            // update the original image
            image[column][row].rgbtRed = sepRed;
            image[column][row].rgbtGreen = sepGreen;
            image[column][row].rgbtBlue = sepBlue;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int column = 0; column < height; column++)
    {
        // since we are working with opposite ends of the picture we will meet in the middle of the image
        for (int row = 0; row < width / 2; row++)
        {
            // original image for a temp value
            RGBTRIPLE temporary = image[column][row];
            // the first pixel of a row  = the last pixel of a row '-1 due to 0 based index'
            // note since we have width, "which is equivilent to the maximum size of an image" we can just reverse the itteration
            // logic.-->
            //--> instead of going from 0 -> max its max -> 0
            image[column][row] = image[column][width - row - 1];
            // now that we have the 1st pixel switched, reference the temp image and switch the last pixel with the first
            image[column][width - row - 1] = temporary;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // this is going to be a copy of our origin image
    RGBTRIPLE tempImg[height][width];
    float red;
    float green;
    float blue;
    float count;
    int currentX;
    int currentY;

    // used to copy our image row by row from our origin image
    for (int column = 0; column < height; column++)
    {
        for (int row = 0; row < width; row++)
        {
            // copying logic
            tempImg[column][row] = image[column][row];
        }
    }
    // this is the main chunk of our blur logic
    for (int column = 0; column < height; column++)
    {
        for (int row = 0; row < width; row++)
        {
            red = 0.0;
            green = 0.0;
            blue = 0.0;
            count = 0.0;
            // used to get the (x,y) area around our selected pixel. 'determined by the intersection of coulmn and row from our
            // grid'
            for (int x = -1; x < 2; x++)
            {
                for (int y = -1; y < 2; y++)
                {
                    currentX = column + x;
                    currentY = row + y;
                    // 0 is our lower bound while height and width are our maximum bounds... if they are over it, then you will get
                    // a seg fault error so dont go over
                    if (currentX < 0 || currentX > (height - 1) || currentY < 0 || currentY > (width - 1))
                    {
                        continue;
                    }
                    red += image[currentX][currentY].rgbtRed;
                    green += image[currentX][currentY].rgbtGreen;
                    blue += image[currentX][currentY].rgbtBlue;
                    count++;
                }
                tempImg[column][row].rgbtRed = round(red / count);
                tempImg[column][row].rgbtGreen = round(green / count);
                tempImg[column][row].rgbtBlue = round(blue / count);
            }
        }
    }
    // replace origin image with image from tempimage line by line
    for (int column = 0; column < height; column++)
    {
        for (int row = 0; row < width; row++)
        {
            image[column][row].rgbtRed = tempImg[column][row].rgbtRed;
            image[column][row].rgbtGreen = tempImg[column][row].rgbtGreen;
            image[column][row].rgbtBlue = tempImg[column][row].rgbtBlue;
        }
    }
    return;
}
