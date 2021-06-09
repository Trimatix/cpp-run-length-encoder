# jRLE
This is a sanitised copy of a private repository, and so does not contain the project commit history.

## A Simple Run-Length Encoder
Simple run-length encoder written in C++, written for an employment test.

## How To Download And Use
Download the latest release from [the releases tab](https://github.com/Trimatix/cpp-run-length-encoder/releases).

To use, invoke jRLE.exe from the command line with the function as your first argument, and your file path as the second argument.
- Function must be either `-e` for encoding, or `-d` for decoding.
- File must have the extension ".txt" and use ASCII encoding.

## Terminology
### Tokens
In this program, the term "token" refers to a description of a string containing one or more of a single character.
Note: While unencoded and encoded tokens are generally distinguished, it is reasonable to encode an already encoded string. However, a string must be encoded at least once before it can be decoded.

+ An example of an unencoded token ("dToken"): aaa
+ An example of an encoded token ("eToken"): 3a

### Encoding Format
- For character sequences of length less than 9, the encoding is the character count followed by the character.

E.g: "aaa" -> "3a"

- For character sequences of length more than 9, the encoding is prefixed by a '#' character.

E.g: "aaaaaaaaaa" -> "#10a"

- For sequences of digit characters, the string is postfixed by a '#' character.

E.g: "111" -> "31#"

- For sequences of # characters, the string is postfixed by a '#' character.

E.g: "###" -> "3##"

### Long Sequences
"Long sequence" refers to a string containing 10 or more of a single character.

+ An example of an unencoded long sequence: aaaaaaaaaa
+ An example of an encoded long sequence: #10a

### #-Cases
1. "#-case a" refers to the special case where a token is a long sequence
2. "#-case b"                            where a token consists of '#' chars
3. "#-case-c"                            where a token consists of a number

#-case c is an issue, as the token must be separated from the next, lest the token be confused with the next one's char count.
 
 Written by Jasper Law 2020
 
 https://github.com/Trimatix/cpp-run-length-encoder
