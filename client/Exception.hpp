#ifndef GuessNumber_EXCEPTION_HPP
#define GuessNumber_EXCEPTION_HPP

#include <exception>
#include <string>

#define EXIT_SUCCESS     0
#define EXIT_FAIL       84

namespace GuessNumber {

    class Exception : public std::exception {
        public:
        ~Exception() override = default;
        Exception() noexcept = delete;
        explicit Exception(std::string what, std::string where = "") noexcept;
        explicit Exception(const char *what, const char *where = "") noexcept;
        Exception(const Exception &other) = default;
        Exception &operator=(const Exception &other) = default;

        static void display(const GuessNumber::Exception &e);

        virtual const std::string exceptionNameToString() const;

        char const *what() const noexcept override;
        virtual char const *where() const noexcept;

        protected:
        std::string _what;
        std::string _where;
    };  // Exception

} // namespace
#endif //GuessNumber_EXCEPTION_HPP
