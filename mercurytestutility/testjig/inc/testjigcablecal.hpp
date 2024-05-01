#ifndef TESTJIGCABLECAL_HPP
#define TESTJIGCABLECAL_HPP

namespace testjig
{
    class TestJigCableCal
    {
    public:
        virtual ~TestJigCableCal() {}

        virtual void calibrateCables()= 0;
        virtual void sanityCheckCables()= 0;
    };
}

#endif // TESTJIGCABLECAL_HPP
