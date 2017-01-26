/**
 * @file
 * example.stateful.countExecutions node implementation.
 *
 * @copyright Copyright © 2012–2017 Kosada Incorporated.
 * This code may be modified and distributed under the terms of the MIT License.
 * For more information, see http://vuo.org/license.
 */

#include "node.h"

#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#  define snprintf _snprintf
#endif
#include <stdlib.h>	 				// malloc(), free()
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif
#include <AR/config.h>
#include <AR/video.h>
#include <AR/param.h>			// arParamDisp()
#include <AR/ar.h>
#include <AR/gsub_lite.h>


VuoModuleMetadata({
                     "title" : "detect markers",
					 "description" : "Keeps track of how many times this node has been executed, and outputs that count.",
					 "keywords" : [ ],
					 "version" : "1.0.0",
					 "dependencies" : [ ],
					 "node": {
						 "isInterface" : false
					 }
				 });


// ============================================================================
//	Constants
// ============================================================================

#define VIEW_SCALEFACTOR		1.0         // Units received from ARToolKit tracking will be multiplied by this factor before being used in OpenGL drawing.
#define VIEW_DISTANCE_MIN		40.0        // Objects closer to the camera than this will not be displayed. OpenGL units.
#define VIEW_DISTANCE_MAX		10000.0     // Objects further away from the camera than this will not be displayed. OpenGL units.



// A node class should not define any global variables.
//quindi qui sotto non va bene


// ============================================================================
//	Global variables
// ============================================================================

// Transformation matrix retrieval.
static AR3DHandle	*gAR3DHandle = NULL;
static ARdouble		gPatt_width     = 80.0;	// Per-marker, but we are using only 1 marker.
static ARdouble		gPatt_trans[3][4];		// Per-marker, but we are using only 1 marker.
static int			gPatt_found = FALSE;	// Per-marker, but we are using only 1 marker.
static int			gPatt_id;				// Per-marker, but we are using only 1 marker.



struct nodeInstanceData
{
    ARPattHandle gARPattHandle;   //       static ARPattHandle	*gARPattHandle = NULL;
    VuoInteger gCallCountMarkerDetect;  //       static long			gCallCountMarkerDetect = 0;
    VuoText LoadedPatternFile;
    VuoText LoadedCameraCalibrationFile;
};


static int setupMarker(const char *patt_name, int *patt_id, ARHandle *arhandle, ARPattHandle **pattHandle_p)
{
        if ((*pattHandle_p = arPattCreateHandle()) == NULL) {
            ARLOGe("setupMarker(): Error: arPattCreateHandle.\n");
            return (FALSE);
        }

        // Loading only 1 pattern in this example.
        if ((*patt_id = arPattLoad(*pattHandle_p, patt_name)) < 0) {
            ARLOGe("setupMarker(): Error loading pattern file %s.\n", patt_name);
            arPattDeleteHandle(*pattHandle_p);
            return (FALSE);
        }

        arPattAttach(arhandle, *pattHandle_p);

        return (TRUE);
}

struct nodeInstanceData *nodeInstanceInit()
{
    struct nodeInstanceData *context = (struct nodeInstanceData *) malloc(sizeof(struct nodeInstanceData));
    VuoRegister(context, free);
//    *context = NULL;
    return context;
}

void nodeInstanceEvent
(
        VuoInstanceData(struct nodeInstanceData *) context,
        VuoInputData(VuoText) CameraCalibrationFile,
        VuoInputData(VuoText) PatternFile,
        VuoInputData(VuoInteger) PatternMm,
        VuoInputData(VuoInteger) Threshold,
        VuoInputData(VuoImage) image,
        VuoOutputData(VuoList_VuoInteger) MarkerStructure
)
{

    arUtilTimerReset();   //cosa è??s

    if (!VuoText_areEqual((*context)->LoadedPatternFile,PatternFile)) {
 //    Load marker(s).
    if (!setupMarker((*context)->LoadedPatternFile, &gPatt_id, gARHandle, &gARPattHandle)) {
        ARLOGe("main(): Unable to set up AR marker.\n");
        cleanup();
        exit(-1);
    }
 //   strcpy((*context)->LoadedPatternFile, PatternFile);
    (*context)->LoadedPatternFile=VuoText_make(PatternFile);
    }

    VuoImage gARTImage = image;

    // Detect the markers in the video frame.
    if (arDetectMarker(gARHandle, gARTImage) < 0) {
        exit(-1);
    }

    // Check through the marker_info array for highest confidence
    // visible marker matching our preferred pattern.
    int k = -1;
    int j;
    for (j = 0; j < gARHandle->marker_num; j++) {
        if (gARHandle->markerInfo[j].id == gPatt_id) {
            if (k == -1) k = j; // First marker detected.
            else if (gARHandle->markerInfo[j].cf > gARHandle->markerInfo[k].cf) k = j; // Higher confidence marker detected.
        }
    }

    if (k != -1) {
        // Get the transformation between the marker and the real camera into gPatt_trans.
        err = arGetTransMatSquare(gAR3DHandle, &(gARHandle->markerInfo[k]), gPatt_width, gPatt_trans);
        gPatt_found = TRUE;
    } else {
        gPatt_found = FALSE;
    }

}


void nodeInstanceFini(
        VuoInstanceData(nodeInstanceData *) context
)
{
//    static void cleanup(void)
//    {
        arglCleanup(gArglSettings);
        gArglSettings = NULL;
        arPattDetach(gARHandle);
        arPattDeleteHandle(gARPattHandle);
        arVideoCapStop();
        ar3DDeleteHandle(&gAR3DHandle);
        arDeleteHandle(gARHandle);
        arParamLTFree(&gCparamLT);
//        arVideoClose();
//    }


}
