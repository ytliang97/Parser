#include <iostream>
#include <stack>
#include <vector>
#include <sstream>
#include <fstream>
#include <set>

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
//== find terminal in RHS 
std::set<std::string> terminalSet;
//== find nonTerminal in LHS
std::set<std::string> nonTerminalSet;
//== set for check empty
std::set<std::string> worklist;
//=== startpoint
std::string startSymbolPos;
//=== for apply rule
std::stack<std::string> mystack;
//=== for same nonterminal's RHS
std::string nowLHS;
//=== for peek
int look = 0;
//=== table size
int tableRow = 5;
int tableCol = 6;
//std::string LLtable[1000][1000];
/* 8.
std::string LLtable[7+1][10+1] = {
		{" ", "floatdcl", "id", "intdcl", "assign", "print", "plus", "minus", "inum", "fnum", "$"},
		{"Prog",  "1", " ", "1", " ",  "1",  " ", " ", " ", " ", "1"},
		{"Dels",  "2", " ", "2", " ",  "3",  " ", " ", " ", " ", "3"},
		{"Del",   "4", " ", "5", " ",  " ",  " ", " ", " ", " ", " "},
		{"Stmts", " ", "6", " ", " ",  "6",  " ", " ", " ", " ", "7"},
		{"Stmt",  " ", "8", " ", " ",  "9",  " ", " ", " ", " ", " "},
		{"Expr",  " ", "12", " ", " ", "12", "10", "11", " ", " ", "12"},
		{"Val",   " ", "13", " ", " ", " ",  " ", " ", "14", "15", " "},
};
/* 7.
std::string LLtable[4+1][5+1] = {
		{" ",        "(", ")", "-", "id", "$"},
		{"Start",    "1", " ", "1", "1", " "},
		{"Expr",     "2", " ", "3", "4", " "},
		{"ExprTail", " ", "6", "5", " ", "6"},
		{"Var",      " ", " ", " ", "7", " "},
		{"VarTail",  "8", "9", "9", " ", "9"},
};
/* 6.
std::string LLtable[3+1][5+1] = {
		{" ", "begin", "end", ";", "simplestmt", "$"},
		{"Program", "1", " ", " ", " ", " "},
		{"Stmts",   "2", "3", " ", "2", " "},
		{"Stmt",    "5", " ", " ", "4", " "},
};
/* 5.
std::string LLtable[4+1][6+1] = {
		{" ", "lparen", "rparen", "num", "prod", "plus", "$"},
		{"Start",  "1", " ", "1", " ", " ", " "},
		{"Value",  "2", " ", "3", " ", " ", " "},
		{"Expr",   " ", " ", " ", "4", "5", " "},
		{"Values", "6", "7", "6", " ", " ", " "},
};
/* 4.
std::string LLtable[4+1][6+1] = {
		{" ", "num", "lp", "rp", "plus", "prod", "$"},
		{"S", "1", "1", " ", " ", " ", "1"},
		{"A", "2", "3", "4", "4", "4", "4"},
		{"B", " ", " ", " ", "5", "6", " "},
		{"C", "7", "7", "8", "7", " ", " "},
};
/* 3.
std::string LLtable[4+1][6+1] = {
		{" ", "(", ")", "v", "f", "+", "$"},
		{"Start", "1", " ", "1", "1", " ", " "},
		{"E",     "2", " ", "3", "2", " ", " "},
		{"Prefix", "5", " ", " ", "4", " ", " "},
		{"Tail", " ", "7", " ", " ", "6", "7"},
};
/* 2.
std::string LLtable[3+1][4+1] = {
		{" ", "c", "a", "b", "$"},
		{"S", "1", "1", "1", " "},
		{"A", "3", "2", "3", " "},
		{"B", "5", " ", "4", " "},
};
/* 1.*/
std::string LLtable[5+1][6+1] = {
		{" ", "a", "d", "b", "c", "q", "$"},
		{"S", "1", " ", "1", "1", "1", "1"},
		{"A", "2", " ", "3", "3", "3", "3"},
		{"B", " ", "5", "4", "5", "5", "5"},
		{"C", " ", "7", " ", "6", " ", "7"},
		{"Q", " ", " ", " ", "9", "8", "9"},
};

/*
std::string LLtable[6][7] = {
		{" ", "a", "b", "c", "d", "q", "$"},
		{"S", "1", "1", "1", " ", "1", "1"},
		{"C", " ", " ", "2", "3", " ", "3"},
		{"A", "4", "5", "5", " ", "5", "5"},
		{"B", " ", "6", "7", "7", "7", "7"},
		{"Q", " ", " ", "9", " ", "8", "9"},
};
*/
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
		current->symbolDerivesEmpty = false;
    	current->ruleDerivesEmpty = false;
    	current->count = current->RHS.size();
    	f_checkForEmpty(current);
    	current = current->next;
    	if( current == NULL ) break;    
    }
    //while(!worklist.empty()) {
    	
    //}


    

}
bool errorflag = false;
void f_errorSyntax() {
	std::cout << "        ERROR::Syntax error" << std::endl;
	errorflag = true;
}
void f_error(std::string token) {
	std::cout << "        ERROR::Expected " << token << std::endl;
	errorflag = true;
}
std::string f_peek(std::vector<std::string> ts) {
	//std::cout << "pass f_peek" << std::endl;
	return ts[look];
}
//procedure MATCH(ts,token)
//    if ts.PEEK() = token
//   then call ts.ADVANCE()
//   else call ERROR(Expected token)
//end
void f_match(std::vector<std::string> ts, std::string token) {
	std::cout << "        f_match" << "  ";
    std::cout << token << " ";
    std::cout << f_peek(ts) << std::endl;
	if(f_peek(ts) == token ) {
		look++; //ADVANCE();
        mystack.pop();
    }
	else
		f_error(token);
}
int f_stoi(std::string s) {
    std::stringstream sso;
    sso << s;
    int it;
    sso >> it;
    return it;
}
std::string f_visitLLtable(std::string LHS, std::string terminal){
	//std::cout << "pass f_visitLLtable" << std::endl;

	int i,j;
	for(i = 0;i < tableRow + 1; i++ ) {
		if( LLtable[i][0] == LHS ) break;
	}
	for(j = 0;j < tableCol + 1; j++ ) {
		if( LLtable[0][j] == terminal ) break;
	}
 ///   std::cout << LHS << " " << terminal << std::endl;
 ///   std::cout << i << " " << j << " " << std::endl;
    ///system("PAUSE");
 ///   std::cout << LLtable[i][j] << std::endl;
	///system("PAUSE");
    return LLtable[i][j];
}
void f_printApplyRule(Rule* r) {
    std::cout << "       Apply " << r->ruleNumber << ": " << r->LHS << " > ";
    for(std::vector<std::string>::iterator iter = r->RHS.begin(); iter < r->RHS.end(); ++iter ) {
        std::cout << *iter;
    }
    std::cout << std::endl;
}
void f_apply(int r) {
	mystack.pop();
	Rule * current = rhead;
	while(true) {
		if(current->ruleNumber == r) {
			break;
		}
		current = current->next;
		if(current == NULL) break;
	}
    f_printApplyRule(current);
	for( int i = current->RHS.size()-1; i >= 0 ; i-- ) {
		std::cout << current->RHS[i];
		mystack.push(current->RHS[i]);
	}
}
void f_printRemainingInput(std::vector<std::string> ts) {
    int count = 0;
    std::cout << "                           ";
    for(std::vector<std::string>::iterator iter = ts.begin(); iter < ts.end() ; ++iter ) {
        if( count < look ) { count++; continue; }
        else std::cout << *iter << " ";
    }
    std::cout << std::endl;
}
void f_LLParser(std::vector<std::string> ts) {
    std::cout << "push item  Action           Remaining Input" << std::endl;
    std::cout << "----------------------------------" << std::endl;
	mystack.push(startSymbolPos);
    std::cout << mystack.top() << "";
    f_printRemainingInput(ts);
	///system("PAUSE");
	bool accepted = false;
    
	while(!accepted) {
		if( f_isTerminal( mystack.top() ) ) {
			f_match( ts, mystack.top() );
			if( mystack.top() == "$" ) {
				accepted = true;
                look = 0;
				mystack.pop();
                std::cout << "Accept" << std::endl;
			}
            if (!accepted) {
                std::cout << mystack.top() << "";
                f_printRemainingInput(ts);
            }
            ///system("PAUSE");
		}
		else {
            ///system("PAUSE");
			std::string p = f_visitLLtable(mystack.top(),f_peek(ts));
			///std::cout << p << std::endl;
			if( p == " " ) f_errorSyntax();
			else {
                ///system("PAUSE");
				f_apply(f_stoi(p));
                std::cout << "           ";
                f_printRemainingInput(ts);
			}
            
		}
		if(errorflag) break;
	}
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

void f_printTerminalSet() {
    for(std::set<std::string>::iterator iter = terminalSet.begin(); iter != terminalSet.end(); ++iter ) {
        std::cout << *iter << ", ";
    }
    std::cout << std::endl;
}

void f_printNonTerminalSet() {
    for(std::set<std::string>::iterator iter = nonTerminalSet.begin(); iter != nonTerminalSet.end() ; ++iter ) {
        std::cout << *iter << ", ";
    }
    std::cout << std::endl;
}

void f_applyRule(char* testFile) {
    
    std::ifstream file;
    file.open(testFile);
    if(file.is_open()) {} else { std::cout << "unable to open testfile.\n";}

    std::string tokenString;
    while( getline( file, tokenString ) ) {
        std::cout << "\n" << tokenString << std::endl;
        
        std::vector<std::string> ts = f_split(tokenString, ' ');
///        std::cout << "hello";
        f_LLParser(ts);
        ts.clear();
        std::vector<std::string>().swap(ts);
///        system("PAUSE");
        std::cout << std::endl;
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
	f_fProduction(line, rhead);
	Rule *current = rhead;
	while( std::getline( myfile, line ) ) {

		current->next = new Rule;
		f_production(line, current);
		current = current->next;		
	}
	
	f_derivesEmptyString();

    std::cout << std::endl;
    f_printTerminalSet();
    f_printNonTerminalSet();
    ///system("PAUSE");

	
	/*
	std::vector<std::string> ts;
	ts.push_back("(");
	ts.push_back("v");
	ts.push_back("+");
	ts.push_back("f");
	ts.push_back("(");
	ts.push_back("v");
    ts.push_back(")");
    ts.push_back(")");
    ts.push_back("$");
	f_LLParser(ts);*/
    f_applyRule(argv[2]);


	return 0;


}
