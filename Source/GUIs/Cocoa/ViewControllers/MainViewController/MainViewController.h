/*!
 *  \brief     Main ViewController for the StrataExtract GUI for OSX
 *  \details
 *  \author    Bradley Clemetson
 *  \version   0.01
 *  \date      July 6, 2013
 *  \copyright GPLv2
 */

#import <AppKit/AppKit.h>
@interface MainViewController : NSWindow
{
    IBOutlet NSWindow *mainWindow;
    IBOutlet NSButton *extractButton;
    IBOutlet NSButton *cancelButton;
    IBOutlet NSPathControl *configurationPath;

    NSArray *configFilePaths;
    NSArray *gameSourceFilePaths;
    NSArray *gameDestinationPaths;
}

//!Choose game configuration file path
/*!  Allows the user to graphically select the game configuration file
 *  \pre NA
 *  \post Sets the file path if valid, otherwise NULL
 *  \note NA*/
- (IBAction)chooseGameConfigFilePath:(id)sender;

//!Choose game source media directory
/*!  Allows the user to graphically select the game configuration file
 *  \pre NA
 *  \post Sets the file path if valid, otherwise NULL
 *  \note NA*/
- (IBAction)chooseGameSourceFilePath:(id)sender;

//!Choose game destination media directory
/*!  Allows the user to graphically select the game configuration file
 *  \pre NA
 *  \post Sets the file path if valid, otherwise NULL
 *  \note NA*/
- (IBAction)chooseGameDestinationFilePath:(id)sender;

//!Begins extracting the media assets
/*!  Starts the process of media extraction
 *  \pre gameMediaSource & configurationFilePath must be valid
 *  \post Runs final checks to ensure the data is valid and process the assets
 *  \note NA*/
- (IBAction)startExtraction:(id)sender;

//!Stops the extraction of media assets
/*!  Stop the current extraction of media process
 *  \pre Extraction process must be running
 *  \post If files are being extracted, cancels the process and cleans up
 *  \note NA*/
- (IBAction)stopExtraction:(id)sender;



@end