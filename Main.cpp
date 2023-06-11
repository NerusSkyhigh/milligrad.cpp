#include <iostream>
#include "engine.cpp"

int main() {
    Value a(2);  // Create an object of MyClass
    Value b(3);  // Create an object of MyClass

    Value c = a+b;
    Value d = a*b;
    Value e = c*a+b;
    Value f = e.pow(3);

    std::cout << std::endl << "Summary of computation steps performed" << std::endl;
    std::cout << "a=" << a <<"\tb=" << b << std::endl;
    std::cout << "c=a+b=" << c << std::endl;
    std::cout << "d=a*b=" << d << std::endl;
    std::cout << "e = c*a+b=" << e << std::endl;
    std::cout << "f = e^3=" << f << std::endl;

    std::cout << std::endl << std::endl << "Computational graph for f" << std::endl;
    std::cout << f.to_graph() << std::endl;

    std::cout << std::endl << "Partial derivatives of f (d=\\partial):" << std::endl;
    f.zero_grad();
    f.backpropagate();
    std::cout << "\tf = e^3=(c*a+b)^3=(a^2+ab+b)^3=" << f << std::endl;
    std::cout << "\t\tdf/df=" << std::to_string(f.get_grad()) << std::endl;
    std::cout << "\t\tde/de=" << std::to_string(e.get_grad()) << std::endl;
    std::cout << "\t\tde/dc=" << std::to_string(c.get_grad()) << std::endl;
    std::cout << "\t\tde/db=" << std::to_string(b.get_grad()) << std::endl;
    std::cout << "\t\tde/da=" << std::to_string(a.get_grad()) << std::endl;
    return 0;
}