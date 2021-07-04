
// numeric constants...

#define METERS_PER_ASTRONOMICAL_UNIT        1.4959787e+11
#define METERS_PER_EARTH_EQUATORIAL_RADIUS  6378160.0
#define EARTH_RADII_PER_ASTRONOMICAL_UNIT   (METERS_PER_ASTRONOMICAL_UNIT/METERS_PER_EARTH_EQUATORIAL_RADIUS)

// macros

#define rad(x)      ((x)*(M_PI/180.0))
#define deg(x)      ((x)*(180.0/M_PI))

#define rad_hr(x)   ((x)*(M_PI/12.0))
#define hr_rad(x)   ((x)*(12.0/M_PI))

// type definitions

typedef struct cartesian_coordinates_s {
    float x;
    float y;
    float z;
    float day;
} cartesian_coordinates_t;

typedef struct planet_s {   // See  http://www.stjarnhimlen.se/comp/ppcomp.html#4
    char * name;            // name of the object, e.g. "Mars".
    float N0, Nc;           // N0 = longitude of the ascending node (deg).  Nc = rate of change in deg/day.
    float i0, ic;           // inclination to the ecliptic (deg).
    float w0, wc;           // argument of perihelion (deg).
    float a0, ac;           // semi-major axis, or mean distance from Sun (AU).
    float e0, ec;           // eccentricity (0=circle, 0..1=ellipse, 1=parabola).
    float M0, Mc;           // M0 = mean anomaly (deg) (0 at perihelion; increases uniformly with time).  Mc ("mean motion") = rate of change in deg/day = 360/period.
} planet_t;

// utility functions (assembly)

extern float rev(float x) __z88dk_fastcall;

