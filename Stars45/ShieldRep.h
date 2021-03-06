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
    FILE:         ShieldRep.h
    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    ShieldRep Solid class
*/

#ifndef ShieldRep_h
#define ShieldRep_h

#include "Types.h"
#include "Solid.h"

// +--------------------------------------------------------------------+

struct ShieldHit;
class  Shot;

class ShieldRep : public Solid
{
public:
    ShieldRep();
    virtual ~ShieldRep();

    // operations
    virtual void   Render(Video* video, DWORD flags);
    virtual void   Energize(double seconds, bool bubble=false);
    int            ActiveHits() const { return nhits; }
    virtual void   Hit(Vec3 impact, Shot* shot, double damage=0);
    virtual void   TranslateBy(const Point& ref);
    virtual void   Illuminate();

protected:
    int            nhits;
    ShieldHit*     hits;
    Point          true_eye_point;
    bool           bubble;
};

#endif ShieldRep_h

