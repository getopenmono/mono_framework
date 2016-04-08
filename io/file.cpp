// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#include "file.h"

#include <unistd.h>
#include <sys/stat.h>

using namespace mono::io;

/// MARK: Public static methods

bool File::exists(String path)
{
    return access(path(), F_OK) != -1 ? true : false;
}

size_t File::size(String path)
{
    struct stat st;
    if (stat(path(), &st) == 0)
        return st.st_size;
    else
        return 0;
}

mono::String File::readFirstLine(mono::String path, int maxLength, char endLineDelimiter)
{
    int scanned = 0;
    
    if (maxLength == 0)
        maxLength = -1;
    
    FILE *file = fopen(path(), "r");
    
    if (file == 0)
        return String();
    
    int c = 'a';
    while(c != endLineDelimiter &&
          !feof(file) &&
          (maxLength == -1 || scanned < maxLength))
    {
        c = getc(file);
        scanned++;
    }
    
    if (scanned <= 0)
    {
        fclose(file);
        return String();
    }
    
    rewind(file);
    String str(scanned);
    
    fread(str.stringData, scanned, 1, file);
    fclose(file);
    
    str.stringData[scanned-1] = '\0';
    
    return str;
}

bool File::writeString(String text, String path)
{
    FILE *file = fopen(path(), "w");
    
    return File::writeString(text, file);
}

bool File::appendString(mono::String text, mono::String path)
{
    FILE *file = fopen(path(), "a");
    
    return File::writeString(text, file);
}

bool File::appendLine(String text, String path, const char *lineDelimiter)
{
    appendString(String::Format("%s%s",text,lineDelimiter), path);
}

/// MARK: Protected Static Methods

bool File::writeString(mono::String text, FILE *file)
{
    if (file == 0)
        return false;
    
    for (int i=0; i<text.Length(); i++)
    {
        if (putc(text.stringData[i], file) == EOF)
        {
            fclose(file);
            return false;
        }
    }
    
    fclose(file);
    
    return true;
}