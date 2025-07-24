# Preprocessing directives

## Preamble

**preprocessing-file**:
- *[group]*
- module-file

**module-file**:
- *[pp-global-module-fragment]* pp-module *[group]* *[pp-private-module-fragment]*

**pp-global-module-fragment**:
- `module` `;` new-line *[group]*

**pp-private-module-fragment**:
- `module` `:` `private` `;` new-line *[group]*

**group**:
- group-part
- group group-part

**group-part**:
- control-line
- if-section
- text-line
- `#` conditionally-supported-directive

**control-line**:
- `# include` pp-tokens new-line
- pp-import
- `# embed` pp-tokens new-line
- `# define` identifier replacement-list new-line
- `# define` identifier lparen *[identifier-list]* `)` replacement-list new-line
- `# define` identifier lparen `...` `)` replacement-list new-line
- `# define` identifier lparen identifier-list `, ...` `)` replacement-list new-line
- `# undef` identifier new-line
- `# line` pp-tokens new-line
- `# error` *[pp-tokens]* new-line
- `# warning` *[pp-tokens]* new-line
- `# pragma` *[pp-tokens]* new-line
- `#` new-line

**if-section**:
- if-group *[elif-groups]* *[else-group]* endif-line

**if-group**:
- `# if` constant-expression new-line *[group]*
- `# ifdef` identifier new-line *[group]*
- `# ifndef` identifier new-line *[group]*

**elif-groups**:
- elif-group *[elif-groups]*

**elif-group**:
- `# elif` constant-expression new-line *[group]*
- `# elifdef` identifier new-line *[group]*
- `# elifndef` identifier new-line *[group]*

**else-group**:
- `# else` new-line *[group]*

**endif-line**:
- `# endif` new-line

**text-line**:
- *[pp-tokens]* new-line

**conditionally-supported-directive**:
- pp-tokens new-line

**lparen**:
- *a `(` character not immediately preceded by whitespace*

**identifier-list**:
- identifier
- identifier-list `,` identifier

**replacement-list**:
- *[pp-tokens]*

**pp-tokens**:
- preprocessing-token *[pp-tokens]*

**embed-parameter-seq**:
- embed-parameter *[embed-parameter-seq]*

**embed-parameter**:
- embed-standard-parameter
- embed-prefixed-parameter

**embed-standard-parameter**:
- `limit` `(` pp-balanced-token-seq `)`
- `prefix` `(` *[pp-balanced-token-seq]* `)`
- `suffix` `(` *[pp-balanced-token-seq]* `)`
- `if_empty` `(` *[pp-balanced-token-seq]* `)`

**embed-prefixed-parameter**:
- identifier `::` identifier
- identifier `::` identifier `(` *[pp-balanced-token-seq]* `)`

**pp-balanced-token-seq**:
- pp-balanced-token *[pp-balanced-token-seq]*

**pp-balanced-token**:
- `(` *[pp-balanced-token-seq]* `)`
- `[` *[pp-balanced-token-seq]* `]`
- `{` *[pp-balanced-token-seq]* `}`
- *any pp-token except:*
  - *parenthesis (left parenthesis and right parenthesis),*
  - *bracket (left square bracket and right square bracket), or*
  - *brace (left curly bracket and right curly bracket).*

**new-line**:
- *the new-line character*

A *preprocessing directive* consists of a sequence of preprocessing tokens that satisfies the following constraints: At the start of translation phase 4, the first preprocessing token in the sequence, referred to as a *directive-introducing token*, begins with the first character in the source file (optionally after whitespace containing no new-line characters) or follows whitespace containing at least one new-line character, and is:

- a `#` preprocessing token, or
- an `import` preprocessing token immediately followed on the same logical source line by a header-name, `<`, identifier, string-literal, or `:` preprocessing token, or
- a `module` preprocessing token immediately followed on the same logical source line by an identifier, `:`, or `;` preprocessing token, or
- an `export` preprocessing token immediately followed on the same logical source line by one of the two preceding forms.

The last preprocessing token in the sequence is the first preprocessing token within the sequence that is immediately followed by whitespace containing a new-line character.

> Note: A new-line character ends the preprocessing directive even if it occurs within what would otherwise be an invocation of a function-like macro.

### Example

```cpp
#                       // preprocessing directive
module ;                // preprocessing directive
export module leftpad;  // preprocessing directive
import <string>;        // preprocessing directive
export import "squee";  // preprocessing directive
import rightpad;        // preprocessing directive
import :part;           // preprocessing directive

module                  // not a preprocessing directive
;                       // not a preprocessing directive

export                  // not a preprocessing directive
import                  // not a preprocessing directive
foo;                    // not a preprocessing directive

export                  // not a preprocessing directive
import foo;             // preprocessing directive (ill-formed at phase 7)

import ::               // not a preprocessing directive
import ->               // not a preprocessing directive
```

A sequence of preprocessing tokens is only a *text-line* if it does not begin with a directive-introducing token.

### Example

```cpp
using module = int;
module i;               // not a text-line and not a control-line
int foo() {
  return i;
}
```

The example is not a valid *preprocessing-file*.

A sequence of preprocessing tokens is only a *conditionally-supported-directive* if it does not begin with any of the directive names appearing after a `#` in the syntax. A *conditionally-supported-directive* is conditionally-supported with implementation-defined semantics.

If one of the *pp-tokens* of a `#embed` directive (before macro replacement) is the *identifier* `limit`, `prefix`, `suffix`, or `if_empty` and that *identifier* is defined as a macro, the program is ill-formed. Any *embed-prefixed-parameter* is conditionally-supported, with implementation-defined semantics.

When in a group that is skipped, the directive syntax is relaxed to allow any sequence of preprocessing tokens to occur between the directive name and the following new-line character.

The only whitespace characters that shall appear between preprocessing tokens within a preprocessing directive (from just after the directive-introducing token through just before the terminating new-line character) are space and horizontal-tab (including spaces that have replaced comments or possibly other whitespace characters in translation phase 3).

The implementation can process and skip sections of source files conditionally, include other source files, import macros from header units, and replace macros. These capabilities are called *preprocessing*, because conceptually they occur before translation of the resulting translation unit.

The preprocessing tokens within a preprocessing directive are not subject to macro expansion unless otherwise stated.

### Example

In:
```cpp
#define EMPTY
EMPTY   #   include <file.h>
```
the sequence of preprocessing tokens on the second line is *not* a preprocessing directive, because it does not begin with a `#` at the start of translation phase 4, even though it will do so after the macro `EMPTY` has been replaced.

## Conditional inclusion

**defined-macro-expression**:
- `defined` identifier
- `defined (` identifier `)`

**h-preprocessing-token**:
- *any preprocessing-token other than* `>`

**h-pp-tokens**:
- h-preprocessing-token *[h-pp-tokens]*

**header-name-tokens**:
- string-literal
- `<` h-pp-tokens `>`

**has-include-expression**:
- `__has_include` `(` header-name `)`
- `__has_include` `(` header-name-tokens `)`

**has-embed-expression**:
- `__has_embed` `(` header-name *[pp-balanced-token-seq]* `)`
- `__has_embed` `(` header-name-tokens *[pp-balanced-token-seq]* `)`

**has-attribute-expression**:
- `__has_cpp_attribute (` pp-tokens `)`

The expression that controls conditional inclusion shall be an integral constant expression except that identifiers (including those lexically identical to keywords) are interpreted as described below and it may contain zero or more *defined-macro-expression*s, *has-include-expression*s, *has-attribute-expression*s, and/or *has-embed-expression*s as unary operator expressions. A *defined-macro-expression* shall not appear within a *has-include-expression* or *has-embed-expression*.

A *defined-macro-expression* evaluates to `1` if the identifier is currently defined as a macro name (that is, if it is predefined or if it has one or more active macro definitions, for example because it has been the subject of a `#define` preprocessing directive without an intervening `#undef` directive with the same subject identifier), `0` if it is not.

The header or source file identified by the parenthesized preprocessing token sequence in each contained *has-include-expression* is searched for as if that preprocessing token sequence were the *pp-tokens* of a `#include` directive, except that no further macro expansion is performed. If the preprocessing token sequence does not consist solely of a *header-name* or cannot be combined into a single *header-name* preprocessing token, the program is ill-formed. The *has-include-expression* evaluates to `1` if the search for the source file succeeds, and to `0` if the search fails.

The parenthesized preprocessing token sequence of each contained *has-embed-expression* is processed as if that preprocessing token sequence were the *pp-tokens* of a `#embed` directive, except that no further macro expansion is performed. The *has-embed-expression* evaluates to:

- `STDC_EMBED_FOUND` if the search for the resource succeeds, all the given *embed-parameter*s in the *embed-parameter-seq* are supported, and the resource is not empty.
- Otherwise, `STDC_EMBED_EMPTY` if the search for the resource succeeds, all the given *embed-parameter*s in the *embed-parameter-seq* are supported, and the resource is empty.
- Otherwise, `STDC_EMBED_NOT_FOUND`.

> Note: An unrecognized *embed-parameter* in an *has-embed-expression* is not ill-formed and is instead treated as not supported.

Each *has-attribute-expression* is replaced by a non-zero *pp-number* matching the form of an *integer-literal* if the implementation supports an attribute with the name specified by interpreting the *pp-tokens*, after macro expansion, as an *attribute-token*, and by `0` otherwise. The program is ill-formed if the *pp-tokens* do not match the form of an *attribute-token*.

For an attribute specified in this document, it is implementation-defined whether the value of the *has-attribute-expression* is `0` or is given by the table below. For other attributes recognized by the implementation, the value is implementation-defined.

> Note: It is expected that the availability of an attribute can be detected by any non-zero result.

| Attribute | Value |
|-----------|-------|
| `assume` | `202207L` |
| `deprecated` | `201309L` |
| `fallthrough` | `201603L` |
| `indeterminate` | `202403L` |
| `likely` | `201803L` |
| `maybe_unused` | `201603L` |
| `no_unique_address` | `201803L` |
| `nodiscard` | `201907L` |
| `noreturn` | `200809L` |
| `unlikely` | `201803L` |

The `#ifdef`, `#ifndef`, `#elifdef`, and `#elifndef` directives, and the `defined` conditional inclusion operator, shall treat `__has_include`, `__has_embed`, and `__has_cpp_attribute` as if they were the names of defined macros. The identifiers `__has_include`, `__has_embed`, and `__has_cpp_attribute` shall not appear in any context not mentioned in this subclause.

Each preprocessing token that remains (in the list of preprocessing tokens that will become the controlling expression) after all macro replacements have occurred shall be in the lexical form of a token.

Preprocessing directives of the forms
```
# if     constant-expression new-line [group]
# elif   constant-expression new-line [group]
```
check whether the controlling constant expression evaluates to nonzero.

Prior to evaluation, macro invocations in the list of preprocessing tokens that will become the controlling constant expression are replaced (except for those macro names modified by the `defined` unary operator), just as in normal text. If replacement of macros in the preprocessing tokens following the sequence `__has_embed (` and before a matching `)` (possibly produced by macro expansion) encounters a preprocessing token that is one of the *identifier*s `limit`, `prefix`, `suffix`, or `if_empty` and that *identifier* is defined as a macro, the program is ill-formed. If the preprocessing token `defined` is generated as a result of this replacement process or use of the `defined` unary operator does not match one of the two specified forms prior to macro replacement, the program is ill-formed, no diagnostic required.

After all replacements due to macro expansion and evaluations of *defined-macro-expression*s, *has-include-expression*s, *has-embed-expression*s, and *has-attribute-expression*s have been performed, all remaining identifiers and keywords, except for `true` and `false`, are replaced with the *pp-number* `0`, and then each preprocessing token is converted into a token.

> Note: An alternative token is not an identifier, even when its spelling consists entirely of letters and underscores. Therefore it is not subject to this replacement.

The resulting tokens comprise the controlling constant expression which is evaluated according to the rules of constant expressions. For the purposes of this token conversion and evaluation all signed and unsigned integer types act as if they have the same representation as, respectively, `intmax_t` or `uintmax_t`. Each subexpression with type `bool` is subjected to integral promotion before processing continues.

Preprocessing directives of the forms
```
# ifdef   identifier new-line [group]
# ifndef  identifier new-line [group]
# elifdef identifier new-line [group]
# elifndef identifier new-line [group]
```
check whether the identifier is or is not currently defined as a macro name. Their conditions are equivalent to `#if defined` *identifier*, `#if !defined` *identifier*, `#elif defined` *identifier*, and `#elif !defined` *identifier*, respectively.

Each directive's condition is checked in order. If it evaluates to false (zero), the group that it controls is skipped: directives are processed only through the name that determines the directive in order to keep track of the level of nested conditionals; the rest of the directives' preprocessing tokens are ignored, as are the other preprocessing tokens in the group. Only the first group whose control condition evaluates to true (nonzero) is processed; any following groups are skipped and their controlling directives are processed as if they were in a group that is skipped. If none of the conditions evaluates to true, and there is a `#else` directive, the group controlled by the `#else` is processed; lacking a `#else` directive, all the groups until the `#endif` are skipped.

### Example

This demonstrates a way to include a library `optional` facility only if it is available:

```cpp
#if __has_include(<optional>)
#  include <optional>
#  if __cpp_lib_optional >= 201603
#    define have_optional 1
#  endif
#elif __has_include(<experimental/optional>)
#  include <experimental/optional>
#  if __cpp_lib_experimental_optional >= 201411
#    define have_optional 1
#    define experimental_optional 1
#  endif
#endif
#ifndef have_optional
#  define have_optional 0
#endif
```

### Example

This demonstrates a way to use the attribute `[[acme::deprecated]]` only if it is available.
```cpp
#if __has_cpp_attribute(acme::deprecated)
#  define ATTR_DEPRECATED(msg) [[acme::deprecated(msg)]]
#else
#  define ATTR_DEPRECATED(msg) [[deprecated(msg)]]
#endif
ATTR_DEPRECATED("This function is deprecated") void anvil();
```

## Source file inclusion

A *header search* for a sequence of characters searches a sequence of places for a header identified uniquely by that sequence of characters. How the places are determined or the header identified is implementation-defined.

A *source file search* for a sequence of characters attempts to identify a source file that is named by the sequence of characters. The named source file is searched for in an implementation-defined manner. If the implementation does not support a source file search for that sequence of characters, or if the search fails, the result of the source file search is the result of a header search for the same sequence of characters.

A preprocessing directive of the form
```
# include header-name new-line
```
causes the replacement of that directive by the entire contents of the header or source file identified by *header-name*.

If the *header-name* is of the form
```
< h-char-sequence >
```
a header is identified by a header search for the sequence of characters of the *h-char-sequence*.

If the *header-name* is of the form
```
" q-char-sequence "
```
the source file or header is identified by a source file search for the sequence of characters of the *q-char-sequence*.

If a header search fails, or if a source file search or header search identifies a header or source file that cannot be processed by the implementation, the program is ill-formed.

> Note: If the header or source file cannot be processed, the program is ill-formed even when evaluating `__has_include`.

A preprocessing directive of the form
```
# include pp-tokens new-line
```
(that does not match the previous form) is permitted. The preprocessing tokens after `include` in the directive are processed just as in normal text (i.e., each identifier currently defined as a macro name is replaced by its replacement list of preprocessing tokens). Then, an attempt is made to form a *header-name* preprocessing token from the whitespace and the characters of the spellings of the resulting sequence of preprocessing tokens; the treatment of whitespace is implementation-defined. If the attempt succeeds, the directive with the so-formed *header-name* is processed as specified for the previous form. Otherwise, the program is ill-formed, no diagnostic required.

> Note: Adjacent *string-literal*s are not concatenated into a single *string-literal* (see the translation phases); thus, an expansion that results in two *string-literal*s is an invalid directive.

The implementation shall provide unique mappings for sequences consisting of one or more *nondigit*s or *digit*s followed by a period (`.`) and a single *nondigit*. The first character shall not be a *digit*. The implementation may ignore distinctions of alphabetical case.

A `#include` preprocessing directive may appear in a source file that has been read because of a `#include` directive in another file, up to an implementation-defined nesting limit.

If the header identified by the *header-name* denotes an importable header, it is implementation-defined whether the `#include` preprocessing directive is instead replaced by an `import` directive of the form
```
import header-name ; new-line
```

> Note: An implementation can provide a mechanism for making arbitrary source files available to the `< >` search. However, using the `< >` form for headers provided with the implementation and the `" "` form for sources outside the control of the implementation achieves wider portability. For instance:
>
> ```cpp
> #include <stdio.h>
> #include <unistd.h>
> #include "usefullib.h"
> #include "myprog.h"
> ```

### Example

This illustrates macro-replaced `#include` directives:

```cpp
#if VERSION == 1
    #define INCFILE  "vers1.h"
#elif VERSION == 2
    #define INCFILE  "vers2.h"  // and so on
#else
    #define INCFILE  "versN.h"
#endif
#include INCFILE
```

## Resource inclusion

### General

A *bracket resource search* for a sequence of characters searches a sequence of places for a resource identified uniquely by that sequence of characters. How the places are determined or the resource identified is implementation-defined.

A *quote resource search* for a sequence of characters attempts to identify a resource that is named by the sequence of characters. The named resource is searched for in an implementation-defined manner. If the implementation does not support a quote resource search for that sequence of characters, or if the search fails, the result of the quote resource search is the result of a bracket resource search for the same sequence of characters.

A preprocessing directive of the form
```
# embed header-name [pp-tokens] new-line
```
causes the replacement of that directive by preprocessing tokens derived from data in the resource identified by *header-name*, as specified below.

If the *header-name* is of the form
```
< h-char-sequence >
```
the resource is identified by a bracket resource search for the sequence of characters of the *h-char-sequence*.

If the *header-name* is of the form
```
" q-char-sequence "
```
the resource is identified by a quote resource search for the sequence of characters of the *q-char-sequence*.

If a bracket resource search fails, or if a quote or bracket resource search identifies a resource that cannot be processed by the implementation, the program is ill-formed.

> Note: If the resource cannot be processed, the program is ill-formed even when processing `#embed` with `limit(0)` or evaluating `__has_embed`.

Either form of the `#embed` directive processes the *pp-tokens*, if present, just as in normal text. The *pp-tokens* shall then have the form *embed-parameter-seq*.

A resource is a source of data accessible from the translation environment. A resource has an *implementation-resource-width*, which is the implementation-defined size in bits of the resource. If the implementation-resource-width is not an integral multiple of `CHAR_BIT`, the program is ill-formed. Let *implementation-resource-count* be implementation-resource-width divided by `CHAR_BIT`. Every resource also has a *resource-count*, which is

- the value as computed from the optionally-provided `limit` *embed-parameter*, if present;
- otherwise, the implementation-resource-count.

A resource is empty if the resource-count is zero.

### Example

```cpp
// ill-formed if the implementation-resource-width is 6 bits
#embed "6_bits.bin"
```

The `#embed` directive is replaced by a comma-separated list of integer literals of type `int`, unless otherwise modified by embed parameters.

The integer literals in the comma-separated list correspond to resource-count consecutive calls to `std::fgetc` from the resource, as a binary file. If any call to `std::fgetc` returns `EOF`, the program is ill-formed.

### Example

```cpp
#include <cstring>
#include <cstddef>
#include <fstream>
#include <vector>
#include <cassert>

int main() {
  // If the file is the same as the resource in the translation environment, no assert in this program should fail.
  constexpr unsigned char d[] = {
#embed <data.dat>
  };
  const std::vector<unsigned char> vec_d = {
#embed <data.dat>
  };

  constexpr std::size_t expected_size = sizeof(d);

  // same file in execution environment as was embedded
  std::ifstream f_source("data.dat", std::ios::binary | std::ios::in);
  unsigned char runtime_d[expected_size];
  char* ifstream_ptr = reinterpret_cast<char*>(runtime_d);
  assert(!f_source.read(ifstream_ptr, expected_size));
  std::size_t ifstream_size = f_source.gcount();
  assert (ifstream_size == expected_size);
  int is_same = std::memcmp(&d[0], ifstream_ptr, ifstream_size);
  assert(is_same == 0);
  int is_same_vec = std::memcmp(vec_d.data(), ifstream_ptr, ifstream_size);
  assert(is_same_vec == 0);
}
```

### Example

```cpp
int i = {
#embed "i.dat"
};  // well-formed if i.dat produces a single value
int i2 =
#embed "i.dat"
;   // also well-formed if i.dat produces a single value
struct s {
  double a, b, c;
  struct { double e, f, g; } x;
  double h, i, j;
};
s x = {
// well-formed if the directive produces nine or fewer values
#embed "s.dat"
};
```

A preprocessing directive of the form
```
# embed pp-tokens new-line
```
(that does not match the previous form) is permitted. The preprocessing tokens after `embed` in the directive are processed just as in normal text (i.e., each identifier currently defined as a macro name is replaced by its replacement list of preprocessing tokens). Then, an attempt is made to form a *header-name* preprocessing token from the whitespace and the characters of the spellings of the resulting sequence of preprocessing tokens immediately after `embed`; the treatment of whitespace is implementation-defined. If the attempt succeeds, the directive with the so-formed *header-name* is processed as specified for the previous form. Otherwise, the program is ill-formed.

> Note: Adjacent *string-literal*s are not concatenated into a single *string-literal* (see the translation phases); thus, an expansion that results in two *string-literal*s is an invalid directive.

Any further processing as in normal text described for the previous form is not performed.

> Note: That is, processing as in normal text happens once and only once for the entire directive.

### Example

If the directive matches the second form, the whole directive is replaced. If the directive matches the first form, everything after the name is replaced.

```cpp
#define EMPTY
#define X myfile
#define Y rsc
#define Z 42
#embed <myfile.rsc> prefix(Z)
#embed EMPTY <X.Y>  prefix(Z)
```

is equivalent to:

```cpp
#embed <myfile.rsc> prefix(42)
#embed <myfile.rsc> prefix(42)
```

### Embed parameters

#### limit parameter

An *embed-parameter* of the form `limit (` *pp-balanced-token-seq* `)` specifies the maximum possible number of elements in the comma-delimited list. It shall appear at most once in the *embed-parameter-seq*. The preprocessing token `defined` shall not appear in the *pp-balanced-token-seq*.

The *pp-balanced-token-seq* is evaluated as a *constant-expression* using the rules as described in conditional inclusion, but without being processed as in normal text an additional time.

### Example

```cpp
#undef DATA_LIMIT
#if __has_embed(<data.dat> limit(DATA_LIMIT))
#endif
```

is equivalent to:

```cpp
#if __has_embed(<data.dat> limit(0))
#endif
```

### Example

```cpp
#embed <data.dat> limit(__has_include("a.h"))

#if __has_embed(<data.dat> limit(__has_include("a.h")))
// ill-formed: __has_include cannot appear here
#endif
```

The *constant-expression* shall be an integral constant expression whose value is greater than or equal to zero. The resource-count becomes implementation-resource-count, if the value of the *constant-expression* is greater than implementation-resource-count; otherwise, the value of the *constant-expression*.

### Example

```cpp
constexpr unsigned char sound_signature[] = {
  // a hypothetical resource capable of expanding to four or more elements
#embed <sdk/jump.wav> limit(2+2)
};

static_assert(sizeof(sound_signature) == 4);    // OK
```

#### prefix parameter

An *embed-parameter* of the form
```
prefix ( [pp-balanced-token-seq] )
```
shall appear at most once in the *embed-parameter-seq*.

If the resource is empty, this *embed-parameter* is ignored. Otherwise, the *pp-balanced-token-seq* is placed immediately before the comma-delimited list of integral literals.

#### suffix parameter

An *embed-parameter* of the form
```
suffix ( [pp-balanced-token-seq] )
```
shall appear at most once in the *embed-parameter-seq*.

If the resource is empty, this *embed-parameter* is ignored. Otherwise, the *pp-balanced-token-seq* is placed immediately after the comma-delimited list of the integral constant expressions.

### Example

```cpp
constexpr unsigned char whl[] = {
#embed "ches.glsl" \
  prefix(0xEF, 0xBB, 0xBF, ) /* a sequence of bytes */ \
  suffix(,)
  0
};
// always null-terminated, contains the sequence if not empty
constexpr bool is_empty = sizeof(whl) == 1 && whl[0] == '\0';
constexpr bool is_not_empty = sizeof(whl) >= 4
  && whl[sizeof(whl) - 1] == '\0'
  && whl[0] == '\xEF' && whl[1] == '\xBB' && whl[2] == '\xBF';
static_assert(is_empty || is_not_empty);
```

#### `if_empty` parameter

An embed-parameter of the form
```
if_empty ( [pp-balanced-token-seq] )
```
shall appear at most once in the *embed-parameter-seq*.

If the resource is not empty, this *embed-parameter* is ignored. Otherwise, the `#embed` directive is replaced by the *pp-balanced-token-seq*.

### Example

`limit(0)` affects when a resource is considered empty. Therefore, the following program:

```cpp
#embed </owo/uwurandom> \
  if_empty(42203) limit(0)
```
expands to
```cpp
42203
```

### Example

This resource is considered empty due to the `limit(0)` *embed-parameter*, always, including in `__has_embed` clauses.

```cpp
int infinity_zero () {
#if __has_embed(</owo/uwurandom> limit(0) prefix(some tokens)) == __STDC_EMBED_EMPTY__
  // if </owo/uwurandom> exists, this conditional inclusion branch is taken and the function returns 0.
  return 0;
#else
  // otherwise, the resource does not exist
#error "The resource does not exist"
#endif
}
```

## Module directive

**pp-module**:
- *[`export`]* `module` *[pp-tokens]* `;` new-line

A *pp-module* shall not appear in a context where `module` or (if it is the first preprocessing token of the *pp-module*) `export` is an identifier defined as an object-like macro.

The *pp-tokens*, if any, of a *pp-module* shall be of the form:
```
pp-module-name [pp-module-partition] [pp-tokens]
```
where the *pp-tokens* (if any) shall not begin with a `(` preprocessing token and the grammar non-terminals are defined as:

**pp-module-name**:
- *[pp-module-name-qualifier]* identifier

**pp-module-partition**:
- `:` *[pp-module-name-qualifier]* identifier

**pp-module-name-qualifier**:
- identifier `.`
- pp-module-name-qualifier identifier `.`

No *identifier* in the *pp-module-name* or *pp-module-partition* shall currently be defined as an object-like macro.

Any preprocessing tokens after the `module` preprocessing token in the `module` directive are processed just as in normal text.

> Note: Each identifier currently defined as a macro name is replaced by its replacement list of preprocessing tokens.

The `module` and `export` (if it exists) preprocessing tokens are replaced by the *module-keyword* and *export-keyword* preprocessing tokens respectively.

> Note: This makes the line no longer a directive so it is not removed at the end of phase 4.

## Header unit importation

**pp-import**:
- *[`export`]* `import` header-name *[pp-tokens]* `;` new-line
- *[`export`]* `import` header-name-tokens *[pp-tokens]* `;` new-line
- *[`export`]* `import` pp-tokens `;` new-line

A *pp-import* shall not appear in a context where `import` or (if it is the first preprocessing token of the *pp-import*) `export` is an identifier defined as an object-like macro.

The preprocessing tokens after the `import` preprocessing token in the `import` *control-line* are processed just as in normal text (i.e., each identifier currently defined as a macro name is replaced by its replacement list of preprocessing tokens).

> Note: An `import` directive matching the first two forms of a *pp-import* instructs the preprocessor to import macros from the header unit denoted by the *header-name*, as described below.

The *point of macro import* for the first two forms of *pp-import* is immediately after the *new-line* terminating the *pp-import*. The last form of *pp-import* is only considered if the first two forms did not match, and does not have a point of macro import.

If a *pp-import* is produced by source file inclusion (including by the rewrite produced when a `#include` directive names an importable header) while processing the *group* of a *module-file*, the program is ill-formed.

In all three forms of *pp-import*, the `import` and `export` (if it exists) preprocessing tokens are replaced by the *import-keyword* and *export-keyword* preprocessing tokens respectively.

> Note: This makes the line no longer a directive so it is not removed at the end of phase 4.

Additionally, in the second form of *pp-import*, a *header-name* token is formed as if the *header-name-tokens* were the *pp-tokens* of a `#include` directive. The *header-name-tokens* are replaced by the *header-name* token.

> Note: This ensures that imports are treated consistently by the preprocessor and later phases of translation.

Each `#define` directive encountered when preprocessing each translation unit in a program results in a distinct *macro definition*.

> Note: A predefined macro name is not introduced by a `#define` directive. Implementations providing mechanisms to predefine additional macros are encouraged to not treat them as being introduced by a `#define` directive.

Each macro definition has at most one point of definition in each translation unit and at most one point of undefinition, as follows:

- The *point of definition* of a macro definition within a translation unit T is
  - if the `#define` directive of the macro definition occurs within T, the point at which that directive occurs, or otherwise,
  - if the macro name is not lexically identical to a keyword or to the *identifier*s `module` or `import`, the first point of macro import in T of a header unit containing a point of definition for the macro definition, if any.
  In the latter case, the macro is said to be *imported* from the header unit.

- The *point of undefinition* of a macro definition within a translation unit is the first point at which a `#undef` directive naming the macro occurs after its point of definition, or the first point of macro import of a header unit containing a point of undefinition for the macro definition, whichever (if any) occurs first.

A macro definition is *active* at a source location if it has a point of definition in that translation unit preceding the location, and does not have a point of undefinition in that translation unit preceding the location.

If a macro would be replaced or redefined, and multiple macro definitions are active for that macro name, the active macro definitions shall all be valid redefinitions of the same macro.

> Note: The relative order of *pp-import*s has no bearing on whether a particular macro definition is active.

### Example

```cpp
// Importable header "a.h"
#define X 123   // #1
#define Y 45    // #2
#define Z a     // #3
#undef X        // point of undefinition of #1 in "a.h"
```

```cpp
// Importable header "b.h"
import "a.h";   // point of definition of #1, #2, and #3, point of undefinition of #1 in "b.h"
#define X 456   // OK, #1 is not active
#define Y 6     // error: #2 is active
```

```cpp
// Importable header "c.h"
#define Y 45    // #4
#define Z c     // #5
```

```cpp
// Importable header "d.h"
import "c.h";   // point of definition of #4 and #5 in "d.h"
```

```cpp
// Importable header "e.h"
import "a.h";   // point of definition of #1, #2, and #3, point of undefinition of #1 in "e.h"
import "d.h";   // point of definition of #4 and #5 in "e.h"
int a = Y;      // OK, active macro definitions #2 and #4 are valid redefinitions
int c = Z;      // error: active macro definitions #3 and #5 are not valid redefinitions of Z
```

```cpp
// Module unit f
export module f;
export import "a.h";

int a = Y;      // OK
```

```cpp
// Translation unit #1
import f;
int x = Y;      // error: Y is neither a defined macro nor a declared name
```

## Macro replacement

### General

Two replacement lists are identical if and only if the preprocessing tokens in both have the same number, ordering, spelling, and whitespace separation, where all whitespace separations are considered identical.

An identifier currently defined as an object-like macro may be redefined by another `#define` preprocessing directive provided that the second definition is an object-like macro definition and the two replacement lists are identical, otherwise the program is ill-formed. Likewise, an identifier currently defined as a function-like macro may be redefined by another `#define` preprocessing directive provided that the second definition is a function-like macro definition that has the same number and spelling of parameters, and the two replacement lists are identical, otherwise the program is ill-formed.

### Example

The following sequence is valid:
```cpp
#define OBJ_LIKE      (1-1)
#define OBJ_LIKE      /* whitespace */ (1-1) /* other */
#define FUNC_LIKE(a)   ( a )
#define FUNC_LIKE( a )(     /* note the whitespace */ \
                a /* other stuff on this line
                  */ )
```
But the following redefinitions are invalid:
```cpp
#define OBJ_LIKE    (0)         // different token sequence
#define OBJ_LIKE    (1 - 1)     // different whitespace
#define FUNC_LIKE(b) ( a )      // different parameter usage
#define FUNC_LIKE(b) ( b )      // different parameter spelling
```

There shall be whitespace between the identifier and the replacement list in the definition of an object-like macro.

If the *identifier-list* in the macro definition does not end with an ellipsis, the number of arguments (including those arguments consisting of no preprocessing tokens) in an invocation of a function-like macro shall equal the number of parameters in the macro definition. Otherwise, there shall be at least as many arguments in the invocation as there are parameters in the macro definition (excluding the `...`). There shall exist a `)` preprocessing token that terminates the invocation.

The identifiers `__VA_ARGS__` and `__VA_OPT__` shall occur only in the *replacement-list* of a function-like macro that uses the ellipsis notation in the parameters.

A parameter identifier in a function-like macro shall be uniquely declared within its scope.

The identifier immediately following the `define` is called the *macro name*. There is one name space for macro names. Any whitespace characters preceding or following the replacement list of preprocessing tokens are not considered part of the replacement list for either form of macro.

A translation unit shall not `#define` or `#undef` names lexically identical to keywords, to the identifiers listed in the table of special identifiers, or to the *attribute-token*s, except that the names `likely` and `unlikely` may be defined as function-like macros.

If a `#` preprocessing token, followed by an identifier, occurs lexically at the point at which a preprocessing directive can begin, the identifier is not subject to macro replacement.

A preprocessing directive of the form
```
# define identifier replacement-list new-line
```
defines an *object-like macro* that causes each subsequent instance of the macro name to be replaced by the replacement list of preprocessing tokens that constitute the remainder of the directive. The replacement list is then rescanned for more macro names as specified below.

### Example

The simplest use of this facility is to define a "manifest constant", as in
```cpp
#define TABSIZE 100
int table[TABSIZE];
```

A preprocessing directive of the form
```
# define identifier lparen [identifier-list] ) replacement-list new-line
# define identifier lparen ... ) replacement-list new-line
# define identifier lparen identifier-list , ... ) replacement-list new-line
```
defines a *function-like macro* with parameters, whose use is similar syntactically to a function call. The parameters are specified by the optional list of identifiers. Each subsequent instance of the function-like macro name followed by a `(` as the next preprocessing token introduces the sequence of preprocessing tokens that is replaced by the replacement list in the definition (an invocation of the macro). The replaced sequence of preprocessing tokens is terminated by the matching `)` preprocessing token, skipping intervening matched pairs of left and right parenthesis preprocessing tokens. Within the sequence of preprocessing tokens making up an invocation of a function-like macro, new-line is considered a normal whitespace character.

The sequence of preprocessing tokens bounded by the outside-most matching parentheses forms the list of arguments for the function-like macro. The individual arguments within the list are separated by comma preprocessing tokens, but comma preprocessing tokens between matching inner parentheses do not separate arguments. If there are sequences of preprocessing tokens within the list of arguments that would otherwise act as preprocessing directives, the program is ill-formed.

### Example

The following defines a function-like macro whose value is the maximum of its arguments. It has the disadvantages of evaluating one or the other of its arguments a second time (including side effects) and generating more code than a function if invoked several times. It also cannot have its address taken, as it has none.

```cpp
#define max(a, b) ((a) > (b) ? (a) : (b))
```

The parentheses ensure that the arguments and the resulting expression are bound properly.

If there is a `...` immediately preceding the `)` in the function-like macro definition, then the trailing arguments (if any), including any separating comma preprocessing tokens, are merged to form a single item: the *variable arguments*. The number of arguments so combined is such that, following merger, the number of arguments is either equal to or one more than the number of parameters in the macro definition (excluding the `...`).

### Argument substitution

**va-opt-replacement**:
- `__VA_OPT__ (` *[pp-tokens]* `)`

After the arguments for the invocation of a function-like macro have been identified, argument substitution takes place. For each parameter in the replacement list that is neither preceded by a `#` or `##` preprocessing token nor followed by a `##` preprocessing token, the preprocessing tokens naming the parameter are replaced by a preprocessing token sequence determined as follows:

- If the parameter is of the form *va-opt-replacement*, the replacement preprocessing tokens are the preprocessing token sequence for the corresponding argument, as specified below.
- Otherwise, the replacement preprocessing tokens are the preprocessing tokens of corresponding argument after all macros contained therein have been expanded. The argument's preprocessing tokens are completely macro replaced before being substituted as if they formed the rest of the preprocessing translation unit with no other preprocessing tokens being available.

### Example

```cpp
#define LPAREN() (
#define G(Q) 42
#define F(R, X, ...)  __VA_OPT__(G R X) )
int x = F(LPAREN(), 0, <:-);    // replaced by int x = 42;
```

An identifier `__VA_ARGS__` that occurs in the replacement list shall be treated as if it were a parameter, and the variable arguments shall form the preprocessing tokens used to replace it.

### Example

```cpp
#define debug(...) fprintf(stderr, __VA_ARGS__)
#define showlist(...) puts(#__VA_ARGS__)
#define report(test, ...) ((test) ? puts(#test) : printf(__VA_ARGS__))
debug("Flag");
debug("X = %d\n", x);
showlist(The first, second, and third items.);
report(x>y, "x is %d but y is %d", x, y);
```
results in
```cpp
fprintf(stderr, "Flag");
fprintf(stderr, "X = %d\n", x);
puts("The first, second, and third items.");
((x>y) ? puts("x>y") : printf("x is %d but y is %d", x, y));
```

The identifier `__VA_OPT__` shall always occur as part of the preprocessing token sequence *va-opt-replacement*; its closing `)` is determined by skipping intervening pairs of matching left and right parentheses in its *pp-tokens*. The *pp-tokens* of a *va-opt-replacement* shall not contain `__VA_OPT__`. If the *pp-tokens* would be ill-formed as the replacement list of the current function-like macro, the program is ill-formed. A *va-opt-replacement* is treated as if it were a parameter, and the preprocessing token sequence for the corresponding argument is defined as follows. If the substitution of `__VA_ARGS__` as neither an operand of `#` nor `##` consists of no preprocessing tokens, the argument consists of a single placemarker preprocessing token. Otherwise, the argument consists of the results of the expansion of the contained *pp-tokens* as the replacement list of the current function-like macro before removal of placemarker tokens, rescanning, and further replacement.

> Note: The placemarker tokens are removed before stringization, and can be removed by rescanning and further replacement.

### Example

```cpp
#define F(...)           f(0 __VA_OPT__(,) __VA_ARGS__)
#define G(X, ...)        f(0, X __VA_OPT__(,) __VA_ARGS__)
#define SDEF(sname, ...) S sname __VA_OPT__(= { __VA_ARGS__ })
#define EMP

F(a, b, c)          // replaced by f(0, a, b, c)
F()                 // replaced by f(0)
F(EMP)              // replaced by f(0)

G(a, b, c)          // replaced by f(0, a, b, c)
G(a, )              // replaced by f(0, a)
G(a)                // replaced by f(0, a)

SDEF(foo);          // replaced by S foo;
SDEF(bar, 1, 2);    // replaced by S bar = { 1, 2 };

#define H1(X, ...) X __VA_OPT__(##) __VA_ARGS__ // error: ## may not appear at
                                                // the beginning of a replacement list

#define H2(X, Y, ...) __VA_OPT__(X ## Y,) __VA_ARGS__
H2(a, b, c, d)      // replaced by ab, c, d

#define H3(X, ...) #__VA_OPT__(X##X X##X)
H3(, 0)             // replaced by ""

#define H4(X, ...) __VA_OPT__(a X ## X) ## b
H4(, 1)             // replaced by a b

#define H5A(...) __VA_OPT__()/**/__VA_OPT__()
#define H5B(X) a ## X ## b
#define H5C(X) H5B(X)
H5C(H5A())          // replaced by ab
```

### The `#` operator

Each `#` preprocessing token in the replacement list for a function-like macro shall be followed by a parameter as the next preprocessing token in the replacement list.

A *character string literal* is a *string-literal* with no prefix. If, in the replacement list, a parameter is immediately preceded by a `#` preprocessing token, both are replaced by a single character string literal preprocessing token that contains the spelling of the preprocessing token sequence for the corresponding argument (excluding placemarker tokens). Let the *stringizing argument* be the preprocessing token sequence for the corresponding argument with placemarker tokens removed. Each occurrence of whitespace between the stringizing argument's preprocessing tokens becomes a single space character in the character string literal. Whitespace before the first preprocessing token and after the last preprocessing token comprising the stringizing argument is deleted. Otherwise, the original spelling of each preprocessing token in the stringizing argument is retained in the character string literal, except for special handling for producing the spelling of *header-name*s, *character-literal*s, and *string-literal*s (including the delimiting quotation mark (`"`)): a reverse solidus character (`\`) is inserted before each quotation mark and reverse solidus character of a *header-name*, *character-literal*, or *string-literal*, and each new-line character is replaced by the two-character sequence `\n`. If the replacement that results is not a valid character string literal, the program is ill-formed. The character string literal corresponding to an empty stringizing argument is `""`. The order of evaluation of `#` and `##` operators is unspecified.

### The `##` operator

A `##` preprocessing token shall not occur at the beginning or at the end of a replacement list for either form of macro definition.

If, in the replacement list of a function-like macro, a parameter is immediately preceded or followed by a `##` preprocessing token, the parameter is replaced by the corresponding argument's preprocessing token sequence; however, if an argument consists of no preprocessing tokens, the parameter is replaced by a placemarker preprocessing token instead.

For both object-like and function-like macro invocations, before the replacement list is reexamined for more macro names to replace, each instance of a `##` preprocessing token in the replacement list (not from an argument) is deleted and the preceding preprocessing token is concatenated with the following preprocessing token. Placemarker preprocessing tokens are handled specially: concatenation of two placemarkers results in a single placemarker preprocessing token, and concatenation of a placemarker with a non-placemarker preprocessing token results in the non-placemarker preprocessing token.

> Note: Concatenation can form a *universal-character-name*.

If the result is not a valid preprocessing token, the program is ill-formed. The resulting preprocessing token is available for further macro replacement. The order of evaluation of `##` operators is unspecified.

### Example

The sequence
```cpp
#define str(s)      # s
#define xstr(s)     str(s)
#define debug(s, t) printf("x" # s "= %d, x" # t "= %s", \
               x ## s, x ## t)
#define INCFILE(n)  vers ## n
#define glue(a, b)  a ## b
#define xglue(a, b) glue(a, b)
#define HIGHLOW     "hello"
#define LOW         LOW ", world"

debug(1, 2);
fputs(str(strncmp("abc\0d", "abc", '\4')        // this goes away
    == 0) str(: @\n), s);
#include xstr(INCFILE(2).h)
glue(HIGH, LOW);
xglue(HIGH, LOW)
```
results in
```cpp
printf("x" "1" "= %d, x" "2" "= %s", x1, x2);
fputs("strncmp(\"abc\\0d\", \"abc\", '\\4') == 0" ": @\n", s);
#include "vers2.h"      (after macro replacement, before file access)
"hello";
"hello" ", world"
```
or, after concatenation of the character string literals,
```cpp
printf("x1= %d, x2= %s", x1, x2);
fputs("strncmp(\"abc\\0d\", \"abc\", '\\4') == 0: @\n", s);
#include "vers2.h"      (after macro replacement, before file access)
"hello";
"hello, world"
```

Space around the `#` and `##` preprocessing tokens in the macro definition is optional.

### Example

In the following fragment:

```cpp
#define hash_hash # ## #
#define mkstr(a) # a
#define in_between(a) mkstr(a)
#define join(c, d) in_between(c hash_hash d)
char p[] = join(x, y);          // equivalent to char p[] = "x ## y";
```

The expansion produces, at various stages:

```cpp
join(x, y)
in_between(x hash_hash y)
in_between(x ## y)
mkstr(x ## y)
"x ## y"
```

In other words, expanding `hash_hash` produces a new preprocessing token, consisting of two adjacent sharp signs, but this new preprocessing token is not the `##` operator.

### Example

To illustrate the rules for placemarker preprocessing tokens, the sequence
```cpp
#define t(x,y,z) x ## y ## z
int j[] = { t(1,2,3), t(,4,5), t(6,,7), t(8,9,),
  t(10,,), t(,11,), t(,,12), t(,,) };
```
results in
```cpp
int j[] = { 123, 45, 67, 89,
  10, 11, 12, };
```

### Rescanning and further replacement

After all parameters in the replacement list have been substituted and `#` and `##` processing has taken place, all placemarker preprocessing tokens are removed. Then the resulting preprocessing token sequence is rescanned, along with all subsequent preprocessing tokens of the source file, for more macro names to replace.

If the name of the macro being replaced is found during this scan of the replacement list (not including the rest of the source file's preprocessing tokens), it is not replaced. Furthermore, if any nested replacements encounter the name of the macro being replaced, it is not replaced. These nonreplaced macro name preprocessing tokens are no longer available for further replacement even if they are later (re)examined in contexts in which that macro name preprocessing token would otherwise have been replaced.

The resulting completely macro-replaced preprocessing token sequence is not processed as a preprocessing directive even if it resembles one, but all pragma unary operator expressions within it are then processed as specified in the section about pragma operators below.

### Example

The sequence
```cpp
#define x       3
#define f(a)    f(x * (a))
#undef  x
#define x       2
#define g       f
#define z       z[0]
#define h       g(~
#define m(a)    a(w)
#define w       0,1
#define t(a)    a
#define p()     int
#define q(x)    x
#define r(x,y)  x ## y
#define str(x)  # x

f(y+1) + f(f(z)) % t(t(g)(0) + t)(1);
g(x+(3,4)-w) | h 5) & m
    (f)^m(m);
p() i[q()] = { q(1), r(2,3), r(4,), r(,5), r(,) };
char c[2][6] = { str(hello), str() };
```
results in
```cpp
f(2 * (y+1)) + f(2 * (f(2 * (z[0])))) % f(2 * (0)) + t(1);
f(2 * (2+(3,4)-0,1)) | f(2 * (~ 5)) & f(2 * (0,1))^m(0,1);
int i[] = { 1, 23, 4, 5, };
char c[2][6] = { "hello", "" };
```

### Scope of macro definitions

A macro definition lasts (independent of block structure) until a corresponding `#undef` directive is encountered or (if none is encountered) until the end of the translation unit. Macro definitions have no significance after translation phase 4.

A preprocessing directive of the form
```
# undef identifier new-line
```
causes the specified identifier no longer to be defined as a macro name. It is ignored if the specified identifier is not currently defined as a macro name.

## Line control

The *string-literal* of a `#line` directive, if present, shall be a character string literal.

The *line number* of the current source line is the line number of the current physical source line, i.e., it is one greater than the number of new-line characters read or introduced in translation phase 1 while processing the source file to the current preprocessing token.

A preprocessing directive of the form
```
# line digit-sequence new-line
```
causes the implementation to behave as if the following sequence of source lines begins with a source line that has a line number as specified by the digit sequence (interpreted as a decimal integer). If the digit sequence specifies zero or a number greater than 2147483647, the program is ill-formed.

A preprocessing directive of the form
```
# line digit-sequence " [s-char-sequence] " new-line
```
sets the presumed line number similarly and changes the presumed name of the source file to be the contents of the character string literal.

A preprocessing directive of the form
```
# line pp-tokens new-line
```
(that does not match one of the two previous forms) is permitted. The preprocessing tokens after `line` on the directive are processed just as in normal text (each identifier currently defined as a macro name is replaced by its replacement list of preprocessing tokens). If the directive resulting after all replacements does not match one of the two previous forms, the program is ill-formed; otherwise, the result is processed as appropriate.

## Diagnostic directives

A preprocessing directive of the form
```
# error [pp-tokens] new-line
```
renders the program ill-formed. A preprocessing directive of the form
```
# warning [pp-tokens] new-line
```
requires the implementation to produce at least one diagnostic message for the preprocessing translation unit.

Any diagnostic message caused by either of these directives should include the specified sequence of preprocessing tokens.

## Pragma directive

A preprocessing directive of the form
```
# pragma [pp-tokens] new-line
```
causes the implementation to behave in an implementation-defined manner. The behavior may cause translation to fail or cause the translator or the resulting program to behave in a non-conforming manner. Any pragma that is not recognized by the implementation is ignored.

## Null directive

A preprocessing directive of the form
```
# new-line
```
has no effect.

## Predefined macro names

The following macro names shall be defined by the implementation:

- `__cplusplus`  
The integer literal `202302L`. Future revisions of this document will replace the value of this macro with a greater value.

- `__DATE__`  
The date of translation of the source file: a character string literal of the form `"Mmm dd yyyy"`, where the names of the months are the same as those generated by the `asctime` function, and the first character of `dd` is a space character if the value is less than 10. If the date of translation is not available, an implementation-defined valid date shall be supplied.

- `__FILE__`  
The presumed name of the current source file (a character string literal).

- `__LINE__`  
The presumed line number (within the current source file) of the current source line (an integer literal).

- `__STDC_EMBED_NOT_FOUND`, `__STDC_EMBED_FOUND`, and `__STDC_EMBED_EMPTY`  
The integer literals `0`, `1`, and `2`, respectively. These represent values replaced from *has-embed-expression*s.

- `__STDC_HOSTED__`  
The integer literal `1` if the implementation is a hosted implementation or the integer literal `0` if it is a freestanding implementation.

- `__STDCPP_DEFAULT_NEW_ALIGNMENT__`  
An integer literal of type `std::size_t` whose value is the alignment guaranteed by a call to `operator new(std::size_t)` or `operator new[](std::size_t)`. Larger alignments will be passed to `operator new(std::size_t, std::align_val_t)`, etc.

- `__TIME__`  
The time of translation of the source file: a character string literal of the form `"hh:mm:ss"` as in the time generated by the `asctime` function. If the time of translation is not available, an implementation-defined valid time shall be supplied.

### Feature-test macros

| Macro name | Value |
|-----------|-------|
| `__cpp_aggregate_bases` | `201603L` |
| `__cpp_aggregate_nsdmi` | `201304L` |
| `__cpp_aggregate_paren_init` | `201902L` |
| `__cpp_alias_templates` | `200704L` |
| `__cpp_aligned_new` | `201606L` |
| `__cpp_attributes` | `200809L` |
| `__cpp_auto_cast` | `202110L` |
| `__cpp_binary_literals` | `201304L` |
| `__cpp_capture_star_this` | `201603L` |
| `__cpp_char8_t` | `202207L` |
| `__cpp_concepts` | `202002L` |
| `__cpp_conditional_explicit` | `201806L` |
| `__cpp_constexpr` | `202406L` |
| `__cpp_constexpr_dynamic_alloc` | `201907L` |
| `__cpp_constexpr_exceptions` | `202411L` |
| `__cpp_constexpr_in_decltype` | `201711L` |
| `__cpp_constexpr_virtual_inheritance` | `202506L` |
| `__cpp_consteval` | `202211L` |
| `__cpp_constinit` | `201907L` |
| `__cpp_contracts` | `202502L` |
| `__cpp_decltype` | `200707L` |
| `__cpp_decltype_auto` | `201304L` |
| `__cpp_deduction_guides` | `201907L` |
| `__cpp_delegating_constructors` | `200604L` |
| `__cpp_deleted_function` | `202403L` |
| `__cpp_designated_initializers` | `201707L` |
| `__cpp_enumerator_attributes` | `201411L` |
| `__cpp_expansion_statements` | `202506L` |
| `__cpp_explicit_this_parameter` | `202110L` |
| `__cpp_fold_expressions` | `201603L` |
| `__cpp_generic_lambdas` | `201707L` |
| `__cpp_guaranteed_copy_elision` | `201606L` |
| `__cpp_hex_float` | `201603L` |
| `__cpp_if_consteval` | `202106L` |
| `__cpp_if_constexpr` | `201606L` |
| `__cpp_impl_coroutine` | `201902L` |
| `__cpp_impl_destroying_delete` | `201806L` |
| `__cpp_impl_three_way_comparison` | `201907L` |
| `__cpp_impl_reflection` | `202506L` |
| `__cpp_implicit_move` | `202207L` |
| `__cpp_inheriting_constructors` | `201511L` |
| `__cpp_init_captures` | `201803L` |
| `__cpp_initializer_lists` | `200806L` |
| `__cpp_inline_variables` | `201606L` |
| `__cpp_lambdas` | `200907L` |
| `__cpp_modules` | `201907L` |
| `__cpp_multidimensional_subscript` | `202211L` |
| `__cpp_named_character_escapes` | `202207L` |
| `__cpp_namespace_attributes` | `201411L` |
| `__cpp_noexcept_function_type` | `201510L` |
| `__cpp_nontype_template_args` | `201911L` |
| `__cpp_nontype_template_parameter_auto` | `201606L` |
| `__cpp_nsdmi` | `200809L` |
| `__cpp_pack_indexing` | `202311L` |
| `__cpp_placeholder_variables` | `202306L` |
| `__cpp_pp_embed` | `202502L` |
| `__cpp_range_based_for` | `202211L` |
| `__cpp_raw_strings` | `200710L` |
| `__cpp_ref_qualifiers` | `200710L` |
| `__cpp_return_type_deduction` | `201304L` |
| `__cpp_rvalue_references` | `200610L` |
| `__cpp_size_t_suffix` | `202011L` |
| `__cpp_sized_deallocation` | `201309L` |
| `__cpp_static_assert` | `202306L` |
| `__cpp_static_call_operator` | `202207L` |
| `__cpp_structured_bindings` | `202411L` |
| `__cpp_template_parameters` | `202502L` |
| `__cpp_template_template_args` | `201611L` |
| `__cpp_threadsafe_static_init` | `200806L` |
| `__cpp_trivial_relocatability` | `202502L` |
| `__cpp_trivial_union` | `202502L` |
| `__cpp_unicode_characters` | `200704L` |
| `__cpp_unicode_literals` | `200710L` |
| `__cpp_user_defined_literals` | `200809L` |
| `__cpp_using_enum` | `201907L` |
| `__cpp_variable_templates` | `201304L` |
| `__cpp_variadic_friend` | `202403L` |
| `__cpp_variadic_templates` | `200704L` |
| `__cpp_variadic_using` | `201611L` |

The following macro names are conditionally defined by the implementation:

- `__STDC__`  
Whether `__STDC__` is predefined and if so, what its value is, are implementation-defined.

- `__STDC_MB_MIGHT_NEQ_WC__`  
The integer literal `1`, intended to indicate that, in the encoding for `wchar_t`, a member of the basic character set need not have a code value equal to its value when used as the lone character in an ordinary character literal.

- `__STDC_VERSION__`  
Whether `__STDC_VERSION__` is predefined and if so, what its value is, are implementation-defined.

- `__STDC_ISO_10646__`  
An integer literal of the form `yyyymmL` (for example, `199712L`). Whether `__STDC_ISO_10646__` is predefined and if so, what its value is, are implementation-defined.

- `__STDCPP_THREADS__`  
Defined, and has the value integer literal 1, if and only if a program can have more than one thread of execution.

The values of the predefined macros (except for `__FILE__` and `__LINE__`) remain constant throughout the translation unit.

If any of the pre-defined macro names in this subclause, or the identifier `defined`, is the subject of a `#define` or a `#undef` preprocessing directive, the program is ill-formed. Any other predefined macro names shall begin with a leading underscore followed by an uppercase letter or a second underscore.

## Pragma operator

A unary operator expression of the form:
```
_Pragma ( string-literal )
```
is processed as follows: The *string-literal* is *destringized* by deleting the `L` prefix, if present, deleting the leading and trailing double-quotes, replacing each escape sequence `\"` by a double-quote, and replacing each escape sequence `\\` by a single backslash. The resulting sequence of characters is processed through translation phase 3 to produce preprocessing tokens that are executed as if they were the *pp-tokens* in a pragma directive. The original four preprocessing tokens in the unary operator expression are removed.

### Example

```cpp
#pragma listing on "..\listing.dir"
```
can also be expressed as:
```cpp
_Pragma ( "listing on \"..\\listing.dir\"" )
```
The latter form is processed in the same way whether it appears literally as shown, or results from macro replacement, as in:
```cpp
#define LISTING(x) PRAGMA(listing on #x)
#define PRAGMA(x) _Pragma(#x)

LISTING( ..\listing.dir )
```