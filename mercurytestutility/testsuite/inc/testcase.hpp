#ifndef TESTCASE_HPP
#define TESTCASE_HPP

#include <QObject>
#include <QList>
#include <QString>

#include "deviceundertest.hpp"
#include "settings.hpp"
#include "messagedialog.hpp"

// Define a test macro which executes an expression if retVal is still true and testsPassed is
// still true or the stopOnFail flag is not set
#define KEEP_GOING (retVal && !m_abort && (testsPassed || !common::Settings::instance()->stopOnFail()))

#define TEST_STEP(expression, description)\
    if (KEEP_GOING)\
    {\
        if (manualSteps)\
        {\
            gui::MessageDialog msg(QString("Next step: <i>%1</i><BR><BR>Press OK to execute step<BR>or Cancel to stop all tests").arg(description),\
                                    gui::MessageDialogType::INFO, true, true);\
            retVal &= msg.exec();\
        }\
        if (retVal)\
        {\
            if (!expression)\
            {\
                common::Logging::instance()->log(QString("ERROR: ") + QString(description) + QString(" failed!"));\
                retVal = false;\
            }\
        }\
    }

#define TEST_RESULT(description, result, min, max, units)\
    if (KEEP_GOING)\
    {\
        testsPassed &= common::Logging::instance()->logResult(QString(description),\
                                                               result, min, max,\
                                                               QString(units));\
    }

#define TEST_BOOL(description, result, note)\
    if (KEEP_GOING)\
    {\
        testsPassed &= common::Logging::instance()->logResult(QString(description),\
                                                               result,\
                                                               QString(note));\
    }

namespace dut
{
    class DeviceUnderTest;
}

namespace testsuite
{
    enum class TestStatus
    {
        NOT_TESTED,
        PASSED,
        FAILED
    };

    class TestCase : public QObject
    {
        Q_OBJECT

    public:
        TestCase(QObject* parent = 0);

        TestCase(QString name = QString(""), QObject* parent = 0);

        virtual ~TestCase();

        QString name() const;

        QString variant() const;

        QString assyRevision() const;

        QList<te::TestEquipment*> teList() const;

        bool isTopLevel() const;

        TestStatus status();

        virtual void setRunMode(dut::RunMethod runMethod);

        virtual dut::RunMethod getRunMode();

        virtual void setSkipTestFlag(bool skipTest);

        virtual bool getSkipTestFlag();

        void setStatus(bool testsComplete, bool testsPassed);

        virtual QString setupMessage();

        virtual bool execute(bool manualSteps = false);

    public slots:
        void resetStatus();

        void abort();

    signals:
        void statusUpdated();

    protected:
        TestStatus m_status;
        QString m_name;
        QString m_variant;
        bool m_abort;

        QList<te::TestEquipment*> m_teList;

        dut::DeviceUnderTest* dut(); //!< Helper method which finds the first ancestor which is a DeviceUnderTest

     private:
        dut::RunMethod m_dummyRunMethod;
        bool m_dummyBool;
        bool m_dutDialogueDone;
    };
}
#endif // TESTCASE_HPP
