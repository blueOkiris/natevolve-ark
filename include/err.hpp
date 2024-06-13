// Error handling for library

#pragma once

#include <variant>
#include <string>

namespace natevolve {
    enum class ErrorType {
        FileOpen,
        FileRead,
        FileFormat
    };

    struct Error {
        ErrorType type;
        std::string message;
    };

    template <typename T>
    using Result = typename std::variant<T, Error>;

    template <typename T>
    static inline bool isErr(const Result<T> &result) {
        return std::holds_alternative<Error>(result);
    }

    template <typename T>
    static inline Error err(const Result<T> &result) {
        return std::get<Error>(result);
    }

    template <typename T>
    static inline T ok(const Result<T> &result) {
        return std::get<T>(result);
    }
}

