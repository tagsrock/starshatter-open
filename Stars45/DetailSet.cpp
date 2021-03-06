/*  Starshatter OpenSource Distribution
    Copyright (c) 1997-2004, Destroyer Studios LLC.
    All Rights Reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name "Destroyer Studios" nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.

    SUBSYSTEM:    Stars.exe
    FILE:         DetailSet.cpp
    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Level of Detail manager class
*/

#include "MemDebug.h"
#include "DetailSet.h"
#include "Random.h"

#include "Game.h"

// +--------------------------------------------------------------------+

SimRegion*    DetailSet::ref_rgn = 0;
Point         DetailSet::ref_loc;

// +--------------------------------------------------------------------+

DetailSet::DetailSet()
{
    for (int i = 0; i < MAX_DETAIL; i++)
    rad[i] = 0;

    index    = -1;
    levels   = 0;
    rgn      = 0;
}

DetailSet::~DetailSet()
{
    Destroy();
}

// +--------------------------------------------------------------------+

int
DetailSet::DefineLevel(double r, Graphic* g, Point* o, Point* spin_rate)
{
    if (levels < MAX_DETAIL && rep[levels].size() == 0) {
        rad[levels] = r;

        if (g)
        rep[levels].append(g);

        if (o)
        off[levels].append(o);

        else if (g)
        off[levels].append(new(__FILE__,__LINE__) Point(0,0,0));

        if (rate.size() == 0) {
            if (spin_rate) {
                rate.append(spin_rate);

                // randomize the initial orientation:
                Point* initial_spin = new(__FILE__,__LINE__) Point();
                if (spin_rate->x != 0) initial_spin->x = Random(-PI, PI);
                if (spin_rate->y != 0) initial_spin->y = Random(-PI, PI);
                if (spin_rate->z != 0) initial_spin->z = Random(-PI, PI);

                spin.append(initial_spin);
            }
            else {
                rate.append(new(__FILE__,__LINE__) Point());
                spin.append(new(__FILE__,__LINE__) Point());
            }
        }
        else {
            if (spin_rate)
            rate[ rep[levels].size() - 1 ] = spin_rate;
        }

        levels++;
    }

    return levels-1;
}

void
DetailSet::AddToLevel(int level, Graphic* g, Point* offset, Point* spin_rate)
{
    if (g && level >= 0 && level < levels) {
        rep[level].append(g);

        if (!offset)
        offset = new(__FILE__,__LINE__) Point(0,0,0);

        off[level].append(offset);

        if (spin_rate) {
            int nrep = rep[level].size();
            if (nrep > rate.size())
            rate.append(spin_rate);
            else
            rate[ nrep-1 ] = spin_rate;
        }

        if (spin.size() < rep[level].size()) {
            Point* initial_spin = new(__FILE__,__LINE__) Point();

            if (spin_rate) {
                // randomize the initial orientation:
                if (spin_rate->x != 0) initial_spin->x = Random(-PI, PI);
                if (spin_rate->y != 0) initial_spin->y = Random(-PI, PI);
                if (spin_rate->z != 0) initial_spin->z = Random(-PI, PI);
            }

            spin.append(initial_spin);
        }
    }
}

int
DetailSet::NumModels(int level) const
{
    if (level >= 0 && level < levels)
    return rep[level].size();

    return 0;
}

// +--------------------------------------------------------------------+

void
DetailSet::ExecFrame(double seconds)
{
    for (int i = 0; i < spin.size() && i < rate.size(); i++)
    (*spin[i]) += (*rate[i]) * seconds;
}

// +--------------------------------------------------------------------+

void
DetailSet::SetLocation(SimRegion* r, const Point& p)
{
    rgn = r;
    loc = p;
}

// +--------------------------------------------------------------------+

void
DetailSet::SetReference(SimRegion* r, const Point& p)
{
    ref_rgn = r;
    ref_loc = p;
}

// +--------------------------------------------------------------------+

int
DetailSet::GetDetailLevel()
{
    index = 0;

    if (rgn == ref_rgn) {
        double         screen_width = Game::GetScreenWidth();
        Point          delta        = loc - ref_loc;
        double         distance     = delta.length();

        for (int i = 1; i < levels && rad[i] > 0; i++) {
            double apparent_feature_size = (rad[i] * screen_width) / distance;
            
            if (apparent_feature_size > 0.4)
            index = i;
        }
    }

    return index;
}

// +--------------------------------------------------------------------+

Graphic*
DetailSet::GetRep(int level, int n)
{
    if (level >= 0 && level < levels && n >= 0 && n < rep[level].size())
    return rep[level].at(n);

    return 0;   
}

Point
DetailSet::GetOffset(int level, int n)
{
    if (level >= 0 && level < levels && n >= 0 && n < off[level].size())
    return *(off[level].at(n));

    return Point();   
}

Point
DetailSet::GetSpin(int level, int n)
{
    if (n >= 0 && n < spin.size())
    return *(spin.at(n));

    return Point();
}

// +--------------------------------------------------------------------+

void
DetailSet::Destroy()
{
    for (int i = 0; i < levels; i++) {
        ListIter<Graphic> iter = rep[i];

        while (++iter) {
            Graphic* g = iter.removeItem();
            g->Destroy();  // this will delete the object (g)
        }

        off[i].destroy();
    }

    rate.destroy();
    spin.destroy();
}
