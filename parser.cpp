#include <iostream>                                                                                              
#include <fstream>

int main(int argc, char* argv[]) {
    std::ifstream myfile;
    char* CFGfile = argv[1];
    myfile.open(CFGfile);
    if(myfile.is_open()) {
        /*parse CFG*/






        /*end of parse CFG*/
        int parseOK = 1;
        if(parseOK) {
            std::cout << "CFG is ready" << std::endl;
            /*print parsing table*/
            std::cout << "parsing table" << std::endl;





            /*end of print parsing table*/
            std::ifstream file;
            char* testFile = argv[2];
            file.open(testFile);
            if(file.is_open()) {
                std::string tokenString;
                while(getline(file, tokenString)) {
                    /*run every tokenString in testFile*/




                    /*end of run every tokenString in testFile*/
                    std::cout << tokenString + "apply rule" << std::endl;
                }
                                            }else {
                std::cout << "unable to open testFile" << std::endl;
            }
        }else {
            std::cout << "parsing error" << std::endl;
        }
        myfile.close();
    }else {
        std::cout << "unable to open CFG file" << std::endl;
    }

    return 0;
}