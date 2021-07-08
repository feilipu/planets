# planets

A rough calculation of planetary motion - to test floating point solutions

# purpose

To test various floating point packages available for the RC2014 and other z88dk supported targets.

# usage

```sh
    zcc +test -v -m -O2 --list -lm -DPRINTF @planets.lst -o planetgm_sccz80_ticks.bin
    zcc +test -v -m -O2 --list -lmath48 -DPRINTF @planets.lst -o planet48_sccz80_ticks.bin
    zcc +test -v -m -O2 --list --math32 -DPRINTF @planets.lst -o planet32_sccz80_ticks.bin

    zcc +test -compiler=sdcc -v -m -SO3 --list -lmath48 -DPRINTF @planets.lst -o planet48_sdcc_ticks.bin
    zcc +test -compiler=sdcc -v -m -SO3 --list --math32 -DPRINTF @planets.lst -o planet32_sdcc_ticks.bin

    z88dk-ticks -counter 99999999999 planetxx_x_ticks.bin

    zcc +rc2014 -subtype=cpm -clib=new -v -m -O2 --list -lm -DPRINTF @planets.lst -o planetnew_cpm -create-app
    zcc +rc2014 -subtype=cpm -v -m --list -lm -DPRINTF @planets.lst -o planet48_cpm -create-app
    zcc +rc2014 -subtype=cpm -v -m --list --math32 -DPRINTF @planets.lst -o planet32_cpm -create-app

    zcc +rc2014 -subtype=cpm -v -m --list --am9511 -DPRINTF @planets.lst -o planetapu_cpm -create-app
    zcc +rc2014 -subtype=cpm -v -m --list --am9511 -DPRINTF @planetsmapu.lst -o planetmapu_cpm -create-app
```

# results

40 calculations for 9 bodies on `z88dk-ticks` - no printing (`-DPRINTF` not defined, no other `printf()`).
```
 -  sccz80/classic/genmath  Ticks: 967407074
 -  sccz80/new/math48       Ticks: 769092327
 -  sccz80/new/math32       Ticks: 252826233

 -  sdcc/new/math48         Ticks: 735457135
 -  sdcc/new/math32         Ticks: 244811563
```

40 calculations for 9 bodies on RC2014 (CPM-IDE) with reduced printing (`-DPRINTF` not defined).
```
 -  sccz80/new/math48       105.5 seconds
 -  sdcc/new/math48         101.4 seconds
 -  sdcc/new/math32          34.3 seconds

 -  sdcc/new/am9511          17.1 seconds
 -  sdcc/new/am9511 4x        9.1 seconds
```

# credits

Based on the work of [Paul Schlyter](http://www.stjarnhimlen.se/english.php).

 - [How to compute planetary postitions](http://www.stjarnhimlen.se/comp/ppcomp.html).
 - [Tutorial on computing planetary positions](http://www.stjarnhimlen.se/comp/tutorial.html).

And the implementations of [Cosine Kitty (Don Cross)](http://cosinekitty.com/)

 - [Solar System Calculator](https://cosinekitty.com/solar_system.html).
 - [Astronomy Sky View](http://cosinekitty.com/sky_view.html).

# licence

MIT License
