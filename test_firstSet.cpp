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
    std::set<std::string> firstSet = {};
    std::set<std::string> predictSet;
    std::vector<std::string> RHS;
    bool visitedFirst = false;
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
    	current->count = 0;
    	Rule *pr = rhead;
		while(true) {
			for(std::vector<std::string>::iterator iter = pr->RHS.begin(); iter != pr->RHS.end(); ++iter ) {
				if( current->LHS == *iter) {
					current->count++;
				}
			}
			pr = pr->next;
			if(pr == NULL ) break;
		}
    	//x/current->count = current->RHS.size();
    	f_checkForEmpty(current);//no while loop
    	current = current->next;
    	if( current == NULL ) break;
    }
//    while(!worklist.empty()) {
		
//	}


    

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
        nowLHS = current->LHS;
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
        nowLHS = rhead->LHS;
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

void f_printFirstSet(Rule* pr) {
	std::cout << "firstSet :{";
	for(std::set<std::string>::iterator iter = pr->firstSet.begin(); iter != pr->firstSet.end(); ++iter ) {
		std::cout << *iter << ", ";
	}
	std::cout << "}" << std::endl;
}


std::string f_internalFirst( Rule* current, std::vector<std::string> RHS ) {
//K	system("PAUSE");
	if(RHS.empty()) return {};
	std::vector<std::string>::iterator iter = RHS.begin();
///	std::cout << *iter << std::endl;
	if( f_isTerminal( *iter ) ) {
		return *iter;
	}
	if( !current->visitedFirst ) {
		current->visitedFirst = true;
		Rule* pr = rhead;
//ok		system("PAUSE");
		while(true) {
			std::cout << "6" << std::endl;
			if( *iter == pr->LHS ) {
///				std::cout << "*****" << pr->ruleNumber << std::endl;
				pr->firstSet.insert( f_internalFirst( pr, pr->RHS )) ;
				std::cout << "LHS: " << pr->LHS << std::endl;
				f_printFirstSet(pr);
			}
			pr = pr->next;
			if( pr == NULL ) break;
		}
	}
	std::cout << "here" << std::endl;
	system("PAUSE");
//	if( current->symbolDerivesEmpty ) {
		system("PAUSE");
		std::vector<std::string> beta;
		for(unsigned int i = 1; i < current->RHS.size(); i++ ) {
			beta.push_back(current->RHS[i]);
		}
		std::cout << "beta: ";
		for(std::vector<std::string>::iterator iter = beta.begin(); iter != beta.end(); ++iter ) std::cout << *iter;
		std::cout << std::endl;
		std::cout << "start from Rule: " << current->ruleNumber << std::endl;
		current->firstSet.insert(f_internalFirst(current, beta ) );
		std::cout << "LHS: " << current->LHS << std::endl;
		f_printFirstSet(current);
//	}
	return {};
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
//K	system("PAUSE");
	current->firstSet.insert( f_internalFirst( current, current->RHS ) );
//	r->firstSet.insert();

}
void f_findPredict(Rule* r) {
//K	system("PAUSE");
	f_findFirst( r->ruleNumber );
///	if(r->ruleDerivesEmpty)
///		f_follow();
}


void f_printAllRule() {
    Rule* pr = rhead;
    while(true) {
    	std::cout << std::endl;
    	std::cout << "Rule " << pr->ruleNumber << ":"<< std::endl;
        std::cout << "count: " << pr->count << std::endl;
		std::cout << "LHS: " << pr->LHS << std::endl;
		std::cout << "RHS: ";
		for(std::vector<std::string>::iterator iter = pr->RHS.begin(); iter != pr->RHS.end(); ++iter ) {
			std::cout << *iter << " ";
		}
		std::cout << std::endl;
		std::cout << "rule empty? " << pr->ruleDerivesEmpty << std::endl;
		std::cout << "symbol empty? " << pr->symbolDerivesEmpty << std::endl;
		std::cout << "visited first? " << pr->visitedFirst << std::endl;
		std::cout << "first Set: {";
		for(std::set<std::string>::iterator iter = pr->firstSet.begin(); iter != pr->firstSet.end(); ++iter ) {
			std::cout << *iter << ", ";
		} 
		std::cout << "}" << std::endl;
		
		
		std::cout << std::endl << "==============================" << std::endl;
		
		pr = pr->next;
		if(pr == NULL ) break;
    }
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
    f_printAllRule();

	system("PAUSE");
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
