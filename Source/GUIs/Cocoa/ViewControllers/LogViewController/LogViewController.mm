#import "LogViewController.h"
@implementation LogDrawerController

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

- (void)windowDidResize:(NSNotification *)notification
{
}


- (void)awakeFromNib
{
    [logOutput setString:@"StrataExtract GUI Initialized\n"];
}


@end