#import "LogViewController.h"
@implementation LogDrawerController

- (void)SetupLogDrawer {
    //[logDrawer setMinContentSize:NSMakeSize(662, 100)];
    //[logDrawer setMaxContentSize:NSMakeSize(400, 400)];
    //[logDrawer setContentSize:NSMakeSize(662, 100)];
    //[logDrawer setMinContentSize:[mainWindow contentMinSize]];
    //[logDrawer setMaxContentSize:[mainWindow contentMaxSize]];
}

- (void)ToggleLogDrawer:(id)sender
{
    NSDrawerState state = [logDrawer state];
    if (NSDrawerOpeningState == state || NSDrawerOpenState == state)
    {
        [logDrawer close];
    }
    else
    {
        [logDrawer openOnEdge:NSMinYEdge];
    }
}

- (void)setLogDrawerOffsets
{
    NSSize frameSize = [mainWindow frame].size;
    NSLog(@"Framesize Width: %d FrameSize Height: %d", frameSize.width, frameSize.height);
    [logDrawer setLeadingOffset: (50 / frameSize.width)];
    // we want a bottomDrawer width of approximately 220 unscaled.  Figure out an offset to accomplish that size.
    //CGFloat bottomDrawerWidth = 220 * [mainWindow userSpaceScaleFactor];
    //[logDrawer setTrailingOffset:frameSize.width - 50];
    //[logDrawer setLeadingOffset:100];
}


- (void)windowDidResize:(NSNotification *)notification {
    NSLog(@"Windows Resize Occured!\n");
    [self setLogDrawerOffsets];
}


- (void)awakeFromNib
{
    NSLog(@"Setup Log Drawer");
    [self SetupLogDrawer];
}


@end