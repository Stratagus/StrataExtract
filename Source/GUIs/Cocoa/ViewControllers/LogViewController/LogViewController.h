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
    IBOutlet NSTextView *logOutput;
    
}

//!Toggles the Log Drawer
/*!  Toggles whether to show the verbose StrataCore information on the
 *  bottom of the window
 *  \pre NA
 *  \post NA
 *  \note NA*/
- (IBAction)ToggleLogDrawer:(id)sender;


@end