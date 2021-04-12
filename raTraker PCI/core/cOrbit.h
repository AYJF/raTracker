//
// cOrbit.h
//
// This is the header file for the class cOrbit. This class accepts a
// single satellite's NORAD two-line element set and provides information 
// regarding the satellite's orbit such as period, axis length,
// ECI coordinates/velocity, etc., using the SGP4/SDP4 orbital models.

// mfh 11/2010
//
#pragma once

#include "cTle.h"
#include "cJulian.h"
#include "cNoradBase.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////////////

namespace Zeptomoby
{
namespace OrbitTools
{
class cVector;
class cGeoCoord;
class cEci;

//////////////////////////////////////////////////////////////////////////////
class cOrbit  
{
public:
   cOrbit(const cTle &tle);
   cOrbit(const cOrbit& src);
   cOrbit& operator=(const cOrbit& rhs);

   virtual ~cOrbit();

   // Return satellite ECI data at given minutes since element's epoch.
   cEci GetPosition(double tsince) const;
   
   double Inclination()   const { return RadGet(cTle::FLD_I);                 }
   double Eccentricity()  const { return m_tle.GetField(cTle::FLD_E);         }
   double RAAN()          const { return RadGet(cTle::FLD_RAAN);              }
   double ArgPerigee()    const { return RadGet(cTle::FLD_ARGPER);            }
   double BStar()         const { return m_tle.GetField(cTle::FLD_BSTAR) / AE;}
   double Drag()          const { return m_tle.GetField(cTle::FLD_MMOTIONDT); }
   double MeanMotionTle() const { return m_tle.GetField(cTle::FLD_MMOTION);   }
   double MeanAnomaly()   const { return RadGet(cTle::FLD_M);                 }
   double MeanAnomaly(cJulian t) const;  // mean anomaly (in radians) at time t

   cJulian Epoch() const { return m_jdEpoch; }

   double TPlusEpoch(const cJulian &t) const;    // time span [t - epoch] in secs

   string SatName(bool fAppendId = false) const;
   string SatId() const;

   
   // "Recovered" from the input elements
   double SemiMajor()  const { return m_aeAxisSemiMajorRec; }
   double SemiMinor()  const { return m_aeAxisSemiMinorRec; }
   double MeanMotion() const { return m_rmMeanMotionRec;    }  // mean motion, rads/min
   double Major()      const { return 2.0 * SemiMajor();    }  // major axis in AE
   double Minor()      const { return 2.0 * SemiMinor();    }  // minor axis in AE
   double Perigee()    const { return m_kmPerigeeRec;       }  // perigee in km
   double Apogee()     const { return m_kmApogeeRec;        }  // apogee in km
   double Period()     const;                                  // period in seconds

protected:
   double RadGet(cTle::eField fld) const { return m_tle.GetField(fld, cTle::U_RAD); }
   double DegGet(cTle::eField fld) const { return m_tle.GetField(fld, cTle::U_DEG); }

private:
   cTle        m_tle;
   cJulian     m_jdEpoch;
   cNoradBase *m_pNoradModel;

   // Caching variables; note units are not necessarily the same as tle units
   mutable double m_secPeriod;

   // Caching variables recovered from the input TLE elements
   double m_aeAxisSemiMajorRec;  // semimajor axis, in AE units
   double m_aeAxisSemiMinorRec;  // semiminor axis, in AE units
   double m_rmMeanMotionRec;     // radians per minute
   double m_kmPerigeeRec;        // perigee, in km
   double m_kmApogeeRec;         // apogee, in km
};
}
}
