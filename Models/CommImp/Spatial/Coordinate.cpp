/** ***********************************************************************************
 *    @File      :  Coordinate.cpp
 *    @Brief     :  None.
 *
 ** ***********************************************************************************/
#include "Coordinate.h"


namespace OpenOasis::CommImp::Spatial
{
using namespace Utils;
using namespace std;

bool Coordinate::Equals(real coorX, real coorY, real coorZ) const
{
    return Equals({coorX, coorY, coorZ});
}

bool Coordinate::Equals(const Coordinate &coor) const
{
    if (!IsEql(x, coor.x) || !IsEql(y, coor.y) || !IsEql(z, coor.z))
        return false;

    return true;
}

}  // namespace OpenOasis::CommImp::Spatial
