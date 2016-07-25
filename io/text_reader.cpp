// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#include "text_reader.h"

using namespace mono;
using namespace mono::io;

/// MARK: Contructors

TextReader::TextReader(String path) : _filePointer(0), _filePath(path)
{

}

TextReader::TextReader(FILE *fileHandle) : _filePointer(fileHandle)
{

}

/// Stream Status Methods

bool TextReader::open()
{
    if (_filePath.Length() > 0)
        _filePointer = fopen(_filePath(), "r");

    return _filePointer == 0 ? false : true;
}

bool TextReader::close()
{
    if (_filePointer != 0)
        return fclose(_filePointer) == 0 ? true : false;
    else
        return false;
}

bool TextReader::IsOpen()
{
    if (_filePointer == 0)
        return false;
    else
        return true;
}

/// MARK: Read line methods

String TextReader::readLine()
{
    if (!IsOpen())
        return String();

    int scanned = 0;
    int c = 'a';
    off_t position = ftell(_filePointer);

    while (c != '\n' && feof(_filePointer) != EOF) {
        c = getc(_filePointer);
        scanned++;
    }

    fseek(_filePointer, position, SEEK_SET);
    int lineSize = scanned;

    if (c != '\n')
        lineSize++; // add space for the terminator char

    String line(lineSize);
    fread(line.stringData, scanned, 1, _filePointer);
    line.stringData[lineSize] = '\0'; // insert terminator

    return line;
}



