#ifndef ESTELLE_H
#define ESTELLE_H

class EstellePrivate;
class Estelle
{
    public:
        explicit Estelle();
        ~Estelle();
        void display();
        void nextframe();
        void setcface( int cface );
    private:
        EstellePrivate* const d;
};

#endif // ESTELLE_H
