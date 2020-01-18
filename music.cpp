#include "music.h"

#include <stdlib.h>

#include <AL/al.h>
#include <AL/alc.h>
#include <sndfile.hh>

class MusicPrivate
{
    public:
        ALCdevice* dev;
        ALCcontext* ctx;
        ALuint buffer;
        ALuint source;
};

Music::Music()
: d(new MusicPrivate)
{
    d->dev = alcOpenDevice( 0 );
    d->ctx = alcCreateContext( d->dev, 0 );
    alcMakeContextCurrent( d->ctx );
    alListener3f( AL_POSITION, 0.0f, 0.0f, 0.0f );
    alListenerf( AL_GAIN, 1.0f );

    SndfileHandle handle( "bgm.ogg" );
    const int channelCount = handle.channels();
    const int sampleCount = channelCount * handle.frames();
    const int sampleRate = handle.samplerate();
    fprintf( stderr, "%d %d %d\n", channelCount, sampleCount, sampleRate );
    /// load data from file
    ALshort* samples = (ALshort*)malloc( sampleCount * sizeof(ALshort) );
    handle.read( samples, sampleCount );
    /// determine file format
    ALenum format = ( channelCount == 1 ) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
    /// create buffer
    alGenBuffers( 1, &d->buffer );
    alBufferData( d->buffer, format, samples, sampleCount * sizeof(ALshort), sampleRate );
    free( samples );
}

Music::~Music()
{
    alSourceStop( d->source );
    alDeleteSources( 1, &d->source );
    alDeleteBuffers( 1, &d->buffer );
    alcDestroyContext( d->ctx );
    alcCloseDevice( d->dev );
    delete d;
}

void Music::play()
{
    alGenSources( 1, &d->source );
    alSource3f( d->source, AL_POSITION, 0.0f, 0.0f, 0.0f );
    alSourcef( d->source, AL_PITCH, 1.0f );
    alSourcef( d->source, AL_GAIN, 1.0f );
    alSourcei( d->source, AL_BUFFER, d->buffer );
    alSourcef( d->source, AL_ROLLOFF_FACTOR, 0.0f );
    alSourcei( d->source, AL_SOURCE_RELATIVE, AL_FALSE );
    alSourcei( d->source, AL_LOOPING, AL_TRUE );
    alSourcePlay( d->source );
}

void Music::stop()
{
    alSourceStop( d->source );
}
