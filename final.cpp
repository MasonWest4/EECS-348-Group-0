#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <map>

class ExpressionEvaluator {
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
        for (char c : token) {
            if (!std::isdigit(c) && c != '.') {
                return false;
            }
        }
        return !token.empty();
    }

    // splits the input expression into tokens 
    std::vector<std::string> tokenize(const std::string& expression) {
        std::vector<std::string> tokens;
        std::string token;
        bool expectUnary = true; // true if the next operator could be unary

        for (size_t i = 0; i < expression.length(); ++i) {

            char c = expression[i];
            if (std::isspace(c)) {
                continue; // skip spaces
            }
            if (std::isdigit(c) || c == '.') {
                token += c; // accumulate digits or decimal points into the current token
                expectUnary = false;
            } else {
                if (!token.empty()) {
                    tokens.push_back(token); // push the completed numeric token
                    token.clear();
                }
                if (c == '*' && i + 1 < expression.length() && expression[i + 1] == '*') {
                    // handle ** as a single operator
                    tokens.push_back("**");
                    ++i; // skip the second *
                    expectUnary = true;
                } else if (c == '(') {
                    tokens.push_back("("); // push an opening parenthesis
                    expectUnary = true; // next operator can be unary

                } else if (c == ')') {
                    tokens.push_back(")"); // push a closing parenthesis
                    expectUnary = false; // next operator cannot be unary
                } else if (std::string("+-*/%").find(c) != std::string::npos) {
                    if (expectUnary && (c == '+' || c == '-')) {
                        tokens.push_back("0"); // convert unary to binary by adding a 0
                    }
                    tokens.push_back(std::string(1, c)); // push the operator
                    expectUnary = true; // next operator can be unary
                } else {
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
        std::vector<std::string> output;
        std::stack<std::string> operators;

        for (const std::string& token : tokens) {
            if (isNumeric(token)) {
                output.push_back(token); // add numbers directly to the output
            } else if (isOperator(token)) {
                // pop operators with higher or equal precedence
                while (!operators.empty() && isOperator(operators.top()) &&
                       ((!isRightAssociative(token) && getPrecedence(token) <= getPrecedence(operators.top())) ||
                        (isRightAssociative(token) && getPrecedence(token) < getPrecedence(operators.top())))) {
                    output.push_back(operators.top());
                    operators.pop();
                }
                operators.push(token); // push the current operator onto the stack
            } else if (token == "(") {
                operators.push(token); // push opening parenthesis onto the stack
            } else if (token == ")") {
                // pop until an opening parenthesis is found
                while (!operators.empty() && operators.top() != "(") {
                    output.push_back(operators.top());
                    operators.pop();
                }
                if (operators.empty()) {
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
            output.push_back(operators.top());
            operators.pop();
        }

        return output;
    }

    // evaluates the rpn expression
    double evaluateRPN(const std::vector<std::string>& rpn) {
        std::stack<double> values;

        for (const std::string& token : rpn) {
            if (isNumeric(token)) {
                values.push(std::stod(token)); // convert token to number and push
            } else if (isOperator(token)) {
                if (values.size() < 2) {
                    throw std::invalid_argument("Invalid expression");
                }
                double b = values.top();
                values.pop();
                double a = values.top();
                values.pop();

                // perform the operation based on the operator
                if (token == "+") {
                    values.push(a + b);
                } else if (token == "-") {
                    values.push(a - b);
                } else if (token == "*") {
                    values.push(a * b);
                } else if (token == "/") {
                    if (b == 0) {
                        throw std::invalid_argument("Division by zero");
                    }
                    values.push(a / b);
                } else if (token == "%") {
                    values.push(std::fmod(a, b));
                } else if (token == "**") {
                    values.push(std::pow(a, b));
                }
            }
        }

        if (values.size() != 1) {
            throw std::invalid_argument("Invalid expression");
        }

        return values.top(); // final result
    }

public:
    // evaluates a given expression string
    double evaluate(const std::string& expression) {
        auto tokens = tokenize(expression); 
        auto rpn = toRPN(tokens); 
        return evaluateRPN(rpn); 
    }
};

int main() {
    ExpressionEvaluator evaluator;
    std::cout << "Enter 'END' to quit." << std::endl;

    while (true) {
        try {
            std::cout << "Enter an expression: ";
            std::string input;
            std::getline(std::cin, input);
            if (input == "END") {
                break;
            }
            double result = evaluator.evaluate(input);
            std::cout << "Result: " << result << std::endl;
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    return 0;
}
