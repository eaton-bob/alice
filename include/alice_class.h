/*  =========================================================================
    alice_class - Do we need at least one class?

    Copyright (c) etc.
    =========================================================================
*/

#ifndef ALICE_CLASS_H_INCLUDED
#define ALICE_CLASS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif


//  @interface
//  Create a new alice_class
ALICE_EXPORT alice_class_t *
    alice_class_new (void);

//  Destroy the alice_class
ALICE_EXPORT void
    alice_class_destroy (alice_class_t **self_p);

//  Print properties of object
ALICE_EXPORT void
    alice_class_print (alice_class_t *self);

//  Self test of this class
ALICE_EXPORT void
    alice_class_test (bool verbose);
//  @end

#ifdef __cplusplus
}
#endif

#endif
