#include<iostream>

int main()
{
    char Y;
    int test;
    if(std::cin >> test)
    {
        std::cout << "1: " << test << std::endl;
    }

    char comma = 'x';

    std::cin >> comma;

    std::cin >> test;
    std::cout  << "2: " << test << std::endl;
    std::cin >> comma;
    
    std::cin >> test;
    std::cout  << "3: " << test << std::endl;
    std::cin >> comma;

    std::cin >> test;
    std::cout  << "4: " << test << std::endl;

    return 0;
}
