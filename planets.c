/*

    build with:

    zcc +test -v -m -O2 --list -lm -DPRINTF @planets.lst -o planetgm_sccz80_ticks.bin
    zcc +test -v -m -O2 --list -lmath48 -DPRINTF @planets.lst -o planet48_sccz80_ticks.bin
    zcc +test -v -m -O2 --list --math32 -DPRINTF @planets.lst -o planet32_sccz80_ticks.bin

    zcc +test -compiler=sdcc -v -m -SO3 --list --reserve-regs-iy -lmath48 -DPRINTF --max-allocs-per-node100000 @planets.lst -o planet48_sdcc_ticks.bin
    zcc +test -compiler=sdcc -v -m -SO3 --list --reserve-regs-iy --math32 -DPRINTF --max-allocs-per-node100000 @planets.lst -o planet32_sdcc_ticks.bin

    z88dk-ticks -counter 99999999999 planetxx_x_ticks.bin

    zcc +rc2014 -subtype=cpm -clib=new -v -m -O2 --list -lm -DPRINTF @planets.lst -o planetnew_cpm -create-app
    zcc +rc2014 -subtype=cpm -v -m --list  -lm     -DPRINTF --max-allocs-per-node100000 @planets.lst -o planet48_cpm -create-app
    zcc +rc2014 -subtype=cpm -v -m --list --math32 -DPRINTF --max-allocs-per-node100000 @planets.lst -o planet32_cpm -create-app

    zcc +rc2014 -subtype=cpm -v -m --list --am9511 -DPRINTF --max-allocs-per-node100000 @planets.lst -o planetapu_cpm -create-app
    zcc +rc2014 -subtype=cpm -v -m --list --am9511 -DPRINTF --max-allocs-per-node100000 @planets_mapu.lst -o planetmapu_cpm -create-app
*/

/*
 *  40 calculations for 9 bodies on z88dk-ticks - no printing.
 *
 *  sccz80/classic/genmath  Ticks: 964795252
 *  sccz80/new/math48       Ticks: 765516204
 *  sccz80/new/math32       Ticks: 243894794
 *
 *  sdcc/new/math48         Ticks: 733985135
 *  sdcc/new/math32         Ticks: 244120776
 *
 *  40 calculations for 9 bodies on RC2014 (CPM-IDE) - reduced printing.
 *
 *  sccz80/new/math48       105.5 seconds
 *  sdcc/new/math48         101.4 seconds
 *  sdcc/new/math32          34.3 seconds
 *
 *  sdcc/new/am9511          16.9 seconds
 *  sdcc/new/am9511 4x        9.1 seconds
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "planets.h"
#include "multi_apu.h"

#ifdef PRINTF
   #define FPRINTF(a,b,c,d,e)   printf(a,b,c,d,e)
#else
   #define FPRINTF(a,b,c,d,e)
#endif

const planet_t sun =      { "Sun", \
                            0.0, 0.0, \
                            0.0, 0.0, \
                            282.9404, 4.70935E-5, \
                            1.0, 0.0, \
                            0.016709, -1.151E-9, \
                            356.0470, 0.9856002585, \
                            (695500/6378) };

const planet_t moon =     { "Moon", \
                            125.1228, -0.0529538083, \
                            5.1454, 0.0, \
                            318.0634, 0.1643573223, \
                            60.2666/EARTH_RADII_PER_ASTRONOMICAL_UNIT, 0.0, \
                            0.054900, 0.0, \
                            115.3654, 13.0649929509, \
                            (1738/6378) };


const planet_t mercury =  { "Mercury", \
                            48.3313, 3.24587e-5, \
                            7.0047, 5.0e-8, \
                            29.1241, 1.01444e-5, \
                            0.387098, 0.0, \
                            0.205635, 5.59e-10, \
                            168.6562, 4.0923344368, \
                            (2440/6378) };

const planet_t venus =    { "Venus", \
                            76.6799, 2.46590e-5, \
                            3.3946, 2.75e-8, \
                            54.8910, 1.38374e-5, \
                            0.723330, 0.0, \
                            0.006773, -1.302e-9, \
                            48.0052, 1.6021302244, \
                            (6052/6378) };

const planet_t mars =     { "Mars", \
                            49.5574, 2.11081e-5, \
                            1.8497, -1.78e-8, \
                            286.5016, 2.92961e-5, \
                            1.523688, 0.0, \
                            0.093405, 2.516e-9, \
                            18.6021, 0.5240207766, \
                            (3390/6378) };

const planet_t jupiter =  { "Jupiter", \
                            100.4542, 2.76854E-5, \
                            1.3030, - 1.557E-7, \
                            273.8777, 1.64505E-5, \
                            5.20256, 0.0, \
                            0.048498, 4.469E-9, \
                            19.8950, 0.0830853001, \
                            (69911/6378) };

const planet_t saturn =   { "Saturn", \
                            113.6634, 2.3898e-5, \
                            2.4886, -1.081e-7, \
                            339.3939, 2.97661e-5, \
                            9.55475, 0.0, \
                            0.055546, -9.499e-9, \
                            316.9670, 0.0334442282, \
                            (58232/6378) };

const planet_t uranus =   { "Uranus", \
                            74.0005, 1.3978E-5, \
                            0.7733, 1.9E-8, \
                            96.6612, 3.0565E-5, \
                            19.18171, - 1.55E-8, \
                            0.047318, 7.45E-9, \
                            142.5905, 0.011725806, \
                            (25362/6378) };

const planet_t neptune =  { "Neptune", \
                            131.7806, 3.0173e-5, \
                            1.7700, -2.55e-7, \
                            272.8461, -6.027e-6, \
                            30.05826, 3.313e-8, \
                            0.008606, 2.15e-9, \
                            260.2471, 0.005995147, \
                            (24622/6378) };


int main()
{
    FLOAT d;
    cartesian_coordinates_t theSun, thePlanet;

    printf("\nGeocentric Coordinates\n\n");

    for (d = 7855.0; d < 7865.0; d+= 0.25)
    {
        printf("Solar Day %.3f\n", (float)d);

        theSun.day = d;
        sunEclipticCartesianCoordinates ( &theSun);
        FPRINTF("%10s x %10.6f y %10.6f z %10.6f\n", sun.name, (float)theSun.x, (float)theSun.y, (float)theSun.z);

        thePlanet.day = d;
        planetEclipticCartesianCoordinates( &thePlanet, &moon );
        FPRINTF("%10s x %10.6f y %10.6f z %10.6f\n", moon.name, (float)thePlanet.x, (float)thePlanet.y, (float)thePlanet.z);

        planetEclipticCartesianCoordinates( &thePlanet, &mercury );
        addCartesianCoordinates( &thePlanet, &theSun );
        FPRINTF("%10s x %10.6f y %10.6f z %10.6f\n", mercury.name, (float)thePlanet.x, (float)thePlanet.y, (float)thePlanet.z);

        planetEclipticCartesianCoordinates( &thePlanet, &venus );
        addCartesianCoordinates( &thePlanet, &theSun );
        FPRINTF("%10s x %10.6f y %10.6f z %10.6f\n", venus.name, (float)thePlanet.x, (float)thePlanet.y, (float)thePlanet.z);

        planetEclipticCartesianCoordinates( &thePlanet, &mars );
        addCartesianCoordinates( &thePlanet, &theSun );
        FPRINTF("%10s x %10.6f y %10.6f z %10.6f\n", mars.name, (float)thePlanet.x, (float)thePlanet.y, (float)thePlanet.z);

        planetEclipticCartesianCoordinates( &thePlanet, &jupiter );
        addCartesianCoordinates( &thePlanet, &theSun );
        FPRINTF("%10s x %10.6f y %10.6f z %10.6f\n", jupiter.name, (float)thePlanet.x, (float)thePlanet.y, (float)thePlanet.z);

        planetEclipticCartesianCoordinates( &thePlanet, &saturn );
        addCartesianCoordinates( &thePlanet, &theSun );
        FPRINTF("%10s x %10.6f y %10.6f z %10.6f\n", saturn.name, (float)thePlanet.x, (float)thePlanet.y, (float)thePlanet.z);

        planetEclipticCartesianCoordinates( &thePlanet, &uranus );
        addCartesianCoordinates( &thePlanet, &theSun );
        FPRINTF("%10s x %10.6f y %10.6f z %10.6f\n", uranus.name, (float)thePlanet.x, (float)thePlanet.y, (float)thePlanet.z);

        planetEclipticCartesianCoordinates( &thePlanet, &neptune );
        addCartesianCoordinates( &thePlanet, &theSun );
        FPRINTF("%10s x %10.6f y %10.6f z %10.6f\n\n", neptune.name, (float)thePlanet.x, (float)thePlanet.y, (float)thePlanet.z);
    }
    printf("\nEnd\n");
    return 0;
}

