#include <iostream>

// Define some macros for the example
#define DEBUG_MODE
#define VERSION 2
// RELEASE_MODE is NOT defined

int main() {
    std::cout << "=== Examples of defined-macro-expression ===\n\n";
    
    // Syntax 1: defined identifierName
    #if defined DEBUG_MODE
        std::cout << "DEBUG_MODE is defined (syntax: defined identifier)\n";
    #else
        std::cout << "DEBUG_MODE is NOT defined\n";
    #endif

    // undefined macro
    #if defined RELEASE_MODE
        std::cout << "RELEASE_MODE is defined\n";
    #else
        std::cout << "RELEASE_MODE is NOT defined\n";
    #endif

    // Syntax 2: defined(identifierName)
    #if defined(VERSION)
        std::cout << "VERSION is defined (syntax: defined(identifier))\n";
    #else
        std::cout << "VERSION is NOT defined\n";
    #endif

    // Combining with logical operators
    #if defined(DEBUG_MODE) && defined(VERSION)
        std::cout << "Both DEBUG_MODE and VERSION macros are defined\n";
    #endif
    
    #if defined DEBUG_MODE || defined RELEASE_MODE
        std::cout << "At least one of the macros is defined\n";
    #endif
    
    return 0;
}
