#include "settings.hpp"
#include "assyrevision.hpp"

namespace common
{
    AssyRevision * AssyRevision::m_pInstance = 0;

    AssyRevision::AssyRevision()
    {
        QString mk0 = Settings::instance()->getKey(AssyKey::NULLASSY);
        QString mk1 = Settings::instance()->getKey(AssyKey::PCB0044);
        QString mk2 = Settings::instance()->getKey(AssyKey::ASY0033);
        QString mk3 = Settings::instance()->getKey(AssyKey::ASY129);
        QString mk4 = Settings::instance()->getKey(AssyKey::PSUMan);
        QString mk5 = Settings::instance()->getKey(AssyKey::PSUVeh);
        QString mk6 = Settings::instance()->getKey(AssyKey::ASY20or21);
        QString mk7 = Settings::instance()->getKey(AssyKey::ASY130);
        QString mk8 = Settings::instance()->getKey(AssyKey::PCB0114);

        m_stringlistMap.insert(mk0, NullStrList);
        m_stringlistMap.insert(mk1, PCB0044StrList);
        m_stringlistMap.insert(mk2, ASY0033StrList);
        m_stringlistMap.insert(mk3, ASY129StrList);
        m_stringlistMap.insert(mk4, PCB0049StrList);
        m_stringlistMap.insert(mk5, PCB0058StrList);
        m_stringlistMap.insert(mk6, ASY20or21StrList);
        m_stringlistMap.insert(mk7, ASY130StrList);
        m_stringlistMap.insert(mk8, PCB0114StrList);
    }

    AssyRevision::AssyRevision(QString  name, QString number)
    {
        setUUTKeyFromNameandNumber(name, number);

        QString mk0 = Settings::instance()->getKey(AssyKey::NULLASSY);
        QString mk1 = Settings::instance()->getKey(AssyKey::PCB0044);
        QString mk2 = Settings::instance()->getKey(AssyKey::ASY0033);
        QString mk3 = Settings::instance()->getKey(AssyKey::ASY129);
        QString mk4 = Settings::instance()->getKey(AssyKey::PSUMan);
        QString mk5 = Settings::instance()->getKey(AssyKey::PSUVeh);
        QString mk6 = Settings::instance()->getKey(AssyKey::ASY20or21);
        QString mk7 = Settings::instance()->getKey(AssyKey::ASY130);
        QString mk8 = Settings::instance()->getKey(AssyKey::PCB0114);

        m_stringlistMap.insert(mk0, NullStrList);
        m_stringlistMap.insert(mk1, PCB0044StrList);
        m_stringlistMap.insert(mk2, ASY0033StrList);
        m_stringlistMap.insert(mk3, ASY129StrList);
        m_stringlistMap.insert(mk4, PCB0049StrList);
        m_stringlistMap.insert(mk5, PCB0058StrList);
        m_stringlistMap.insert(mk6, ASY20or21StrList);
        m_stringlistMap.insert(mk7, ASY130StrList);
        m_stringlistMap.insert(mk8, PCB0114StrList);
    }

    void AssyRevision::setUUTKey(AssyKey key)
    {
        m_UUTkey = key;
    }

    AssyKey AssyRevision::getUUTKey()
    {
        return m_UUTkey;
    }

    int AssyRevision::getUUTRevInt()
    {
        common::AssyKey enumKey = getUUTKey();
        int rev = Settings::instance()->getRevision(enumKey);
        return rev;
    }

    QString AssyRevision::getUUTRevStr()
    {
        QString uutRevStr;
        common::AssyKey enumKey = getUUTKey();
        QString mk1 = Settings::instance()->getKey(enumKey);
        int rev = Settings::instance()->getRevision(enumKey);

        QStringList sl = m_stringlistMap.value(mk1);
        uutRevStr = sl.at(rev);
        return uutRevStr;
    }

    AssyRevision* AssyRevision::instance()
    {
        if (m_pInstance == 0)
        {
            m_pInstance = new AssyRevision();
        }

        return m_pInstance;
    }

    void AssyRevision::setUUTKeyFromNameandNumber(QString name, QString number)
    {
        if(name.compare("Null Assembly") == 0)
        {
            setUUTKey(NULLASSY);
        }
        else if(name.compare("DDS Source Board") == 0)
        {
            setUUTKey(PCB0044);
        }
        else if(name.compare("Daughter Board") == 0)
        {
            if(number.compare("ASY0033") == 0)
            {
                setUUTKey(ASY0033);
            }
            else if(number.compare("ASY0129") == 0)
            {
                setUUTKey(ASY129);
            }
        }
        else if(name.compare("PSU Module") == 0)
        {
            if(number.compare("PCB0049") == 0)
            {
                setUUTKey(PSUMan);
            }
            else if(number.compare("PCB0058") == 0)
            {
                setUUTKey(PSUVeh);
            }
        }
        else if(name.compare("Power Monitor") == 0)
        {
            if(number.compare("ASY0020") == 0)
            {
                setUUTKey(ASY20or21);
            }
            else if(number.compare("ASY0021") == 0)
            {
                setUUTKey(ASY20or21);
            }
            else if(number.compare("ASY0130") == 0)
            {
                setUUTKey(ASY130);
            }
        }
        else if(name.compare("Confidence Test Set Board") == 0)
        {
            setUUTKey(PCB0114);
        }
    }

    void AssyRevision::setRevisionDefaults()
    {
        // No Assembly (Multiple variantss, variant not selected)
        if (!Settings::instance()->revExists(NULLASSY))
        {
            Settings::instance()->setRevision(NULLASSY, static_cast<int>(nullRev::NullRev2));
        }

        // Source board
        else if (!Settings::instance()->revExists(PCB0044))
        {
            Settings::instance()->setRevision(PCB0044, static_cast<int>(PCB0044Rev::RevJ));
        }

        // LB Daughter Board
        else if (!Settings::instance()->revExists((ASY0033)))
        {
            Settings::instance()->setRevision(ASY0033, static_cast<int>(ASY0033Rev::REVDIss2));
        }

        // HB Daughter Board
        else if (!Settings::instance()->revExists((ASY129)))
        {
            Settings::instance()->setRevision(ASY129, static_cast<int>(ASY129Rev::REVCtoCIss2));
        }

        // PSU PCB0049 (Manpack)
        else if (!Settings::instance()->revExists((PSUMan)))
        {
            Settings::instance()->setRevision(PSUMan, static_cast<int>(PCB0049Rev::RevCIss2));
        }

        // PSU PCB0058 (Vehicle)
        else if (!Settings::instance()->revExists((PSUVeh)))
        {
            Settings::instance()->setRevision(PSUVeh, static_cast<int>(PCB0058Rev::RevFIss4));
        }

        // Low or Mid Band Power Monitor
        else if (!Settings::instance()->revExists((ASY20or21)))
        {
            Settings::instance()->setRevision(ASY20or21, static_cast<int>(ASY20or21Rev::REVC));
        }

        // High Band Power Monitor
        else if (!Settings::instance()->revExists((ASY130)))
        {
            Settings::instance()->setRevision(ASY130, static_cast<int>(ASY130Rev::REVDtoDIss2));
        }

        // CTS Board
        else if (!Settings::instance()->revExists(PCB0114))
        {
            Settings::instance()->setRevision(PCB0114, static_cast<int>(PCB0114Rev::RevBOn));
        }
    }
}
