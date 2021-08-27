#include <iostream>

using namespace std;

int main()
{
    double sale, tax=0.09, tax_amount, total;
    while (true) {
        cout << "Sale amount: ";
        cin >> sale;
        tax_amount = sale*tax;
        cout << "Tax amount: " << tax_amount << endl;
        total = sale + tax_amount;
        cout << "Total amount due: " << total << endl;
        cout << "" << endl;
    }
    return 0;
}
