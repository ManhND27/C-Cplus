#ifndef TWO_H
#define TWO_H

class Two {
private:
   int x;
   char a;
public:
   Two(int x = 0, char a = 0);
   int getX() const;
   void setX(int x);
   char getA() const;
   void setA(char a);
   void print() const;
};

#endif // TWO_H
