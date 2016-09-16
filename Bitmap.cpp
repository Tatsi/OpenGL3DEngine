#include "stdafx.h"

//This parser only supports 24bit (or greater) bitmaps. This doesnt work atm
void Bitmap::loadFromBitmap(std::string file)
{
		FILE* input;
		input = fopen(file.c_str(), "rb");
		//TODO check if file is open
		fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, input);//Read file header to variable		
		fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, input);//Read info header to variable
		width = infoHeader.biWidth;//store image width in pixels
		height = infoHeader.biHeight;//store image heigth in pixels
		if (infoHeader.biBitCount < 24)
		{
			fclose(input);
			writeToLog("BITMAP", "Only 24bit bitmaps or greater are supported.");
			throw std::logic_error("Error loading bitmap from file.");
		}
		size = fileHeader.bfSize-fileHeader.bfOffBits;//Store image size
		BYTE* tempData = new BYTE[size];//Store image pixel data temporarily
		if (tempData == NULL)
		{
			fclose(input);
			writeToLog("BITMAP", "Out of memory.");
			throw std::logic_error("Error loading bitmap from file.");
		}
		fread(tempData, sizeof(BYTE), size, input);//Read temporary data
		LONG widthInBytes = LONG ((float) width * (float) infoHeader.biBitCount / 8.0); //width in bytes
		LONG widthInBytesWithPadding = LONG ((float) width * (float) infoHeader.biBitCount / 8.0); //width with padding

		while (widthInBytesWithPadding % 4 != 0) //Add extra space
		{
			widthInBytesWithPadding++;
		}
		DWORD diff = height*widthInBytes;
		data = new BYTE[diff];
		if (data == NULL)
		{
			fclose(input);
			writeToLog("BITMAP", "Out of memory.");
			throw std::logic_error("Error loading bitmap from file.");
		}
		int offset;
		int tmpheight;

		if (height>0)
		{
			int j=size-3;
			offset=widthInBytesWithPadding-widthInBytes;
			for(int i=0;i<size;i+=3)
			{
                if((i+1)%widthInBytesWithPadding==0) {
                        i+=offset;
                }
                *(data+j+2)=*(tempData+i);
                *(data+j+1)=*(tempData+i+1);
                *(data+j)=*(tempData+i+2);
                j--;
			}
		}
		else
		{
			tmpheight=tmpheight*-1;
			offset=0;
			do {
                memcpy((data+(offset*widthInBytes)),
                           (tempData+(offset*widthInBytesWithPadding)),
                                widthInBytes);
                offset++;
			} while(offset<height);
		}
		delete[] tempData;
		fclose(input);

		loaded = true;
}

void Bitmap::loadFromRaw(std::string file, int width, int heigth)
{
	 data = (BYTE*) malloc( width * heigth * 3 );
	 FILE* f = fopen(file.c_str(), "rb" );
	 fread(data, width * heigth * 3, 1, f );
	 fclose(f);
	 loaded = true;
}

void Bitmap::unload()
{
	if (loaded)
	{
		loaded = false;
		free(data);//TODO fix this
		data = NULL;
	}
}
BYTE* Bitmap::getData()
{
	return data;
}
LONG Bitmap::getWidth()
{
	return width;
}
LONG Bitmap::getHeight()
{
	return height;
}	
