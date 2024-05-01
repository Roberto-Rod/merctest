#ifndef DOCKCONTROLLERCOMMS_HPP
#define DOCKCONTROLLERCOMMS_HPP

#include "registercontrol.hpp"

#include <QObject>

namespace comms
{
    class DockControllerComms : public QObject, public RegisterControl
    {
        Q_OBJECT

    public:
        DockControllerComms(RegisterControl* pJigRegs, uint16_t addrOffset, QObject* parent = 0);

        ~DockControllerComms() {}

        bool readRegister(uint16_t addr, uint32_t& data) const;

        bool writeRegister(uint16_t addr, uint32_t data) const;

    private:
        RegisterControl* m_pJigRegs; //!< Pointer to jig registers object
        uint16_t m_addrOffset;       //!< Address offset of dock channel within address space of jig
    };

}

#endif // DOCKCONTROLLERCOMMS_HPP
