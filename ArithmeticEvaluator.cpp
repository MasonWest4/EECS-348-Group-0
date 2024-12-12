//import libraries
#include <iostream> //input output
#include <string> //allow strings
#include <stack> //easy of use for pushing and popping to track parenethesis
#include <vector> //imporved version of array for easier stroage
#include <stdexcept> //stop the program from crashing on underfined error
#include <cmath> //allow mathmatical function succh as pow
#include <map> //dirctionary  to help assign operators

//intiliaize class for the evaluator
class ExpressionEvaluator {
    //privvate section
private:
    // struct to store operator properties
    struct Operator {
        int precedence;
        bool rightAssociative;
    };
    // map of operators and their precedence and asociativity
    std::map<std::string, Operator> operators{
        {"+", {1, false}}, {"-", {1, false}}, 
        {"*", {2, false}}, {"/", {2, false}},
        {"%", {2, false}}, {"**", {3, true}}
    };
    // checks if a token is an operator
    bool isOperator(const std::string& token) {
        return operators.find(token) != operators.end();
    }
    // gets the precedence of an operator
    int getPrecedence(const std::string& op) {
        return operators[op].precedence;
    }
    // checks if an operator is right associative (you evaluate from right to left)
    bool isRightAssociative(const std::string& op) {
        return operators[op].rightAssociative;
    }
    // checks if a token is numeric 
    bool isNumeric(const std::string& token) {
        //iterate through all tokens
        for (char c : token) {
            //check if the operatior is a digit and not a decimal
            if (!std::isdigit(c) && c != '.') {
                return false;
            }
        }
        //return 
        return !token.empty();
    }
    // splits the input expression into tokens 
    std::vector<std::string> tokenize(const std::string& expression) {
        std::vector<std::string> tokens; //intilize vector
        std::string token; //set string to go inside of vector
        bool expectUnary = true; // true if the next operator could be unary
        //iuterate through the expression
        for (size_t i = 0; i < expression.length(); ++i) {
            char c = expression[i]; //store the current part of the expression
            if (std::isspace(c)) {
                continue; // skip spaces
            }
            if (std::isdigit(c) || c == '.') {
                token += c; // accumulate digits or decimal points into the current token
                expectUnary = false;
            }
            else {
                //deal with mathmatical symbols
                if (!token.empty()) {
                    tokens.push_back(token); // push the completed numeric token
                    token.clear();
                }
                //check for exponenet special case
                if (c == '*' && i + 1 < expression.length() && expression[i + 1] == '*') {
                    // handle ** as a single operator
                    tokens.push_back("**");
                    ++i; // skip the second *
                    expectUnary = true;
                }
                //check for left para
                else if (c == '(') {
                    tokens.push_back("("); // push an opening parenthesis
                    expectUnary = true; // next operator can be unary
                }
                //check for right para
                else if (c == ')') {
                    tokens.push_back(")"); // push a closing parenthesis
                    expectUnary = false; // next operator cannot be unary
                }
                // cehck for operators
                else if (std::string("+-*/%").find(c) != std::string::npos) {
                    if (expectUnary && (c == '+' || c == '-')) {
                        tokens.push_back("0"); // convert unary to binary by adding a 0
                    }
                    tokens.push_back(std::string(1, c)); // push the operator
                    expectUnary = true; // next operator can be unary
                }
                //unknown character found, throw error
                else {
                    throw std::invalid_argument("Invalid character in expression");
                }
            }
        }
        if (!token.empty()) {
            tokens.push_back(token); // push the last token if theres any
        }
        return tokens;
    }
    // converts tokens to reverse polish notation 
    std::vector<std::string> toRPN(const std::vector<std::string>& tokens) {
        std::vector<std::string> output; //initialize vectors
        std::stack<std::string> operators;
        for (const std::string& token : tokens) {
            if (isNumeric(token)) {
                output.push_back(token); // add numbers directly to the output
            }
            else if (isOperator(token)) {
                // pop operators with higher or equal precedence
                //check higher level operators are still in the code
                while (!operators.empty() && isOperator(operators.top()) && ((!isRightAssociative(token) && getPrecedence(token) <= getPrecedence(operators.top())) || (isRightAssociative(token) && getPrecedence(token) < getPrecedence(operators.top())))) {
                    output.push_back(operators.top()); //consume operators
                    operators.pop();
                }
                operators.push(token); // push the current operator onto the stack
            }
            else if (token == "(") {
                operators.push(token); // push opening parenthesis onto the stack
            }
            else if (token == ")") {
                // pop until an opening parenthesis is found
                while (!operators.empty() && operators.top() != "(") {
                    output.push_back(operators.top());
                    operators.pop();
                }
                if (operators.empty()) { //throw error if there is no matching parenthesis
                    throw std::invalid_argument("Mismatched parentheses");
                }
                operators.pop(); // discard the opening parenthesis
            }
        }
        // pop any remaining operators onto the output
        while (!operators.empty()) {
            if (operators.top() == "(") {
                throw std::invalid_argument("Mismatched parentheses");
            }
            output.push_back(operators.top()); //consume the operator
            operators.pop();
        }
        return output;
    }
    // evaluates the rpn expression
    //reverse polish notation
    double evaluateRPN(const std::vector<std::string>& rpn) {
        std::stack<double> values; //initialize vector
        for (const std::string& token : rpn) { 
            if (isNumeric(token)) {
                values.push(std::stod(token)); // convert token to number and push
            }
            else if (isOperator(token)) {//check for operators
                if (values.size() < 2) { //ensure no back to back operators
                    throw std::invalid_argument("Invalid expression");
                } 
                //consume characters
                double b = values.top();
                values.pop();
                double a = values.top();
                values.pop();
                // perform the operation based on the operator
                if (token == "+") { //addition
                    values.push(a + b);
                }
                else if (token == "-") { //subtraction
                    values.push(a - b);
                }
                else if (token == "*") { //multiplication
                    values.push(a * b);
                }
                else if (token == "/") { // division
                    if (b == 0) {
                        throw std::invalid_argument("Division by zero");
                    }
                    values.push(a / b);
                }
                else if (token == "%") { //modulo
                    values.push(std::fmod(a, b));
                }
                else if (token == "**") { //exponenet
                    values.push(std::pow(a, b));
                }
            }
        }
        //ensure the program contains 1 value per side of the operation
        if (values.size() != 1) {
            throw std::invalid_argument("Invalid expression");
        }
        return values.top(); // final result
    }
//public section
public:
    // evaluates a given expression string
    // input: recives desired expresion from the user
    // output: the final result
    double evaluate(const std::string& expression) {
        auto tokens = tokenize(expression); //tokenize the equation
        auto rpn = toRPN(tokens); //turn the equaation into postfix
        return evaluateRPN(rpn); //evalutate the equation
    }
};

//main function
int main() {
    ExpressionEvaluator evaluator; //create an instance of our class
    std::cout << "Enter 'END' to quit." << std::endl; //ask the user if they wish to exit the program
    while (true) {
        //program that should run
        try {
            //ask user for an expression
            std::cout << "Enter an expression: ";
            //define a varaible to store the expression
            std::string input;
            //store the expresion
            std::getline(std::cin, input);
            //check if the user wishes to exit the program
            if (input == "END") {
                break;
            }
            //calullate the result of the expression
            double result = evaluator.evaluate(input);
            //print the result
            std::cout << "Result: " << result << std::endl;
        }
        //ensure the program doesn't unexpectedly crash
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
    return 0;
}