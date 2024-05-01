#ifndef LOGGING_HPP
#define LOGGING_HPP

#include "testcase.hpp"

#include <QString>
#include <QObject>

class QFile;

namespace common
{
    class Logging : public QObject
    {
        Q_OBJECT

    public:
        static Logging* instance();

        ~Logging();

        void log(QString msg, int32_t flags = LOG_DEST_FILE | LOG_DEST_PROGRESS);

        bool logResult(QString testName, testsuite::TestStatus status, QString note = "");

        bool logResult(QString testName, bool pass, QString note = "");

        template <typename T>
        bool logResult(QString testName, T value, T min, T max, QString units = "");

        bool isLogDirWritable();

        bool openLogFile(const dut::DeviceUnderTest* pDut);

        void closeLogFile(testsuite::TestStatus status = testsuite::TestStatus::NOT_TESTED);

        void indent();

        void deindent();

        void insertBreak();

        void insertLine();

        static const int32_t LOG_DEST_FILE = 1;
        static const int32_t LOG_DEST_PROGRESS = 2;

    signals:
        void progressMessage(QString msg);
        void statusMessage(QString msg);

    protected:
        Logging(QObject* parent = 0);

    private:
        static const int32_t kMaxIndentLevel = 6;

        static Logging* m_pInstance;

        QFile*   m_pFile;

        uint32_t m_indentLevel;
    };

    class LogIndent
    {
    public:
        LogIndent();
        ~LogIndent();
    };

    template <typename T>
    bool Logging::logResult(QString testName, T value, T min, T max, QString units)
    {
        QString note = QString ("%1 %2, min = %3 %2, max = %4 %2").arg(value).arg(units).arg(min).arg(max);

        // Perform test
        bool retVal = true;
        if ((value < min) || (value > max))
        {
            retVal = false;
        }

        // Log result
        logResult(testName, retVal ? testsuite::TestStatus::PASSED : testsuite::TestStatus::FAILED, note);

        return retVal;
    }
}

#endif // LOGGING_HPP
