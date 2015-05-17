#import "LogViewController.h"
@implementation LogDrawerController

- (void)ToggleLogDrawer:(id)sender
{
}

- (void)windowDidResize:(NSNotification *)notification
{
}


- (void)awakeFromNib
{
    [logOutput setString:@"StrataExtract GUI Initialized\n"];
}


@end