#define TYPE_TO_PAR(R,DATA,I,ELEM) (ELEM _A##I)
#define PAR_SEQ(NR,PARS) BOOST_PP_SEQ_FOR_EACH_I(TYPE_TO_PAR,0,BOOST_PP_TUPLE_TO_SEQ(NR,PARS))
#define PARS(NR,PARS) BOOST_PP_SEQ_ENUM(PAR_SEQ(NR,PARS))
#define ARGS(NR) BOOST_PP_ENUM_PARAMS(NR,_A)

#define BOOST_CLASS_TRACKING_TEMPLATE(T,NR,P,TRACKING) 			\
	template<PARS(NR,P)> struct tracking_level<T<ARGS(NR)> > {	\
	typedef mpl::integral_c_tag tag;							\
	typedef mpl::int_<TRACKING> type;							\
	BOOST_STATIC_CONSTANT(int, value = tracking_level::type::value);	\
};
