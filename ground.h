#ifndef GROUND_H
#define GROUND_H

class GroundPrivate;
class Ground
{
    public:
        explicit Ground();
        ~Ground();
        void display();
    private:
        GroundPrivate* const d;
};

#endif // GROUND_H
