#ifndef SQUARE_H
#define SQUARE_H

class SquarePrivate;
class Square
{
    public:
        explicit Square();
        ~Square();
        void display();
    private:
        SquarePrivate* const d;
};

#endif // SQUARE_H
