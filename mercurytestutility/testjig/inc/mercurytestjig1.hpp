#ifndef MERCURYTESTJIG1_HPP
#define MERCURYTESTJIG1_HPP

#include "testjig.hpp"
#include "testjigcablecal.hpp"
#include "cablecal.hpp"
#include "sanitycheckcal.hpp"
#include "calibrationstatus.hpp"
#include "rfpath.hpp"
#include "rfswitchmatrix.hpp"

namespace comms
{
    class DirectSerialComms;
}

namespace testjig
{
    class MercuryTestJig1 : public TestJig, public TestJigCableCal, public common::CalibrationStatus
    {
        Q_OBJECT

    public:
        MercuryTestJig1(QObject* parent = 0);

        ~MercuryTestJig1();

        QString name() const;

        virtual bool connect();

        virtual void disconnect();

        virtual bool isConnected() const;

        void calibrateCables();

        void sanityCheckCables();

        void calibrateSourceModuleCables();

        void calibratePowerMonitorCables();

        void calibrateHighBandCables();

        void sanityCheckPowerMeterCable();

        void sanityCheckTestJigSpecAnCable();

        void sanityCheckHighBandCable();

        QDate calExpiry() const;

        bool isCalExpired() const;

        bool isCalNearlyExpired() const;

    signals:
        void cableCalOk(int index);
        void sanityCheckOk(int index);

    public slots:
        void runCableCal(int index);
        void runSanityCheckl(int index);

    private:
        static const QString kJigName;
        static const QString kSourceAssyCableCalKey;
        static const QString kPwrMonCableCalKey;
        static const QString kHBCableCalKey;

        comms::DirectSerialComms* m_pComms;
    };
}

#endif // MERCURYTESTJIG1_HPP
