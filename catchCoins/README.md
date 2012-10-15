Catch Coins!
============

> Catch coins, but don't get caught!
> 
> -- <cite>A great game's introductory text</cite>

Aim
---

In this game, you have to catch the coins that appear randomly on the screen.
Beware, some monsters might try to steal them from you!

Hardware
--------

* Arduino Board (Tested with Arduino Uno)
* LCD screen (sized 16x2)

I also used [this shield](http://www.dfrobot.com/index.php?route=product/product&path=123_124&product_id=51) to connect the screen to the board.

Software
--------

* [LiquidCrystal](http://arduino.cc/en/Reference/LiquidCrystal) library

How to use
----------

1. Connect the arduino to the LCD Board
2. get the code in your sketchbook folder
3. update the values to load the librarie with your pins

Info
----

You can move 3 times as fast as those scary monster, but don't forget that great power comes with great responsibility!
Try pressing the right button from start to experiment it!

The game speed increases with time and each time you catch a coin. Be fast to get the best score!

Some blips are remaining, but it's for the fun! If you are precise enough, you can cross a monster without him seeing you!

Possible Extensions
-------------------

* Add LEDs that blink greed/red depending on events
* Add IR to make the game multiplayer (maybe send monsters to other's screens?)
* Add alcohol sensor to adapt the speed game depending on how drunk you are

*Many thanks to [joseole](https://github.com/joseole) for his help*
