//
//  PBPebbleCentral.h
//  PebbleSDK-iOS
//
//  Created by Martijn Thé on 4/24/12.
//  Copyright (c) 2012 Pebble Technology. All rights reserved.
//

#import <Foundation/Foundation.h>

@class PBWatch;
@protocol PBPebbleCentralDelegate;

/**
 PebbleCentral plays the central role for client iOS apps (e.g. RunKeeper).
 */
@interface PBPebbleCentral : NSObject

/**
 @returns The default central singleton instance.
 */
+ (PBPebbleCentral*)defaultCentral;

/**
 The watches that are currently connected. Do not cache the array because it can change over time.
 */
@property (nonatomic, readonly, strong) NSArray *connectedWatches;

/**
 The watches that are stored in the user preferences of the application.
 */
@property (nonatomic, readonly, strong) NSArray *registeredWatches;

/**
 The central's delegate.
 */
@property (nonatomic, readwrite, weak) id<PBPebbleCentralDelegate> delegate;

/**
 @returns YES if the Pebble iOS app is installed, NO if it is not installed.
 */
- (BOOL)isMobileAppInstalled;

/**
 Redirects to Pebble in the App Store, so the user can install the app.
 */
- (void)installMobileApp;

/**
 Wipes out the data associated with the registered watches, that is stored on the phone.
 */
- (void)unregisterAllWatches;

/**
 Returns the most recently connected watch from the -registeredWatches array.
 */
- (PBWatch*)lastConnectedWatch;

@end


@protocol PBPebbleCentralDelegate <NSObject>
@optional

/**
 @param central The Pebble Central responsible for calling the delegate method.
 @param watch The PBWatch object representing the watch that was connected.
 @param isNew YES if the watch has been connected for the first time since the app has been installed or NO if not.
 */
- (void)pebbleCentral:(PBPebbleCentral*)central watchDidConnect:(PBWatch*)watch isNew:(BOOL)isNew;

/**
 @param central The Pebble Central responsible for calling the delegate method.
 @param watch The PBWatch object representing the watch that was disconnected.
 */
- (void)pebbleCentral:(PBPebbleCentral*)central watchDidDisconnect:(PBWatch*)watch;

@end
