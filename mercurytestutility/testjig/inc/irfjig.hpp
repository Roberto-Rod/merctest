#ifndef IRFJIG_HPP
#define IRFJIG_HPP

namespace testjig
{
    class IRfJig
    {
        virtual ~IRfJig {};

        virtual bool calibrateCables(bool& cancelled) = 0;
    };
}

#endif // IRFJIG_HPP
