//
//  cpm_message_storage_test.h
//  CPMMessageStorage
//
//  Created by jeh on 31/10/2021.
//

#ifndef cpm_message_storage_test_h
#define cpm_message_storage_test_h
#include <cxxtest/TestSuite.h>
#include "filetransferhistory.h"

class MyTestSuite : public CxxTest::TestSuite
{
public:
   void testFileTransferHistory( void )
   {
	   FileTransferHistory fileTransferHistory;
	   fileTransferHistory.headers.FROM = "toi";
	   TS_ASSERT_EQUALS(fileTransferHistory.headers.FROM,"toi");
	   TS_ASSERT_EQUALS(fileTransferHistory.headers.getMap()["From:"],"toi");
	   
	   
   }
};

#endif /* cpm_message_storage_test_h */
