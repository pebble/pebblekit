//
//  EAAccessoryFramework+OCMock.h
//  PebbleKit
//
//  Created by Martijn The on 7/30/12.
//  Copyright (c) 2012 Pebble Technology. All rights reserved.
//

#import <ExternalAccessory/ExternalAccessory.h>

@interface EAAccessory (OCMock)
+ (EAAccessory*)pebbleMock;
@end

@interface EAAccessoryManager (OCMock)
+ (EAAccessoryManager*)mockManager;
@end

@interface EASession (OCMock)
+ (EASession*)mockSessionWithAccessory:(EAAccessory*)mockAccessory protocolString:(NSString*)protocol inputStream:(NSInputStream*)inStream outputStream:(NSOutputStream*)outStream;
@end