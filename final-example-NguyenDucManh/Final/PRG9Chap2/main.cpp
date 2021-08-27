#include <iostream>
using namespace std;

int main()
{
    float purchase_price;
    cin >> purchase_price;
    float tax_rate = 0.09;
    float sale_tax = tax_rate * purchase_price;
    cout << sale_tax;
    return 0;
}
