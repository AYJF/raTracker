//
// cSite.cpp

// mfh 10/2010
//
#include "stdafx.h"

#include "cSite.h"
#include "globals.h"

namespace Zeptomoby 
{
namespace OrbitTools
{

//////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
cSite::cSite(const cCoordGeo &geo) : m_geo(geo)
{}

//////////////////////////////////////////////////////////////////////////////
// c'tor accepting:
//    Latitude  in degress (negative south)
//    Longitude in degress (negative west)
//    Altitude  in km
cSite::cSite(double degLat, double degLon, double kmAlt) :
   m_geo(deg2rad(degLat), deg2rad(degLon), kmAlt)
{}

cSite::~cSite()
{}

//////////////////////////////////////////////////////////////////////////////
// SetGeo()
// Set a new geographic position
void cSite::SetGeo(const cCoordGeo &geo)
{
   m_geo = geo;
}

//////////////////////////////////////////////////////////////////////////////
// GetPosition()
// Return the ECI coordinate of the site at the given time.
cEci cSite::GetPosition(const cJulian &date) const
{
   return cEci(m_geo, date);
}

//////////////////////////////////////////////////////////////////////////////
// GetLookAngle()
// Return the topocentric (azimuth, elevation, etc.) coordinates for a target
// object described by the input ECI coordinates.
cCoordTopo cSite::GetLookAngle(const cEci &eci) const
{
   // Calculate the ECI coordinates for this cSite object at the time
   // of interest.
   cJulian date = eci.GetDate();
   cEci eciSite(m_geo, date); 

   // The Site ECI units are km-based; ensure target ECI units are same
   assert(eci.UnitsAreKm());

   cVector vecRgRate(eci.GetVel().m_x - eciSite.GetVel().m_x,
                     eci.GetVel().m_y - eciSite.GetVel().m_y,
                     eci.GetVel().m_z - eciSite.GetVel().m_z);

   double x = eci.GetPos().m_x - eciSite.GetPos().m_x;
   double y = eci.GetPos().m_y - eciSite.GetPos().m_y;
   double z = eci.GetPos().m_z - eciSite.GetPos().m_z;
   double w = sqrt(sqr(x) + sqr(y) + sqr(z));

   cVector vecRange(x, y, z, w);

   // The site's Local Mean Sidereal Time at the time of interest.
   double theta = date.ToLmst(GetLon());

   double sin_lat   = sin(GetLat());
   double cos_lat   = cos(GetLat());
   double sin_theta = sin(theta);
   double cos_theta = cos(theta);

   double top_s = sin_lat * cos_theta * vecRange.m_x + 
                  sin_lat * sin_theta * vecRange.m_y - 
                  cos_lat * vecRange.m_z;
   double top_e = -sin_theta * vecRange.m_x + 
                   cos_theta * vecRange.m_y;
   double top_z = cos_lat * cos_theta * vecRange.m_x + 
                  cos_lat * sin_theta * vecRange.m_y + 
                  sin_lat * vecRange.m_z;
   double az    = atan(-top_e / top_s);

   if (top_s > 0.0)
      az += PI;

   if (az < 0.0)
      az += 2.0*PI;

   double el   = asin(top_z / vecRange.m_w);
   double rate = (vecRange.m_x * vecRgRate.m_x + 
                  vecRange.m_y * vecRgRate.m_y + 
                  vecRange.m_z * vecRgRate.m_z) / vecRange.m_w;

   cCoordTopo topo(az,           // azimuth,   radians
                   el,           // elevation, radians
                   vecRange.m_w, // range, km
                   rate);        // rate,  km / sec

#ifdef WANT_ATMOSPHERIC_CORRECTION
   // Elevation correction for atmospheric refraction.
   // Reference:  Astronomical Algorithms by Jean Meeus, pp. 101-104
   // Note:  Correction is meaningless when apparent elevation is below horizon
   topo.m_El += deg2rad((1.02 / 
                        tan(deg2rad(rad2deg(el) + 10.3 / 
                                    (rad2deg(el) + 5.11)))) / 60.0);
   if (topo.m_El < 0.0)
   {
      topo.m_El = el;    // Reset to true elevation
   }

   if (topo.m_El > (PI / 2))
   {
      topo.m_El = (PI / 2);
   }
#endif

   return topo;
}

//////////////////////////////////////////////////////////////////////////////
// ToString()
//
string cSite::ToString() const
{
   const int TEMP_SIZE = 128;
   char sz[TEMP_SIZE];

   bool LatNorth = true;
   bool LonEast  = true;

   if (m_geo.m_Lat < 0.0) 
   {
      LatNorth = false;
   }

   if (m_geo.m_Lon < 0.0)
   {
      LonEast = false;
   }

   _snprintf(sz, TEMP_SIZE,
               "%06.3f%c, ", 
               fabs(rad2deg(m_geo.m_Lat)),
               (LatNorth ? 'N' : 'S'));

   string strLoc = sz;

   _snprintf(sz, TEMP_SIZE,
               "%07.3f%c, ", 
               fabs(rad2deg(m_geo.m_Lon)),
               (LonEast ? 'E' : 'W'));
   strLoc += sz;


   _snprintf(sz, TEMP_SIZE,
              "%.1fm\n", 
              (m_geo.m_Alt * 1000.0));
   strLoc += sz;

   return strLoc;
}
}
}
