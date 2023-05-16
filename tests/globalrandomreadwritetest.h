#include <cxxtest/TestSuite.h>
#include <random>
#include "cpmmanager.h"
#include "conversationhistory.h"
#include "sessionhistory.h"
#include "message.h"
#include "filetransferhistory.h"
#include "media.h"

class GlobalRandomReadWriteTest : public CxxTest::TestSuite {

    public:

        void setUp() {
            if (filesystem::exists(managerPath) and filesystem::is_directory(managerPath))
                filesystem::remove_all(managerPath.c_str());
            filesystem::create_directory(managerPath.c_str());
        }

        void testGlobal(void) {
            shared_ptr<CpmManager> manager;
            TS_ASSERT_THROWS_NOTHING(manager = make_shared<CpmManager>(managerPath,managerName));
            for (int i = 0; i < dist100(gen); i++) {
                string name = randomString(15);
                shared_ptr<ConversationHistory> conversationHistory;
                TS_ASSERT_THROWS_NOTHING(conversationHistory = make_shared<ConversationHistory>(name));
                TS_ASSERT_THROWS_NOTHING(manager->append(conversationHistory));

                for (int i = 0; i < dist100(gen); i++) {
                    string name = randomString(15);
                    shared_ptr<SessionHistory> sessionHistory;
                    TS_ASSERT_THROWS_NOTHING(sessionHistory = make_shared<SessionHistory>());
                    sessionHistory->sessionInfo->headers.CONTRIBUTION_ID = randomString(dist30(gen));
                    sessionHistory->sessionInfo->headers.CONVERSATION_ID = randomString(dist30(gen));
                    sessionHistory->sessionInfo->headers.DATE = randomString(dist30(gen));
                    sessionHistory->sessionInfo->headers.FROM = randomString(dist30(gen));
                    sessionHistory->sessionInfo->xcpm.headers.INVITED_PARTICIPANTS = randomStringList(10,dist10(gen));
                    sessionHistory->sessionInfo->xcpm.headers.TYPE = uniform_int_distribution<>(0,2)(gen);
                    sessionHistory->groupState->headers.GROUP_TYPE = randomString(dist30(gen));
                    sessionHistory->groupState->headers.LASTFOCUSSESSIONID = randomString(dist30(gen));
                    sessionHistory->groupState->headers.TIMESTAMP = randomString(dist30(gen));
                    TS_ASSERT_THROWS_NOTHING(conversationHistory->append(sessionHistory));

                    for (int i = 0; i < dist100(gen); i++) {
                        shared_ptr<Message> message;
                        TS_ASSERT_THROWS_NOTHING(message = make_shared<Message>());
                        message->headers.FROM = randomString(dist30(gen));
                        message->headers.CONTENT_TYPE = randomString(dist30(gen));
                        message->headers.DATE = randomString(dist30(gen));
                        message->headers.TO = randomString(dist30(gen));
                        TS_ASSERT_THROWS_NOTHING(sessionHistory->append(message));
                    }

                    for (int i = 0; i < dist30(gen); i++) {
                        shared_ptr<FileTransferHistory> fileTransferHistory;
                        TS_ASSERT_THROWS_NOTHING(fileTransferHistory = make_shared<FileTransferHistory>());
                        fileTransferHistory->headers.CONTRIBUTION_ID = randomString(dist30(gen));
                        fileTransferHistory->headers.CONVERSATION_ID = randomString(dist30(gen));
                        fileTransferHistory->headers.CONTENT_TYPE = randomString(dist30(gen));
                        fileTransferHistory->headers.DATE = randomString(dist30(gen));
                        fileTransferHistory->headers.FROM = randomString(dist30(gen));
                        fileTransferHistory->xcpm.headers.INVITED_PARTICIPANTS = randomStringList(10,dist10(gen));
                        fileTransferHistory->xcpm.headers.TYPE = uniform_int_distribution<>(0,2)(gen);

                        for (int i = 0; i < dist30(gen); i++) {
                            Media media;
                            TS_ASSERT_THROWS_NOTHING(media = Media());
                            media.headers.TRANSFER_ENCODING = randomString(dist30(gen));
                            media.headers.TYPE = randomString(dist30(gen));
                            media.headers.ID = randomString(dist30(gen));
                            TS_ASSERT_THROWS_NOTHING(fileTransferHistory->xcpm.addMedia(media));
                        }

                        TS_ASSERT_THROWS_NOTHING(sessionHistory->append(fileTransferHistory));
                    }

                }
            }
        }

    private:
        string managerPath = createPath({"..","build","Testing","Temporary","cpm_data"});
        string managerName = "GlobalTest";
        string allowedChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890()_- ";

        mt19937 gen = mt19937(random_device{}());
        uniform_int_distribution<> dist100 = uniform_int_distribution<>(30, 100);
        uniform_int_distribution<> dist30 = uniform_int_distribution<>(10, 30);
        uniform_int_distribution<> dist10 = uniform_int_distribution<>(3, 10);
        uniform_int_distribution<> distString = uniform_int_distribution<>(0, allowedChars.size() -1);

        string createPath(list<string> dirnames) {
            ostringstream path;
            for (string dirname : dirnames) { path << dirname << PATH_SEP; }
            return path.str();
        }

        string randomString(size_t size) {
            string randomChars;
            for (size_t i = 0; i < size; ++i) {randomChars += allowedChars[distString(gen)];};
            return randomChars;
        }

        list<string> randomStringList(size_t stringSize, size_t size) {
            list<string> randomCharsList;
            for (size_t i = 0; i < size; ++i) {
                uniform_int_distribution<> distStringSize(int(0.7*stringSize), int(1.3*stringSize));
                randomCharsList.push_back(randomString(distStringSize(gen)));
            };
            return randomCharsList;
        }

};