#include <iostream>
#include "Vivoka.hpp"
#include "Exception.hpp"

GuessNumber::Exception::Exception(const char *what, const char *where) noexcept
        :_what(what), _where(where)
{
}

GuessNumber::Exception::Exception(std::string what, std::string where) noexcept
        :_what(std::move(what)), _where(std::move(where))
{
}

inline const std::string GuessNumber::Exception::exceptionNameToString() const
{
    return std::string("Exception");
}

inline char const *GuessNumber::Exception::what() const noexcept
{
    return _what.c_str();
}

inline char const *GuessNumber::Exception::where() const noexcept
{
    return _where.c_str();
}


void GuessNumber::Exception::display(const GuessNumber::Exception &e)
{
    const std::string exceptionOrigin(e.where());

    std::cerr << RED << e.exceptionNameToString() << " caught : " << BLUE << e.what();
    if (!exceptionOrigin.empty()) {
        std::cerr << YELLOW << " from " << exceptionOrigin;
    }
    std::cerr << RESET << std::endl;
}
