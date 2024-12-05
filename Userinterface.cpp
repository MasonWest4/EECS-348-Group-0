#include <iostream>
#include <string>

class UserInterface {
public:
    // Function to display the welcome message
    void displayWelcomeMessage() {
        std::cout << "Welcome to the Arithmetic Calculator!\n";
        std::cout << "You can enter arithmetic expressions, and I'll help process them.\n";
        std::cout << "Type 'exit' to quit the program.\n";
    }

    // Function to get user input
    std::string getUserInput() {
        std::cout << "Enter your expression: ";
        std::string input;
        std::getline(std::cin, input);
        return input;
    }

    // Function to display the goodbye message
    void displayGoodbyeMessage() {
        std::cout << "Thank you for using the Arithmetic Calculator. Goodbye!\n";
    }
};

int main() {
    UserInterface ui;
    ui.displayWelcomeMessage();

    std::string input;
    while (true) {
        input = ui.getUserInput();
        if (input == "exit") {
            ui.displayGoodbyeMessage();
            break;
        }
        // Placeholder for handling the input
        std::cout << "You entered: " << input << "\n"; 
    }

    return 0;
}
