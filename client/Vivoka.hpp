#ifndef VIVOKA_VIVOKA_HPP
#define VIVOKA_VIVOKA_HPP

namespace GuessNumber {

    #ifdef _WINDOWS
    #define GREEN		""
		#define RED			""
		#define MAGENTA		""
		#define CYAN		""
		#define BLUE		""
		#define YELLOW		""
		#define RESET		""
    #else
        #define GREEN		"\033[1;32m"
        #define RED			"\033[1;31m"
        #define MAGENTA		"\033[1;35m"
        #define CYAN		"\033[1;36m"
        #define BLUE		"\033[1;34m"
        #define YELLOW		"\033[1;33m"
        #define RESET		"\033[0m"
    #endif

}

#endif //VIVOKA_VIVOKA_HPP
