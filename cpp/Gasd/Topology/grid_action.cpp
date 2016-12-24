
#include "grid_action.h"

FilterIn &operator>>(FilterIn &si, GridAction &buf) {
    si >> StdKey >> buf.Action >> StdKey >> buf.Index >> StdKey >> buf.Param;
    return si;
};
FilterOut &operator<<(FilterOut &so, GridAction &buf) {
    so << KeyDesc("Action") << buf.Action << KeyDesc("Index") << buf.Index
       << KeyDesc("Param") << buf.Param;
    return so;
};

FilterIn &operator>>(FilterIn &si, GridBoundPnt &buf) {
    si >> StdKey >> StdKey >> buf.Center >> StdKey >> buf.NormalDir >> StdKey >>
        buf.Surface;
    si >> StdKey >> buf.First >> StdKey >> buf.FirstPos >> StdKey >> buf.Second >>
        StdKey >> buf.SecondPos;
    si >> StdKey;
    return si;
};
FilterOut &operator<<(FilterOut &so, GridBoundPnt &buf) {
    so << KeyDesc("GridBoundPnt") << KeyDesc("Center") << buf.Center
       << KeyDesc("NormalDir") << buf.NormalDir << KeyDesc("Surface") << buf.Surface
       << KeyDesc("First") << buf.First << KeyDesc("FirstPos") << buf.FirstPos
       << KeyDesc("Second") << buf.Second << KeyDesc("SecondPos") << buf.SecondPos
       << KeyDesc("GridBoundPnt_End\n");
    //  so.flush();
    return so;
};
