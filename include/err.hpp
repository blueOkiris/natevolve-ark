// Error handling for library

#pragma once

#include <string>

namespace sndwrp {
    enum class ErrorType {
        FileOpen,
        FileRead,
        FileFormat
    };

    struct Error {
        ErrorType type;
        std::string message;
    };
}

