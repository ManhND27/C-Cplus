#include<iostream>

using namespace std;

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


void Tron_Mang(int a[], int b[], int c[], int n, int m, int &l)
{

    for (int i = 0; i < n; i++) {
        c[l] = a[i];
        l++;
    }


    for (int i = 0; i < m; i++) {
        c[l] = b[i];
        l++;
    }
}
int main() {

    int a[5];
    int n;
    int b[5];
    int m;
    int c[10];
    int l;

    cout << "Nhap so luong phan tu mang a: ";
    cin >> n;
    Nhap_Mang(a, n);
    Xuat_Mang(a, n);

    cout << "\nNhap so luong phan tu mang b: ";
    cin >> m;
    Nhap_Mang(b, m);
    Xuat_Mang(b, m);
    cout << "\nMang sau khi tron: ";
    Tron_Mang(a, b, c, n, m, l);
    Xuat_Mang(c, l);
    return 0;
}
