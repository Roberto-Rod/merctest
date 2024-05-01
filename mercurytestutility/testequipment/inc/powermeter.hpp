#ifndef POWERMETER_HPP
#define POWERMETER_HPP

#include <stdint.h>

namespace te
{
    /**
     * @brief Power Meter Interface
     *
     * This abstract class defines a power meter interface providing
     * a set of operations to control the equipment.
     */
    class IPowerMeter
    {
    public:
        virtual ~IPowerMeter() { }

        /**
         * @brief Reset the power meter to a known initial state
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool reset()= 0;

        /**
         * @brief Zero the power meter
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool zeroSensor()= 0;

        /**
         * @brief Set a fixed offset in the power meter
         *
         * @param offset The offset in dB
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool setOffset(double offset)= 0;

        /**
         * @brief Get the power meter's fixed offset
         *
         * @param offset Reference to the returned offset in dB
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool getOffset(double& offset)= 0;

        /**
         * @brief Set the power meter's reference frequency
         *
         * @param freq Frequency in Hz
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool setFrequency(uint64_t freq)= 0;

        /**
         * @brief Get the power meter's reference frequency
         *
         * @param freq Reference to the returned frequency in Hz
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool getFrequency(uint64_t& freq)= 0;

        /**
         * @brief Get a power reading from the power meter
         *
         * @param power Reference to the returned power reading in dB
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool getPower(double& power)= 0;
    };
}

#endif // POWERMETER_HPP
