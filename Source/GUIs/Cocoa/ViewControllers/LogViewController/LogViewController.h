/*!
 *  \brief     The viewcontroller for the log view that appears as a NSDrawer
 *  \details
 *  \author    Bradley Clemetson
 *  \version   0.01
 *  \date      June 30, 2013
 *  \copyright GPLv2
 */

#import <AppKit/AppKit.h>
@interface LogDrawerController : NSObject <NSDrawerDelegate>
{
    IBOutlet NSWindow *mainWindow;
    IBOutlet NSDrawer *logDrawer;
    
}

- (IBAction)ToggleLogDrawer:(id)sender;


@end