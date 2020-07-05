Gigabyte Aero 15x (v8) Fan Controller
=====================================
This provides a naive implementation of a fan controller for the Aero 15x 
platform using the embedded controller system. Mostly because I wanted to write
a `systemd` daemon and I was fed up with relying on the internal fan speed control
with the laptop being quiet and then suddenly sounding like a jet engine.

First and foremost, I know very little of dealing with embedded devices, so
please use at your own risk.

The original work found on this was from the
[p37-ec repository](https://github.com/jertel/p37-ec).

The core of the application consists of:

- `ec_sys` based interface for the Aero15x;
- State based fan speeds based on CPU and GPU temperature;
- Filtering to remove erroneous transient temperatures;
- Logging facility for systemd/journald.


And the applications provided are:
- A `systemd` "daemon" that controls the fan speed using detected temperatures;
- A utility app to enable custom speeds and turn fans to maximum;
- Utility app to disable custom speeds;


Prerequisites
-------------
- Must be a Gigabyte Aero15x laptop. This has only been tested on the v8 model.
- For the standalone applications, `modprobe ec_sys write_support=1` must have
  been setup. The `systemd` service does this on startup automatically.
- Applications must be started as root. This should be done when the resultant
  packages are installed, but be mindful when starting from the build directory.


Detecting the Platform
----------------------
We expect the contents of `/sys/devices/virtual/dmi/id/sys_vendor` to equal
"GIGABYTE" and the contents of `/sys/devices/virtual/dmi/id/product_name` to 
equal "P65Q".


Detecting Changes
-----------------
The daemon reads temperatures from `ec_sys`, filters it with a 
median filter with a window of 3 samples, passes it to the change detector and
writes the required fan speed back to `ec_sys`.

The states aren't particularly clever, I currently have them hard coded as:

|State   | Up  | Down | Fan speed |
|--------|-----|------|-----------|
| max    | 255 |   75 |       229 |
| high   | 80  |   65 |       195 |
| medium | 70  |   55 |       160 |
| low    | 60  |   45 |       125 |
| min    | 50  | -255 |        90 |

For a given state, if the temperature is greater than or equal to the "up"
temperature, it will jump the next highest state, next lowest for "down". Down
temperatures are lower than the up temps so that we know we are a decent
distance away from going back up and therefore at little risk of immediately
changing back up again. For the max and min states, up and down values are
ignored.

I used a `std::array` to store the states, but I could have used a linked list.
Although using an index into an array has an advantage, the whole instance can
be copied without side-effects. If you were to use a pointer or
`std::reference_wrapper` and the thing is copied, what should the pointer to the
thing be? An index into a list could be also used but lookups are done in linear
time (not that it matters for like 5 entries...).

The initial state is max; this will eventually settle to the correct state. It
is better to take a cautious approach when starting; if the system is already
melting down, it makes little sense to have to wait before the fans are set
high. Also it's simpler than scanning through all the states to find the best
one to start with. (Although arguably this could still happen if the fans were
at the lowest state and you threw it into the sun, or started Chrome.)


Logging
-------
Possibly went a bit overboard with this, but I rolled my own logger 
implementation. The only real requirement (for the daemon) was that I could emit
messages compatible with journald, and that I wanted to be able to access using
global functions rather than a class instance. The technique for this was
inspired by `libstdc++`'s method of storing memory resources for the (as of the
moment) experimental polymorphic memory resources. Very basically, the default
logger is instantiated statically in a getter function (initialisation on first
use idiom), the pointer to which stored in a `std::atomic` holder which is then
dereferenced any time a logging call is made. Because of the use of 
`std::atomic`, it is my understanding that replacing the logger pointer should
happen in a thread-safe way.

Caveat is that the lifetime of the replacement logger must exist for longer than
any expected call being made to it. This gets a little more ropey if you need
to have a replacement logger used in a static context (e.g. construction of
static objects) because of the static-initialisation-order fiasco.

As an additional note, this may not be liked because of it's reliance on
globals. While the default logger static variable could be factored out, there
would still be a global `std::atomic` holder. Maybe `instance.log(...)` is not
so bad after all...


Testing
-------
I did what I could.