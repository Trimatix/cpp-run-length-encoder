/**
 * Simple run-length encoder, written for an employment test.
 * 
 *                   == Terminology ==
 * - In this program, the term "token" refers to a description of
 *   a string containing one or more of a single character.
 *   Note: While unencoded and encoded tokens are generally distinguished,
 *         it is reasonable to encode an already encoded string.
 *         However, a string must be encoded at least once before
 *         it can be decoded.
 *   + An example of an unencoded token ("dToken"): aaa
 *   + An example of an encoded token ("eToken"): 3a
 * 
 * - "Long sequence" refers to a string containing 10 or
 *   more of a single character.
 *   + An example of an unencoded long sequence: aaaaaaaaaa
 *   + An example of an encoded long sequence: #10a
 * 
 * - "#-case a" refers to the special case where a token is a long sequence
 * - "#-case b"                            where a token consists of '#' chars
 * - "#-case-c"                            where a token consists of a number
 *             This case is an issue, as the token must be separated from the
 *             next, lest the token be confused with the next one's char count
 * 
 * Invoke from the command line with your file path as the first argument.
 * File must have the extension ".txt" and use ASCII encoding.
 * 
 * Written by Jasper Law 2020
 * https://github.com/Trimatix/cpp-run-length-encoder
 */
#include "jRLE.h"


/**
 * Read the contents of the requested text file, and return it as a string
 * 
 * @param fname path to the desired text file
 * @return std::string containing the contents of the file
 * @throw std::ios_base::failure if the file could not be opened
 */
std::string readFile(std::string fname) {
    // Validate provided file extension
    if (fname.length() < 4 ||
            fname.substr(fname.length() - 4, 4) != ".txt") {

        std::cerr << "Invalid file path '" + fname
            + "' - path must end with the file extension '.txt'";
        throw std::invalid_argument("Invalid file path '" + fname
            + "' - path must end with the file extension '.txt'");
    }

    // Initialize file reader
    std::ifstream freader{ fname };

    // Handle invalid file paths
    if (!freader) {
        std::cerr << "Error opening file '" + fname
            + "' - Ensure path is correct and file is not in use.\n";
        throw std::ios_base::failure("Error opening file '" + fname
            + "' - Ensure path is correct and file is not in use.");
    }
    
    // Read file contents
    std::string fcontents;
    std::string line;
    while(std::getline(freader, line)){
        fcontents += line + '\n';
    }

    freader.close();
    return fcontents;
}


/**
 * Write the passed string to the requested text file
 * 
 * @param fname path to the desired text file
 * @return std::string containing the desired contents of the file
 * @throw std::ios_base::failure if the file could not be opened
 */
void writeFile(std::string fname, std::string inText) {
    // Validate provided file extension
    if (fname.length() < 4 ||
            fname.substr(fname.length() - 4, 4) != ".txt") {

        std::cerr << "Invalid file path '" + fname
            + "' - path must end with the file extension '.txt'";
        throw std::invalid_argument("Invalid file path '" + fname
            + "' - path must end with the file extension '.txt'");
    }

    // Initialize file reader
    std::ofstream fwriter{ fname };

    // Handle invalid file paths
    if (!fwriter) {
        std::cerr << "Error opening file '" + fname
            + "' - Ensure path is correct and file is not in use.\n";
        throw std::ios_base::failure("Error opening file '" + fname
            + "' - Ensure path is correct and file is not in use.");
    }
    
    // Write file contents
    fwriter << inText;
    fwriter.close();
}


/**
 * Split an unencoded string into a vector of unencoded tokens
 * 
 * @param inText std::string containing the unencoded text to tokenize
 * @return std::vector of std::strings representing dTokens
 */
std::vector<std::string> tokenizeUnencoded(std::string inText) {
    // Special case: empty string
    if (inText.empty()) {
        return {};
    }

    // Starting index of the current token in the string
    int currentSeqStart{0};
    // The finished vector of unencoded tokens
    std::vector<std::string> dTokens{};

    // Loop over all characters in the string, start with the 1st
    for (int i{0}; i < inText.length(); i++) {
        // If the current char is different to the previous, or if X is the
        // last char in the string.
        if (i == inText.length() - 1 || inText.at(i) != inText.at(i+1)) {
            // Add a new token containing the chars in inText, from
            // currentSeqStart to the previous char
            dTokens.push_back(inText.substr(currentSeqStart,
                i - currentSeqStart + 1));
            // Start a new sequence from the current char
            currentSeqStart = i+1;
        }
    }

    return dTokens;
}


/**
 * Split a run-length encoded string into a vector of encoded tokens
 * 
 * @param inText std::string containing the encoded text to tokenize
 * @return std::vector of std::strings representing eTokens
 * @throw std::invalid_argument if a token is not accompanied by its char count
 */
std::vector<std::string> tokenizeEncoded(std::string inText) {
    // Special case: empty string
    if (inText.empty()) {
        return {};
    }

    // The finished vector of eTokens
    std::vector<std::string> eTokens{};
    // The starting index of the current long sequence
    // -1 indicates the current token is not a long sequence
    int longSeqStart {-1};
    // Temporary variable containing the token currently being read.
    // Not always used.
    std::string newToken;

    // Iterate over all characters in inText
    for (int i {0}; i < inText.length(); i++) {
        // Temporary variable containing the currently read character
        char currentChar = inText.at(i);
        // If the current token is a long sequence
        if (longSeqStart != -1) {
            if (!isdigit(currentChar)) {
                if (currentChar == '#') {
                    // If the next character is #
                    if (inText.at(i+1) == '#') {
                        // #-case b
                        // Add a new token with the chars in inText,
                        // from longSeqStart to char after the current
                        newToken = inText.substr(longSeqStart,
                            i - longSeqStart + 2);
                        // In case #-case a is also present,
                        // start a new long sequence
                        longSeqStart = i + 2;
                        // Skip the iteration that would handle the extra #
                        i++;
                    // If the current char is not a number or a #
                    } else {
                        // #-case a and/or #-case c
                        // Add a new token with the chars from longSeqStart to
                        // the previous char
                        newToken = inText.substr(longSeqStart,
                            i - longSeqStart);
                        // In case #-case a is also present in the next token,
                        // start a new long sequence
                        longSeqStart = i+1;
                    }
                // If the current character is not a number or a #
                } else {
                    // #-case a
                    // Add a new token with the chars from longSeqStart
                    // to the current char
                    newToken = inText.substr(longSeqStart,
                        i - longSeqStart + 1);
                    // No # detected, so disable long sequence tracker
                    longSeqStart = -1;
                }
                // Unless current char is EOF with no char count,
                // push the new token to the vector
                if (newToken != "\n") {
                    eTokens.push_back(newToken);
                }
            }
        // If the current token is not a long sequence
        } else {
            // Start a new long sequence if one is marked
            if (currentChar == '#') {
                longSeqStart = i + 1;
            } else if (!isdigit(inText.at(i))) {
                // If a char exists that is not accompanied by a char count
                // (except LFCR on EOF), the encoding is invalid. Throw error.
                if (inText.at(i) != '\n' && i < inText.length() - 1) {
                    throw std::invalid_argument(
                        "Invalid encoded sequence. Non-digit char at position "
                         + std::to_string(i));
                }
            // If current char is a digit, this is a single-char token.
            } else if (inText.substr(i, 2) != "\n") {
                // Add a new token containing the count and the char.
                eTokens.push_back(inText.substr(i, 2));
                // Skip the token-defining char.
                i++;
            }
        }
    }

    return eTokens;
}


/**
 *  Concatenate the items in a vector of strings into a single string.
 * 
 * @param inVect the std::vector of std::strings to concatenate
 * @return All items of inVect, in order, in a single std::string
 */
std::string vectorConcatenate(std::vector<std::string> inVect) {
    std::string outStr{};
    for (std::string currentToken: inVect) {
        outStr += currentToken;
    }
    return outStr;
}


/**
 * Individually run-length encode each item in a vector of tokens
 * Each token must only contain one or more of a single character
 * 
 * @param dTokens vector of ASCII-encoded std::strings to run-length encode
 * @return vector of encoded std::strings
 */
std::vector<std::string> encodeTokens(std::vector<std::string> dTokens) {
    // Special case: empty vector
    if (dTokens.empty()) {
        return {};
    }

    // The finished vector of eTokens
    std::vector<std::string> eTokens{};
    // Temporary variables for the current processing and read tokens
    std::string newToken{};
    std::string currentToken;

    // Iterate over all tokens in dTokens
    for (int i{0}; i < dTokens.size(); i++) {
        currentToken = dTokens.at(i);
        newToken = "";
        // #-case a
        if (currentToken.length() > 9 ||
        // OR #-case c (or both)
                (i > 0 && isdigit(dTokens.at(i-1).front()))) {
            // Preceed the token with a # symbol
            newToken = "#";
        }

        // Add a new token by concatenating the length of the token with
        // one of its characters
        newToken += std::to_string(currentToken.length()) + currentToken.front();
        // If the token consists of # characters, mark it with an extra #
        if (currentToken.front() == '#') {
            newToken += "#";
        }

        eTokens.push_back(newToken);
    }

    return eTokens;
}


/**
 * Individually run-length decode each item in a vector of tokens
 * Each token must contain an encoding of one or more of a single character
 * 
 * @param eTokens vector of ASCII-encoded std::strings to run-length decode
 * @return vector of decoded std::strings
 */
std::vector<std::string> decodeTokens(std::vector<std::string> eTokens) {
    // Special case: empty vector
    if (eTokens.empty()) {
        return {};
    }
    // The finished vector of decoded tokens
    std::vector<std::string> dTokens{};

    // Iterate over all encoded tokens
    for (int i {0}; i < eTokens.size(); i++) {
        std::string currentToken = eTokens.at(i);
        // If the token describes a single non-# character
        if (currentToken.length() == 2){
            // Add a new token by concatenating the back character x times,
            // where x is the front character, converted to int
            dTokens.push_back(std::string(currentToken.front() - '0',
                currentToken.back()));
        // #-case b
        } else if (currentToken.back() == '#') {
            // Assume #-case a as well to be safe.
            // Add a new token by concatenating character x, y times,
            // where x is the third character from the back (the back 2 are #)
            // and y is all preceeding characters
            dTokens.push_back(std::string(stoi(currentToken.substr(0,
                currentToken.length() - 2)), '#'));
        // #-case a
        } else {
            // Same as above, but using the last character as
            // the token-defining character, and all preceeding for the count.
            dTokens.push_back(std::string(stoi(currentToken.substr(0,
                currentToken.length() - 1)), currentToken.back()));
        }
    }

    return dTokens;
}


/**
 * Run-length encode or decode the given text file, and write back the result.
 * 
 * @param argc The number of arguments passed in argv
 * @param argv A two-long array of strings, defining the arguments below:
 * argv[0] is the function to perform: -e for encode, -d for decode.
 * argv[1] is the path to the input file. File must have the extension .txt
 * @throw std::invalid_argument If argv is not length 2
 * @throw std::invalid_argument If argv[0] is neither -e nor -d.
 */
int main(int argc, char* argv[]) {
    // Require both arguments
    if (argc < 3) {
        std::string errstring =
            "Missing required argument. Correct command format: ";
        errstring += "jRLE.exe <-e | -d> file-path";
        std::cerr << errstring << std::endl;
        throw std::invalid_argument(errstring);
    }

    std::string fileName = std::string(argv[2]);
    std::string func = std::string(argv[1]);
    std::string fileText = readFile(fileName);
    std::vector<std::string> inTokens;
    std::vector<std::string> outTokens;
    std::string outText;

    // Handle decoding
    if (func == "-d") {
        inTokens = tokenizeEncoded(fileText);
        outTokens = decodeTokens(inTokens);
        
    // Handle encoding
    } else if (func == "-e") {
        inTokens = tokenizeUnencoded(fileText);
        outTokens = encodeTokens(inTokens);
    
    // Handle invalid function arguments
    } else {
        std::cerr << "Invalid argument '" + func
            + "' - arg1 must be behaviour flag '-e' or '-d'\n";
        throw std::invalid_argument("Invalid argument '" + func
            + "' - arg1 must be behaviour flag '-e' or '-d'");
    }

    // Concatenate and write the resulting vector to file
    outText = vectorConcatenate(outTokens);
    writeFile(fileName, outText);
 
    // Report the compression ratio
    float ratio;
    if (func == "-e") {
        ratio = static_cast<float>(fileText.length()) /
                    static_cast<float>(outText.length());
    } else {
        ratio = static_cast<float>(outText.length()) /
                    static_cast<float>(fileText.length());
    }
    std::cout << "Original file length: " + std::to_string(fileText.length())
        + "\nNew length: " + std::to_string(outText.length())
        + "\nCompression ratio: " + std::to_string(ratio);

    return 0;
}
