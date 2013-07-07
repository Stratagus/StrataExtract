/*!
 *  \brief     The viewcontroller for the log view that appears as a NSDrawer
 *  \details
 *  \author    Bradley Clemetson
 *  \version   0.01
 *  \date      June 30, 2013
 *  \copyright GPLv2
 */

#import <AppKit/AppKit.h>
@interface MainViewController : NSObject <NSWindowDelegate>
{
    IBOutlet NSWindow *mainWindow;
    IBOutlet NSButton *extractButton;
    IBOutlet NSButton *cancelButton;
    IBOutlet NSPathControl *configurationPath;

    NSArray *configFilePaths;
    NSArray *gameSourceFilePaths;
    NSArray *gameDestinationPaths;
}


- (IBAction)chooseGameConfigFilePath:(id)sender;
- (IBAction)chooseGameSourceFilePath:(id)sender;
- (IBAction)chooseGameDestinationFilePath:(id)sender;

- (IBAction)startExtraction:(id)sender;
- (IBAction)stopExtraction:(id)sender;



@end