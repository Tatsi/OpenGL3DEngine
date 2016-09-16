#ifndef BITMAP_H
#define BITMAP_H

class Bitmap
{
public:
	Bitmap() : loaded(false) { }
	void loadFromBitmap(std::string file);//this doesn't work
	//This functions loads texture data from file in RAW format, width and heigth are sizes of the image
	void Bitmap::loadFromRaw(std::string file, int width, int heigth);
	void unload();

	BYTE* getData();
	LONG getWidth();
	LONG getHeight();
private:
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;

	bool loaded;
	BYTE* data;
	DWORD size;
	LONG width;//Image width in pixels
	LONG height;//Image heigth in pixels
};

#endif