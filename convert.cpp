#include <iostream>
#include <fstream>
#include <sstream>
#include <Magick++.h>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

struct CifarImage
{
    char label;
    char r[32 * 32];
    char g[32 * 32];
    char b[32 * 32];

    bool read(istream &in);

    char *asPixels() const;
};

bool CifarImage::read(istream &in)
{
	return in.read(reinterpret_cast<char*>(this), sizeof(CifarImage));
}

char *CifarImage::asPixels() const
{
	char *stream = new char[3 * 32 * 32];
	for (int i = 0, s = 0; i < 32 * 32; ++i)
	{
		stream[s++] = r[i];
		stream[s++] = g[i];
		stream[s++] = b[i];
	}

	return stream;
}

void makeOutputDirectories()
{
	char *dirname = new char[6];
	
	for (int i = 0; i < 10; ++i)
	{
		sprintf(dirname, "png/%d", i);
		mkdir(dirname, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	}

	delete[] dirname;
}

void process(string const &fileName, int &n)
{
	ifstream in(fileName.c_str(), ios_base::in);

	while (in.good())
	{
		CifarImage img;
		Magick::Image image;

		img.read(in);

		char *stream = img.asPixels();
		image.read(32, 32, "RGB", Magick::CharPixel, stream);
		delete[] stream;

		stringstream dest;
		dest << "png/" << (int) img.label << "/" << (n++) << ".png";
		cout << "Writing " << dest.str() << endl;

		image.write(dest.str());
	}
}

int main(int argc, char **argv)
{
	makeOutputDirectories();
	int n = 0;

	for (int i = 1; i < argc; ++i)
		process(argv[i], n);
	
	return 0;
}