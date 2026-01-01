# example1.ccp
* sequence of preprocessing tokens
  * if it does NOT begin with a directive-introducing token -> == *text-line*

# example2.ccp
* ❌NOT valid *preprocessing-file*❌

* sequence of preprocessing tokens
  * if it does NOT begin with any of the directive names / appear after a `#` | syntax -> == *conditionally-supported-directive* 

* *conditionally-supported-directive*
  * conditionally-supported -- with -- implementation-defined semantics

* TODO: If one of the *pp-tokens* of a `#embed` directive (before macro replacement) is the *identifier* `limit`, `prefix`, `suffix`, or `if_empty` and that *identifier* is defined as a macro, the program is ill-formed
Any *embed-prefixed-parameter* is conditionally-supported, with implementation-defined semantics.

When in a group that is skipped, the directive syntax is relaxed to allow any sequence of preprocessing tokens to occur between the directive name and the following new-line character.

The only whitespace characters that shall appear between preprocessing tokens within a preprocessing directive (from just after the directive-introducing token through just before the terminating new-line character) are space and horizontal-tab (including spaces that have replaced comments or possibly other whitespace characters in translation phase 3).

The implementation can process and skip sections of source files conditionally, include other source files, import macros from header units, and replace macros
These capabilities are called *preprocessing*, because conceptually they occur before translation of the resulting translation unit.

The preprocessing tokens within a preprocessing directive are not subject to macro expansion unless otherwise stated.

# example3.cpp
* sequence of preprocessing tokens | second line
  * != preprocessing directive
    * because it does not begin with a `#` at the start of translation phase 4, even though it will do so after the macro `EMPTY` has been replaced.