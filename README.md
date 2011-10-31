# Convert
Convert is a simple utility to convert the CIFAR binary image set to png images.

## Usage
Create a folder named `png` in your current working directory, convert will create 10 subfolders with the label numbers in them.
Call convert with the binary files as arguments. Every image in each of the files will be extracted and stored in one of the then label folders as 32x32 png file. The name of the file is just an incrementing counter, and has no special meaning.

## Dependencies
- Magick++
