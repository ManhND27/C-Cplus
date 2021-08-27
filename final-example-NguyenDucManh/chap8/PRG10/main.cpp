#include <iostream>

using namespace std;
#define COL 100
void Nhap_Mang(int a[], int n)
{

        for (int i = 0; i < n; i++) {
            cout << "Nhap gia tri phan tu thu " << i << " = ";
            cin >> a[i];

    }
}


void Xuat_Mang(int a[], int n)
{
    for (int i = 0; i < n; i++) {
        cout << a[i] << " ";
    }
}

void Tron_Mang(int a[], int b[], int c[][COL], int n, int m, int &l, int &s)
{

    for (int i = 0; i < n; i++) {
        c[l][s] = a[i];
        l++;
        s++;
    }


    for (int i = 0; i < m; i++) {
        c[l][s] = b[i];
        l++;
        s++;
    }
}

int main()
{
    int a[5];
    int n;
    int b[5];
    int m;
    int c[5][5];
    int l;
    int s;
    cout << "Nhap so luong phan tu mang a: ";
    cin >> n;
    Nhap_Mang(a, n);
    Xuat_Mang(a, n);

    cout << "\nNhap so luong phan tu mang b: ";
    cin >> m;
    Nhap_Mang(b, m);
    Xuat_Mang(b, m);
    cout << "\nMang sau khi tron: ";
    Tron_Mang(a, b, c, n, m, l, s);
    Xuat_Mang(c, l);

    return 0;
}
