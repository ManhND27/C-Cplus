#ifndef ONE_H
#define ONE_H

class One {
private:
   int x, y;
public:
   One(int x = 0, int y = 0);
   int getX() const;
   int getY() const;
   void print() const;
};

#endif
