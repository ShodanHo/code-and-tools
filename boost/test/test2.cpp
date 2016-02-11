#include <string>

#define BOOST_TEST_MODULE atoi
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

namespace impl {

int atoi(const std::string &val) {
    int result = 0;
    int multiplier = 1;
    bool negative = false;

    for (std::string::const_reverse_iterator i = val.rbegin();
         i < val.rend();  ++i) {
        if (*i <= '9' && *i>= '0' ) {
            result += (*i - '0') * multiplier;
            multiplier *= 10;
        } else {
            if (*i == '-' && i == val.rend() - 1) {
                negative = true;
            } else {
                return 0;
            }
        }
    }
    return negative ? -result : result;
}

} // namespace impl

#if 0

BOOST_AUTO_TEST_CASE(unit_position) {
    BOOST_CHECK_EQUAL(impl::atoi("6"), 6);
}

#else
/*
 * found by:
 * c++ -E test2.cpp
 */
struct unit_position : public BOOST_AUTO_TEST_CASE_FIXTURE {
  void test_method();
};

static void unit_position_invoker() {
  unit_position t;
  t.test_method();
}

struct unit_position_id {};

static boost::unit_test::ut_detail::auto_test_unit_registrar
unit_position_registrar33(boost::unit_test::make_test_case( &unit_position_invoker, "unit_position" ),
			  boost::unit_test::ut_detail::auto_tc_exp_fail< unit_position_id>::instance()->value() );

void unit_position::test_method() {
  do {
    ::boost::unit_test::unit_test_log.set_checkpoint( boost::unit_test::const_string( "test2.cpp",
										      sizeof( "test2.cpp" ) - 1 ),
						      static_cast<std::size_t>(34) );
    ::boost::test_tools::tt_detail::check_frwd( ::boost::test_tools::tt_detail::equal_impl_frwd(),
						::boost::unit_test::lazy_ostream::instance() << "",
						boost::unit_test::const_string( "test2.cpp",
										sizeof( "test2.cpp" ) - 1 ),
						static_cast<std::size_t>(34),
						::boost::test_tools::tt_detail::CHECK,
						::boost::test_tools::tt_detail::CHECK_EQUAL ,
						impl::atoi("6"),
						"impl::atoi(\"6\")" ,
						6,
						"6" );
  } while( ::boost::test_tools::dummy_cond );
}

#endif
