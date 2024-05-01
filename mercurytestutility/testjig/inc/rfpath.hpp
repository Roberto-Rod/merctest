#ifndef RFPATH_HPP
#define RFPATH_HPP

#include <QString>
#include <QMap>

namespace testjig
{
    class RfPath
    {
    public:
       // RfPath();

        virtual ~RfPath() {}

        virtual QString calInstruction(RfPath* pPrevious = 0) const = 0;

        virtual QString brief() const = 0;

        virtual QString key() const = 0;

        virtual double testPower() const = 0;

    protected:
        static constexpr double kTestPowerDefault = 0.0;        //!< Default test power
    };

    class RfPathSourceAssy : public RfPath
    {
    public:
        enum class DutRfCable
        {
            UNKNOWN,
            SHORT,
            LONG
        };

        enum class TePort
        {
            UNKNOWN,
            SPECTRUM_ANALYSER,
            POWER_METER
        };

        enum class DutRfPort
        {
            UNKNOWN,
            LHS,
            RHS
        };

        RfPathSourceAssy(DutRfCable dutCable = DutRfCable::UNKNOWN,
                          DutRfPort dutPort = DutRfPort::UNKNOWN,
                          TePort tePort = TePort::UNKNOWN);

        ~RfPathSourceAssy() {}

        QString calInstruction(RfPath* pPrevious = 0) const;

        QString brief() const;

        QString key() const;

        double testPower() const;

        DutRfCable dutCable() const;

        QString dutCableString() const;

        void setDutCable(const DutRfCable &dutCable);

        DutRfPort dutPort() const;

        QString dutPortString() const;

        void setDutPort(const DutRfPort &dutPort);

        TePort tePort() const;

        QString tePortString() const;

        void setTePort(const TePort &tePort);

    private:
        DutRfCable m_dutCable;

        DutRfPort m_dutPort;

        TePort m_tePort;
    };

    class RfPathPowerMonitor : public RfPath
    {
    public:
        enum class RfCableSet
        {
            UNKNOWN,
            SIG_GEN_AND_POWER_METER,
            SIG_GEN_AT_15_DBM
        };

        RfPathPowerMonitor(RfCableSet cableSet = RfCableSet::UNKNOWN);

        ~RfPathPowerMonitor() {}

        QString calInstruction(RfPath* pPrevious = 0) const;

        QString brief() const;

        QString key() const;

        double testPower() const;

        RfCableSet cableSet() const;

    private:
        static constexpr double kTestPower15dbm = 15.0;        //!< Test power for SIG_GEN_AT_15_DBM case

        RfCableSet m_cableSet;

        QMap<RfCableSet, double> m_testPowerMap;
    };

    class RfPathHighBandModule : public RfPath
    {
     public:
          enum class RfCableSet
          {
              UNKNOWN,
              SIG_GEN_AND_CAB0255,
              POWER_METER_AND_CAB0255,
              SPEC_AN_AND_CAB0255
          };

          RfPathHighBandModule(RfCableSet cableSet = RfCableSet::UNKNOWN);

          ~RfPathHighBandModule() {}

          QString calInstruction(RfPath* pPrevious = 0) const;

          QString brief() const;

          QString key() const;

          double testPower() const;

          RfCableSet cableSet() const;

      private:
          static constexpr double kTestPower15dbm = 15.0;        //!< Test power for SIG_GEN_AT_15_DBM case

          RfCableSet m_cableSet;

          QMap<RfCableSet, double> m_testPowerMap;
      };

}

#endif // RFPATH_HPP
