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
// AnnotationMixin
//
// This mixin (class with templated superclass) adds OpenSteer::App-based
// graphical annotation functionality to a given base class, which is
// typically something that supports the AbstractVehicle interface.
//
// 10-04-04 bk:  put everything into the OpenSteer namespace
// 04-01-03 cwr: made into a mixin
// 07-01-02 cwr: created (as Annotation.h) 
//
//
// ----------------------------------------------------------------------------


#ifndef OPENSTEER_ANNOTATION_H
#define OPENSTEER_ANNOTATION_H


#include "App.h"


// ----------------------------------------------------------------------------


namespace OpenSteer {

    const Vec3 gBlack   (0, 0, 0);
    const Vec3 gWhite   (1, 1, 1);

    const Vec3 gRed     (1, 0, 0);
    const Vec3 gYellow  (1, 1, 0);
    const Vec3 gGreen   (0, 1, 0);
    const Vec3 gCyan    (0, 1, 1);
    const Vec3 gBlue    (0, 0, 1);
    const Vec3 gMagenta (1, 0, 1);

    const Vec3 gOrange (1, 0.5, 0);

    inline Vec3 grayColor (const float g) {return Vec3 (g, g, g);}

    const Vec3 gGray10 = grayColor (0.1);
    const Vec3 gGray20 = grayColor (0.2);
    const Vec3 gGray30 = grayColor (0.3);
    const Vec3 gGray40 = grayColor (0.4);
    const Vec3 gGray50 = grayColor (0.5);
    const Vec3 gGray60 = grayColor (0.6);
    const Vec3 gGray70 = grayColor (0.7);
    const Vec3 gGray80 = grayColor (0.8);
    const Vec3 gGray90 = grayColor (0.9);

    // ----------------------------------------------------------------------------

    template <class Super>
    class AnnotationMixin : public Super
    {
    public:

        // constructor
        AnnotationMixin ();

        // destructor
        virtual ~AnnotationMixin ();

        // ------------------------------------------------------------------------
        // trails / streamers
        //
        // these routines support visualization of a vehicle's recent path
        //
        // XXX conceivable trail/streamer should be a separate class,
        // XXX Annotation would "has-a" one (or more))

        // record a position for the current time, called once per update
        void recordTrailVertex (const float currentTime, const Vec3 position);

        // draw the trail as a dotted line, fading away with age
        void drawTrail (void) {drawTrail (grayColor (0.7), gWhite);}
        void drawTrail  (const Vec3& trailColor, const Vec3& tickColor);

        // set trail parameters: the amount of time it represents and the
        // number of samples along its length.  re-allocates internal buffers.
        void setTrailParameters (const float duration, const int vertexCount);

        // forget trail history: used to prevent long streaks due to teleportation
        void clearTrailHistory (void);

        // ------------------------------------------------------------------------
        // drawing of lines, circles and (filled) disks to annotate steering
        // behaviors.  When called during OpenSteer::App's simulation update phase,
        // these functions call a "deferred draw" routine which buffer the
        // arguments for use during the redraw phase.
        //
        // note: "circle" means unfilled
        //       "disk" means filled
        //       "XZ" means on a plane parallel to the X and Z axes (perp to Y)
        //       "3d" means the circle is perpendicular to the given "axis"
        //       "segments" is the number of line segments used to draw the circle

        // draw an opaque colored line segment between two locations in space
        void annotationLine (const Vec3& startPoint,
                             const Vec3& endPoint,
                             const Vec3& color) const;

        // draw a circle on the XZ plane
        void annotationXZCircle (const float radius,
                                 const Vec3& center,
                                 const Vec3& color,
                                 const int segments) const
        {
            annotationXZCircleOrDisk (radius, center, color, segments, false);
        }


        // draw a disk on the XZ plane
        void annotationXZDisk (const float radius,
                               const Vec3& center,
                               const Vec3& color,
                               const int segments) const
        {
            annotationXZCircleOrDisk (radius, center, color, segments, true);
        }


        // draw a circle perpendicular to the given axis
        void annotation3dCircle (const float radius,
                                 const Vec3& center,
                                 const Vec3& axis,
                                 const Vec3& color,
                                 const int segments) const
        {
            annotation3dCircleOrDisk (radius, center, axis, color, segments, false);
        }


        // draw a disk perpendicular to the given axis
        void annotation3dDisk (const float radius,
                               const Vec3& center,
                               const Vec3& axis,
                               const Vec3& color,
                               const int segments) const
        {
            annotation3dCircleOrDisk (radius, center, axis, color, segments, true);
        }

        //

        // ------------------------------------------------------------------------
        // support for annotation circles

        void annotationXZCircleOrDisk (const float radius,
                                       const Vec3& center,
                                       const Vec3& color,
                                       const int segments,
                                       const bool filled) const
        {
            annotationCircleOrDisk (radius,
                                    Vec3::zero,
                                    center,
                                    color,
                                    segments,
                                    filled,
                                    false); // "not in3d" -> on XZ plane
        }


        void annotation3dCircleOrDisk (const float radius,
                                       const Vec3& center,
                                       const Vec3& axis,
                                       const Vec3& color,
                                       const int segments,
                                       const bool filled) const
        {
            annotationCircleOrDisk (radius,
                                    axis,
                                    center,
                                    color,
                                    segments,
                                    filled,
                                    true); // "in3d"
        }

        void annotationCircleOrDisk (const float radius,
                                     const Vec3& axis,
                                     const Vec3& center,
                                     const Vec3& color,
                                     const int segments,
                                     const bool filled,
                                     const bool in3d) const;

        // ------------------------------------------------------------------------
    private:

        // trails
        int trailVertexCount;       // number of vertices in array (ring buffer)
        int trailIndex;             // array index of most recently recorded point
        float trailDuration;        // duration (in seconds) of entire trail
        float trailSampleInterval;  // desired interval between taking samples
        float trailLastSampleTime;  // global time when lat sample was taken
        int trailDottedPhase;       // dotted line: draw segment or not
        Vec3 curPosition;           // last reported position of vehicle
        Vec3* trailVertices;        // array (ring) of recent points along trail
        char* trailFlags;           // array (ring) of flag bits for trail points
    };

} // namespace OpenSteer



// ----------------------------------------------------------------------------
// Constructor and destructor


template<class Super>
OpenSteer::AnnotationMixin<Super>::AnnotationMixin (void)
{
    trailVertices = NULL;
    trailFlags = NULL;

    // xxx I wonder if it makes more sense to NOT do this here, see if the
    // xxx vehicle class calls it to set custom parameters, and if not, set
    // xxx these default parameters on first call to a "trail" function.  The
    // xxx issue is whether it is a problem to allocate default-sized arrays
    // xxx then to free them and allocate new ones
    setTrailParameters (5, 100);  // 5 seconds with 100 points along the trail
}


template<class Super>
OpenSteer::AnnotationMixin<Super>::~AnnotationMixin (void)
{
    delete[] trailVertices;
    delete[] trailFlags;
}


// ----------------------------------------------------------------------------
// set trail parameters: the amount of time it represents and the number of
// samples along its length.  re-allocates internal buffers.


template<class Super>
void 
OpenSteer::AnnotationMixin<Super>::setTrailParameters (const float duration, 
                                                       const int vertexCount)
{
    // record new parameters
    trailDuration = duration;
    trailVertexCount = vertexCount;

    // reset other internal trail state
    trailIndex = 0;
    trailLastSampleTime = 0;
    trailSampleInterval = trailDuration / trailVertexCount;
    trailDottedPhase = 1;

    // prepare trailVertices array: free old one if needed, allocate new one
    delete[] trailVertices;
    trailVertices = new Vec3[trailVertexCount];

    // prepare trailFlags array: free old one if needed, allocate new one
    delete[] trailFlags;
    trailFlags = new char[trailVertexCount];

    // initializing all flags to zero means "do not draw this segment"
    for (int i = 0; i < trailVertexCount; i++) trailFlags[i] = 0;
}


// ----------------------------------------------------------------------------
// forget trail history: used to prevent long streaks due to teleportation
//
// XXX perhaps this coudl be made automatic: triggered when the change in
// XXX position is well out of the range of the vehicles top velocity


template<class Super>
void 
OpenSteer::AnnotationMixin<Super>::clearTrailHistory (void)
{
    // brute force implementation, reset everything
    setTrailParameters (trailDuration, trailVertexCount);
}


// ----------------------------------------------------------------------------
// record a position for the current time, called once per update


template<class Super>
void 
OpenSteer::AnnotationMixin<Super>::recordTrailVertex (const float currentTime,
                                                      const Vec3 position)
{
    const float timeSinceLastTrailSample = currentTime - trailLastSampleTime;
    if (timeSinceLastTrailSample > trailSampleInterval)
    {
        trailIndex = (trailIndex + 1) % trailVertexCount;
        trailVertices [trailIndex] = position;
        trailDottedPhase = (trailDottedPhase + 1) % 2;
        const int tick = (floorXXX (currentTime) >
                          floorXXX (trailLastSampleTime));
        trailFlags [trailIndex] = trailDottedPhase | (tick ? 2 : 0);
        trailLastSampleTime = currentTime;
    }
    curPosition = position;
}


// ----------------------------------------------------------------------------
// draw the trail as a dotted line, fading away with age


template<class Super>
void 
OpenSteer::AnnotationMixin<Super>::drawTrail (const Vec3& trailColor,
                                              const Vec3& tickColor)
{
    if (OpenSteer::App::annotationIsOn())
    {
        int index = trailIndex;
        for (int j = 0; j < trailVertexCount; j++)
        {
            // index of the next vertex (mod around ring buffer)
            const int next = (index + 1) % trailVertexCount;

            // "tick mark": every second, draw a segment in a different color
            const int tick = ((trailFlags [index] & 2) ||
                              (trailFlags [next] & 2));
            const Vec3 color = tick ? tickColor : trailColor;

            // draw every other segment
            if (trailFlags [index] & 1)
            {
                if (j == 0)
                {
                    // draw segment from current position to first trail point
                    Draw::drawLine (curPosition, trailVertices [index], color);
                }
                else
                {
                    // draw trail segments with opacity decreasing with age
                    const float minO = 0.05f; // minimum opacity
                    const float fraction = (float) j / trailVertexCount;
                    const float opacity = (fraction * (1 - minO)) + minO;
                    Draw::drawLine (trailVertices [index], trailVertices [next], color, opacity);
                }
            }
            index = next;
        }
    }
}


// ----------------------------------------------------------------------------
// request (deferred) drawing of a line for graphical annotation
//
// This is called during OpenSteer::App's simulation phase to annotate behavioral
// or steering state.  When annotation is enabled, a description of the line
// segment is queued to be drawn during OpenSteer::App's redraw phase.


template<class Super>
void 
OpenSteer::AnnotationMixin<Super>::annotationLine (const Vec3& startPoint,
                                                   const Vec3& endPoint,
                                                   const Vec3& color) const
{
    if (OpenSteer::App::annotationIsOn())
    {
        Draw::drawLine (startPoint, endPoint, color);
    }
}


// ----------------------------------------------------------------------------
// request (deferred) drawing of a circle (or disk) for graphical annotation
//
// This is called during OpenSteer::App's simulation phase to annotate behavioral
// or steering state.  When annotation is enabled, a description of the
// "circle or disk" is queued to be drawn during OpenSteer::App's redraw phase.


template<class Super>
void 
OpenSteer::AnnotationMixin<Super>::annotationCircleOrDisk (const float radius,
                                                           const Vec3& axis,
                                                           const Vec3& center,
                                                           const Vec3& color,
                                                           const int segments,
                                                           const bool filled,
                                                           const bool in3d) const
{
    if (OpenSteer::App::annotationIsOn())
    {
        Draw::drawCircle (radius, axis, center, color, segments, filled, in3d);
    }
}


// ----------------------------------------------------------------------------
#endif // OPENSTEER_ANNOTATION_H
