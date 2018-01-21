/*1. input CFG
    1.a) find non-Terminal
    1.b) find Terminal
    1.c) find production(rule), and also record from which non-Terminal, and rule number: use struct
    1.d) use non-Terminal & Terminal amount to declare 2 dim array(LLtable[][])
  2. Filltable
    2.a) initialize to 0
    2.b) set rule
          i.) find every non-Terminal's First set and save it to production?
         ii.) verify non-T will derive lamda
        iii.) if(derive lamda's rule) find Follow set
         iv.) fill table
          v.) let every rule's First set U Follow set
         vi.) find every rule's predict set
    < 2.c) print table >
  3. input inputString
    < 3.a) print string >
    3.b) slice token
    3.c) give token string's first token a pointer(*PEEK?)
  4. enter LLPARSER(ts)
    4.a) let 1st rule's LHS push to stack
    4.b)    boolean accepted = false
            while(!accepted) {
                if(isTerminal(TOS())) {
                    MATCH(ts,TOS())
                }
            }

    4.c)    boolean isTerminal(token) {
                boolean checkbit = false;
                while(Terminal set not yet finish) {
                    if(token == Terminal) {
                        checkbit = true;
                        break;
                    }
                }
                return checkbit;
            }

            void MATCH(ts, TOS()) {
                if(*PEEK == TOS())
                    ADVANCE();
                else
                    ERROR(TOS());
            }

            void ADVANCE() {
                PEEK = PEEK + 1;
            }

            void ERROR(token) {
                std::cout << "Expected" + token << std::endl;
            }
*/

#include <iostream>
#include <fstream> // ==> for file in/out
#include <vector> // ==> for tokenSplie
#include <sstream>
#include <stack> // ==> for finding non-T

/* tokenSplit start */
template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while( getline(ss, item, delim) ) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}
/* tokenSplit end */

/* find terminal in RHS */
bool f_isTerminal(std::string token) {
    char ch = token[0];
    if( !( ch <= 'Z' && ch >= 'A') ) {
        return true;
    }
    return false;
}

/* Rule struct */
struct


int main(int argc, char* argv[]) {
    std::ifstream myfile;
    char* CFGfile = argv[1];
    myfile.open(CFGfile);
    if(myfile.is_open()) {
		std::cout << "CFG which is used this time: " << CFGfile << '\n' << std::endl;
		std::string line;
        while( std::getline( myfile, line ) ) {
            std::cout << line << '\n';
        }
        
        myfile.clear(); // ==> help my restart at CFGfile beginning
        myfile.seekg(0, std::ios::beg);

        std::cout << "\n-----------------------------\n" << std::endl;
        
        /*parse CFG*/

        // ==> write token into file
        std::ofstream tokenFile;
        tokenFile.open("token.txt");
        if( tokenFile.is_open() ) {
            while( std::getline( myfile, line ) ) {
                std::vector<std::string> token = split(line, ' ');
                for(unsigned int i = 0; i < token.size(); i++) {
                    tokenFile << token[i] << std::endl;
                }
            }
        } else { std::cout << "token.txt failed to generate!" << std::endl; }
        myfile.close(); ///close CFG file
        tokenFile.close();
        // ==> read token.txt if the next stack contains ">" and then store last one as non-Terminal
        std::ifstream itokenFile;
        itokenFile.open("token.txt");
        if( itokenFile.is_open() ) {
            std::stack<std::string> myStack;
            // 1.a)
            int startSymbolPos = 1, count = 0;
            while( std::getline( itokenFile, line ) ) {
                myStack.push(line);
                if( count == startSymbolPos ) {
                    std::cout << "+++++++++StartSymbol: " << myStack.top() << std::endl;
                }
                if(myStack.top() == ">") {
                    std::cout << "non-T: ";
                    myStack.pop(); ///pop ">"
                    std::cout << myStack.top() << std::endl;
                }
                count++;
            }
            while( !myStack.empty() ) { ///clean the stack
                myStack.pop();
            }
            itokenFile.clear();
            itokenFile.seekg(0, std::ios::beg);
            //1.b)
            bool isRHS = false;
            while( std::getline( itokenFile, line ) ) {
                myStack.push(line);
                if( myStack.top() == ">" || myStack.top() == "|") {
                    isRHS = true;
                }
                else if( isRHS ) {
                    if( myStack.top() >= "0" && myStack.top() <= "9" ) {
                        isRHS = false;
                    }
                    else if( f_isTerminal( myStack.top() ) ){/// token isn't the rule number
                            std::cout << myStack.top() << std::endl;
                    }

                }
            }
            while(!myStack.empty()) {
                myStack.pop();
            }



        }
		




		itokenFile.close();
		return 0;
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
