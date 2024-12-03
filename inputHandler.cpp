#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <string>
#include <vector>
#include <stack>
#include <cctype>
#include <iostream>

class InputHandler {
public:
    // Function to get input from the user
    std::string getInput() {
        std::string input;
        std::cout << "Enter a mathematical expression: ";
        std::getline(std::cin, input);
        return input;
    }

    // Function to validate parentheses
    bool validateParentheses(const std::string& input) {
        std::stack<char> parentheses;

        for (char ch : input) {
            if (ch == '(') {
                parentheses.push(ch);
            } else if (ch == ')') {
                if (parentheses.empty()) {
                    std::cerr << "Error: Unmatched closing parenthesis ')'." << std::endl;
                    return false;
                }
                parentheses.pop();
            }
        }

        if (!parentheses.empty()) {
            std::cerr << "Error: Unmatched opening parenthesis '('." << std::endl;
            return false;
        }

        return true;
    }

    // Function to validate sequences
    bool validateSequences(const std::vector<std::string>& tokens) {
        for (size_t i = 0; i < tokens.size() - 1; ++i) {
            const std::string& current = tokens[i];
            const std::string& next = tokens[i + 1];

            // Invalid sequences: consecutive operators, number followed by '('
            if (isOperator(current[0]) && isOperator(next[0])) {
                std::cerr << "Error: Consecutive operators '" << current << next << "' found." << std::endl;
                return false;
            }
            if (isNumber(current) && next == "(") {
                std::cerr << "Error: Missing operator before '(" << "'." << std::endl;
                return false;
            }
            if (current == ")" && isNumber(next)) {
                std::cerr << "Error: Missing operator after ')'. " << std::endl;
                return false;
            }
        }
        return true;
    }

    // Function to tokenize the input
    std::vector<std::string> tokenizeInput(const std::string& input) {
        std::vector<std::string> tokens;
        std::string currentToken;

        bool expectingUnary = true;  // True if a unary operator is valid
        for (char ch : input) {
            if (std::isdigit(ch) || ch == '.') {
                currentToken += ch;  // Accumulate digits and decimal points
                expectingUnary = false;
            } else if (isOperator(ch) || ch == '(' || ch == ')') {
                if (!currentToken.empty()) {
                    tokens.push_back(currentToken);  // Add the accumulated number
                    currentToken.clear();
                }

                // Handle unary operators
                if (isOperator(ch) && expectingUnary) {
                    currentToken += ch;  // Accumulate unary operator
                } else {
                    tokens.push_back(std::string(1, ch));  // Add the operator or parenthesis
                }

                expectingUnary = (ch == '(' || isOperator(ch));
            } else if (!std::isspace(ch)) {
                std::cerr << "Error: Invalid character '" << ch << "' in input." << std::endl;
            }
        }

        if (!currentToken.empty()) {
            tokens.push_back(currentToken);  // Add the last number or operator
        }

        return tokens;
    }

private:
    // Helper function to check if a character is an operator
    bool isOperator(char ch) {
        return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^' || ch == '%';
    }

    // Helper function to check if a token is a number
    bool isNumber(const std::string& token) {
        for (char ch : token) {
            if (!std::isdigit(ch) && ch != '.') {
                return false;
            }
        }
        return !token.empty();
    }
};

#endif // INPUTHANDLER_H