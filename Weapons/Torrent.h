/**
*
* @author: Owen Glofcheski
* @date: March 19, 2012
*
*/

#ifndef TORRENT_H_INCLUDED
#define TORRENT_H_INCLUDED

#include "Projectile.h"

class Torrent : public Projectile
{
private:
    void createParticles();
public:
    Torrent(PixMap *pixMap, Vect pos, Vect velo, int level);

    // Events
    void onDie();
    bool onActivate();
    bool onPixCollide();
    bool onPlayerCollide();
    bool onBoundCollide();
    bool onNoCollide();
};

class TorrentProj : public Projectile
{
private:
    void createParticles();
public:
    TorrentProj(PixMap *pixMap, Vect pos, Vect velo, int level);

    // Events
    void onDie();
    bool onActivate();
    bool onPixCollide();
    bool onPlayerCollide();
    bool onBoundCollide();
    bool onNoCollide();
};

#endif // TORRENT_H_INCLUDED
