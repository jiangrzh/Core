/*
 * loops.h
 * Defines several loop specific macros and functions
 *
 * These loops are as follows:
 *
 * Generates a simple for loop that repeats numiter times. Use the
 * long version for repetitions larger than can fit in an unsigned
 * int.
 *
 *
 *         forRepeat(numiter)
 *         forRepeatLong(numiter)
 *
 *
 * Runs a loop iterator from start to end repetitions. This is mostly
 * unnecessary compared to the standard forEach loop, but in cases
 * where direct access to the iterator is needed, this becomes useful
 * again. The loop increments iter each iteration until it matches end.
 *
 *
 *      forIter(iter, start, end)
 *      reverseForIter(iter, start, end)
 *
 *
 * Creates a for loop that iterating through each item in a container.
 * This is a simple renaming of BOOST_FOREACH, which is a fantastic
 * macro for iterating over arbitrary data structures.
 *
 *
 *      forEach(item, container)
 *      reverseForEach(item, container)
 *
 *
 * Performs a forEach on the item and container, but also sets count to
 * the number of times for which the loop as iterated. Thus the first
 * iteration count will be 0, then 1 on the next, and so on until the
 * loop ends.
 *
 *
 *      enumerateEach(count, item, container)
 *      reverseEnumerateEach(count, item, container)
 *
 *
 * Runs the function on each item in container. This is the only loop
 * from this file which is an actual function and not macro based.
 * Since the function is passed to us, it's syntactically possible to
 * keep this inside a function and hold tighter type checks.
 *
 *
 *      onEach(item, function, container)
 *      onEach(function, container)
 *         reverseOnEach(item, function, container)
 *         reverseOnEach(function, container)
 */

#ifndef LOOPS_H_
#define LOOPS_H_

// Don't listen to warnings about boost on msvc
#if defined(_MSC_VER) && (_MSC_VER >= 1500)
#   pragma warning(push, 0)
#endif
#include <boost/foreach.hpp>
#include <boost/any.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#if defined(_MSC_VER) && (_MSC_VER >= 1500)
#   pragma warning(pop)
#endif

/* For Repeat */
#define forRepeat(numiter)                                                          \
    if (unsigned int BOOST_FOREACH_ID(_iter_unique_) = 0) {} else                   \
        for(unsigned int BOOST_FOREACH_ID(_iter_maxiter_) = numiter;                \
            BOOST_FOREACH_ID(_iter_unique_) != BOOST_FOREACH_ID(_iter_maxiter_);    \
            BOOST_FOREACH_ID(_iter_unique_)++)

#define forRepeatLong(numiter)                                                      \
    if (unsigned long long int BOOST_FOREACH_ID(_iter_unique_) = 0) {} else         \
        for(unsigned long long int BOOST_FOREACH_ID(_iter_maxiter_) = numiter;      \
            BOOST_FOREACH_ID(_iter_unique_) != BOOST_FOREACH_ID(_iter_maxiter_);    \
            BOOST_FOREACH_ID(_iter_unique_)++)

/* End For Repeat */

/* For Iter */
#define forIter(iter, start, end) for(iter = start; iter != end; iter++)
#define reverseForIter(iter, start, end) for(iter = start; iter != end; iter--)
/* End For Iter */

/* For Each */
#define forEach BOOST_FOREACH
#define reverseForEach BOOST_REVERSE_FOREACH
/* End For Each */

/* Enumerate */
namespace core { namespace loops { namespace detail {
template<typename T>
void incrementT(T *t) {
    (*t)++;
}

struct IncrementCounterPassthrough {
    bool first;
    boost::function0<void> incrementer;

    template<typename Count>
    IncrementCounterPassthrough(Count& t) {
        t = 0;
        // Use first = true to avoid negative assignment to t
        first = true;
        incrementer = boost::bind(&incrementT<Count>, &t);
    }

    template<typename T>
    T& operator=(T& rhs) {
        if (first) {
            first = false;
        } else {
            incrementer();
        }
        return rhs;
    }
};
}}}

#define enumerateEach(count, item, container)                                                               \
    if (bool BOOST_FOREACH_ID(_enum_unique_) = false) {} else                                               \
    for(::core::loops::detail::IncrementCounterPassthrough BOOST_FOREACH_ID(_foreach_count_pass_)(count);   \
            !BOOST_FOREACH_ID(_enum_unique_);                                                               \
            BOOST_FOREACH_ID(_enum_unique_) = true)                                                         \
        BOOST_FOREACH(item = BOOST_FOREACH_ID(_foreach_count_pass_), container)

#define reverseEnumerateEach(count, item, container)                                                        \
    if (bool BOOST_FOREACH_ID(_enum_unique_) = false) {} else                                               \
    for(::core::loops::detail::IncrementCounterPassthrough BOOST_FOREACH_ID(_foreach_count_pass_)(count);   \
            !BOOST_FOREACH_ID(_enum_unique_);                                                               \
            BOOST_FOREACH_ID(_enum_unique_) = true)                                                         \
        BOOST_REVERSE_FOREACH(item = BOOST_FOREACH_ID(_foreach_count_pass_), container)
/* End Enumerate */

/* On Each */
template <class I, class C>
inline void onEach(void (*function)(I), C & container) {
    forEach(I& item, container) function(item);
}
template <class I, class C>
inline void onEach(I& item, void (*function)(I), C & container) {
    forEach(item, container) function(item);
}
template <class I, class C>
inline void onEach(void (*function)(I&), C & container) {
    forEach(I& item, container) function(item);
}
template <class I, class C>
inline void onEach(I& item, void (*function)(I&), C & container) {
    forEach(item, container) function(item);
}
// Do the same for boost::function
template <class I, class C>
inline void onEach(boost::function1<void, I>& function, C & container) {
    forEach(I& item, container) function(item);
}
template <class I, class C>
inline void onEach(I& item, boost::function1<void, I>& function, C & container) {
    forEach(item, container) function(item);
}
template <class I, class C>
inline void onEach(boost::function1<void, I&>& function, C & container) {
    forEach(I& item, container) function(item);
}
template <class I, class C>
inline void onEach(I& item, boost::function1<void, I&>& function, C & container) {
    forEach(item, container) function(item);
}
/* End On Each */

/* On Each Reverse */
template <class I, class C>
inline void reverseOnEach(void (*function)(I), C & container) {
    reverseForEach(I& item, container) function(item);
}
template <class I, class C>
inline void reverseOnEach(I& item, void (*function)(I), C & container) {
    reverseForEach(item, container) function(item);
}
template <class I, class C>
inline void reverseOnEach(void (*function)(I&), C & container) {
    reverseForEach(I& item, container) function(item);
}
template <class I, class C>
inline void reverseOnEach(I& item, void (*function)(I&), C & container) {
    reverseForEach(item, container) function(item);
}
// Do the same for boost::function
template <class I, class C>
inline void reverseOnEach(boost::function1<void, I>& function, C & container) {
    reverseForEach(I& item, container) function(item);
}
template <class I, class C>
inline void reverseOnEach(I& item, boost::function1<void, I>& function, C & container) {
    reverseForEach(item, container) function(item);
}
template <class I, class C>
inline void reverseOnEach(boost::function1<void, I&>& function, C & container) {
    reverseForEach(I& item, container) function(item);
}
template <class I, class C>
inline void reverseOnEach(I& item, boost::function1<void, I&>& function, C & container) {
    reverseForEach(item, container) function(item);
}
/* End On Each Reverse */

#endif /* LOOPS_H_ */
