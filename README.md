# planets

A rough calculation of planetary motion - to test floating point solutions

# purpose

To test various floating point packages available for the RC2014 and other z88dk supported targets.

# usage

```sh
    zcc +rc2014 -subtype=cpm -v -m --list -lm -DPRINTF -Ca-D__MATH_MATH48 @planets.lst -o planet48_cpm -create-app
    zcc +rc2014 -subtype=cpm -v -m --list --math32 -DPRINTF @planets.lst -o planet32_cpm -create-app
    zcc +rc2014 -subtype=cpm -v -m --list --am9511 -DPRINTF @planets.lst -o planetapu_cpm -create-app
    zcc +rc2014 -subtype=cpm -v -m --list --am9511 -DPRINTF @planetsmapu.lst -o planetmapu_cpm -create-app
```

# results

40 calculations for 9 bodies on RC2014 (CPM-IDE) with reduced printing (`-DPRINTF` not defined).

 - math48     103.1 seconds
 - math32      34.4 seconds
 - am9511      17.2 seconds
 - am9511 4x   12.5 seconds

# licence

MIT License
