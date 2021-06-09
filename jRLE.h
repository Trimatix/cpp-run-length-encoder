/**
 * Simple run-length encoder, written for an employment test.
 * 
 * This header provides access to the jRLE utilities for use in your code.
 * The correct order of usage to process a string is:
 *  1) tokenize
 *  2) en/decode
 *  3) concatenate
 *  
 * For example, to encode "aaa":
 * string encoded = vectorConcatenate(encodeTokens(tokenizeUnencoded("aaa")));
 * 
 * However, the functions provided as part of this program have potential
 * use cases outside of run-length encoding, to this program's spec.
 * 
 * 
 *                   == Terminology ==
 * In this program, the term "token" refers to a description of
 * a string containing one or more of a single character.
 * Note: While unencoded and encoded tokens are generally distinguished,
 *       it is reasonable to encode an already encoded string.
 *       However, a string must be encoded at least once before
 *       it can be decoded.
 *  + An example of an unencoded token ("dToken"): aaa
 *  + An example of an encoded token ("eToken"): 3a
 * 
 * 
 *                == Encoding Format ==
 * - For character sequences of length less than 9,
 *   The encoding is the character count followed by the character.
 *   E.g: "aaa" -> "3a"
 * 
 * - For character sequences of length more than 9,
 *   The encoding is prefixed by a '#' character.
 *   E.g: "aaaaaaaaaa" -> "#10a"
 * 
 * - For sequences of digit characters,
 *   the string is postfixed by a '#' character.
 *   E.g: "111" -> "31#"
 * 
 * - For sequences of # characters,
 *   The string is postfixed by a '#' character.
 *   E.g: "###" -> "3##"
 * 
 * 
 * Written by Jasper Law 2020
 * https://github.com/Trimatix/cpp-run-length-encoder
 */

#include <fstream>
#include <iostream>
#include <string>
#include <vector>


/**
 * Read the contents of the requested text file, and return it as a string.
 * The file must have the extension '.txt'
 * 
 * @param fname path to the desired text file
 * @return std::string containing the contents of the file
 * @throw std::ios_base::failure if the file could not be opened
 */
std::string readFile(std::string fname);


/**
 * Write the passed string to the requested text file.
 * The file must have the extension '.txt'
 * 
 * @param fname path to the desired text file
 * @return std::string containing the desired contents of the file
 * @throw std::ios_base::failure if the file could not be opened
 */
void writeFile(std::string fname, std::string inText);


/**
 * Split an unencoded string into a vector of unencoded tokens
 * 
 * @param inText std::string containing the unencoded text to tokenize
 * @return std::vector of std::strings representing dTokens
 */
std::vector<std::string> tokenizeUnencoded(std::string inText);


/**
 * Split a run-length encoded string into a vector of encoded tokens
 * 
 * @param inText std::string containing the encoded text to tokenize
 * @return std::vector of std::strings representing eTokens
 * @throw std::invalid_argument if a token is not accompanied by its char count
 */
std::vector<std::string> tokenizeEncoded(std::string inText);


/**
 * Concatenate the items in a vector of strings into a single string.
 * 
 * @param inVect the std::vector of std::strings to concatenate
 * @return All items of inVect, in order, in a single std::string
 */
std::string vectorConcatenate(std::vector<std::string> inVect);


/**
 * Individually run-length encode each item in a vector of tokens
 * Each token must only contain one or more of a single character
 * 
 * @param dTokens vector of ASCII-encoded std::strings to run-length encode
 * @return vector of encoded std::strings
 */
std::vector<std::string> encodeTokens(std::vector<std::string> dTokens);


/**
 * Individually run-length decode each item in a vector of tokens
 * Each token must contain an encoding of one or more of a single character
 * 
 * @param eTokens vector of ASCII-encoded std::strings to run-length decode
 * @return vector of decoded std::strings
 */
std::vector<std::string> decodeTokens(std::vector<std::string> eTokens);
