#ifndef SPECTRUMANALYSER_HPP
#define SPECTRUMANALYSER_HPP

#include <stdint.h>

namespace te
{
    /** @brief Enumerated decibels per division - used for vertical scaling */
    enum class DbPerDiv
    {
        UNKNOWN,
        DB_1  = 1u,     //!< 1 dB per division
        DB_2  = 2u,     //!< 2 dB per division
        DB_5  = 5u,     //!< 5 dB per division
        DB_10 = 10u     //!< 10 dB per division
    };

    /**
     * @brief Spectrum Analyser Interface
     *
     * This abstract class defines a spectrum analyser interface providing
     * a set of operations to control the equipment.
     */
    class ISpectrumAnalyser
    {
    public:
        virtual ~ISpectrumAnalyser() { }

        /**
         * @brief Reset the spectrum analyser to a known initial state
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool reset()= 0;

        /**
         * @brief Set the spectrum analyser's centre frequency
         *
         * @param freq The frequency in Hertz
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool setFrequency(uint64_t freq)= 0;

        virtual bool setStartFrequency(uint64_t freq)= 0;
        virtual bool setStopFrequency(uint64_t freq)= 0;

        /**
         * @brief Get the spectrum analyser's centre frequency
         *
         * @param freq Reference to the returned frequency in Hertz
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool getFrequency(uint64_t& freq)= 0;

        /**
         * @brief Set the spectrum analyser's span
         *
         * @param span The span in Hertz
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool setSpan(uint64_t span)= 0;

        /**
         * @brief Get the spectrum analyser's span
         *
         * @param span Reference to the returned span in Hertz
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool getSpan(uint64_t& span)= 0;

        /**
         * @brief Set the spectrum analyser's resolution bandwidth
         *
         * @param resBw The resolution bandwidth in Hertz
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool setResBw(uint64_t resBw)= 0;

        /**
         * @brief Get the spectrum analyser's resolution bandwidth
         *
         * @param resBw Reference to the returned resolution bandwidth in Hertz
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool getResBw(uint64_t& resBw)= 0;

        /**
         * @brief Set the spectrum analyser's reference level
         *
         * @param refLevel The reference level in dBm
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool setRefLevel(double refLevel)= 0;

        /**
         * @brief Get the spectrum analyser's reference level
         *
         * @param refLevel Reference to the reference level in dBm
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool getRefLevel(double& refLevel)= 0;

        /**
         * @brief Set the spectrum analyser's vertical scale (dB/div)
         *
         * @param scale The enumerated scale
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool setDbPerDiv(DbPerDiv scale)= 0;

        /**
         * @brief Get the spectrum analyser's vertical scale (dB/div)
         *
         * @param scale Reference to the returned enumerated scale
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool getDbPerDiv(DbPerDiv& scale)= 0;

        /**
         * @brief Set the spectrum analyser's continuous trigger state
         *
         * @param cont true to set continuous triggering,
         *             false to clear continuous triggering
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool setContTrigger(bool cont)= 0;

        /**
         * @brief Get the spectrum analyser's continuous trigger state
         *
         * @param cont Reference to the returned continuous triggering state
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool getContTrigger(bool& cont)= 0;

        /**
         * @brief Get a new peak marker reading
         *
         * Executes marker->peak and returns the marker frequency
         * and power readings.
         *
         * @param freq Reference to the returned frequency in Hertz
         * @param power Reference to the returned power in dBm
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool getPeak(uint64_t& freq, double& power)= 0;
        virtual bool getPeak(bool next, uint64_t& freq, double& power)= 0;

        /**
         * @brief Get a new peak marker power reading
         *
         * Executes marker->peak and returns the marker power reading
         *
         * @param power Reference to the returned power in dBm
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool getPeakPower(double& power)= 0;
        virtual bool getPeakPower(bool nextPeak, double& power)= 0;

        /**
         * @brief Get a new peak marker frequency reading
         *
         * Executes marker->peak and returns the marker frequency reading
         *
         * @param freq Reference to the returned frequency in Hertz
         *
         * @return true if the operation was successful, otherwise false
         */
        virtual bool getPeakFrequency(uint64_t& freq)= 0;
        virtual bool getPeakFrequency(bool nextPeak, uint64_t& freq)= 0;
    };
}

#endif // SPECTRUMANALYSER_HPP
