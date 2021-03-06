#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <Magick++.h>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <cassert>

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

void makeDirectory(string const &directory)
{
	stringstream dirname;
	dirname << "png/" << directory;

	mkdir(dirname.str().c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

vector<string> readCategories(string const &fileName)
{
	ifstream in(fileName.c_str(), ios_base::in);
	vector<string> categories;
	
	copy(
		istream_iterator<string>(in),
		istream_iterator<string>(),
		back_inserter(categories));

	return categories;
}

void process(string const &fileName, int &n, vector<string> const &categories)
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

		// assumption: the meta file describing the labels was complete
		assert((size_t) img.label < categories.size());

		stringstream dest;
		dest << "png/" << categories[(int) img.label] << "/" << (n++) << ".png";
		cout << "Writing " << dest.str() << endl;

		image.write(dest.str());
	}
}

int usage(string const &programName)
{
	cout << "Usage: " << programName << " batches.meta.txt [data_batch_1.bin, ..]" << endl;
	return 1;
}

int main(int argc, char **argv)
{
	if (argc < 2)
		return usage(argv[0]);

	// read all the labels from the meta file.
	vector<string> categories = readCategories(argv[1]);

	// make output directories
	mkdir("png", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	for_each(categories.begin(), categories.end(), makeDirectory);
	
	// process the rest of the arguments, i.e. the batch files.
	int n = 0;
	for (int i = 2; i < argc; ++i)
		process(argv[i], n, categories);
	
	return 0;
}