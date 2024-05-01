#ifndef SIGNALGENERATOR_HPP
#define SIGNALGENERATOR_HPP

#include <stdint.h>
#include <QStringList>

namespace te
{
    /**
     * @brief Signal Generator Interface
     *
     * This abstract class defines a signal generator interface providing
     * a set of operations to control the equipment.
     */
    class ISignalGenerator
    {
    public:
        virtual ~ISignalGenerator() { }

        /**
         * @brief Reset the test equipment to a known initial state
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool reset()= 0;

        /**
         * @brief Set the signal generator's output frequency
         *
         * @param freq The output frequency in Hz
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool setFrequency(uint64_t freq)= 0;
        virtual bool setFrequency(uint64_t freq, QString unit)= 0;

        /**
         * @brief Get the signal generator's output frequency
         *
         * @param freq Reference to the returned output frequency in Hz
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool getFrequency(uint64_t& freq)= 0;

        /**
         * @brief Set the signal generator's output power
         *
         * @param power The output power in dBm
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool setOutputPower(double power)= 0;

        /**
         * @brief Get the signal generator's output power
         *
         * @param power Reference to the returned output power in dBm
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool getOutputPower(double& power)= 0;

        /**
         * @brief Set the signal generator's RF (output) enabled state
         *
         * @param en true to enable the RF output, false to disable it
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool setRfEnabled(bool en)= 0;

        /**
         * @brief Get the signal generator's RF (output) enabled state
         *
         * @param en Reference to the enabled state
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool getRfEnabled(bool& en)= 0;
    };
}

#endif // SIGNALGENERATOR_HPP
