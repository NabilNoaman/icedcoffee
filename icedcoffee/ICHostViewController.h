//  
//  Copyright (C) 2012 Tobias Lensing
//  
//  Permission is hereby granted, free of charge, to any person obtaining a copy of
//  this software and associated documentation files (the "Software"), to deal in
//  the Software without restriction, including without limitation the rights to
//  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
//  of the Software, and to permit persons to whom the Software is furnished to do
//  so, subject to the following conditions:
//  
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
//  
//  Note: ICHostViewController was inspired by CCDirector's design from the cocos2d project.

#import <Foundation/Foundation.h>

#import "ICResponder.h"
#import "ICEventDelegate.h"

#import "icMacros.h"

@class ICScene;
@class ICGLView;
@class ICTextureCache;
@class ICRenderTexture;
@class ICResponder;
@class ICScheduler;


#if defined(__IC_PLATFORM_MAC)
#define IC_VIEWCONTROLLER NSObject
#elif defined(__IC_PLATFORM_IOS)
#define IC_VIEWCONTROLLER UIViewController
#endif


/**
 @brief View controller base class for displaying and updating an IcedCoffee framebuffer
 
 <h3>Overview</h3>
 
 ICHostViewController defines a view controller for platform-specific views (host views.)
 The class provides the following main functionality:
 <ul>
    <li><b>Rendering OpenGL scenes</b>: the class renders an ICScene object defined
    by a call to ICHostViewController::runWithScene: when the application is initialized.
    It uses a CoreVideo (Mac) or CoreAnimation (iOS) display link callback to perform
    frame updates continuously, synchronized to the refresh rate of the device's display.</li>
    <li><b>Scheduling updates for animations</b>: [TODO]</li>
    <li><b>Dispatching HID events</b>: HID events are dispatched to the scene rendered
    by ICHostViewController on the display link thread. You may handle these events
    using specialized ICNode objects or by subclassing ICScene.</li>
    <li><b>Retina display support</b>: retina display support may be enabled using
    the enableRetinaDisplaySupport: method. The class provides access to a globally
    defined content scale factor, which is set to 1 for non-retina displays or to 2
    when retina display support is enabled.</li>
    <li><b>Providing access to auxiliary managers</b>: ICHostViewController manages
    an ICTextureCache object (see textureCache property.) The texture cache is bound
    to the view controlled by the ICHostViewController instance.</li>
 </ul>
 
 <h3>Subclassing</h3>

 ICHostViewController is an abstract base class, that is it needs to be specialized for
 each supported platform. IcedCoffee currently ships with two built-in platform-specific
 subclasses: ICHostViewControllerIOS and ICHostViewControllerMac.  Depending on the
 target platform, ICHostViewController may inherit from different super classes. On
 the Mac it is a subclass of NSObject while on iOS it inherits from UIViewController.

 As a framework user, you should subclass the ICHostViewControllerIOS or
 ICHostViewControllerMac class if you require a custom view controller for managing your
 application's native OS view(s). Note, however, that in a standard application there
 usually is no need to subclass the host view controller class.
 */
@interface ICHostViewController : IC_VIEWCONTROLLER <EVENT_PROTOCOLS>
{
@protected
    ICScene *_scene;
    
    ICTextureCache *_textureCache;
    ICScheduler *_scheduler;
    
    ICResponder *_currentFirstResponder;
    NSMutableArray *_eventDelegates;
    
    BOOL _retinaDisplayEnabled;

    BOOL _isRunning;
    NSThread *_thread;
    CGSize _viewSize;

    icTime _deltaTime;
    struct timeval _lastUpdate;
}


#pragma mark - Retina Display Support
/** @name Initialization */

/**
 @brief Instanciates an ICHostViewController subclass suitable for the target platform
 */
+ (ICHostViewController *)platformSpecificHostViewController;


#pragma mark - Event Handling
/** @name Event Handling */

/**
 @brief The current first responder
 */
@property (nonatomic, retain, setter=setCurrentFirstResponder:) ICResponder *currentFirstResponder;

@property (nonatomic, readonly) NSArray *eventDelegates;

- (void)addEventDelegate:(id<ICEventDelegate>)eventDelegate;

- (void)removeEventDelegate:(id<ICEventDelegate>)eventDelegate;


#pragma mark - Caches and Management
/** @name Caches and Management */

/**
 @brief The ICTextureCache object associated with the host view controller
 */
@property (nonatomic, readonly) ICTextureCache *textureCache;


#pragma mark - Run Loop, Drawing and Animation
/** @name Run Loop, Drawing and Animation */

/**
 @brief The thread used to draw the scene and process HID events
 */
@property (nonatomic, readonly) NSThread *thread;

/**
 @brief The scene that is currently drawn by the host view controller
 */
@property (nonatomic, retain) ICScene *scene;

/**
 @brief Draws the scene in the host view's frame buffer
 */
- (void)drawScene;

/**
 @brief Sets the given scene as the controller's current scene and starts animation
 */
// FIXME: runWithScene is deprecated, should assign scene, then start animation if necessary
// only. On iOS, animation is started automatically via UIViewController::viewWillAppear.
- (void)runWithScene:(ICScene *)scene;

/**
 @brief A boolean flag indicating whether the host view controller is currently running
 */
@property (nonatomic, readonly) BOOL isRunning;

/**
 @brief Starts continuous drawing of the currently set scene
 */
- (void)startAnimation;

/**
 @brief Stops continuous drawing of the currently set scene
 */
- (void)stopAnimation;

/**
 @brief Updates the view's size internally and invalidates the current scene's camera
 */
- (void)reshape:(CGSize)newViewSize;


#pragma mark - Update Scheduling
/** @name Update Scheduling */

@property (nonatomic, readonly) ICScheduler *scheduler;


#pragma mark - Host View
/** @name Host View */

/**
 @brief Sets the view associated with the host view controller
 */
- (void)setView:(ICGLView *)view;

#if defined(__MAC_OS_X_VERSION_MAX_ALLOWED)
- (ICGLView *)view;
#endif

/**
 @brief The size of the view controller's view in points
 */
@property (nonatomic, assign) CGSize viewSize;


#pragma mark - Hit Testing
/** @name Hit Test */

/**
 @brief Performs a hit test on the current scene
 @sa ICScene::hitTest:
 */
- (NSArray *)hitTest:(CGPoint)point;


#pragma mark - Retina Display Support
/** @name Retina Display Support */

/**
 @brief Enables or disables retina display support (iOS only)
 
 @param retinaDisplayEnabled Set to YES to enable retina display support or to NO to disable it.
 
 @remarks Retina display support can only be enabled when appropriate hardware and software
 are available on the device.
 
 @return Returns YES if retina display support could be enabled or NO otherwise.
 */
- (BOOL)enableRetinaDisplaySupport:(BOOL)retinaDisplayEnabled;

/**
 @brief Returns YES when retina display support is enabled, NO otherwise
 */
- (BOOL)retinaDisplaySupportEnabled;

/**
 @brief Returns the current content scale factor used to map points to pixels internally
 */
- (float)contentScaleFactor;

/**
 @brief Sets the current content scale factor
 */
- (void)setContentScaleFactor:(float)contentScaleFactor;


@end

