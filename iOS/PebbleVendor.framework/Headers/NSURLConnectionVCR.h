//
//  NSURLConnectionVCR.h
//
//  Created by Martijn Thé on 19-02-12.
//  Copyright (c) 2012 martijnthe.nl All rights reserved.
//

#import <Foundation/Foundation.h>

extern NSString* NSURLConnectionVCRErrorDomain;

enum NSURLConnectionVCRErrorCodes {
    NSURLConnectionVCRErrorAlreadyStarted = 0x8BADF00D,
    NSURLConnectionVCRErrorAlreadyStopped,
    NSURLConnectionVCRErrorCouldNotCreateDirectory,
    NSURLConnectionVCRErrorPathIsNotADirectory,
    NSURLConnectionVCRErrorStorageFailed,
};


/**
 *  NSURLConnectionVCR provides a way to re-play NSURLConnection HTTP responses.
 *  It is inspired on Ruby's VCR.
 */

@interface NSURLConnectionVCR : NSObject


/**
 *	Start the VCR.
 *	@param path VCR will use this path to store and retrieve caches.
 *	@param error On input, a pointer to an error object.
 *	@returns YES if VCR was started successfully, and NO in case of an error.
 *  @see NSURLConnectionVCRErrorCodes for error codes that are used in assigned error objects.
 */
+ (BOOL)startVCRWithPath:(NSString*)path error:(NSError**)error;


/**
 *	Stop the VCR.
 *	@param error On input, a pointer to an error object.
 *	@returns YES if VCR was stopped successfully, and NO in case of an error.
 *  @see NSURLConnectionVCRErrorCodes for error codes that are used in assigned error objects.
 */
+ (BOOL)stopVCRWithError:(NSError**)error;


/**
 *	@returns YES if VCR is running, NO if it is stopped.
 */
+ (BOOL)isVCRStarted;


/**
 *	Set the path where VCR will store and retrieve caches.
 *	@param path VCR will use this path to store and retrieve caches.
 *	@param error On input, a pointer to an error object.
 *	@returns YES if the path was set successfully, and NO in case of an error.
 *  @see NSURLConnectionVCRErrorCodes for error codes that are used in assigned error objects.
 */
+ (BOOL)setPath:(NSString*)path error:(NSError **)error;


/**
 *	@returns The currently set path where VCR will store and retrieve caches.
 */
+ (NSString*)path;


/**
 *	Returns a Boolean value that indicates whether VCR can find a
 *  cache for the given request at the currently set path.
 *	@param request The request of which VCR needs to check its existence
 *	@returns YES if a cache for request can be found at the currently set path, NO if there was none.
 *  @see +setPath:error:
 *  @see +path
 */
+ (BOOL)hasCacheForRequest:(NSURLRequest*)request;


/**
 *	Deletes the cache (if existing) for the given request at the currently active path.
 *	@param request The request for which the cache will be deleted.
 *	@param error On input, a pointer to an error object.
 *	@returns YES if the cache was successfully deleted, or NO if an error occured.
 *  @see NSURLConnectionVCRErrorCodes for error codes that are used in assigned error objects.
 *  @see +setPath:error:
 *  @see +path
 */
+ (BOOL)deleteCacheForRequest:(NSURLRequest*)request error:(NSError**)error;

@end
