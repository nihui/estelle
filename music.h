#ifndef MUSIC_H
#define MUSIC_H

class MusicPrivate;
class Music
{
    public:
        explicit Music();
        ~Music();
        void play();
        void stop();
    private:
        MusicPrivate* const d;
};

#endif // MUSIC_H
