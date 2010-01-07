/* MyOpenGLView */

#import <Cocoa/Cocoa.h>
#import <OpenGL/gl.h>
#import <QuartzCore/QuartzCore.h>

//include for shared memory
#include <sys/mman.h>

//custom class
#import "PlayerWindow.h"
#import "PlayerFullscreenWindow.h"
#import "PlayListController.h"

#import "Debug.h"

// MPlayer OS X VO Protocol
@protocol MPlayerOSXVOProto
- (int) startWithWidth: (bycopy int)width
            withHeight: (bycopy int)height
             withBytes: (bycopy int)bytes
            withAspect: (bycopy int)aspect;
- (void) stop;
- (void) render;
- (void) toggleFullscreen;
- (void) ontop;
@end

#define		WSM_SCALE		1
#define		WSM_FIT_SCREEN	2
#define		WSM_FIT_WIDTH	3

@interface VideoOpenGLView : NSOpenGLView <MPlayerOSXVOProto>
{
	bool isFullscreen;
	bool switchingToFullscreen;
	bool switchingInProgress;
	bool isClosing;
	bool useFullscreen;
	bool isOntop;
	bool isPlaying;
	bool keepAspect;
	bool panScan;
	NSString *buffer_name;
	
	//CoreVideo
	CVPixelBufferRef currentFrameBuffer;
	CVOpenGLTextureCacheRef textureCache;
	NSRect textureFrame;
    GLfloat	lowerLeft[2]; 
    GLfloat lowerRight[2]; 
    GLfloat upperRight[2];
    GLfloat upperLeft[2];
	
	//video texture
	unsigned char *image_data;
	unsigned char *image_buffer;
	uint32_t image_width;
	uint32_t image_height;
	uint32_t image_bytes;
	float image_aspect;
	float org_image_aspect;
	
	// video size mode
	int videoSizeMode;
	// zoom factor
	float zoomFactor;
	// fit width
	int fitWidth;
	
	//shared memory
	int shm_fd;
	
	// fullscreen switching
	NSSize old_win_size;
	NSRect old_view_frame;
	
	// screen blacking
	NSMutableArray *blackingWindows;
	
	// window dragging
	NSPoint dragStartPoint;
	
	// animations
	unsigned int runningAnimations;
	
	//struct shmid_ds shm_desc;
	
	//Movie menu outlets
	IBOutlet NSMenuItem* HalfSizeMenuItem;
	IBOutlet NSMenuItem* NormalSizeMenuItem;
	IBOutlet NSMenuItem* DoubleSizeMenuItem;
	//IBOutlet id FullScreenMenuItem;
	IBOutlet NSMenuItem* KeepAspectMenuItem;
	IBOutlet NSMenuItem* PanScanMenuItem;
	IBOutlet NSMenuItem* OriginalAspectMenuItem;
	IBOutlet NSMenuItem* Aspect4to3MenuItem;
	IBOutlet NSMenuItem* Aspect3to2MenuItem;
	IBOutlet NSMenuItem* Aspect5to3MenuItem;
	IBOutlet NSMenuItem* Aspect16to9MenuItem;
	IBOutlet NSMenuItem* Aspect185to1MenuItem;
	IBOutlet NSMenuItem* Aspect239to1MenuItem;
	IBOutlet NSMenuItem* CustomAspectMenuItem;
	
	// other controllers outlets
	IBOutlet id playerController;
	
	NSThread *renderThread;
	
	IBOutlet NSWindow *fcControlWindow;
	IBOutlet PlayerFullscreenWindow* fullscreenWindow;
}

// Render Thread methods
- (void)threadMain;
- (void)prepareOpenGL;
- (int) startWithWidth: (int)width withHeight: (int)height withBytes: (int)bytes withAspect: (int)aspect;
- (void) stop;
- (void) render;
- (void) doRender;
- (void) clear;
- (void) adaptSize;
- (void) toggleFullscreen;
- (void) finishToggleFullscreen;
- (void) updateInThread;
- (void) drawRectInThread;

// Main Thread methods
- (NSString *)bufferName;
- (void) startOpenGLView;
- (BOOL) isFullscreen;
- (void) toggleFullscreenWindow;
- (void) toggleFullscreenWindowContinued;
- (void) toggleFullscreenEnded;
- (void) blackScreensExcept:(int)fullscreenId;
- (void) unblackScreens;
- (void) reshape;
- (void) resizeView;
- (void) reshapeAndResize;
- (void) close;
- (void) finishClosing;
- (void) setWindowSizeMode:(int)mode withValue:(float)val;
- (void) ontop;
- (void) setOntop:(BOOL)ontop;
- (void) updateOntop;
- (void) setAspectRatio:(float)aspect;
- (void) setAspectRatioFromPreferences;
- (void) toggleKeepAspect;
- (void) togglePanScan;

//Event
- (void) mouseDown: (NSEvent *) theEvent;

// Helper methods
- (void) setFrame:(NSRect)frame onWindow:(NSWindow *)window blocking:(BOOL)blocking;
- (void) fadeWindow:(NSWindow *)window withEffect:(NSString *)effect;

@end
