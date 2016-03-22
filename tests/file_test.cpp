// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "../io/file.h"
#include "../io/text_reader.h"

using namespace mono;
using namespace mono::io;

SCENARIO("static io::File methods works","[file]")
{
    GIVEN("An empty file called: testFile")
    {
        mono::String path = "testFile";
        FILE *testFile = fopen(path(), "w");
        fclose(testFile);
        
        WHEN("it existence is checked")
        {
            
            bool exists = mono::io::File::exists(path);
            printf("File exists: %i\n",exists);
            
            THEN("File::exists must return true")
            {
                REQUIRE(exists == true);
            }
            
        }
        
        WHEN("we add some characters")
        {
            const char *text = "Some characters";
            FILE *file = fopen(path(), "a");
            fprintf(file, "%s", text);
            fclose(file);
            
            THEN("its length sould be more than zero")
            {
                size_t len = mono::io::File::size(path);
                printf("File length is: %zu\n",len);
                REQUIRE(len > 0);
            }
            
            THEN("readFirstLine should read out the same data")
            {
                mono::String output = mono::io::File::readFirstLine(path);
                printf("Read: %s\n",output());
                
                REQUIRE(output.Length() == strlen(text));
            }
        }
        
        WHEN("file is overwritten, with another string")
        {
            String another = "this is something else";
            bool success = File::writeString(another, path);
            
            THEN("write must succeed")
            {
                REQUIRE(success == true);
            }
            
            THEN("new file content must match")
            {
                String control = File::readFirstLine(path);
                
                REQUIRE(strcmp(control(), another()) == 0);
            }
        }
        
        WHEN("file is appended with another string")
        {
            size_t oldLen = File::size(path);
            String another = "this is another appended string";
            bool success = File::appendString(another, path);
            
            THEN("New file length must be sum of strings")
            {
                REQUIRE(oldLen+another.Length() == File::size(path));
            }
        }
        
        WHEN("we clear and append lines")
        {
            String first = "first line";
            String rawLine = "first line\n";
            File::writeString(rawLine, path);
            File::appendString("second line\n", path);
            File::appendString("third line\n", path);
            File::appendString("fourth line\n", path);
            
            String firstLine = File::readFirstLine(path);
            
            THEN("the first read line must match")
            {
                REQUIRE(strcmp(first(), firstLine()) == 0);
            }
        }
    }
}


