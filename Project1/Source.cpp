#include <ostream>
#include<iostream>
#include <typeinfo>
void print_values(std::ostream& os) {
    return;
}


template<typename T, typename ...Args>
void print_values(std::ostream& os, T value, Args... arg)
{
    print_values(os, arg...);
    os << typeid(value).name() << ": " << value << '\n';
}




int main() {
 

    print_values(std::cout, int(2), double(3.5), "Hello" ,"e",char("t"));
    return 0;


}