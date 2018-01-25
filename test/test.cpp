#include <iostream>
#include <set>

std::set<std::string> worklist;

int main() {
	worklist.insert("C");
	worklist.insert("A");
	worklist.insert("B");
	worklist.insert("Q");

	while(!worklist.empty()){
		std::set<std::string>::iterator it = worklist.begin();
		std::cout << *it << std::endl;
		std::string t = *it;
		worklist.erase(it);
		std::cout << t << std::endl;
	}
}





/*

#include <iostream>

#include <vector>

int main() {
	// Format: vector<DataType> nameOfVector
	// myVector.push_back(value) ==> adds an element to the END of the vector (also resizes it)
	// myVector.at(index) ==> return element at specified index number
	// myVector.size() ==> returns an unsigned int equal top the number of elements
	// myVector.begin() ==> reads vector from first element (index 0)
	// myVector.insert(myVector.begin() + integer, new value) ==> adds element BEFORE specified index number
	// myVector.erase(myVector.begin() + integer) ==> removes element AT specified index number
	// myVector.clear ==> removes all elements in vector
	// myVector.empty() ==> returns boolean value if whether vector is empty
	
	std::vector<int> myVector;
	std::vector<std::string> myVector2;
	
	myVector.push_back(3);
	myVector.push_back(7);
	myVector.push_back(4);
	myVector.push_back(12);
	myVector.push_back(9);
	
	myVector2.push_back("1 Start > E $");
	myVector2.push_back("2 E > Prefix ( E )");
	
	
	std::cout << "Vector: ";
	
	for(unsigned int i = 0; i < myVector.size(); i++) {
		std::cout << myVector[i] << " ";
	}
	
	std::cout << std::endl;
	
	for(unsigned int i = 0; i < myVector2.size(); i++) {
		std::cout << myVector2[i] << " " << std::endl;
	}
	//myVector.clear();
	
	if (myVector.empty()) {
		std::cout << std::endl << "Is Empty!";
	}
	else {
		std::cout << std::endl << "Is not empty!";
	}
	std::cout << std::endl;



	myVector2.erase(myVector2.begin());

	for(unsigned int i = 0; i < myVector2.size(); i++) {
		std::cout << myVector2[i] << " " << std::endl;
	}

	
	return 0;
	
	 
}
*/
