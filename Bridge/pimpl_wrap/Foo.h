#pragma once

#include "pimpl.h"

class Foo
{
  class impl;
  pimpl<impl> impl; // provides all the all the details you need, effectively hides everything inside this class.

public:
  void greet();
};

// shrink-wrapped implementation of the pimpled idiom
// don't have to manage a real pointer anyway because we're using a smart pointer
// also you just have this prepackaged approach

