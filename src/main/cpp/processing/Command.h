#pragma once

#define CARGO_TARGET_CODE 'c'
#define SHIP_TARGET_CODE 's'
#define NULL_TARGET_CODE 'n'

#define INFINITE_RETURN_METHOD 'u'
#define SINGLE_RETURN_METHOD 'o'
#define NO_RETURN_METHOD 'n'

#define VALIDATION_CHAR '~'

typedef struct Command{
    bool is_valid;
    char target;
    char method;
} Command;
