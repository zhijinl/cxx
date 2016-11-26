Author: Zhijin Li  
Date:   Nov 27, 2016

##
Some C++ tricks that I think worth sharing

The list may be getting bigger in the future: I'll probably
write down whatever I think is interesting here :-)

- I use a lot of stuff from `boost` and `Eigen`, so if you are
  interested in testing mu code, you'll probably want to have
  the **lastest version** of these two libraries.
- Almost all codes require at least C++11 (sometimes C++14).
  The last time I checked, we are in 2016 :p

Current stuff that I've got:

1. **Expression templates**: sorted of C++'s implementation of
   the __Lazy-Evaluation__ for numerical computations, especially
   linear algebra. Basically, expression templates construct an
   evaluation graph of the computation flow at compile-time. This
   avoids immediate generation of temporary computation results,
   which in some (most) cases, are unnecessary. I wrote a minimal
   example expression template project, demonstrating how it could
   be done by hand for simple point-wise computation cases, and
   what the performance boost is compared to naive computations.

Expression templates are rather complicated, especially when you
have more advanced computations involved. And they also present
some caveats / pitfalls (such as the use of C++11 `auto` together
with expression templates). The `Eigen` library is a good place to
go if you want to learn more about them. I recommand that you take
a look the amazing documentations of `Eigen` (ref)
[https://eigen.tuxfamily.org/dox/TopicInsideEigenExample.html?_sm_au_=i5HRq6Vr7tWjtH5D],
if you are interested in doing more cool stuff with expression
templates!

