# planets

A rough calculation of planetary motion - to test floating point solutions

# purpose

To test various floating point packages available for the RC2014 and other z88dk supported targets.

# usage

```sh
    zcc +test -v -m -O2 --list -lm -DPRINTF @planets.lst -o planet_ticks.bin

    zcc +test -v -m -O2 --list -lmath48 -DPRINTF @planets.lst -o planet48_sccz80_ticks.bin
    zcc +test -compiler=sdcc -v -m -SO3 --list -lmath48 -DPRINTF @planets.lst -o planet48_sdcc_ticks.bin

    zcc +rc2014 -subtype=cpm -clib=new -v -m -O2 --list -lm -DPRINTF @planets.lst -o planetnew_cpm -create-app
    zcc +rc2014 -subtype=cpm -v -m --list -lm -DPRINTF @planets.lst -o planet48_cpm -create-app
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

# credits

Based on the work of [Paul Schlyter](http://www.stjarnhimlen.se/english.php).

 - [How to compute planetary postitions](http://www.stjarnhimlen.se/comp/ppcomp.html).
 - [Tutorial on computing planetary positions](http://www.stjarnhimlen.se/comp/tutorial.html).

And the implementations of [Cosine Kitty (Don Cross)](http://cosinekitty.com/)

 - [Solar System Calculator](https://cosinekitty.com/solar_system.html).
 - [Astronomy Sky View](http://cosinekitty.com/sky_view.html).

# licence

MIT License
