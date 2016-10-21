//
//  regex.cpp
//  mono framework
//
//  Created by Kristoffer Andersen on 17/11/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#include "regex.h"
#include <string.h>

using namespace mono;

Regex::Regex() {}

Regex::Regex(String aPattern)
{
    this->pattern = aPattern;
}


bool Regex::IsMatch(mono::String matchStr)
{
    int res = slre_match(pattern(), matchStr(), matchStr.Length(), 0, 0, 0);

    return res > 0 ? true : false;
}

bool Regex::Match(mono::String matchStr, Capture *captureArray, uint32_t capArraySize)
{
    //zero out capture array to prevents bugs from non-init mem.
    memset(captureArray, 0, sizeof(Capture)*capArraySize);
    int res = slre_match(pattern(), matchStr(), matchStr.Length(), captureArray, capArraySize, 0);
    //debug("match res: %i\r\n",res);

    return res > 0 ? true : false;
}

String Regex::Value(Capture &cap)
{
    if (cap.len <= 0)
        return String();
    else if (cap.ptr[cap.len] == '\0')
    {
        return String((char*)cap.ptr, cap.len);
    }
    else
    {
        String capture((char*)cap.ptr, cap.len+1); // add terminator
        char *txt = capture();
        txt[cap.len] = '\0';

        return capture;
    }
}

//bool Regex::NextMatch(CaptureGroup &match, String matchStr)
//{
//    if (match.Context != this)
//    {
//        debug("CaptureGroup context did not match Regex object!\r\n");
//        return false;
//    }
//
//    struct slre_cap caps[2];
//    int i, j = match.MatchIndex, str_len = matchStr.Length();
//
//    while (j < str_len &&
//          (i = slre_match(pattern(), matchStr() + j, str_len - j, caps, 2, 0)) > 0)
//    {
//        match.MatchIndex = j + i;
//        match.Value = String((char*)caps[0].ptr, caps[0].len);
//        match.Match = true;
//
//        return true;
//    }
//
//    match.Match = false;
//    match.Value = String();
//    match.MatchIndex = 0;
//    return false;
//}