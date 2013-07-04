//
//  PBWatch+AppMessages.h
//  PebbleKit
//
//  Created by Martijn The on 3/20/13.
//  Copyright (c) 2013 Pebble Technology. All rights reserved.
//

#import <PebbleKit/PebbleKit.h>

@interface PBWatch (AppMessages)

/**
 *  Queries the watch whether AppMessages are supported.
 *  @param fetchedBlock The block that will be called whenthe inquiry has finished. The fetchedBlock will be called
 *  asynchronously on the queue that was originally used when calling this method.
 *  @param watch The watch on which the query was performed.
 *  @param isAppMessagesSupported YES if AppMessages are supported, NO if not.
 *  @note This API is the only AppMessages method that can (and should) be used before -appMessagesSetUUID:
 */
- (void)appMessagesGetIsSupported:(void(^)(PBWatch *watch, BOOL isAppMessagesSupported))fetchedBlock;

/**
 *  Configures the UUID of the application. The UUID is used as the identifier of the watch/phone app pair and is used
 *  to make sure that the message that an app sends, arrives at its companion app on the other device (and not in another app).
 *  @param uuid The 16 byte UUID of your app.
 *  @note When -appMessagesSetUUID: is called, the UUID will be used for all incoming and outgoing messages thereafter.
 *  The UUID needs to be set before doing any other AppMessage operations (with one exeception, -appMessagesGetIsSupported: ).
 */
- (void)appMessagesSetUUID:(NSData*)uuid;

/**
 *  Returns the current UUID of the application as set earlier using -appMessagesSetUUID:
 *  @return The UUID of your app
 *  @see -appMessagesSetUUID:
 */
- (NSData*)appMessagesUUID;

/**
 *  Verifies the currently set application UUID.
 *  @return YES if the currently set UUID is valid, NO if it is not.
 *  @see -appMessagesSetUUID:
 */
- (BOOL)appMessagesHasValidAppUUID;

/**
 *  Pushes an update to the accompanying watch application.
 *  @param dictionary Contains the key/value pairs to update. The dictionary can only contain items with an NSNumber
 *  key and only contain NSString, NSNumber or NSData values. Use the methods in the NSNumber (stdint) category to
 *  create NSNumbers with specific types standard integer types.
 *  @param onSent The block that will be called when the message was accepted, rejected or timed out.
 *  @param watch The watch to which the update was sent.
 *  @param update The dictionary that was sent.
 *  @param error If there was a problem, this will contain information about the underlying problem. See PBError.h for error codes.
 */
- (void)appMessagesPushUpdate:(NSDictionary*)dictionary onSent:(void(^)(PBWatch *watch, NSDictionary *update, NSError *error))onSent;

/**
 *  Add a receive handler for incoming updates that are send by the watch application.
 *  @param onReceive The block that will be called every time a new update message arrives.
 *  @param watch The watch that has sent the update.
 *  @param update The dictionary containing the values sent by the watch.
 *  @return An opaque handle object representing the installed receive handler, that can be used in -appMessagesRemoveUpdateHandler:
 *  @see -appMessagesRemoveUpdateHandler:
 */
- (id)appMessagesAddReceiveUpdateHandler:(BOOL(^)(PBWatch *watch, NSDictionary *update))onReceive;

/**
 *  Removes a receive handler that was previously installed using -appMessagesAddReceiveUpdateHandler:
 *  @param opaqueHandle The handle object as returned by -appMessagesAddReceiveUpdateHandler:
 *  @see -appMessagesAddReceiveUpdateHandler:
 */
- (void)appMessagesRemoveUpdateHandler:(id)opaqueHandle;

/**
 *  Sends a command to launch the application on the watch.
 *  @param watch The watch to which the command was sent.
 *  @param error If there was a problem, this will contain information about the underlying problem. See PBError.h for error codes.
 */
- (void)appMessagesLaunch:(void(^)(PBWatch *watch, NSError *error))onSent;

/**
 *  Sends a command to kill the application on the watch.
 *  @param watch The watch to which the command was sent.
 *  @param error If there was a problem, this will contain information about the underlying problem. See PBError.h for error codes.
 */
- (void)appMessagesKill:(void(^)(PBWatch *watch, NSError *error))onSent;

@end
