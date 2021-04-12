//
// coord.cpp

// mfh 10/2010
//
#include "stdafx.h"

#include "coord.h"
#include "globals.h"

namespace Zeptomoby 
{
namespace OrbitTools
{
//////////////////////////////////////////////////////////////////////
// cCoordGeo Class
//////////////////////////////////////////////////////////////////////

cCoordGeo::cCoordGeo()
{
   m_Lat = 0.0;
   m_Lon = 0.0;
   m_Alt = 0.0;
}

//////////////////////////////////////////////////////////////////////
// cCoordTopo Class
//////////////////////////////////////////////////////////////////////

cCoordTopo::cCoordTopo()
{
   m_Az = 0.0;
   m_El = 0.0;       
   m_Range = 0.0;    
   m_RangeRate = 0.0;

}
}
}
