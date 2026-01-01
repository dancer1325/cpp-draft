# [defined-macro-expression](defined_macro_expression.cpp)
* run it
* check the logs

# [h-preprocessing-token](h_preprocessing_token.cpp)
* check the code
* run it

# example1.cpp

* goal
  * how to include a library `optional` facility

# example2.cpp

* goal
  * how to use the attribute `[[acme::deprecated]]`

# Notes
## how to run locally?
* ways
  * -- via -- IDE
  * -- via -- clang
    * `clang++ -o executableOutputGenerated fileInput.cpp -stdlib=libc++`
      * compile generating the executable
      * _Example:_ `clang++ -o defined_macro_expression defined_macro_expression.cpp -stdlib=libc++`
    * `./executableOutputGenerated`
      * execute the generated executable
      * _Example:_ `./defined_macro_expression`