#include <iostream>
#include <stack>

int main(){
	std::stack<std::string> myStack;

	myStack.push("0");
	myStack.push("3");
	myStack.push("1");

	std::cout << "Number of ints on the stack " << myStack.size() << std::endl;

	while( !myStack.empty() ) {
		std::cout << "popping " << myStack.top() << std::endl;
		myStack.pop();
	}

}