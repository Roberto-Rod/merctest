#include "logging.hpp"
#include "settings.hpp"
#include "deviceundertest.hpp"
#include "assyrevision.hpp"

#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QApplication>

namespace common
{
    Logging* Logging::m_pInstance = 0;

    Logging::Logging(QObject* parent) :
        QObject(parent),
        m_pFile(0),
        m_indentLevel(0)
    {
    }

    Logging::~Logging()
    {
        // Close the log file when the object is deleted
        closeLogFile();
    }

    Logging* Logging::instance()
    {
        if (m_pInstance == 0)
        {
            m_pInstance = new Logging(qApp);
        }

        return m_pInstance;
    }

    void Logging::log(QString msg, int32_t flags)
    {
        // Indent the message
        for (uint32_t i = 0; i < m_indentLevel; i++)
        {
            msg = "    " + msg;
        }

        // If the progress pane is one of the destinations then emit the message
        if ((flags & LOG_DEST_PROGRESS) != 0)
        {
            emit progressMessage(msg);
        }

        // If the log file is one of the destinations then write the message to the log
        if ((flags & LOG_DEST_FILE) != 0)
        {
            if (m_pFile != 0)
            {
                // Textstream is used to read QString in to handle writing new lines
                // to file properly via "endl"
                QTextStream in(&msg, QIODevice::ReadOnly);
                QTextStream out(m_pFile);

                QString line;
                while(true)
                {
                    // Read line and break loop when we hit the end
                    if ((line = in.readLine()).isNull())
                    {
                        break;
                    }
                    out << line << endl;
                }
            }
        }
    }

    bool Logging::logResult(QString testName, testsuite::TestStatus status, QString note)
    {
        // Return true only if the test result is "PASS"
        bool retVal = false;

        // Create the log string
        QString msg = testName + ": ";

        if (status == testsuite::TestStatus::PASSED)      { msg += "Pass"; retVal = true; }
        else if (status == testsuite::TestStatus::FAILED) { msg += "**FAIL**"; }
        else                                              { msg += "Not Tested"; }

        // Add note (if provided) in parentheses
        if (note != "")                                   { msg += " (" + note + ")"; }

        // Log to default destinations
        log(msg);

        return retVal;
    }

    bool Logging::logResult(QString testName, bool pass, QString note)
    {
        // Convert the boolean parameter into a TestStatus type
        testsuite::TestStatus status;
        if (pass) { status = testsuite::TestStatus::PASSED; }
        else      { status = testsuite::TestStatus::FAILED; }

        // Log the result
        return logResult(testName, status, note);
    }

    bool Logging::isLogDirWritable()
    {
        bool retVal = false;

        QFileInfo info(QFileInfo(common::Settings::instance()->reportsDirectory()));

        if (info.isWritable())
        {
            retVal = true;
        }

        return retVal;
    }

    bool Logging::openLogFile(const dut::DeviceUnderTest* pDut)
    {
        bool retVal = false;

        // Close previous log file if there is one open
        closeLogFile();

        // Test whether log directory is writable
        if (isLogDirWritable())
        {
            QString fileName = QString("test_report_%1_%2_%3_%4.txt").arg(pDut->selectedVariant()->m_number.toLower())
                                                                     .arg(pDut->serialNumber())
                                                                     .arg(QDate::currentDate().toString("yyyy-MM-dd"))
                                                                     .arg(QTime::currentTime().toString("hh-mm-ss"));

            // Get file path
            QString path = common::Settings::instance()->reportsDirectory() + "/";

            // Create QFile object
            m_pFile = new QFile(path + fileName);

            if (m_pFile->open(QIODevice::WriteOnly | QIODevice::Text))
            {
                // We should now have a usable file pointer
                emit statusMessage(QString("Logging To File: %1").arg(fileName));

                // Write some header info into the log file
                insertLine();
                log(QString("Test Utility Version: %1").arg(GIT_VERSION));
                log(QString("Test Commenced At:    %1 on %2").arg(QTime::currentTime().toString("hh:mm:ss"))
                                                             .arg(QDate::currentDate().toString("ddd, MMM dd, yyyy")));
                log(QString("Device Under Test:    %1 (%2)").arg(pDut->selectedVariant()->m_number)
                                                            .arg(pDut->selectedVariant()->m_name));
                log(QString("Serial Number:        %1").arg(pDut->serialNumber()));
                common::AssyRevision* revPtr = pDut->assyRevision();
                log(QString("Revision:             %1").arg(revPtr->getUUTRevStr()));
                insertLine();

                retVal = true;
            }
            else
            {
                // Something went wrong, delete the QFile object
                delete m_pFile;
                m_pFile = 0;
                emit log(tr("Cannot open report file: opening for writing failed!"));
            }
        }
        else
        {
            emit log(tr("Cannot open report file: reports directory is not writable!"));
        }

        return retVal;
    }

    void Logging::closeLogFile(testsuite::TestStatus status)
    {
        if (m_pFile != 0)
        {
            // Write test status as a footer of the file
            insertLine();

            if (status == testsuite::TestStatus::PASSED)
            {
                log("All Tests **PASSED**");
            }
            else if (status == testsuite::TestStatus::FAILED)
            {
                log("One or More Tests **FAILED**");
            }
            else
            {
                log("Tests Incomplete/Cancelled");
            }

            insertLine();

            m_pFile->close();
            delete m_pFile;
            m_pFile = 0;
        }

        emit statusMessage("Stopped Logging To File");
    }

    void Logging::indent()
    {
        if (m_indentLevel < kMaxIndentLevel)
        {
            m_indentLevel++;
        }
    }

    void Logging::deindent()
    {
        if (m_indentLevel > 0)
        {
            m_indentLevel--;
        }
    }

    void Logging::insertBreak()
    {
        log("");
    }

    void Logging::insertLine()
    {
        QString line;

        // Create a string of 80 hyphens
        for (int i = 0; i < 80; i++)
        {
            line += '-';
        }

        // And write to log
        log(line);
    }

    LogIndent::LogIndent()
    {
        // When an instance is created, indent the log
        Logging::instance()->indent();
    }

    LogIndent::~LogIndent()
    {
        // When an instance is destroyed, de-indent the log
        Logging::instance()->deindent();
    }
}
