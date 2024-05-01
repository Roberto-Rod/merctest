#ifndef ASSYREVISION_H
#define ASSYREVISION_H
#include "settings.hpp"

#include <QMetaType>

namespace common
{
    struct AssyRev
    {
        common::AssyKey asy;
        int rev;
    };

    class AssyRevision
    {
    public:
        AssyRevision();
        AssyRevision(QString name, QString number = "");
        ~AssyRevision();
        static AssyRevision* instance();

        void setUUTKey(AssyKey key);
        AssyKey getUUTKey();

        enum class nullRev      { NullRev = 0,     NullRev2 = 1 };
        enum class PCB0044Rev   { REVBtoGIss2 = 0, GIss2toIIss2 = 1, RevJ = 2 };
        enum class ASY0033Rev   { REVBtoC = 0,     REVCIss4 = 1,     REVDIss2 = 2 };
        enum class ASY129Rev    { RevB = 0,        REVCtoCIss2 = 1 };
        enum class PCB0049Rev   { RevBtoC = 0,     RevCIss2 = 1 };
        enum class PCB0058Rev   { RevBtoFIss3 = 0, RevFIss4 = 1 };
        enum class ASY20or21Rev { RevBtoBIss3 = 0, REVC = 1 };
        enum class ASY130Rev    { REVBtoC = 0,     REVDtoDIss2 = 1 };
        enum class PCB0114Rev   { RevBOn = 0 };

        QStringList NullStrList      = {""                                  , ""};
        QStringList PCB0044StrList   = {"Rev B to G Iss 2"                  , "G Iss 2 to I Iss 2 ",  "Rev J"};
        QStringList ASY0033StrList   = {"PCB0060 Rev B to C"                , "PCB0060 Rev C Iss 4",  "PCB0060 Rev D Iss 2"};
        QStringList ASY129StrList    = {"PCB0082 Rev B"                     , "PCB0082 Rev C to C Iss 2"};
        QStringList PCB0049StrList   = {"PCB0049 Rev B to C"                , "PCB0049 Rev C Iss 2"};
        QStringList PCB0058StrList   = {"PCB0058 Rev B to F Iss 3"          , "PCB0058 Rev F Iss 4"};
        QStringList ASY20or21StrList = {"PCB0055 Rev B to B Iss 3 "         , "PCB0055 Rev C"};
        QStringList ASY130StrList    = {"PCB0080 D Iss 2 + MECH00001 Rev C" , "PCB0080 D Iss 2 + MECH00001 Rev D"};
        QStringList PCB0114StrList   = {"Rev B or Above"};

        void setUUTKeyFromNameandNumber(QString name, QString number);

        static void setRevisionDefaults();
        QString getUUTRevStr();
        int getUUTRevInt();

    private:
        static AssyRevision* m_pInstance;
        QMap<QString, QStringList> m_stringlistMap;
        AssyKey m_UUTkey;
    };

}

//  !!!!!!!! DECLARATION BELOW MUST GO HERE, NOT IN THE CLASS OR THER NAMESPACE
//  OTHERWISE YOU WILL SPEND TE REST OF YOUR LIFE FIXING A META OBJECT
//  COMPILER ERROR !!!!!!!!

Q_DECLARE_METATYPE(common::AssyRev)

#endif // ASSYREVISION_H
