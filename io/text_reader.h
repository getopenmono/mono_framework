// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#ifndef text_reader_h
#define text_reader_h

#include <stdio.h>

#include "../mn_string.h"

namespace mono { namespace io {

    class TextReader
    {
    protected:

        FILE *_filePointer;
        String _filePath;


    public:

        TextReader(String path);

        TextReader(FILE *textFile);

        bool open();
        bool close();

        bool IsOpen();

        String readLine();
        //String read(uint32_t length);

        bool hasNext();
    };

} }

#endif /* text_reader_h */
