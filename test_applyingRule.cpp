#include <iostream>
#include <fstream>
#include <stack>
#include <vector>

std::string startSymbolPos = "Start";

std::stack<std::string> mystack;

//=== for peek
int look = 0;

std::string LLtable[4+1][6+1] = {
		{" ", "(", ")", "v", "f", "+", "$"},
		{"Start", "1", " ", "1", "1", " ", " "},
		{"E", "2", " ", "3", "2", " ", " "},
		{"Prefix", "5", " ", " ", "4", " ", " "},
		{"Tail", " ", "7", " ", " ", "6", "7"},
	};

void f_printLLtable() {
	for(int i = 0;i < 5; i++ ) {
		for(int j = 0; j < 7; j++ ) {
			std::cout << LLtable[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

bool f_isTerminal(std::string token) {
    char ch = token[0];
    if( !( ch <= 'Z' && ch >= 'A') && token != "lamda" ) {
        return true;
    }
    return false;
}



void f_LLParser(std::vector<std::string> ts) {
	mystack.push(startSymbolPos);
	bool accepted = false;
	while(!accepted) {
		if( f_isTerminal( mystack.top() ) ) {
			//f_match( ts, mystack.top() );
			if( mystack.top() == "$" ) {
				accepted = true;
				mystack.pop();
			}
		}
		else {
			std::string p = f_visitLLtable(mystack.top(),f_peek(ts));
		}
	}
}

void f_applyRule(char* testFile) {
	
	std::ifstream file;
	file.open(testFile);
	if(file.is_open()) {} else { std::cout << "unable to open testfile.\n";}

	std::string tokenString;
	while( getline( file, tokenString ) ) {
		std::cout << "\n" << tokenString << "  ";
		
		std::vector<std::string> ts = split(tokenString, ' ');
		f_LLParser(ts);
		
		std::cout << std::endl;
	}
}

int main(int argc, char *argv[]) {
	
	f_printLLtable();
	f_applyRule(argv[2]);
	return 0;
}


/*

//=== main
procedure LLPARSER(ts)
    call PUSH(S)
    accepted <- false
    while not accepted do
        if TOS() belongs sigma
        then
            call MATCH(ts, TOS())
            if TOS() = $
            then accepted <- true
            call POP()
        else
            p <- LLtable[TOS(), ts.PEEK()]
            if p = 0
            then call ERROR(syntax error)
        else call APPLY(p)
end

//=== sub
//--STACK
procedure APPLY(p: A -> X1...Xm)
    call POP()
    for i = m downto 1 do
        call PUSH(Xi)
end

procedure MATCH(ts.token)
    if ts.PEEK() = token
    then call ts.ADVANCE()
    else call ERROR(Expected token)
end

function TOS()//...

function POP()//...

function PUSH()//...

function ADVANCE()//...Advance the input by one token

function ERROR()//...

*/
