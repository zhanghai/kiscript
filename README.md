# KiScript

ECMAScript, kept simple.

## Thesis

- Simple: Less magic.
- Strict: Less error-prone.
- Sweet: Allow syntax sugar.
- Functional: Classes are derived.

## Specification

Based on ECMAScript 5.0, with reference to ECMAScript 6.0, and some customization.

## Grammar Variation

- Lexical
    - UTF-8 instead of UTF-16.
    - No `/regex/` literal.
    - No `Keyword` as `IdentifierName`.
    - No `.1` or `1.`.
    - Allow multiple leading `0`s as decimal.
    - `\0` is always `NUL` in `StringLiteral`.
- Syntactic
    - No automatic semicolon insertion, so no restriction on no line terminator.
    - No multiple or trailing `,` in `ArrayLiteral`.
    - No `in` or `for in`.
    - For each for `iterable` with `for ( : )`.
    - No `EmptyStatement`.
    - `Block` instead of `Statement` for `if` / `for` / `while` / `do while` body.
    - No trailing semicolon in `do while`.
- Language
    - No primitive. (Refer to Python)
    -
