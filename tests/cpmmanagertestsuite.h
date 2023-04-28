#include <cxxtest/TestSuite.h>
#include <filesystem>
#include <thread>
#include <unordered_set>
#include <random>
#include "cpmmanager.h"

class CpmManagerTestSuite : public CxxTest::TestSuite {

    public:

        void setUp() {
            if (not filesystem::exists(cpmManagerPath) or not filesystem::is_directory(cpmManagerPath))
                filesystem::create_directory(cpmManagerParentPath.c_str());
        }
        void tearDown() {
            if (filesystem::exists(cpmManagerPath) and filesystem::is_directory(cpmManagerPath))
                filesystem::remove_all(cpmManagerParentPath.c_str());
        }

        void testConstruction(void) {
            TS_TRACE("Creating a CpmManager object");
            shared_ptr<CpmManager> cpmManager;
            TS_ASSERT_THROWS_NOTHING(cpmManager = make_shared<CpmManager>(cpmManagerParentPath,cpmManagerName));
        }

        void testDestruction(void) {
            TS_TRACE("Creating a CpmManager object");
            shared_ptr<CpmManager> cpmManager = make_shared<CpmManager>(cpmManagerParentPath,cpmManagerName);
            TS_ASSERT_EQUALS(getFileList(cpmManagerPath).size(),0)
            cpmManager.reset();
            
            TS_ASSERT_EQUALS(*getFileList(cpmManagerPath).rbegin(),cpmManagerPath+"0000000000000001")
        }
        
        void testConstructionWithUnexistentPath(void) {
            TS_TRACE("Check that it is impossible to create a CpmManager object with an unexistant path");
            TS_ASSERT_THROWS_EQUALS(shared_ptr<CpmManager> cpmManager = make_shared<CpmManager>(cpmManagerParentPath+"9786245","CpmLibraryTest"),const Errors &e,e.getCode(),WrongPath);
        }

        void testGetPath(void) {
            TS_TRACE("Creating a CpmManager object");
            shared_ptr<CpmManager> cpmManager = make_shared<CpmManager>(cpmManagerParentPath,cpmManagerName);
            TS_TRACE("Check that getPath() return the good path");
            TS_ASSERT_EQUALS(cpmManager->getPath(),cpmManagerPath);
        }

        void testGetNextUID(void) {
            TS_TRACE("Creating a CpmManager object");
            shared_ptr<CpmManager> cpmManager = make_shared<CpmManager>(cpmManagerParentPath,cpmManagerName);
            TS_TRACE("Check that getNextUID() gives unique ids even if it is called by several at the same time");
            list<uint32_t> uids;
            thread t1([&uids,&cpmManager]() {for (int i = 0; i < 100000; i++) { uids.push_back(cpmManager->getNextUID()); }}),
                   t2([&uids,&cpmManager]() {for (int i = 0; i < 100000; i++) { uids.push_back(cpmManager->getNextUID()); }});
            t1.join();
            t2.join();
            unordered_set<uint32_t> uids_set(begin(uids), end(uids));
            TS_ASSERT_EQUALS(uids.size(),200000)
            TS_ASSERT_EQUALS(uids.size(),uids_set.size());
            
        }

        void testUIDstoHexStringRef(void) {
            TS_TRACE("Check that UIDstoHexString(pair<uint32_t,uint32_t> uids) correctly convert standard uint32 uids to hex uids");
            for (pair<pair<uint32_t,uint32_t>,string> val : uidsReference) {
                TS_ASSERT_EQUALS(CpmManager::UIDstoHexString(val.first),val.second);
            }
        }

        void testHexStringtoIntUIDsRef(void) {
            TS_TRACE("Check that hexStringtoIntUIDs(string hexuid) correctly convert standard hex uids to uint32 uids");
            for (pair<pair<uint32_t,uint32_t>,string> val : uidsReference) {
                TS_ASSERT_EQUALS(CpmManager::hexStringtoIntUIDs(val.second),val.first);
            }
        }

        void testHexStringtoIntUIDsInvalidString(void) {
            TS_TRACE("Check that hexStringtoIntUIDs(string hexuid) throws with an invalid string in argument");
            TS_ASSERT_THROWS_EQUALS(CpmManager::hexStringtoIntUIDs("011e554c00k1560b"),const Errors &e,e.getCode(),InvalidUID)
            TS_ASSERT_THROWS_EQUALS(CpmManager::hexStringtoIntUIDs("011e554c001560b"),const Errors &e,e.getCode(),InvalidUID)
            TS_ASSERT_THROWS_EQUALS(CpmManager::hexStringtoIntUIDs("011e554c001560b1a"),const Errors &e,e.getCode(),InvalidUID)
        }

        void testUIDConversionComplementarity(void) {
            TS_TRACE("Check that UIDstoHexString(pair<uint32_t,uint32_t> uids) is the inverse of hexStringtoIntUIDs(string hexuid) and vice versa");
            mt19937 gen(random_device{}());;
            uniform_int_distribution<> stringDist(0, 15);
            uniform_int_distribution<uint32_t> uint32Dist;
            for (int i = 0; i < 100; i++) {
                string hexUID;
                for (std::size_t i = 0; i < 16; ++i) {hexUID += "0123456789abcdef"[stringDist(gen)];}
                pair<uint32_t,uint32_t> uint32UID = make_pair<uint32_t,uint32_t>(uint32Dist(gen),uint32Dist(gen));
                TS_ASSERT_EQUALS(CpmManager::UIDstoHexString(CpmManager::hexStringtoIntUIDs(hexUID)),hexUID);
                TS_ASSERT_EQUALS(CpmManager::hexStringtoIntUIDs(CpmManager::UIDstoHexString(uint32UID)),uint32UID);
            }
        }

    private:
        string cpmManagerParentPath = createPath({"..","build","Testing","Temporary","cpm_data"});
        string cpmManagerName = "CpmLibraryTest";
        string cpmManagerPath = createPath({cpmManagerParentPath,cpmManagerName});
        list<pair<pair<uint32_t,uint32_t>,string>> uidsReference 
                = {make_pair<pair<uint32_t,uint32_t>,string>(make_pair<uint32_t,uint32_t>(0,0),"0000000000000000"),
                   make_pair<pair<uint32_t,uint32_t>,string>(make_pair<uint32_t,uint32_t>(32546,9098765),"00007f22008ad60d"),
                   make_pair<pair<uint32_t,uint32_t>,string>(make_pair<uint32_t,uint32_t>(18765132,87563),"011e554c0001560b"),
                   make_pair<pair<uint32_t,uint32_t>,string>(make_pair<uint32_t,uint32_t>(4294967295,4294967295),"ffffffffffffffff")};

        string createPath(list<string> dirnames) {
            ostringstream path;
            for (string dirname : dirnames) { path << dirname << PATH_SEP; }
            return path.str();
        }

        list<string> getFileList(string path) {
            list<string> files;
            if (filesystem::exists(path) and filesystem::is_directory(path)) {
                for (const auto &entry : filesystem::directory_iterator(path)) {
                    files.push_back(entry.path());
                }
            }
            return files;
        }

};