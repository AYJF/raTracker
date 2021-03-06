//
// cOrbit.cpp
//

// mfh 12/2010
// 
#include "stdafx.h"

#include "coreLib.h"
#include "cOrbit.h"
#include "cNoradSGP4.h"
#include "cNoradSDP4.h"

namespace Zeptomoby
{
namespace OrbitTools
{
//////////////////////////////////////////////////////////////////////
cOrbit::cOrbit(const cTle &tle) :
   m_tle(tle),
   m_pNoradModel(NULL)
{
   m_tle.Initialize();

   int    epochYear = (int)m_tle.GetField(cTle::FLD_EPOCHYEAR);
   double epochDay  =      m_tle.GetField(cTle::FLD_EPOCHDAY );

   if (epochYear < 57)
      epochYear += 2000;
   else
      epochYear += 1900;

   m_jdEpoch = cJulian(epochYear, epochDay);

   m_secPeriod = -1.0;

   // Recover the original mean motion and semimajor axis from the
   // input elements.
   double mm     = MeanMotionTle();
   double rpmin  = mm * TWOPI / MIN_PER_DAY;   // rads per minute

   double a1     = pow(XKE / rpmin, TWOTHRD);
   double e      = Eccentricity();
   double i      = Inclination();
   double temp   = (1.5 * CK2 * (3.0 * sqr(cos(i)) - 1.0) / 
                   pow(1.0 - e * e, 1.5));   
   double delta1 = temp / (a1 * a1);
   double a0     = a1 * 
                   (1.0 - delta1 * 
                   ((1.0 / 3.0) + delta1 * 
                   (1.0 + 134.0 / 81.0 * delta1)));

   double delta0 = temp / (a0 * a0);

   m_rmMeanMotionRec    = rpmin / (1.0 + delta0);
   m_aeAxisSemiMajorRec = a0 / (1.0 - delta0);
   m_aeAxisSemiMinorRec = m_aeAxisSemiMajorRec * sqrt(1.0 - (e * e));
   m_kmPerigeeRec       = XKMPER_WGS72 * (m_aeAxisSemiMajorRec * (1.0 - e) - AE);
   m_kmApogeeRec        = XKMPER_WGS72 * (m_aeAxisSemiMajorRec * (1.0 + e) - AE);

   if (TWOPI / m_rmMeanMotionRec >= 225.0)
   {
      // SDP4 - period >= 225 minutes.
      m_pNoradModel = new cNoradSDP4(*this);
   }
   else
   {
      // SGP4 - period < 225 minutes
      m_pNoradModel = new cNoradSGP4(*this);
   }
}

/////////////////////////////////////////////////////////////////////////////
// Copy constructor
cOrbit::cOrbit(const cOrbit& src) :
  m_tle(src.m_tle),
  m_jdEpoch(src.m_jdEpoch),
  m_pNoradModel(NULL)
{
   try
   {
      m_pNoradModel = src.m_pNoradModel->Clone(*this);
   }
   catch (...)
   {
      delete m_pNoradModel;
      throw;
   }
}

/////////////////////////////////////////////////////////////////////////////
cOrbit& cOrbit::operator=(const cOrbit& rhs)
{
   if (this != &rhs)
   {
      m_tle       = rhs.m_tle;
      m_jdEpoch   = rhs.m_jdEpoch;

      cNoradBase *pNoradModel = NULL;

      try
      {
         pNoradModel = rhs.m_pNoradModel->Clone(*this);
      }
      catch (...)
      {
         delete pNoradModel;
         throw;
      }

      delete m_pNoradModel;
      m_pNoradModel = pNoradModel;
   }

   return *this;
}

/////////////////////////////////////////////////////////////////////////////
cOrbit::~cOrbit()
{
   delete m_pNoradModel;
}

//////////////////////////////////////////////////////////////////////////////
// Return the period in seconds
double cOrbit::Period() const
{
   if (m_secPeriod < 0.0)
   {
      // Calculate the period using the recovered mean motion.
      if (m_rmMeanMotionRec == 0)
         m_secPeriod = 0.0;
      else
         m_secPeriod = TWOPI / m_rmMeanMotionRec * 60.0;
   }

   return m_secPeriod;
}

//////////////////////////////////////////////////////////////////////////////
// Returns elapsed number of seconds from epoch to given time.
// Note: "Predicted" TLEs can have epochs in the future.
double cOrbit::TPlusEpoch(const cJulian &gmt) const
{
   return gmt.SpanSec(Epoch());
}

//////////////////////////////////////////////////////////////////////////////
// Returns the mean anomaly in radians at given GMT.
// At epoch, the mean anomaly is given by the elements data.
double cOrbit::MeanAnomaly(cJulian gmt) const
{
   double span = TPlusEpoch(gmt);
   double P    = Period();

   assert(P != 0.0);

   return fmod(MeanAnomaly() + (TWOPI * (span / P)), TWOPI);
}

//////////////////////////////////////////////////////////////////////////////
// GetPosition()
// This procedure returns the ECI position and velocity for the satellite
// at "tsince" minutes from the (GMT) TLE epoch. The vectors returned in
// the ECI object are kilometer-based.
// tsince  - Time in minutes since the TLE epoch (GMT).
cEci cOrbit::GetPosition(double tsince) const
{
   cEci eci = m_pNoradModel->GetPosition(tsince);

   eci.Ae2Km();

   return eci;
}
   
//////////////////////////////////////////////////////////////////////////////
// SatName()
// Return the name of the satellite. If requested, the NORAD number is
// appended to the end of the name, i.e., "ISS (ZARYA) #25544".
// The name of the satellite with the NORAD number appended is important
// because many satellites, especially debris, have the same name and
// would otherwise appear to be the same satellite in ouput data.
string cOrbit::SatName(bool fAppendId /* = false */) const
{
   string str = m_tle.GetName();

   if (fAppendId)
   {
      str = str + " #" + SatId();
   }

   return str;
}

//////////////////////////////////////////////////////////////////////////////
// SatId()
// Return the NORAD number of the satellite.
string cOrbit::SatId() const
{
   string strId;

   m_tle.GetField(cTle::FLD_NORADNUM, cTle::U_NATIVE, &strId);

   return strId;
}
}
}
