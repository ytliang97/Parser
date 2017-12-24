#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    std::ifstream myfile;
    char* filename = argv[1]; 
    myfile.open(filename);
    if(myfile.is_open()) {
        std::string line;
        while(getline(myfile, line)) {
            std::cout << line << std::endl;                                                                      
        }   
        myfile.close();
    }else {
        std::cout << "unable to open file" << std::endl;
    }   

    return 0;
}