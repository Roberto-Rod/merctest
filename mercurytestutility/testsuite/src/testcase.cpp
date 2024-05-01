#include "testcase.hpp"
#include "testjig.hpp"
#include "logging.hpp"
#include "settings.hpp"
#include "deviceundertest.hpp"
#include "dutdialog.hpp"
#include "assyrevision.hpp"

#include <QApplication>
#include <QDebug>

namespace testsuite
{
    TestCase::TestCase(QObject* parent) :
        TestCase("", parent)
    {
    }

    TestCase::TestCase(QString name, QObject* parent) :
        QObject(parent),
        m_status(TestStatus::NOT_TESTED),
        m_name(name),
        m_variant(""),
        m_abort(false),
        m_dutDialogueDone(false)
    {
        // If the parent is a test case then chain the statusUpdated signal upwards
        TestCase* parentTestCase = dynamic_cast<TestCase*>(parent);

        if (parentTestCase != 0)
        {
            connect(this, SIGNAL(statusUpdated()), parentTestCase, SIGNAL(statusUpdated()));
        }
    }

    TestCase::~TestCase()
    {
    }

    QString TestCase::name() const
    {
        QString name = m_name;

        // If we can cast the parent to a DeviceUnderTest then this is the top
        // level test case (lower levels have another TestCase as their parent)
        // in that case, get the generic name of the DUT
        dut::DeviceUnderTest* dut = dynamic_cast<dut::DeviceUnderTest*>(parent());
        if (dut != 0)
        {
            name = dut->genericVariant()->m_name;
        }

        return name;
    }

    QString TestCase::variant() const
    {
        QString variant = m_variant;
        // If we can cast the parent to a DeviceUnderTest then this is the top
        // level test case (lower levels have another TestCase as their parent)
        // in that case, get the generic name of the DUT
         dut::DeviceUnderTest* dut = dynamic_cast<dut::DeviceUnderTest*>(parent());
         if (dut)
         {
             variant = dut->selectedVariant()->m_number;
         }
         return variant;
    }

    QString TestCase::assyRevision() const
    {
        QString revision = "";
        // If we can cast the parent to a DeviceUnderTest then this is the top
        // level test case (lower levels have another TestCase as their parent)
        // in that case, get the generic name of the DUT
        dut::DeviceUnderTest* dut = dynamic_cast<dut::DeviceUnderTest*>(parent());
        if (dut)
        {
            // Note UUTKey setup in constructor of deviceundertest child e.g.
            // in sourceboard.cpp
            common::AssyRevision* revPtr = dut->assyRevision();
            if (revPtr)
            {
                revision = revPtr->getUUTRevStr();
            }
        }
        return revision;
    }

    QString TestCase::setupMessage()
    {
        return QString();
    }

    void TestCase::setRunMode(dut::RunMethod runMethod)
    {
        // Function only usefull in overriden variety
        m_dummyRunMethod = runMethod;
    }

    dut::RunMethod TestCase::getRunMode()
     {
          return dut::RunMethod::AUTOMATIC_RUN;
     }

    void TestCase::setSkipTestFlag(bool skipTest)
    {
        // Function only usefull in overidden variety
        m_dummyBool = skipTest;
    }

    bool TestCase::getSkipTestFlag()
    {
        return false;
    }

    QList<te::TestEquipment*> TestCase::teList() const
    {
        // Start with our own list
        QList<te::TestEquipment*> list = m_teList;

        // Go through all the children and for each bit of Test Equipment on
        // its list, check against those we've already seen, adding those that
        // we haven't

        // Loop for each child
        for (int i = 0; i < children().length(); i++)
        {
            TestCase* testCase = dynamic_cast<TestCase*>(children().at(i));

            if (testCase != 0)
            {
                QList<te::TestEquipment*> childList = testCase->teList();

                // Loop for each TE item in the child's list
                for (int j = 0; j < childList.length(); j++)
                {
                    bool teFound = false;

                    // Loop for each TE item in the locally accumulated list
                    for (int k = 0; k < list.length(); k++)
                    {
                        if (childList.at(j) == list.at(k))
                        {
                            teFound = true;
                        }
                    }

                    // If the TE was not found then add it to the local list
                    if (!teFound)
                    {
                        list.append(childList.at(j));
                    }
                }
            }
        }

        return list;
    }

    bool TestCase::isTopLevel() const
    {
        bool retVal = false;

        // If we can cast the parent to a DeviceUnderTest then this is the top
        // level test case (lower levels have another TestCase as their parent)
        if (dynamic_cast<dut::DeviceUnderTest*>(parent()) != 0)
        {
            retVal = true;
        }

        return retVal;
    }

    TestStatus TestCase::status()
    {
        // If there are no children the return the status member variable
        TestStatus retVal = m_status;

        if (children().length() > 0)
        {
            // If any of the children have not been tested then the overall status is
            // not tested. If the children have all been tested but there is one or more
            // failures then the overall status is fail. Otherwise overall status is pass.
            retVal = TestStatus::PASSED;

            for (int i = 0; (retVal != TestStatus::NOT_TESTED) && (i < children().length()); i++)
            {
                TestCase* testCase = dynamic_cast<TestCase*>(children().at(i));

                if (testCase != 0)
                {
                    if (testCase->status() == TestStatus::NOT_TESTED)
                    {
                        retVal = TestStatus::NOT_TESTED;
                    }
                    else if (testCase->status() == TestStatus::FAILED)
                    {
                        retVal = TestStatus::FAILED;
                    }
                }
            }
        }

        return retVal;
    }

    void TestCase::resetStatus()
    {
        // Reset the status of this and all of its children
        m_status = TestStatus::NOT_TESTED;

        for (int i = 0; i < children().length(); i++)
        {
            TestCase* testCase = dynamic_cast<TestCase*>(children().at(i));

            if (testCase != 0)
            {
                testCase->resetStatus();
            }
        }

        // Emit the signal to tell connected objects that status has been updated
        emit statusUpdated();

        // Process events to update the displayed test status
        QApplication::processEvents();
    }

    void TestCase::abort()
    {
        m_abort = true;

        // Loop for each child
        for (int i = 0; i < children().length(); i++)
        {
            TestCase* testCase = dynamic_cast<TestCase*>(children().at(i));

            if (testCase != 0)
            {
                testCase->abort();
            }
        }
    }

    void TestCase::setStatus(bool testsComplete, bool testsPassed)
    {
        // If the tests were cancelled then reset status to Not Tested, otherwise set
        // to Pass or Fail based on result
        if (testsComplete && !m_abort)
        {
            if (testsPassed)
            {
                m_status = TestStatus::PASSED;
                common::Logging::instance()->log("----------");
                common::Logging::instance()->log("-- Pass --");
                common::Logging::instance()->log("----------");
            }
            else
            {
                m_status = TestStatus::FAILED;
                common::Logging::instance()->log("--------------");
                common::Logging::instance()->log("-- **FAIL** --");
                common::Logging::instance()->log("--------------");
            }
        }
        else
        {
            m_status = TestStatus::NOT_TESTED;
            common::Logging::instance()->log("----------------");
            common::Logging::instance()->log("-- CANCELLED  --");
            common::Logging::instance()->log("----------------");
        }

        // Emit the signal to tell connected objects that status has been updated
        emit statusUpdated();

        // Process events to update the displayed test status
        QApplication::processEvents();
    }

    dut::DeviceUnderTest* TestCase::dut()
    {
        // Loop around getting parent pointers until we get to an ancestor
        // which is a DeviceUnderTest. If we hit a null pointer then return
        // null i.e. failed to find DeviceUnderTest amongst ancestors

        QObject* ancestor = this;
        dut::DeviceUnderTest* retPtr = 0;

        while ((ancestor != 0) && (retPtr == 0))
        {
            ancestor = ancestor->parent();
            retPtr = dynamic_cast<dut::DeviceUnderTest*>(ancestor);
        }

        return retPtr;
    }

    bool TestCase::execute(bool manualSteps)
    {
        m_abort = false;

        bool cont = true;

        // Check that necessary TE is all available
        bool retVal = te::TestEquipment::checkTeAvailable(teList());

        // Carry on if TE is all available
        if (retVal)
        {
            // If this is the top-level test case then get details
            if (isTopLevel())
            {
                // Make the DUT dialog box and fire it up
                gui::DutDialog dialog(dut());
                retVal &= dialog.exec();
            }

            // Setup the revision stuff using the assembly and selected variant
            if(retVal && isTopLevel())
            {
                const dut::Variant *gptr = dut()->genericVariant();
                const dut::Variant *sptr1 = dut()->selectedVariant();
                // The UUTKey allows the correct revision to be read for a given assembly and variant.

                common::AssyRevision* revPtr = dut()->assyRevision();
                revPtr->setUUTKeyFromNameandNumber(gptr->m_name, sptr1->m_number);

                // flag below signifys its OK to display the revision in all cases (single and multiple variants)
                m_dutDialogueDone = true;
            }

            // If there is an initial setup mesasge then display it
            if (retVal)
            {
                QString msg = setupMessage();
                if (!msg.isEmpty())
                {
                    gui::MessageDialog dialog(msg);

                    retVal = dialog.exec();
                }
            }

            if (retVal)
            {
                // Reset the test state of this and all its children
                resetStatus();

                // Open the log file if this is top-level test
                if (isTopLevel())
                {
                    common::Logging::instance()->openLogFile(dut());

                    // Add the test equipment list
                    QList<te::TestEquipment*> list = teList();

                    if (!list.isEmpty())
                    {
                        common::Logging::instance()->insertBreak();
                        common::Logging::instance()->insertBreak();
                        common::Logging::instance()->insertLine();
                        common::Logging::instance()->log(QString("Test Equipment Details"));
                        common::Logging::instance()->insertLine();

                        for (int i = 0; i < list.length(); i++)
                        {
                            common::Logging::instance()->log (list.at(i)->formattedDetails());
                            common::Logging::instance()->insertLine();
                        }

                        common::Logging::instance()->insertBreak();
                        common::Logging::instance()->insertBreak();
                    }
                }
                else
                {
                    // Not top-level test case - just write name to log and then ident the log
                    common::Logging::instance()->log(name() + ":");
                    common::Logging::instance()->indent();
                }

                // Execute all of the child Test Cases
                for (int i = 0; cont && (i < children().length()); i++)
                {
                    TestCase* testCase = dynamic_cast<TestCase*>(children().at(i));

                    if (testCase != 0)
                    {
                        retVal &= testCase->execute(manualSteps);

                        // Decide whether to continue or not
                        cont = false;
                        if (retVal)
                        {
                            if (testCase->status() == TestStatus::PASSED ||
                                 (testCase->status() == TestStatus::FAILED &&
                                   !common::Settings::instance()->stopOnFail()))
                            {
                                cont = true;
                            }
                        }
                        // If the test failed to execute then log this - only log at the lowest level
                        // (i.e. a TestCase with no children)
                        else if (testCase->children().length() == 0)
                        {
                            common::Logging::instance()->log(QString("ERROR: failed to execute test!"));
                        }
                    }
                }

                // De-indent the log
                common::Logging::instance()->deindent();

                // Close log file if this is the top-level test case
                if (isTopLevel())
                {
                    common::Logging::instance()->closeLogFile(status());
                }
            }
        }

        return retVal;
    }
}
