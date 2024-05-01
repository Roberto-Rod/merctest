#ifndef  TESTCASESOURCEBOARDSTOREDDSFILE_HPP
#define  TESTCASESOURCEBOARDSTOREDDSFILE_HPP

#include "testcasesourceboard.hpp"

namespace testsuite
{
    class TestCaseSourceBoardStoreDdsFile : public TestCaseSourceBoard
    {
        Q_OBJECT

    public:
         TestCaseSourceBoardStoreDdsFile(QObject* parent = 0);

        bool execute(bool manualSteps = false);

        QString setupMessage();

    private:
        static const QString kBootloaderConfigData; //!< The magic bytes to write into the bootloader to make it work
        static const uint8_t kMaxAttempts = 3;      //!< Maximum attempts to erase/program/verify flash
    };

}

#endif //  TESTCASESOURCEBOARDSTOREDDSFILE_HPP
