#pragma once

#define MAX_CHUNK_SIZE 4096

enum class ErrCode
{
    ErrOpeningFile  = -4,
    ErrReadingFile  = -3,
    ErrFileNotFound = -2,
    ErrSavingFile   = -1,
    NoError         = 0
};