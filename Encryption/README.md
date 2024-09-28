# Project in C++

In this encryption project, I developed a C++ program designed to encrypt or decrypt a text file based on user-provided letter pairs. The program followed specific input and output specifications and included error handling to enhance the user experience and provide clear feedback in case of errors.

Key components and features of the project:
1. **Input Handling:** The program received a filename and a set of letter pairs from the standard input (std::cin). The letter pairs served as the encoding key for the encryption or decryption process. The program continued to load letter pairs until either no additional input was provided or the letter 'q' was entered.

2. **File Processing:** The specified input file was loaded, and the program processed the text, converting every occurrence of the first character in each encoding pair to the second character in the pair. The resulting output was directed to the standard output (std::cout).

3. **Duplicate Output Lines:** To enhance efficiency, the program ensured that if an output line was identical to a previous output line, it was only output the first time it appeared, reducing redundancy in the output.

4. **Error Handling:** The project incorporated robust error handling to notify users of potential issues. Error conditions that were caught and appropriately reported included:
   - Inaccessibility of the specified input file, with a descriptive error message.
   - Invalid encoding pairs, such as single-character pairs or those with more than two characters, with specific error messages.
   - Duplicates in the first characters of encoding pairs, with clear error messages indicating the conflicting pairs.

This project demonstrates my proficiency in C++ programming, input/output handling, error detection, and effective error reporting. It provides a practical solution for encrypting or decrypting text files using user-defined encoding rules while maintaining a user-friendly interface with informative error messages.

