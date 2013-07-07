#import "MainViewController.h"
@implementation MainViewController

- (void)chooseGameConfigFilePath:(id)sender
{
    // Create the File Open Dialog class.
    NSOpenPanel* openConfigurationFileDialog =  [[NSOpenPanel alloc] init];
    
    // Enable the selection of files in the dialog.
    [openConfigurationFileDialog setCanChooseFiles:YES];
    //Do not allow to hide
    [openConfigurationFileDialog setCanHide:YES];
    
    // Enable the selection of directories in the dialog.
    [openConfigurationFileDialog setCanChooseDirectories:NO];
    
    if ([openConfigurationFileDialog runModal] == NSOKButton)
    {
        NSLog(@"Pressed ok, pass to strataconfig");
    }
    else
    {
        NSLog(@"Pressed Cancel, pass null");
    }
    
    [openConfigurationFileDialog release];
    
}
- (void)chooseGameSourceFilePath:(id)sender
{
    // Create the File Open Dialog class.
    NSOpenPanel* openGameSourceFileDialog =  [[NSOpenPanel alloc] init];
    
    // Enable the selection of files in the dialog.
    [openGameSourceFileDialog setCanChooseFiles:NO];
    //Do not allow to hide
    [openGameSourceFileDialog setCanHide:YES];
    
    // Enable the selection of directories in the dialog.
    [openGameSourceFileDialog setCanChooseDirectories:YES];
    
    if ([openGameSourceFileDialog runModal] == NSOKButton)
    {
        NSLog(@"Pressed ok, pass to strataconfig");
    }
    else
    {
        NSLog(@"Pressed Cancel, pass null");
    }
    
    [openGameSourceFileDialog release];
}
- (void)chooseGameDestinationFilePath:(id)sender
{
    // Create the File Open Dialog class.
    NSOpenPanel* openGameDestinationFileDialog =  [[NSOpenPanel alloc] init];
    
    // Enable the selection of files in the dialog.
    [openGameDestinationFileDialog  setCanChooseFiles:NO];
    //Do not allow to hide
    [openGameDestinationFileDialog  setCanHide:YES];
    
    // Enable the selection of directories in the dialog.
    [openGameDestinationFileDialog  setCanChooseDirectories:YES];
    
    if ([openGameDestinationFileDialog  runModal] == NSOKButton)
    {
        NSLog(@"Pressed ok, pass to strataconfig");
    }
    else
    {
        NSLog(@"Pressed Cancel, pass null");
    }
    
    [openGameDestinationFileDialog  release];
}

- (void)startExtraction:(id)sender
{
    NSLog(@"Starting Extraction");
}
- (void)stopExtraction:(id)sender
{
    NSLog(@"Stop Extraction");
}

- (void)windowDidResize:(NSNotification *)notification
{
}


- (void)awakeFromNib
{
    //[configurationPath setURL:@"/Applications"];
}
@end