//
//  EntryViewController.m
//  StarfallExample
//
//  Created by lingtonke on 2019/4/15.
//  Copyright © 2019 Starfall. All rights reserved.
//

#import "EntryViewController.h"
#import "RenderViewController.h"

@interface EntryViewController ()

@property (nonatomic) NSArray *list;

@end

@implementation EntryViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.list = @[
                  @"星星龙卷",
                  ];
}

#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return self.list.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"RenderCell" forIndexPath:indexPath];
    
    UILabel *label = cell.contentView.subviews[0];
    label.text = self.list[indexPath.row];
    
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    RenderViewController *vc = [[RenderViewController alloc] init];
    vc.name = [self.list[indexPath.row] stringByAppendingString:@".scene"];
    [self presentViewController:vc animated:YES completion:nil];
}

@end
