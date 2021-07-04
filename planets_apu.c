
#include <math.h>
#include "planets.h"
#include "multi_apu.h"

// utility functions (C)

void sunEclipticCartesianCoordinates ( cartesian_coordinates_t * sun ) __z88dk_fastcall;

void planetEclipticCartesianCoordinates ( cartesian_coordinates_t * location, planet_t * planet ) __z88dk_callee;

float eccentricAnomaly (float e, float M ) __z88dk_callee;

void addCartesianCoordinates ( cartesian_coordinates_t * base, cartesian_coordinates_t * addend ) __z88dk_callee;


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
    float L0 = 280.46645 + (36000.76983 * T) + (0.0003032 * sqr(T)); // Sun's mean longitude, in degrees
    float M0 = 357.52910 + (35999.05030 * T) - (0.0001559 * sqr(T)) - (0.00000048 * T * sqr(T));  // Sun's mean anomaly, in degrees

                                                                    // Sun's equation of center in degrees
    float C = (1.914600 - 0.004817 * T - 0.000014 * T * T) * sin(rad(M0)) + (0.01993 - 0.000101 * T) * sin(rad(2*M0)) + 0.000290 * sin(rad(3*M0));

    float LS = L0 + C;                                              // true ecliptical longitude of Sun

    float e = 0.016708617 - T * (0.000042037 + T * 0.0000001236);   // The eccentricity of the Earth's orbit.
    float distanceInAU = (1.000001018 * (1 - sqr(e))) / (1 + e * cos(rad(M0 + C))); // distance from Sun to Earth in astronomical units (AU)

    rad_0(LS);
    rad_1(LS);
    cos_0();
    sin_1();
    mul_0(distanceInAU);
    mul_1(distanceInAU);
    
    sun->x = pop_0();
    sun->y = pop_1();
    sun->z = 0.0;                                                   // the Earth's center is always on the plane of the ecliptic (z=0), by definition!
}


void planetEclipticCartesianCoordinates ( cartesian_coordinates_t * location, planet_t * planet ) __z88dk_callee
{
    float day = location->day;

    push_0(day);
    push_1(day);
    push_2(day);
    push_3(day);

    mul_0(planet->Nc);
    mul_1(planet->ic);
    mul_2(planet->ec);
    mul_3(planet->Mc);

    add_0(planet->N0);
    add_1(planet->i0);
    add_2(planet->e0);
    add_3(planet->M0);

    float N = pop_0();
    float i = pop_1();
    float e = pop_2();
    float M = pop_3();

    push_2(day);
    push_3(day);

    mul_2(planet->wc);
    mul_3(planet->ac);

    N = rev( N );

    add_2(planet->w0);
    add_3(planet->a0);

    float w = pop_2();
    float a = pop_3();

    rad_2(N);
    rad_3(N);
    cos_2();
    sin_3();

    i = rev( i );

    rad_0(i);
    rad_1(i);
    cos_0();
    sin_1();

    float cosN = pop_2();
    float sinN = pop_3();

    float cosi = pop_0();
    float sini = pop_1();

    e = rev( e );
    M = rev( M );

    float E = eccentricAnomaly (e, M);

    // Calculate the body's position in its own orbital plane, and its distance from the thing it is orbiting
    rad_2(E);
    rad_3(E);
    cos_2();
    sin_3();

    mul_3( sqrt(1.0 - sqr(e)) );        // use 0 for math while 2 & 3 are doing trigonometry
    add_2( -e );
    mul_3( a );
    mul_2( a );

    float yv = pop_3();
    float xv = pop_2();

    float v = deg(atan2(yv, xv));       // True anomaly in degrees: the angle from perihelion of the body as seen by the Sun

    rad_2(v+w);
    rad_3(v+w);
    cos_2();
    sin_3();

    float r = hypot(xv, yv);            // Distance from the Sun to the planet in AU.

    float cosVW = pop_2();
    float sinVW = pop_3();

    // Now we are ready to calculate (unperturbed) ecliptic cartesian heliocentric coordinates
    push_0(cosN); 
    push_3(cosN); 
    push_1(sinN);
    push_2(-sinN);

    mul_0(cosVW);
    mul_1(cosVW);
    mul_2(sinVW);
    mul_3(sinVW);

    float cosNcosVW = pop_0();
    float sinNcosVW = pop_1();

    push_1(sinVW);

    mul_2(cosi);
    mul_3(cosi);

    mul_1(sini);

    add_2(cosNcosVW);
    add_3(sinNcosVW);

    mul_1(r);
    mul_2(r);
    mul_3(r);

    location->z = pop_1();
    location->x = pop_2();
    location->y = pop_3();
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
    push_0(base->x);
    push_1(base->y);
    push_2(base->z);
    add_0(addend->x);
    add_1(addend->y);
    add_2(addend->z);
    base->x = pop_0();
    base->y = pop_1();
    base->z = pop_2();
}

