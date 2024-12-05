#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include <cctype>
#include <cmath>
#include <stdexcept>

using namespace std;


//Syntactical Errors
bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '%' || c == '^' || c == '/');
}

bool isValidChar(char c) {
    return (isdigit(c) || c == '+' || c == '-' || c == '*' || c == '%' || c == '/' || c == '(' || c == '.' || c == '^' || c == ' ' || c == ')');
}

// Check for balanced parentheses
// Creates a counter to keep track of parens, if the counter is not 0 or if it is negative, it returns False.
bool areParenthesesBalanced(const string &input) {
    stack<char> parentheses;
    for (char c : input) {
        if (c == '(') parentheses.push(c);
        else if (c == ')') {
            if (parentheses.empty()) return false;
            parentheses.pop();
        }
    }
    return parentheses.empty();
}

//Check if the expression has consecutive operators (invalid syntax like 2 ++ 3)
bool hasConsecutiveOperators(const std::string& input) {
    bool prevWasOperator = false;
    bool prevWasStar = false;  // To track if the previous character was a '*'

    // Iterate through the input string character by character
    for (size_t i = 0; i < input.size(); ++i) {
        char current = input[i];

        // Check if the current character is an operator
        if (current == '+' || current == '-' || current == '*' || current == '/' || current == '%' || current == '^') {
            //If the previous character was also an operator
            if (prevWasOperator) {
                if (prevWasStar && current == '*') {
                    //Allow '**' for squaring
                    prevWasStar = false; //Reset the star flag
                    continue; //Skip the current '*' as it's part of '**'
                }
                return true; //Invalid consecutive operator (e.g., '++', '+*', etc.)
            }
            prevWasOperator = true;

            // Special case for '*' being part of '**'
            if (current == '*') {
                prevWasStar = true; // Mark that '*' was seen
            }
        } else {
            prevWasOperator = false; // Reset if the current character is not an operator
            prevWasStar = false; // Reset the '*' tracking if it's not followed by another '*'
        }
    }

    return false; // No consecutive operators found
}



/*Mathmatical Errors:
These can be used throughout the program to easily check for input errors.*/

//Check for division by 0, the parameter can be changed an integer if needed.
//The return values can also be changed if the divided value would be prefered.
bool divisionByZero(const char denom) {
    if(denom == 0){
        throw invalid_argument("Error: Division by zero is not allowed.");
        return true;
    }
    return false;
}

//Checks if 0 is raised to a negative exponent.
double zeroRaisedToNegativePower(double base, int exponent) {
    // Check if the base is zero and the exponent is negative
    if (base == 0 && exponent < 0) {
        throw std::invalid_argument("Error: Zero cannot be raised to a negative power.");
    }

    //Perform the exponentiation if no errors, can be changed to return T/F if needed.
    return pow(base, exponent);
}



// Basic input validation for the calculator
void validateInput(const string &input) {
    if (input.empty()) {
        throw invalid_argument("Input is empty.");
    }
    for (char c : input) {
        if (!isValidChar(c)) {
            throw invalid_argument("Invalid character detected: " + string(1, c));
        }
    }
    if (!areParenthesesBalanced(input)) {
        throw invalid_argument("Unbalanced parentheses.");
    }
    if (hasConsecutiveOperators(input)) {
        throw invalid_argument("Consecutive operators are not allowed.");
    }
}

double evaluateExpression(const string &input) {
    /*For ease of use sake, just returned 0... This is where the evaluation phase would take place.*/
    return 0.0;
}

int main() {
    while(true){
    string input;
    cout << "Enter a mathematical expression: ";
    getline(cin, input);
    if(input == "end"){
        break;
    }

    try {
        validateInput(input);  // Validate the input
        double result = evaluateExpression(input);  // Evaluate the expression
        cout << "Result: " << result << endl;
    } catch (const invalid_argument &e) {
        cout << "Error: " << e.what() << endl;
    }
    }
    return 0;
}
