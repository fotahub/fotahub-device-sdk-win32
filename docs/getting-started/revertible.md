# Revertible FOTA update example

The *Revertible* example is an enhanced version of the [Simple](simple.md) example. It demonstrates how a firmware update can automatically revert itself to the previous version in case that something is wrong with the new one.

## Operating principle

The firmware over-the-air update procedure gets triggered and works in the same way as in the [Simple](simple.md) example up to the point where the new firmware version gets activated. Right after starting the new firmware version, a built-in self test procedure is launched. Only when the latter completes successfully, the new firmware version is confirmed and becomes definite. In the opposite case, the previous firmware version is restored.

For obvious reasons, the ultimately required self test logic depends heavily on the nature of the underlying IoT application and device. The revertible example therefore implements only a very simple self test stub which returns a positive or negative result on a pseudo-random basis (see [DemoFOTAUpdateSelfTest.c](../../examples/revertible/DemoFOTAUpdateSelfTest.c) for details). Consequently, the firmware update carried out in this example may succeed and remain in place or fail and be reverted to the previous version depending on how it goes. To see both scenarios happening, just run the same example multiple times.

## Usage

1. Follow the usage instructions of the Simple example as described [here](simple.md).

2. In the very last step, you will observe the same result and output as with the Simple example in case the built-in self test succeeds. Otherwise you will be able to see how the new fimware version stops executing and the previous firmware version resumes instead:

![](revertible-1.png "Reversion to previous firmware version") 