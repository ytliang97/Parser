#include <iostream>
#include <sstream>
#include <vector>

/* when use vector, you need to include <vector> */
/* template does not need to include library */

template<typename Out>
void split(const std::string &s, char delim, Out result) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

int main() {
	std::vector<std::string> x = split("one:two::three", ':');
	for(unsigned int i = 0; i < x.size(); i++)
		std::cout << x[i] << std::endl;
	return 0;
}