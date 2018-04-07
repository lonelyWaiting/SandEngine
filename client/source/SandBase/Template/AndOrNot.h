#pragma once

template<typename... Types>
struct TAnd;

template<bool LHSValue , typename... RHS>
struct TAndValue
{
	enum
	{
		Value = TAnd<RHS...>::Value
	};
};

template<typename... RHS>
struct TAndValue<false , RHS...>
{
	enum
	{
		Value = false
	};
};

template<typename LHS , typename... RHS>
struct TAnd<LHS , RHS...> : TAndValue<LHS::Value , RHS...>
{

};

template<>
struct TAnd<>
{
	enum
	{
		Value = true
	};
};

// Does a boolean Or of the ::Value static member of each type, but short-circuits if any Type::Value == true. 
template<typename... Types>
struct TOr;

template<bool LHSValue , typename... RHS>
struct TOrValue
{
	enum
	{
		// in here, first element of arg list(RHS) will be bind to TOr's first template arg
		// and the rest of element will to form a new arg list.
		// recursive call until arg list is empty
		// short-circuits if any Type::Value == true
		Value = TOr<RHS...>::Value
	};
};

// short-circuits implement
// if any Type::Value == true , then termination recursion.
template<typename... RHS>
struct TOrValue<true , RHS...>
{
	enum
	{
		Value = true
	};
};

template<typename LHS , typename... RHS>
struct TOr<LHS , RHS...> : TOrValue<LHS::Value , RHS...>
{

};

// if arg list is empty , Value is false
template<>
struct TOr<>
{
	enum
	{
		Value = false
	};
};

// Does a boolean NOT of the ::Value static member of the types.
template<typename Type>
struct TNot
{
	enum
	{
		Value = !Type::Value
	};
};