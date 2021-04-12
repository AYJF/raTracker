//
// cEci.h

#pragma once

#include "cVector.h"
#include "cJulian.h"
#include "Coord.h"

namespace Zeptomoby 
{
namespace OrbitTools
{
//////////////////////////////////////////////////////////////////////
// class cEci
// Encapsulates an Earth-Centered Inertial position, velocity, and time.
class cEci
{
public:
   cEci() { m_VecUnits = UNITS_NONE; }
   cEci(const cCoordGeo &geo, const cJulian &cJulian);
   cEci(const cVector &pos, const cVector &vel, 
        const cJulian &date, bool IsAeUnits = true);
   virtual ~cEci() {};

   cCoordGeo ToGeo(); 

   const cVector& GetPos()  const { return m_pos;  }
   const cVector& GetVel()  const { return m_vel;  }
   cJulian GetDate() const { return m_date; }

   void SetUnitsAe() { m_VecUnits = UNITS_AE; }
   void SetUnitsKm() { m_VecUnits = UNITS_KM; }
   bool UnitsAreAe() const { return m_VecUnits == UNITS_AE; }
   bool UnitsAreKm() const { return m_VecUnits == UNITS_KM; }
   void Ae2Km();  // Convert position, velocity vector units from AE to km

protected:
   void MulPos(double factor) { m_pos.Mul(factor); }
   void MulVel(double factor) { m_vel.Mul(factor); }

   enum VecUnits
   {
      UNITS_NONE, // not initialized
      UNITS_AE,
      UNITS_KM,
   };

   cVector  m_pos;
   cVector  m_vel;
   cJulian  m_date;
   VecUnits m_VecUnits;
};
}
}
