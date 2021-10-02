// ----------------------------------------------------------------------------
//
//
// OpenSteer -- Steering Behaviors for Autonomous Characters
//
// Copyright (c) 2002-2003, Sony Computer Entertainment America
// Original author: Craig Reynolds <craig_reynolds@playstation.sony.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
//
// ----------------------------------------------------------------------------
//
//
// One vehicle turning way: a (near) minimal OpenSteer::App PlugIn
//
// 06-24-02 cwr: created 
//
//
// ----------------------------------------------------------------------------


#include "OpenSteer/SimpleVehicle.h"
#include "OpenSteer/App.h"

#include <iomanip>
#include <sstream>


using namespace OpenSteer;


// ----------------------------------------------------------------------------


class OneTurning : public SimpleVehicle
{
public:

    // constructor
    OneTurning () {reset ();}

    // reset state
    void reset (void)
    {
        SimpleVehicle::reset (); // reset the vehicle 
        setSpeed (1.5);         // speed along Forward direction.
        setMaxForce (0.3);      // steering force is clipped to this magnitude
        setMaxSpeed (5);         // velocity is clipped to this magnitude
        clearTrailHistory ();    // prevent long streaks due to teleportation 
    }

    // per frame simulation update
    void update (const float currentTime, const float elapsedTime)
    {
        applySteeringForce (Vec3 (-2, 0, -3), elapsedTime);
        annotationVelocityAcceleration ();
        recordTrailVertex (currentTime, position());
    }

    // draw this character/vehicle into the scene
    void draw (void)
    {
        OpenSteer::App::drawBoxHighlightOnVehicle (*this, gGray50);
        drawTrail ();
    }
};


// ----------------------------------------------------------------------------
// PlugIn for OpenSteer App


class OneTurningPlugIn : public PlugIn
{
public:
    
    const char* name (void) {return "One Turning Away";}

    float selectionOrderSortKey (void) {return 0.06;}

    // be more "nice" to avoid a compiler warning
    virtual ~OneTurningPlugIn() {}

    void open (void)
    {
        gOneTurning = new OneTurning;
        OpenSteer::App::selectedVehicle = gOneTurning;
        theVehicle.push_back (gOneTurning);

        // initialize camera
        OpenSteer::App::init2dCamera (*gOneTurning);
        OpenSteer::App::camera.setPosition (10,
                                           OpenSteer::App::camera2dElevation,
                                           10);
        OpenSteer::App::camera.fixedPosition.set (40, 40, 40);
    }

    void update (const float currentTime, const float elapsedTime)
    {
        // update simulation of test vehicle
        gOneTurning->update (currentTime, elapsedTime);
    }

    void redraw (const float currentTime, const float elapsedTime)
    {
        // draw test vehicle
        gOneTurning->draw ();

        // textual annotation (following the test vehicle's screen position)
        std::ostringstream annote;
        annote << std::setprecision (2) << std::setiosflags (std::ios::fixed);
        annote << "      speed: " << gOneTurning->speed() << std::ends;
        Draw::drawTextAt3dLocation (annote, gOneTurning->position(), gRed);
        Draw::drawTextAt3dLocation ("start", Vec3::zero, gGreen);

        // update camera, tracking test vehicle
        OpenSteer::App::updateCamera (currentTime, elapsedTime, *gOneTurning);

        // draw "ground plane"
        OpenSteer::App::gridUtility (gOneTurning->position());
    }

    void close (void)
    {
        theVehicle.clear ();
        delete (gOneTurning);
        gOneTurning = NULL;
    }

    void reset (void)
    {
        // reset vehicle
        gOneTurning->reset ();
    }

    const AVGroup& allVehicles (void) {return (const AVGroup&) theVehicle;}

    OneTurning* gOneTurning;
    std::vector<OneTurning*> theVehicle; // for allVehicles
};


OneTurningPlugIn gOneTurningPlugIn;




// ----------------------------------------------------------------------------
