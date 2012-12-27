//
//  WIDAppDelegate.m
//  ShaderTest
//
//  Created by Home on 2012-12-27.
//  Copyright (c) 2012 Tobias Lensing. All rights reserved.
//

#import "WIDAppDelegate.h"

NSString *coolBackgroundShaderFSH = IC_SHADER_STRING(
#ifdef GL_ES
    precision mediump float;
#endif
    
    uniform float time;
    uniform vec2 resolution;
    
    void main( void )
    {
        vec2 uPos = ( gl_FragCoord.xy / resolution.xy );//normalize wrt y axis
        //suPos -= vec2((resolution.x/resolution.y)/2.0, 0.0);//shift origin to center
        
        uPos.x -= 0.5;
        uPos.y -= 0.5;
        
        float vertColor = 0.0;
        //for( float i = 0.0; i < 10.0; ++i )
        {
            float t = time * ( 0.5 );
            
            uPos.x += sin( uPos.y * 1.0 + t ) * 0.5;
            uPos.y += cos( uPos.x * 1.0 + t ) * 0.5;
            
            float fTemp = abs(1.0 / uPos.x/uPos.y / 50.0);
            vertColor += fTemp;
        }
        
        vec4 color = vec4( vertColor * 2.5, vertColor * 0.5, 0.2+ vertColor * 2.0, 1.0 );
        gl_FragColor = color;
    }
);

@implementation WIDAppDelegate
{
    ICLabel* _fpsLabel;
}

- (void)update:(icTime)dt
{
    _fpsLabel.text = [NSString stringWithFormat:@"%.1f", 1.0f/dt];
}

- (void)setUpScene
{
    self.scene = [ICScene scene];
    self.scene.performsDepthTesting = YES;
    
    self.scene.clearColor = (icColor4B){0,0,255,255};
    
    IC_CHECK_GL_ERROR_DEBUG();
    
    [self.hostViewController runWithScene:self.scene];
    
    [self.hostViewController.scheduler scheduleUpdateForTarget:self];
    
    ICShaderCache *shaderCache = [ICShaderCache currentShaderCache];
    ICAnimatedShaderProgram *shader =
    [ICAnimatedShaderProgram shaderProgramWithName:@"CoolBackgroundShader"
                                vertexShaderString:[shaderCache.shaderFactory vertexShaderStringForKey:ICShaderPositionTextureColor]
                              fragmentShaderString:coolBackgroundShaderFSH];
    [shader addAttribute:ICAttributeNamePosition index:0];
    [shader addAttribute:ICAttributeNameColor index:1];
    [shader addAttribute:ICAttributeNameTexCoord index:2];
    [shader link];
    
    IC_CHECK_GL_ERROR_DEBUG();
    
    ICSprite *shaderSprite = [ICSprite sprite];
    shaderSprite.size = self.scene.size;
    shaderSprite.shaderProgram = shader;
    [shaderSprite.shaderProgram setShaderValue:[ICShaderValue shaderValueWithVec2:kmVec2Make(self.scene.size.x, self.scene.size.y)]
                                    forUniform:@"resolution"];
    shaderSprite.tag = 1;
    [self.scene addChild:shaderSprite];
    
    _fpsLabel = [ICLabel labelWithText:@"TTT" fontName:@"Arial" fontSize:18.0f];
    _fpsLabel.position = kmVec3Make(0, 0, 0);
    _fpsLabel.autoresizesToTextSize = YES;
    [self.scene addChild:_fpsLabel];
    
    
    [self.scene addObserver:self forKeyPath:@"size" options:NSKeyValueObservingOptionNew context:nil];
    [self.scene addChild:shaderSprite];
}

- (void)observeValueForKeyPath:(NSString *)keyPath
                      ofObject:(id)object
                        change:(NSDictionary *)change
                       context:(void *)context
{
    kmVec3 size;
    [[change objectForKey:NSKeyValueChangeNewKey] getValue:&size];
    ICSprite *shaderSprite = (ICSprite *)[self.scene childForTag:1];
    shaderSprite.size = size;
    [shaderSprite.shaderProgram setShaderValue:[ICShaderValue shaderValueWithVec2:kmVec2Make(self.scene.size.x, self.scene.size.y)]
                                    forUniform:@"resolution"];
}


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    self.hostViewController = [ICHostViewController platformSpecificHostViewController];
    
    // Initialize a GL view with depth buffer support
    ICGLView *glView = [ICGLView viewWithFrame:[self.window bounds]
                                   pixelFormat:kEAGLColorFormatRGBA8
                                   depthFormat:GL_DEPTH24_STENCIL8_OES
                            preserveBackbuffer:NO
                                    sharegroup:nil
                                 multiSampling:NO
                               numberOfSamples:0];
    
    [glView setHostViewController:self.hostViewController];
    [self.hostViewController enableRetinaDisplaySupport:YES];
    
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    // Override point for customization after application launch.
    self.window.rootViewController = self.hostViewController;
    [self.window makeKeyAndVisible];
    [self setUpScene];
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

@end
