#pragma once


#define DEF_FIELDS_B constexpr static auto __fields = std::make_tuple(

#define DEF_FIELDS_C(C,F) &##C##::##F,#F

#define DEF_FIELDS_E );

#define DEF_GET_FIELD_NAME	\
template<size_t I, typename CLS, typename F>					\
constexpr static const char * get_field_name_sub(F CLS::* f) {	\
																\
	if constexpr (std::is_same_v<std::remove_const_t<std::remove_reference_t<decltype(std::get<I>(__fields))>>, F CLS::*>)	\
	{																														\
		if (std::get<I>(__fields) == f)																						\
		{																													\
			return std::get<I + 1>(__fields);																				\
		}																													\
	}																														\
	if constexpr ((I + 2) < std::tuple_size<decltype(__fields)>::value)														\
	{																														\
		return get_field_name_sub<I + 2, CLS, F>(f);																		\
	}																														\
	return nullptr;																											\
}																															\
																															\
template<typename CLS, typename F>																							\
constexpr static const char * get_field_name(F CLS::* f) {																	\
	return get_field_name_sub<0, CLS, F>(f);																				\
}																															\

#define DEF_GET_CLS_NAME(C)	constexpr static const char *get_class_name() { return #C; }