#ifndef VISARESOURCE_HPP
#define VISARESOURCE_HPP

#include "visa.h"
#include <QString>

namespace te
{
    /**
     * @brief The VisaResource class - base class for VISA compatible test equipment
     *
     * Accessed using National Instruments VISA which encapsulates SCPI:
     * http://www.ivifoundation.org/docs/SCPI-99.PDF
     */
    class VisaResource
    {
    public:
        VisaResource();
        virtual ~VisaResource();

        bool isSessionOpen() const;

        /**
         * @brief resourceName
         * @return the visa resource name for this piece of equipment
         */
        QString resourceName() const;

        /**
         * @brief setResourceName - set the VISA resource name for this piece of equipment
         */
        void setResourceName(const QString &resourceName);

        bool details(QString &details) const;

        QString errorString();

        bool openSession();
        void closeSession();
        bool setTimeout(int32_t milliseconds) const;
        bool sendCommand(const QString& command) const;
        bool sendCommand(const QString& command, QString& response) const;
        bool checkResponse(const QString& command, const QString& expected) const;
        bool checkResponse(const QString& command, const QChar& expected) const;
        bool systemStatus() const;
        bool waitOpc() const;
        bool waitStatOperCond() const;

    private:
        static const int32_t kResponseBufferSize = 4096;    //!< Size of response buffer for all VISA commands
        static const int32_t kWaitCompletionAttempts = 20;  //!< Number of times to retry "wait for completion" command. There is a 100 ms sleep between attempts.
        static const int32_t kDefaultTimeout = 5000;        //!< Default VISA timeout attribute in milliseconds

        QString m_resourceName;                             //!< The Visa resource name (as identified in the NI-MAX software)
        bool m_sessionOpen;
        ViSession m_viDefaultRm;
        ViSession m_viSession;
        ViStatus m_status;
    };
}
#endif // VISARESOURCE_HPP
