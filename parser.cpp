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
#include <iterator> // ==> for printing Set
#include <set>

/* tokenSplit start */
template<typename Out>
void f_split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while( getline(ss, item, delim) ) {
        *(result++) = item;
    }
}

std::vector<std::string> f_split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    f_split(s, delim, std::back_inserter(elems));
    return elems;
}
/* tokenSplit end */

std::string startSymbolPos;
std::string nowLHS;

//== table parameter 
int tableRow = 0;
int tableCol = 0;

//== find terminal in RHS 
std::set<std::string> terminalSet;
//== find nonTerminal in LHS
std::set<std::string> nonTerminalSet;

bool f_isTerminal(std::string token) {
    char ch = token[0];

    if( !( ch <= 'Z' && ch >= 'A') && token != "lamda" ) {
        return true;
    }
    return false;
}
void f_terminal(std::string s) {
    if( f_isTerminal( s ) ) {
        tableCol++;
        terminalSet.insert( s );
    }
}
/* find nonTerminal */
bool f_isNonTerminal(std::string s) {
    if(s != "|") { 
        tableRow++;
        return true;
    }
    else false;
}



/* Rule struct */
//struct
struct Rule {
    int ruleNumber;
    bool symbolDerivesEmpty;
    bool ruleDerivesEmpty;
    std::vector<std::string> followSet;
    std::set<std::string> firstSet;
    std::vector<std::string> RHS;
    bool visitedFirst;
    std::string LHS;
    Rule *next;
};

void f_checkForEmpty(Rule *pr) {
    if(pr->RHS.size() == 0 ) {
        pr->ruleDerivesEmpty = true;
        if( !( pr->symbolDerivesEmpty ) ) {
            pr->symbolDerivesEmpty = true;
            /*worklist*/
        }
    }
}
void f_derivesEmptyString(Rule *pr) { // ==> use it after RHS_push_back
    pr->symbolDerivesEmpty = false;
    pr->ruleDerivesEmpty = false;
    f_checkForEmpty(pr);
}


int f_stoi(std::string s) {
    std::stringstream sso;
    sso << s;
    int it;
    sso >> it;
    return it;
}

void f_production(std::string line, Rule *current) {
    
    current->next = new Rule;
    current = current->next;
    
    std::vector<std::string> token = f_split( line, ' ' );
    // 1.c)
    current->ruleNumber = f_stoi(token[0]);
    // 1.a) 
    if( f_isNonTerminal( token[1] ) ) {
        current->LHS = token[1];
        nowLHS = token[1];
        std::cout << current->LHS << std::endl;
        // 1.b)
        for(unsigned int i = 3; i < token.size(); i++ ) {
            current->RHS.push_back( token[i] );
            f_terminal( token[i] );
        }

    }
    else {
        for(unsigned int i = 2; i < token.size(); i++ ) {
            current->RHS.push_back( token[i] );
            f_terminal( token[i] );
        }
        current->LHS = nowLHS;
        std::cout << current->LHS << std::endl;
    }
    f_derivesEmptyString(current);
    current->next = NULL;
}

/* special for fisrt production */
void f_fProduction(std::string line, Rule *rhead) {
    
    std::vector<std::string> token = f_split( line, ' ' );
    // 1.c)
    rhead->ruleNumber = f_stoi(token[0]);
    if( f_stoi(token[0]) == 1 ) { /* find Start Symbol */ 
        startSymbolPos = token[1];
    }
    // 1.a) 
    if( f_isNonTerminal( token[1] ) ) {
        rhead->LHS = token[1];
        std::cout << rhead->LHS << std::endl;
        // 1.b)
        for(unsigned int i = 3; i < token.size(); i++ ) {
            rhead->RHS.push_back( token[i] );
            f_terminal( token[i] );
        }

    }
    else {
        for(unsigned int i = 2; i < token.size(); i++ ) {
            rhead->RHS.push_back( token[i] );
            f_terminal( token[i] );
        }
        rhead->LHS = "trytry";
        std::cout << rhead->LHS << std::endl;
    }
    f_derivesEmptyString(rhead);
}

void f_printGrammer(char* CFGfile) {
    std::ifstream myfile;
    myfile.open(CFGfile);
    if(myfile.is_open()) {
        std::cout << "CFG which is used this time: " << CFGfile << '\n' << std::endl;
        std::string line;
        while( std::getline( myfile, line ) ) {
            std::cout << line << '\n';
        }
        
        myfile.close();

        std::cout << "\n-----------------------------\n" << std::endl;
    }else {
        std::cout << "unable to open CFG file" << std::endl;
    }

}

int main(int argc, char* argv[]) {
    
    char* grammer = argv[1];
    f_printGrammer(grammer);

        /*parse CFG*/

        // ==> write token into file
/*        std::ofstream tokenFile;
        tokenFile.open("token.txt");
        if( tokenFile.is_open() ) {
            while( std::getline( myfile, line ) ) {
                std::vector<std::string> token = split(line, ' ');
                for(unsigned int i = 0; i < token.size(); i++) {
                    tokenFile << token[i] << std::endl;
                }
            }
        } else { std::cout << "token.txt failed to generate!" << std::endl; }
*/      

        Rule *rhead = new Rule;


        std::ifstream myfile;
        myfile.open(grammer);
		std::string line;
		
		
        std::getline( myfile, line);
        f_fProduction(line, rhead);

        Rule *current = rhead;
        while( std::getline( myfile, line ) ) {
            current->next = new Rule;
            f_production(line, current);
            current = current->next;
        }
        for(std::set<std::string>::iterator iter = terminalSet.begin(); iter != terminalSet.end(); ++iter ) {
                std::cout << *iter << std::endl;
            }
        myfile.close(); ///close CFG file

        // 2.
        std::string LLtable[tableRow+1][tableCol+1];
        // 2.a)
        for(int i = 0; i <= tableRow; i++ ) 
            for(int j = 0; j <= tableCol; j++ )
                LLtable[i][j] = " ";
        for(std::set<std::string>::iterator iter = terminalSet.begin(); iter != terminalSet.end(); ++iter ) {
            int i = 1;
            LLtable[0][i] = *iter;
            i++;
        }
        for(int i = 0; i <= tableRow; i++ ) 
            for(int j = 0; j <= tableCol; j++ )
                std::cout << LLtable[i][j] << " ";
            std::cout << "\n";


        return 0;

//        tokenFile.close();
        // ==> read token.txt if the next stack contains ">" and then store last one as non-Terminal
/*        std::ifstream itokenFile;
        itokenFile.open("token.txt");
        if( itokenFile.is_open() ) {
            std::stack<std::string> myStack;
            // 1.a)
            
            while( std::getline( itokenFile, line ) ) {
                myStack.push(line);
                if( f_stoi(token[0]) == 1 ) {
                    std::cout << "+++++++++StartSymbol: " << myStack.top() << std::endl;
                }
                if(myStack.top() == ">") {
                    std::cout << "non-T: ";
                    myStack.pop(); ///pop ">"
                    std::cout << myStack.top() << std::endl;
                }
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
    

    return 0;
}
