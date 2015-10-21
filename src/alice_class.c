/*  =========================================================================
    alice_class - Do we need at least one class?

    Copyright (c) etc.
    =========================================================================
*/

/*
@header
    alice_class - Do we need at least one class?
@discuss
@end
*/

#include "../include/alice.h"

//  Structure of our class

struct _alice_class_t {
    int filler;
    //  TODO: Declare properties
};


//  --------------------------------------------------------------------------
//  Create a new alice_class.

alice_class_t *
alice_class_new ()
{
    alice_class_t *self = (alice_class_t *) zmalloc (sizeof (alice_class_t));
    assert (self);

    //  TODO: Initialize properties

    return self;
}

//  --------------------------------------------------------------------------
//  Destroy the alice_class.

void
alice_class_destroy (alice_class_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        alice_class_t *self = *self_p;

        //  TODO: Free class properties

        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}


//  --------------------------------------------------------------------------
//  Print properties of the alice_class object.

void
alice_class_print (alice_class_t *self)
{
    assert (self);
}


//  --------------------------------------------------------------------------
//  Self test of this class.

void
alice_class_test (bool verbose)
{
    printf (" * alice_class: ");

    //  @selftest
    //  Simple create/destroy test
    alice_class_t *self = alice_class_new ();
    assert (self);
    alice_class_destroy (&self);
    //  @end

    printf ("OK\n");
}
