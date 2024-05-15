#ifndef _FUNCTIONS_HPP
#define _FUNCTIONS_HPP

#include <common.hpp>
#include "../rage/rage.hpp"

class Functions {
public:
    typedef scrNativeHandler(__fastcall* GetNativeHandlerFn)(scrNativeRegistrationTable*, scrNativeHash);
    typedef void(__fastcall* FixVectorsFn)(scrNativeCallContext*);

    typedef bool(__fastcall* ConstructBasketFn)(__int64 a1, int* transactionId, int categoryType, int actionType, int target);
    typedef bool(__fastcall* AddItemToBasketFn)(__int64 a1, int* item);
    typedef bool(__fastcall* ProcessTransactionFn)(__int64 a1, int transaction, __int64 a3);
};

#endif