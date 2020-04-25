#include <iostream>
#include <stack>
#include <string>

struct Bracket {
    Bracket(char type, int position):
        type(type),
        position(position)
    {}

    bool Matchc(char c) {
        if (type == '[' && c == ']')
            return true;
        if (type == '{' && c == '}')
            return true;
        if (type == '(' && c == ')')
            return true;
        return false;
    }

    char type;
    int position;
};

int main() {
    std::string text;
    getline(std::cin, text);

    std::stack <Bracket> opening_brackets_stack;
    for (int position = 0; position < text.length(); ++position) {
        char next = text[position];

        if (next == '(' || next == '[' || next == '{') {
            Bracket openBracket = Bracket(next, position);
            opening_brackets_stack.push(openBracket);
        } else if (next == ')' || next == ']' || next == '}') {
            if(opening_brackets_stack.empty()) {
                Bracket closeBracket = Bracket(next, position);
                opening_brackets_stack.push(closeBracket);
                break;
            };

            Bracket topOpenBracket = opening_brackets_stack.top();
            opening_brackets_stack.pop();

            if( (topOpenBracket.type == '(' && next != ')') || 
                (topOpenBracket.type == '[' && next != ']') ||
                (topOpenBracket.type == '{' && next != '}') ) 
            {
                Bracket closeBracket = Bracket(next, position);
                opening_brackets_stack.push(closeBracket);
                break;
            }
            
        }
    }

    if (opening_brackets_stack.empty()) {
        std::cout << "Success";
    } else {
        Bracket brokenBracket = opening_brackets_stack.top();
        std::cout << brokenBracket.position + 1;
    }

    return 0;
}
