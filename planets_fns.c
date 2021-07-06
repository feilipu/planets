
#include <math.h>

#include "planets.h"

// utility functions (C)

void sunEclipticCartesianCoordinates ( cartesian_coordinates_t * sun ) __z88dk_fastcall;

void planetEclipticCartesianCoordinates ( cartesian_coordinates_t * location, planet_t * planet ) __z88dk_callee;

float eccentricAnomaly (float e, float M) __z88dk_callee;

void addCartesianCoordinates ( cartesian_coordinates_t * base, cartesian_coordinates_t * addend ) __z88dk_callee;

#if ! defined(__MATH_MATH32) && ! defined(__MATH_AM9511)
float rev(float x) __z88dk_fastcall;
#endif

#ifndef __MATH_MATH32
#define sqr(x) ((x)*(x))
#endif


void sunEclipticCartesianCoordinates ( cartesian_coordinates_t * sun) __z88dk_fastcall
{
    // We use formulas for finding the Sun as seen from Earth, 
    // then negate the (x,y,z) coordinates obtained to get the Earth's position 
    // from the Sun's perspective.

    // http://www.astro.uio.no/~bgranslo/aares/calculate.html
    // http://www.meteorobs.org/maillist/msg09197.html              <== Correct formulas, more accurate (complex)

    // These formulas use 'd' based on days since 1/Jan/2000 12:00 UTC ("J2000.0"), instead of 0/Jan/2000 0:00 UTC ("day value").
    // Correct by subtracting 1.5 days...
    float T = (sun->day - 1.5) * 0.0000273785;                      // 36525.0 Julian centuries since J2000.0
    float L0 = rev(280.46645 + (36000.76983 * T) + (0.0003032 * sqr(T))); // Sun's mean longitude, in degrees
    float M0 = rev(357.52910 + (35999.05030 * T) - (0.0001559 * sqr(T)) - (0.00000048 * T * sqr(T)));   // Sun's mean anomaly, in degrees

                                                                    // Sun's equation of center in degrees
    float C = rev((1.914600 - 0.004817 * T - 0.000014 * T * T) * sin(rad(M0)) + (0.01993 - 0.000101 * T) * sin(rad(2*M0)) + 0.000290 * sin(rad(3*M0)));

    float LS = rev(L0 + C);                                         // true ecliptical longitude of Sun

    float e = 0.016708617 - T * (0.000042037 + T * 0.0000001236);   // The eccentricity of the Earth's orbit.
    float distanceInAU = (1.000001018 * (1 - e*e)) / (1 + e * cos(rad(M0 + C))); // distance from Sun to Earth in astronomical units (AU)
    sun->x = distanceInAU * cos(rad(LS));
    sun->y = distanceInAU * sin(rad(LS));
    sun->z = 0.0;                                                   // the Earth's center is always on the plane of the ecliptic (z=0), by definition!
}


void planetEclipticCartesianCoordinates ( cartesian_coordinates_t * location, planet_t * planet ) __z88dk_callee
{
    float day = location->day;

    float N = rev( planet->N0 + (day * planet->Nc) );
    float i = rev( planet->i0 + (day * planet->ic) );
    float w = rev( planet->w0 + (day * planet->wc) );
    float a = rev( planet->a0 + (day * planet->ac) );

    float e = rev( planet->e0 + (day * planet->ec) );
    float M = rev( planet->M0 + (day * planet->Mc) );

    float E = rev(eccentricAnomaly (e, M));

    // Calculate the body's position in its own orbital plane, and its distance from the thing it is orbiting.
    float xv = a * (cos(rad(E)) - e);
    float yv = a * sqrt(1.0 - (e*e)) * sin(rad(E));

    float v = deg(atan2(yv, xv));       // True anomaly in degrees: the angle from perihelion of the body as seen by the Sun.
    float r = hypot(xv, yv);            // Distance from the Sun to the planet in AU

    float cosN  = cos(rad(N));
    float sinN  = sin(rad(N));
    float cosi  = cos(rad(i));
    float sini  = sin(rad(i));
    float cosVW = cos(rad(v+w));
    float sinVW = sin(rad(v+w));

    // Now we are ready to calculate (unperturbed) ecliptic cartesian heliocentric coordinates.
    location->x = r * (cosN*cosVW - sinN*sinVW*cosi);
    location->y = r * (sinN*cosVW + cosN*sinVW*cosi);
    location->z = r * sinVW * sini;
}


float eccentricAnomaly (float e, float M) __z88dk_callee
{
    float E, F;
    float error;

    E = M + (e * sin(rad(M)) * (1.0 + (e * cos(rad(M)))));

    do {
        F = E - (E - deg(e * sin(rad(E))) - M) / (1 - e * cos(rad(E)));
        error = fabs(F - E);
        E = F;

    } while (error >= 1.0e-4);          // the angle is good enough for our purposes

    return E;
}

void addCartesianCoordinates ( cartesian_coordinates_t * base, cartesian_coordinates_t * addend ) __z88dk_callee
{
    base->x += addend->x;
    base->y += addend->y;
    base->z += addend->z;
}

#if ! defined(__MATH_MATH32) && ! defined(__MATH_AM9511)
float rev (float x) __z88dk_fastcall
{
    return x - floor(x*(1/360.0))*360.0;
}
#endif

