#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <sstream>

//== table parameter 
int tableRow = 0;
int tableCol = 0;


//== find terminal in RHS 
std::set<std::string> terminalSet;
//== find nonTerminal in LHS
std::set<std::string> nonTerminalSet;
//== set for check empty
std::set<std::string> worklist;

struct Rule {
    int ruleNumber;
    bool symbolDerivesEmpty;
    bool ruleDerivesEmpty;
    std::set<std::string> followSet;
    std::set<std::string> firstSet;
    std::set<std::string> predictSet;
    std::vector<std::string> RHS;
    bool visitedFirst;
    std::string LHS;
    Rule *next;
    int count;
};
Rule *rhead;


std::string startSymbolPos;
std::string nowLHS;

//== tokenSplit start 
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
//== tokenSplit end


bool f_isTerminal(std::string token) {
    char ch = token[0];
    if( !( ch <= 'Z' && ch >= 'A') && token != "lamda" ) {
        return true;
    }
    return false;
}
void f_collectTerminal(std::string s) {
    if( f_isTerminal( s ) ) {
        terminalSet.insert( s );
    }
}
//== find nonTerminal 
bool f_isNonTerminal(std::string token) {
    char ch = token[0];
    if(token != "|" && ( token[0] >= 'A' && token[0] <= 'Z' ) ) { 
        return true;
    }
    else false;
}

void f_collectNonTerminal(std::string s) {
	if( f_isNonTerminal( s ) ) {
		nonTerminalSet.insert( s );
	}
}

void f_checkForEmpty(Rule *pr) {
    if(pr->count == 0 ) {
        pr->ruleDerivesEmpty = true;
        if( !( pr->symbolDerivesEmpty ) ) {
            pr->symbolDerivesEmpty = true;
            worklist.insert(pr->LHS);
        }
    }
}
void f_derivesEmptyString() { // ==> use it after RHS_push_back
    Rule *current = rhead;
    while(true) {
//		std::cout << "2" << std::endl;
		current->symbolDerivesEmpty = false;
    	current->ruleDerivesEmpty = false;
    	current->count = current->RHS.size();
    	f_checkForEmpty(current);//no while loop
    	current = current->next;
    	if( current == NULL ) break;
    }
 //   while(!worklist.empty()) {
 // 	
 // }


    

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
    	f_collectNonTerminal( token[1] );
        current->LHS = token[1];
        std::cout << current->LHS << std::endl;
        // 1.b)
        for(unsigned int i = 3; i < token.size(); i++ ) {
        	if (token[i] == "lamda") continue;
            current->RHS.push_back( token[i] );
            f_collectTerminal( token[i] );
        }

    }
    else {
        for(unsigned int i = 2; i < token.size(); i++ ) {
        	if (token[i] == "lamda") continue;
            current->RHS.push_back( token[i] );
            f_collectTerminal( token[i] );
        }
        current->LHS = nowLHS;
        std::cout << current->LHS << std::endl;
    }
	current->next = NULL;
}
void f_fProduction(std::string line, Rule *rhead) {
	
	std::vector<std::string> token = f_split( line, ' ' );
    // 1.c)
    rhead->ruleNumber = f_stoi(token[0]);
    if( f_stoi(token[0]) == 1 ) { // ==> find Start Symbol 
        startSymbolPos = token[1];
    }
    // 1.a) 
    if( f_isNonTerminal( token[1] ) ) {
    	f_collectNonTerminal( token[1] );
        rhead->LHS = token[1];
        std::cout << rhead->LHS << std::endl;
        // 1.b)
        for(unsigned int i = 3; i < token.size(); i++ ) {
        	if (token[i] == "lamda") continue;
            rhead->RHS.push_back( token[i] );
            f_collectTerminal( token[i] );
        }

    }
    else {
        for(unsigned int i = 2; i < token.size(); i++ ) {
            if (token[i] == "lamda") continue;
            rhead->RHS.push_back( token[i] );
            f_collectTerminal( token[i] );
        }
        rhead->LHS = nowLHS;
    	std::cout << rhead->LHS << std::endl;
    }
}



std::string f_internalFirst( Rule* current, std::vector<std::string> RHS ) {
	if(RHS.empty()) return {};
	std::vector<std::string>::iterator iter = RHS.begin();
	if( f_isTerminal( *iter ) ) {
		return *iter;
	}
	if( !current->visitedFirst ) {
		current->visitedFirst = true;
		Rule* pr = rhead;
		while(true) {
			std::cout << "6" << std::endl;
			if( *iter == pr->LHS ) {
				pr->firstSet.insert( f_internalFirst( pr, pr->RHS )) ;
			}
			pr = pr->next;
			if( pr == NULL ) break;
		}
	}
	if( current->symbolDerivesEmpty ) {
		std::vector<std::string> beta;
		for(unsigned int i = 1; i < current->RHS.size(); i++ ) {
			beta.push_back(current->RHS[i]);
		}
		current->firstSet.insert(f_internalFirst(current, beta ) );
	}
}
void f_findFirst(int rNum) {
	Rule* current = rhead;
	while(true) {
		std::cout << "4" << std::endl;
		current->visitedFirst = false;
		current = current->next;
		if( current == NULL ) break;
	}
	current = rhead;
	while(true) {
		std::cout << "5" << std::endl;
		if( current->ruleNumber == rNum ) break;
		current = current->next;
		if( current == NULL ) break;
	}
	current->firstSet.insert( f_internalFirst( current, current->RHS ) );
//	r->firstSet.insert();

}
void f_findPredict(Rule* r) {
	f_findFirst( r->ruleNumber );
///	if(r->ruleDerivesEmpty)
///		f_follow();
}











int main(int argc, char* argv[]) {
	std::ifstream myfile;
    char* CFGfile = argv[1];
    myfile.open(CFGfile);
    if(myfile.is_open()) {} else { std::cout << "can't open file" << std::endl; return 0;}

    std::string line;
	rhead = new Rule;

	std::getline( myfile, line );
	f_fProduction(line, rhead);//no while loop
    
	Rule *current = rhead;
	while( std::getline( myfile, line ) ) {
//		std::cout << "1" << std::endl;
		current->next = new Rule;
		f_production(line, current);//no while loop
		current = current->next;
		
	}
	
	f_derivesEmptyString();
	
	//==> find Predict set (includ find first set and follow set)
	current = rhead;	
	while(true) {
//		std::cout << "3" << std::endl;
		f_findPredict(current);
		current = current->next;
		if(current == NULL) break;
	}
	
	current = rhead;
	while(true) {
		for(std::set<std::string>::iterator iter = current->firstSet.begin(); iter != current->firstSet.end(); ++iter ) {
			std::cout << *iter << " ";
		}
		std::cout << std::endl;
		current = current->next;
		if(current == NULL ) break;
	}
	
	return 0;
}


/* terminalset nonterminalSet
#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <sstream>

//== table parameter 
int tableRow = 0;
int tableCol = 0;

//== find terminal in RHS 
std::set<std::string> terminalSet;
//== find nonTerminal in LHS
std::set<std::string> nonTerminalSet;

struct Rule {
    int ruleNumber;
    bool symbolDerivesEmpty;
    bool ruleDerivesEmpty;
    std::set<std::string> followSet;
    std::set<std::string> firstSet;
    std::set<std::string> predictSet;
    std::vector<std::string> RHS;
    bool visitedFirst;
    std::string LHS;
    Rule *next;
};

std::string startSymbolPos;
std::string nowLHS;

//== tokenSplit start 
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
//== tokenSplit end


bool f_isTerminal(std::string token) {
    char ch = token[0];
    if( !( ch <= 'Z' && ch >= 'A') && token != "lamda" ) {
        return true;
    }
    return false;
}
void f_collectTerminal(std::string s) {
    if( f_isTerminal( s ) ) {
        terminalSet.insert( s );
    }
}
//== find nonTerminal 
bool f_isNonTerminal(std::string token) {
    char ch = token[0];
    if(token != "|" && ( token[0] >= 'A' && token[0] <= 'Z' ) ) { 
        return true;
    }
    else false;
}

void f_collectNonTerminal(std::string s) {
	if( f_isNonTerminal( s ) ) {
		nonTerminalSet.insert( s );
	}
}

void f_checkForEmpty(Rule *pr) {
    if(pr->RHS.size() == 0 ) {
        pr->ruleDerivesEmpty = true;
        if( !( pr->symbolDerivesEmpty ) ) {
            pr->symbolDerivesEmpty = true;
            //*worklist
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
    	f_collectNonTerminal( token[1] );
        current->LHS = token[1];
        std::cout << current->LHS << std::endl;
        // 1.b)
        for(unsigned int i = 3; i < token.size(); i++ ) {
        	if (token[i] == "lamda") continue;
            current->RHS.push_back( token[i] );
            f_collectTerminal( token[i] );
        }

    }
    else {
        for(unsigned int i = 2; i < token.size(); i++ ) {
        	if (token[i] == "lamda") continue;
            current->RHS.push_back( token[i] );
            f_collectTerminal( token[i] );
        }
        current->LHS = nowLHS;
        std::cout << current->LHS << std::endl;
    }
    f_derivesEmptyString(current);
	current->next = NULL;
}
void f_fProduction(std::string line, Rule *rhead) {
	
	std::vector<std::string> token = f_split( line, ' ' );
    // 1.c)
    rhead->ruleNumber = f_stoi(token[0]);
    if( f_stoi(token[0]) == 1 ) { // ==> find Start Symbol 
        startSymbolPos = token[1];
    }
    // 1.a) 
    if( f_isNonTerminal( token[1] ) ) {
    	f_collectNonTerminal( token[1] );
        rhead->LHS = token[1];
        std::cout << rhead->LHS << std::endl;
        // 1.b)
        for(unsigned int i = 3; i < token.size(); i++ ) {
        	if (token[i] == "lamda") continue;
            rhead->RHS.push_back( token[i] );
            f_collectTerminal( token[i] );
        }

    }
    else {
        for(unsigned int i = 2; i < token.size(); i++ ) {
            if (token[i] == "lamda") continue;
            rhead->RHS.push_back( token[i] );
            f_collectTerminal( token[i] );
        }
        rhead->LHS = nowLHS;
    	std::cout << rhead->LHS << std::endl;
    }
    f_derivesEmptyString(rhead);
}
int main(int argc, char* argv[]) {
	std::ifstream myfile;
    char* CFGfile = argv[1];
    myfile.open(CFGfile);
    if(myfile.is_open()) {} else { std::cout << "can't open file" << std::endl; return 0;}

    std::string line;
	Rule *rhead = new Rule;

	std::getline( myfile, line );
	f_fProduction(line, rhead);
    
	Rule *current = rhead;
	while( std::getline( myfile, line ) ) {

		current->next = new Rule;
		f_production(line, current);
		current = current->next;		
	}
	



	
	int go = 1;
	while(go) {
		int input;
		std::cin >> input;
		if(input>0) {
			current = rhead;
			while(true) {
				if(current->ruleNumber == input) {
					std::cout << current->LHS << " ";
					for( unsigned int i = 0; i < current->RHS.size() ; i++ ) {
						std::cout << current->RHS[i] << " ";
					}
					if(current->ruleDerivesEmpty) std::cout << "rule will derive empty" << " ";
					if(current->symbolDerivesEmpty) std::cout << "symbol will derive empty" << " ";
					std::cout << std::endl;
				}
				current = current->next;	
				if(current == NULL) break;
			}
		}
		else if(input == -1) {
			for(std::set<std::string>::iterator iter = terminalSet.begin(); iter != terminalSet.end(); ++iter ) {
				std::cout << *iter << ", ";
			}
			std::cout << std::endl;
		}
		else if (input == -2 ) {
			for(std::set<std::string>::iterator iter = nonTerminalSet.begin(); iter != nonTerminalSet.end() ; ++iter ) {
				std::cout << *iter << ", ";
			}
			std::cout << std::endl;
		}

		else break;
	}	

}
*/

	

/* rule has same non-Terminal
#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <sstream>

/* table parameter */
/*int tableRow = 0;
int tableCol = 0;

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
	
std::string startSymbolPos;
std::string nowLHS;
*/
/* tokenSplit start */
/*template<typename Out>
void f_split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::	string item;
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

/* find terminal in RHS */
/*std::set<std::string> terminalSet;

bool f_isTerminal(std::string token) {
    char ch = token[0];

    if( !( ch <= 'Z' && ch >= 'A') && token != "lamda" ) {
        return true;
    }
    return false;
}
void f_collectTerminal(std::string s) {
    if( f_isTerminal( s ) ) {
        tableCol++;
        terminalSet.insert( s );
    }
}
/* find nonTerminal */
/*bool f_isNonTerminal(std::string s) {
    if(s != "|") { 
        tableRow++;
        return true;
    }
    else false;
}

void f_checkForEmpty(Rule *pr) {
    if(pr->RHS.size() == 0 ) {
        pr->ruleDerivesEmpty = true;
        if( !( pr->symbolDerivesEmpty ) ) {
            pr->symbolDerivesEmpty = true;
            /*worklist*/
/*        }
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
        std::cout << current->LHS << std::endl;
        // 1.b)
        for(unsigned int i = 3; i < token.size(); i++ ) {
        	if (token[i] == "lamda") continue;
            current->RHS.push_back( token[i] );
            f_collectTerminal( token[i] );
        }

    }
    else {
        for(unsigned int i = 2; i < token.size(); i++ ) {
        	if (token[i] == "lamda") continue;
            current->RHS.push_back( token[i] );
            f_collectTerminal( token[i] );
        }
        current->LHS = nowLHS;
        std::cout << current->LHS << std::endl;
    }
    f_derivesEmptyString(current);
	current->next = NULL;
}

void f_fProduction(std::string line, Rule *rhead) {
	
	std::vector<std::string> token = f_split( line, ' ' );
    // 1.c)
    rhead->ruleNumber = f_stoi(token[0]);
    if( f_stoi(token[0]) == 1 ) { /* find Start Symbol */ 
/*      startSymbolPos = token[1];
    }
    // 1.a) 
    if( f_isNonTerminal( token[1] ) ) {
        rhead->LHS = token[1];
        std::cout << rhead->LHS << std::endl;
        // 1.b)
        for(unsigned int i = 3; i < token.size(); i++ ) {
        	if (token[i] == "lamda") continue;
            rhead->RHS.push_back( token[i] );
 	           f_collectTerminal( token[i] );
        }

    }
    else {
        for(unsigned int i = 2; i < token.size(); i++ ) {
            if (token[i] == "lamda") continue;
            rhead->RHS.push_back( token[i] );
            f_collectTerminal( token[i] );
        }
        rhead->LHS = nowLHS;
    	std::cout << rhead->LHS << std::endl;
    }
    f_derivesEmptyString(rhead);
}

int main(int argc, char* argv[]) {
	std::ifstream myfile;
    char* CFGfile = argv[1];
    myfile.open(CFGfile);
    if(myfile.is_open()) {} else { std::cout << "can't open file" << std::endl; return 0;}

    std::string line;
	Rule *rhead = new Rule;

	std::getline( myfile, line );
	f_fProduction(line, rhead);
    
	Rule *current = rhead;
	while( std::getline( myfile, line ) ) {

		current->next = new Rule;
		f_production(line, current);
		current = current->next;		
		
	}
	
	
	int go = 1;
	while(go) {
		int input;
		std::cin >> input;
		if(input!=0) {
			current = rhead;
			while(true) {
				if(current->ruleNumber == input) {
					std::cout << current->LHS << " ";
					for( unsigned int i = 0; i < current->RHS.size() ; i++ ) {
						std::cout << current->RHS[i] << " ";
					}
					if(current->ruleDerivesEmpty) std::cout << "rule will derive empty" << " ";
					if(current->symbolDerivesEmpty) std::cout << "symbol will derive empty" << " ";
					std::cout << std::endl;
				}
				current = current->next;	
				if(current == NULL) break;
			}
		}
		else break;	
	}	

}
*/

/* Rule as linked list
#include <iostream>
#include <set>	
#include <vector>

/* Rule struct */
//struct
/*struct Rule {
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

/* nonTerminal struct */

/*int main() {
	Rule *rhead = new Rule;
	bool cfgNotEnd = true;
	rhead->ruleNumber = 1;
	rhead->firstSet.insert("q");
	rhead->RHS.push_back("E");
	rhead->RHS.push_back("$");	
	rhead->LHS = "Start";
	rhead->next = NULL;

	Rule *current = rhead;
	/*while(cfgNotEnd) {
		current->next = new Rule;

		current = current->next;	

		current->next = NULL;
	}*/
/*	current->next = new Rule;

	current = current->next;

	current->ruleNumber = 2;
	current->firstSet.insert("c");
	current->RHS.push_back("Prefix");
	current->RHS.push_back("(");
	current->RHS.push_back("E");
	current->RHS.push_back(")");
	current->LHS = "Start";


	current->next = NULL;



	current = rhead;
	while(true) {
		if(current->LHS == "Start" ) {
			std::cout << current->ruleNumber << std::endl;
		}
		current = current->next;
		if( current == NULL ) break;
	}

	return 0;

}



/* test table
#include <iostream>
#include <set>
#include <iterator>

int tableRow = 3;
int tableCol = 5;
int offset = 6;

std::set<std::string> terminalSet;
std::set<std::string> nonTerminalSet;

int main() {
	terminalSet.insert("$");
	terminalSet.insert("(");
	terminalSet.insert(")");
	terminalSet.insert("v");
	terminalSet.insert("f");
	nonTerminalSet.insert("Start");
	nonTerminalSet.insert("E");
	nonTerminalSet.insert("Prefix");

	// 2.
    std::string LLtable[tableRow+1][tableCol+1];
    // 2.a)
    for(int i = 0; i < tableRow; i++ ) 
        for(int j = 0; j < tableCol; j++ )
            LLtable[i][j] = " ";

	int i = 1;
    for(std::set<std::string>::reverse_iterator iter = terminalSet.rbegin(); iter != terminalSet.rend(); ++iter ) {
///        std::cout << *iter << " " << std::endl;
        LLtable[0][i] = *iter;
        ++i;
    }
    i = 1;
    for(std::set<std::string>::reverse_iterator iter = nonTerminalSet.rbegin(); iter != nonTerminalSet.rend(); ++iter ) {
///    	std::cout << *iter << " " << std::endl;
    	LLtable[i][0] = *iter;
    	++i;
    }
/*
    LLtable[1][0] = "Start";
    LLtable[2][0] = "E";
    LLtable[3][0] = "Prefix";
    LLtable[0][1] = "$";
    LLtable[0][2] = "(";
    LLtable[0][3] = ")";
    LLtable[0][4] = "v";
    LLtable[0][5] = "f";
*/
/*
    for(int i = 0; i <= tableRow; i++ ) {
        for(int j = 0; j <= tableCol; j++ ) {
        	if(i == 0 && j == 0) {
        		for( int k = 0; k < offset ; k++ ) {
        			std::cout << " ";
        		} 
        	}
            std::cout << LLtable[i][j] << " ";
        }
        std::cout << "\n";
    }
}

*/





/* === test global Set
#include <iostream>
#include <vector>
#include <set>
#include <iterator>

struct Rule {
    int ruleNumber;
    bool symbolDerivesEmpty;
    bool ruleDerivesEmpty;
    std::vector<std::string> followSet;
    std::set<std::string> firstSet;
    std::vector<std::string> RHS;
    std::string LHS;
    bool visitedFirst;
};

bool f_isNonTerminal(std::string s) {
	if(s != "|") return true;
	else false;
}

std::set<std::string> terminalSet;

bool f_isTerminal(std::string token) {
    char ch = token[0];
    if( !( ch <= 'Z' && ch >= 'A') && token != "lamda") {
        return true;
    }
    return false;
}
void f_collectTerminal(std::string s) {
	if( f_isTerminal( s ) ) {
		terminalSet.insert( s );
	}
}



int main() {

	Rule *pr;
	pr = new Rule;
	std::vector<std::string> token;
	token.push_back("1");
	token.push_back("Start");
	token.push_back(">");
	token.push_back("lamda");

	if( f_isNonTerminal( token[1] ) ) {
        pr->LHS = token[1];
            // 1.b)
        for(unsigned int i = 3; i < token.size(); i++ ) {
            pr->RHS.push_back( token[i] );
            f_collectTerminal( token[i] );
        }

    }
    else {
        for(unsigned int i = 2; i < token.size(); i++ ) {
            pr->RHS.push_back( token[i] );
            f_collectTerminal( token[i] );
        }
    }

    for(std::set<std::string>::iterator iter = terminalSet.begin(); iter != terminalSet.end(); ++iter ) {
    	std::cout << *iter << std::endl;
    }

    std::cout << "\n\n" << std::endl;


    Rule *pr1;
	pr1 = new Rule;
	std::vector<std::string> token1;
	token1.push_back("2");
	token1.push_back("E");
	token1.push_back(">");
	token1.push_back("Prefix");
	token1.push_back("(");
	token1.push_back("E");
	token1.push_back(")");

	if( f_isNonTerminal( token1[1] ) ) {
        pr->LHS = token1[1];
            // 1.b)
        for(unsigned int i = 3; i < token1.size(); i++ ) {
            pr->RHS.push_back( token1[i] );
            f_collectTerminal( token1[i] );
        }

    }
    else {
        for(unsigned int i = 2; i < token1.size(); i++ ) {
            pr->RHS.push_back( token1[i] );
            f_collectTerminal( token1[i] );
        }
    }

    for(std::set<std::string>::iterator iter = terminalSet.begin(); iter != terminalSet.end(); ++iter ) {
    	std::cout << *iter << std::endl;
    }

    std::cout << "\n\n" << std::endl;

    return 0;
}
*/


/* === test derivesEmpty function
#include <iostream>
#include <vector>
struct Rule {
	int ruleNumber;
	bool symbolDerivesEmpty;
	bool ruleDerivesEmpty;
	std::vector<std::string> followSet;
	std::vector<std::string> firstSet;
	std::vector<std::string> RHS;
	std::string LHS;
}	;

void f_checkForEmpty(Rule *pr) {
	if(pr->RHS.size() == 0 ) {
		pr->ruleDerivesEmpty = true;
		if( !( pr->symbolDerivesEmpty ) ) {
			pr->symbolDerivesEmpty = true;
			/*worklist*/
/*		}
	}
}
void f_derivesEmptyString(Rule *pr) { // ==> use it after RHS_push_back
	pr->symbolDerivesEmpty = false;
	pr->ruleDerivesEmpty = false;
	f_checkForEmpty(pr);
}


int main() {
	Rule *pr;
	pr = new Rule;
	
		pr->RHS.push_back("E");
		pr->RHS.push_back("$");
	f_derivesEmptyString(pr);	
	
}

*/

/* === test struct Rule
#include <iostream>	
#include <vector>

bool f_isNonTerminal(std::string s) {
	if(s != "|") return true;
	else false;
}
struct Rule {
	int ruleNumber;
	bool isDeriveLamda;
	std::vector<std::string> followSet;
	std::vector<std::string> firstSet;
	std::vector<std::string> RHS;
	std::string LHS;
};


int main() {
	Rule *pr;
	pr = new Rule;
	std::vector<std::string> token;
	token.push_back("0");
		token.push_back("Start");
		token.push_back(">");
		token.push_back("E");
		token.push_back("$");
		token.push_back("E");
		token.push_back("$");	
		token.push_back("E");
		token.push_back("$");
	if( f_isNonTerminal(token[1]) ) {
		pr->LHS = token[1];
		for( unsigned int i = 3; i < token.size(); i++ ) {
			pr->RHS.push_back(token[i]);
		}	
		for( unsigned int i = 0; i < pr->RHS.size(); i++ ) {
			std::cout << pr->RHS[i] << std::endl;
		}
	}
	else { std::cout << "no" << std::endl; }
}

*/

/* === test f_isNonTerminal 
#include <iostream>
#include <vector>

bool f_isNonTerminal(std::string s) {
	if(s != "|") return true;
	else false;
}

int main() {
	std::vector<std::string> token;
	token.push_back("0");
		token.push_back("|");
		token.push_back(">");
		token.push_back("E");
		token.push_back("$");
	if( f_isNonTerminal(token[1]) ) {
		std::cout << "ok" << std::endl;
	}
	else { std::cout << "no" << std::endl; }
}


*/

/* ===> test f_stoi
#include <iostream>
#include <vector>
#include <sstream>

struct Rule {
	int ruleNumber;
	bool isDeriveLamda;
	std::vector<std::string> followSet;
	std::vector<std::string> firstSet;
	Rule
};

int f_stoi(std::string s) {
	std::stringstream sso;
	sso << s;
	int it;
	sso >> it;
	return it;
}

int main() {

//	while( std::getline( myfile, line ) ) {
//		std::vector<std::string> token = split(line, ' ');
		std::vector<std::string> token;
		token.push_back("0");
		token.push_back("Start");
		token.push_back(">");
		token.push_back("E");
		token.push_back("$");
		Rule *pr;
		pr = new Rule;
		
		pr->ruleNumber =  f_stoi(token[0]);
		std::cout << pr->ruleNumber;

//	}

}

*/
