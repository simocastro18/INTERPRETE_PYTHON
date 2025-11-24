#if !defined(EXCEPTION_H)
#define EXCEPTION_H

#include <stdexcept>
#include <string>

//tipi di eccezione usati per segnalare errori

struct Error: public std::runtime_error {
    Error(const std::string& msg, int line=-1): std::runtime_error(msg), line_(line) {}

    int line() const { return line_; }

private:
    int line_;
};

struct LexicalError: public Error{
    using Error::Error;
};

struct SyntaxError: public Error{
    using Error::Error;
};

struct RuntimeError: Error{ 
	using Error::Error; 
};

#endif
