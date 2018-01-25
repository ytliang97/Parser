#include <iostream>
#include <set>
#include <string>
#include <iterator>
#include <vector>

int main()
{
    std::set<std::string> set;
    set.insert("int");
    set.insert("main()");
    set.insert("#include");
    set.insert("<iterator>");
    set.insert("#include");
    set.insert("<string>");
    set.insert("<set>");
    set.insert("<iostream");




    {
        // iterators (legacy)
        for( std::set<std::string>::iterator iter = set.begin() ; iter != set.end() ; ++iter ) std::cout << *iter << ' ' ;
        std::cout << '\n' ;

        // iterators, in reverse (legacy)
        for( std::set<std::string>::reverse_iterator iter = set.rbegin() ; iter != set.rend() ; ++iter ) std::cout << *iter << ' ' ;
        std::cout << '\n' ;
    }
}
