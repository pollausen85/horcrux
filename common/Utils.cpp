#include "Utils.hpp"
#include "Defs.hpp"

std::string Utils::errorCodeToStr(int errCode)
{
    std::string strErr;
    switch(errCode)
    {
        case (int)ErrCode::NoError:
        {
            strErr = "No Error";
            break;
        }
        case (int)ErrCode::ErrFileNotFound:
        {
            strErr = "Error file not found";
            break;
        }
        case (int)ErrCode::ErrReadingFile:
        {
            strErr = "Error reading file";
            break;
        }
        case (int)ErrCode::ErrSavingFile:
        {
            strErr = "Error saving file";
            break;
        }
        case (int)ErrCode::ErrOpeningFile:
        {
            strErr = "Error opening file";
            break;
        }
        default:
        {
            strErr = "Generic Error";
            break;
        }
    }

    return strErr;
}