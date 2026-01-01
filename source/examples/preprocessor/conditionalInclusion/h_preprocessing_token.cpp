#include <iostream>

// 5. using > -- as -- h-preprocessing-token

// 5.1 COMPILATION ERROR EXAMPLES:
// These would fail because > terminates #include < > prematurely:
// #include <iostream>vector>     // ERROR: > ends the directive
// #include <sys/types.h>config>  // ERROR: > breaks the sequence

// Correct usage:
#include <vector>

// 1. using identifiers -- as -- h-preprocessing-token
#define HEADER_NAME stdio.h
#define CONFIG_FILE "config.txt"

// 2. using numbers -- as -- h-preprocessing-token
#define MAX_SIZE 1024
#define PI 3.14159

// 3. using operators -- as -- h-preprocessing-token (> ALSO works | macros)
#define PLUS +
#define MINUS -
#define GREATER_THAN >

// 4. using punctuation -- as -- h-preprocessing-token
#define OPEN_PAREN (
#define CLOSE_PAREN )

int main() {
    std::cout << "=== h-preprocessing-token restriction example ===\n\n";
    
    //  >    works | being used -- as -- normal token | macros
    bool result = (5 GREATER_THAN 3);
    std::cout << "5 > 3 = " << result << "\n";
    
    return 0;
}
