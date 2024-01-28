#pragma once

#define MAX_CHUNK_SIZE 4096

enum class ErrCode
{
    ErrFileNotFound = -2,
    ErrSavingFile = -1,
    NoError = 0
};