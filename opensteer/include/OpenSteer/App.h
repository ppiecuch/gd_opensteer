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
// OpenSteer App
//
// This class encapsulates the state of the OpenSteerDemo application and
// the services it provides to its plug-ins
//
// 10-04-04 bk:  put everything into the OpenSteer namespace
// 11-14-02 cwr: recast App class as OpenSteerDemo 
// 06-26-02 cwr: App class created 
//
//
// ----------------------------------------------------------------------------


#ifndef OPENSTEER_APP_H
#define OPENSTEER_APP_H


#include "OpenSteer/Vec3.h"
#include "OpenSteer/Clock.h"
#include "OpenSteer/PlugIn.h"
#include "OpenSteer/Camera.h"
#include "OpenSteer/Utilities.h"

#include <sstream>

namespace OpenSteer {


    class App
    {
    public:
        // ------------------------------------------------------ component objects

        // clock keeps track of both "real time" and "simulation time"
        static Clock clock;

        // camera automatically tracks selected vehicle
        static Camera camera;

        // ------------------------------------------ addresses of selected objects

        // currently selected plug-in (user can choose or cycle through them)
        static PlugIn* selectedPlugIn;

        // currently selected vehicle.  Generally the one the camera follows and
        // for which additional information may be displayed.  Clicking the mouse
        // near a vehicle causes it to become the Selected Vehicle.
        static AbstractVehicle* selectedVehicle;

        // -------------------------------------------- initialize, update and exit

        // initialize OpenSteerDemo
        //     XXX  if I switch from "totally static" to "singleton"
        //     XXX  class structure this becomes the constructor
        static void initialize (void);

        // main update function: step simulation forward and redraw scene
        static void updateSimulationAndRedraw (void);

        // exit OpenSteerDemo with a given text message or error code
        static void errorExit (const char* message);
        static void exit (int exitCode);

        // ------------------------------------------------------- PlugIn interface

        // select the default PlugIn
        static void selectDefaultPlugIn (void);
        
        // select the "next" plug-in, cycling through "plug-in selection order"
        static void selectNextPlugIn (void);

        // handle function keys an a per-plug-in basis
        static void functionKeyForPlugIn (int keyNumber);

        // return name of currently selected plug-in
        static const char* nameOfSelectedPlugIn (void);

        // open the currently selected plug-in
        static void openSelectedPlugIn (void);

        // do a simulation update for the currently selected plug-in
        static void updateSelectedPlugIn (const float currentTime,
                                          const float elapsedTime);

        // redraw graphics for the currently selected plug-in
        static void redrawSelectedPlugIn (const float currentTime,
                                          const float elapsedTime);

        // close the currently selected plug-in
        static void closeSelectedPlugIn (void);

        // reset the currently selected plug-in
        static void resetSelectedPlugIn (void);

        static const AVGroup& allVehiclesOfSelectedPlugIn(void);

        // ---------------------------------------------------- OpenSteerDemo phase

        static bool phaseIsDraw     (void) {return phase == drawPhase;}
        static bool phaseIsUpdate   (void) {return phase == updatePhase;}
        static bool phaseIsOverhead (void) {return phase == overheadPhase;}

        static float phaseTimerDraw     (void) {return phaseTimers[drawPhase];}
        static float phaseTimerUpdate   (void) {return phaseTimers[updatePhase];}
        // XXX get around shortcomings in current implementation, see note
        // XXX in updateSimulationAndRedraw
        //static float phaseTimerOverhead(void){return phaseTimers[overheadPhase];}
        static float phaseTimerOverhead (void)
        {
            return (clock.getElapsedRealTime() -
                    (phaseTimerDraw() + phaseTimerUpdate()));
        }

        // ------------------------------------------------------ delayed reset XXX

        // XXX to be reconsidered
        static void queueDelayedResetPlugInXXX (void);
        static void doDelayedResetPlugInXXX (void);

        // ------------------------------------------------------ vehicle selection

        // select the "next" vehicle: cycle through the registry
        static void selectNextVehicle (void);

        // select vehicle nearest the given screen position (e.g.: of the mouse)
        static void selectVehicleNearestScreenPosition (int x, int y);

        // ---------------------------------------------------------- mouse support

        // Find the AbstractVehicle whose screen position is nearest the
        // current the mouse position.  Returns NULL if mouse is outside
        // this window or if there are no AbstractVehicles.
        static AbstractVehicle* vehicleNearestToMouse (void);

        // Find the AbstractVehicle whose screen position is nearest the
        // given window coordinates, typically the mouse position.  Note
        // this will return NULL if there are no AbstractVehicles.
        static AbstractVehicle* findVehicleNearestScreenPosition (int x, int y);

        // for storing most recent mouse state
        static int mouseX;
        static int mouseY;
        static bool mouseInWindow;

        // ------------------------------------------------------- camera utilities

        // set a certain initial camera state used by several plug-ins
        static void init2dCamera (AbstractVehicle& selected);
        static void init2dCamera (AbstractVehicle& selected,
                                  float distance,
                                  float elevation);
        static void init3dCamera (AbstractVehicle& selected);
        static void init3dCamera (AbstractVehicle& selected,
                                  float distance,
                                  float elevation);

        // set initial position of camera based on a vehicle
        static void position3dCamera (AbstractVehicle& selected);
        static void position3dCamera (AbstractVehicle& selected,
                                      float distance,
                                      float elevation);
        static void position2dCamera (AbstractVehicle& selected);
        static void position2dCamera (AbstractVehicle& selected,
                                      float distance,
                                      float elevation);

        // camera updating utility used by several (all?) plug-ins
        static void updateCamera (const float currentTime,
                                  const float elapsedTime,
                                  const AbstractVehicle& selected);

        // some camera-related default constants
        static const float camera2dElevation;
        static const float cameraTargetDistance;
        static const Vec3 cameraTargetOffset;

        // ------------------------------------------------ graphics and annotation

        // get drawing view size
        static int drawViewWidth();
        static int drawViewHeight();

        // do all initialization related to graphics
        static void initializeGraphics (void);

        // ground plane grid-drawing utility used by several plug-ins
        static void gridUtility (const Vec3& gridTarget);

        // draws a gray disk on the XZ plane under a given vehicle
        static void highlightVehicleUtility (const AbstractVehicle& vehicle);

        // draws a gray circle on the XZ plane under a given vehicle
        static void circleHighlightVehicleUtility (const AbstractVehicle& vehicle);

        // draw a box around a vehicle aligned with its local space
        // xxx not used as of 11-20-02
        static void drawBoxHighlightOnVehicle (const AbstractVehicle& v,
                                               const Vec3 color);

        // draws a colored circle (perpendicular to view axis) around the center
        // of a given vehicle.  The circle's radius is the vehicle's radius times
        // radiusMultiplier.
        static void drawCircleHighlightOnVehicle (const AbstractVehicle& v,
                                                  const Vec3 color,
                                                  const float radiusMultiplier = 1);

        // graphical annotation: master on/off switch
        static bool annotationIsOn (void) {return enableAnnotation;}
        static void setAnnotationOn (void) {enableAnnotation = true;}
        static void setAnnotationOff (void) {enableAnnotation = false;}
        static bool toggleAnnotationState (void) {return (enableAnnotation = !enableAnnotation);}

        // ----------------------------------------------------------- console text

        // print a line on the console with "OpenSteerDemo: " then the given ending
        static void printMessage (const char* message);
        static void printMessage (const std::ostringstream& message);

        // like printMessage but prefix is "OpenSteerDemo: Warning: "
        static void printWarning (const char* message);
        static void printWarning (const std::ostringstream& message);

        // print list of known commands
        static void keyboardMiniHelp (void);

        // ---------------------------------------------------------------- private

    private:
        static bool enableAnnotation;

        static int phase;
        static int phaseStack[];
        static int phaseStackIndex;
        static float phaseTimers[];
        static float phaseTimerBase;
        static const int phaseStackSize;
        static void pushPhase (const int newPhase);
        static void popPhase (void);
        static void initPhaseTimers (void);
        static void updatePhaseTimers (void);

        // XXX apparently MS VC6 cannot handle initialized static const members,
        // XXX so they have to be initialized not-inline.
        // static const int drawPhase = 2;
        // static const int updatePhase = 1;
        // static const int overheadPhase = 0;
        static const int drawPhase;
        static const int updatePhase;
        static const int overheadPhase;
    };

    namespace Draw {
        // Define scene's camera (viewing transformation) in terms of the camera's
        // position, the point to look at (an "aim point" in the scene which will
        // end up at the center of the camera's view), and an "up" vector defining
        // the camera's "roll" around the "view axis" between cameraPosition and
        // pointToLookAt (the image of the up vector will be vertical in the
        // camera's view).
        void drawCameraLookAt (const Vec3& cameraPosition, const Vec3& pointToLookAt, const Vec3& up);
        // return a normalized direction vector pointing from the camera towards a
        // given point on the screen: the ray that would be traced for that pixel
        Vec3 directionFromCameraToScreenPosition (int x, int y);
        // draw 3d "graphical annotation" lines, used for debugging
        void drawLine (const Vec3& startPoint,
                       const Vec3& endPoint,
                       const Vec3& color);
        // draw a line with alpha blending
        void drawLine (const Vec3& startPoint,
                       const Vec3& endPoint,
                       const Vec3& color,
                       const float alpha);
        // draw a wide line
        void drawWideLine (const Vec3& startPoint,
                           const Vec3& endPoint,
                           const Vec3& color,
                           const float width);
        // General purpose circle/disk drawing routine.  Draws circles or disks (as
        // specified by "filled" argument) and handles both special case 2d circles
        // on the XZ plane or arbitrary circles in 3d space (as specified by "in3d"
        // argument)
        void drawCircle (const float radius,
                         const Vec3& axis,
                         const Vec3& center,
                         const Vec3& color,
                         const int segments,
                         const bool filled = false,
                         const bool in3d = false);
        // General purpose 2d circle/disk drawing routine.  Draws circles or disks
        // on the horizontal (XZ) plane
        void drawCircle (const float radius,
                         const Vec3& center,
                         const Vec3& color,
                         const int segments,
                         const bool filled = false);
        // General purpose quadrangle drawing routine. Draw quadrangle from given 4 corners
        // and color.
        void drawQuadrangle (const Vec3& p1, const Vec3& p2, const Vec3& p3, const Vec3& p4,
                             const Vec3& color);
        // Draw a (filled-in, polygon-based) square checkerboard grid on the horizontal
        // (XZ) plane.
        // ("size" is the length of a side of the overall grid, "subsquares" is the
        // number of subsquares along each edge (for example a standard checkboard
        // has eight), "center" is the 3d position of the center of the grid,
        // color1 and color2 are used for alternating subsquares.)
        void drawCheckerboardGrid (const float size,
                                   const int subsquares,
                                   const Vec3& center,
                                   const Vec3& color1,
                                   const Vec3& color2);
        // Draw the edges of a box with a given position, orientation, size
        // and color. The box edges are aligned with the axes of the given
        // LocalSpace, and it is centered at the origin of that LocalSpace.
        // "size" is the main diagonal of the box.
        void drawBox  (const AbstractLocalSpace& localSpace,
                       const Vec3& size,
                       const Vec3& color,
                       bool filled = false);
        // Draw circle with given color and radius. It is centered
        // at the origin of the given LocalSpace.
        void drawCircle (const AbstractLocalSpace& localSpace,
                         const Vec3& color,
                         float radius,
                         bool filled = false,
                         float up_offset = 0);
        // Draw text at 2d or 3d position
        void drawTextAt2dLocation(const std::ostringstream& text, const Vec3& position, const Vec3& color);
        void drawTextAt2dLocation(const char *text, const Vec3& position, const Vec3& color);
        void drawTextAt3dLocation(const std::ostringstream& text, const Vec3& position, const Vec3& color);
        void drawTextAt3dLocation(const char *text, const Vec3& position, const Vec3& color);
	} // namespace Draw

} // namespace OpenSteer


// ----------------------------------------------------------------------------

#endif // OPENSTEER_APP_H
