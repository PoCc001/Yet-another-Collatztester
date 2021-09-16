# Yet-another-Collatztester
This program tests, if the Collatz conjecture holds for all positive integers. To be able to compile and run this program you must use Visual Studio and a x64 processor, as it depends on compiler intrinsics to speed up the calculations. Alternatively, you would have the option to either write machine-independent C++ code, instead of the intrinsics or make use of inline assembly or intrinsics that are supported by your hardware and compiler.

Feel free to expand this small piece of software, as it is not very user-friendly and feature-rich. You could also make it multi-threaded and optimize it further.

If a positive integer happens to be found to (likely) not go to 1 eventually, you will most certainly have to verify this specific number using another program for it to be publicly accepted. This is because Yet-another-Collatztester doesn't provide a way of proving that your result was actually calculated.
