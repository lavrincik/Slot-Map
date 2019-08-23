#include "slotmap.hpp"

#include "catch.hpp"

// explicitly instantiate all non-templated members
template struct slot_map< int >;

TEST_CASE( "ctors" ) {
	slot_map< int > sm;
	REQUIRE( sm.empty() );
	REQUIRE( sm.size() == 0 );

	slot_map< int > sm2( sm );
	REQUIRE( sm.empty() );
	REQUIRE( sm.size() == 0 );
	
	typename slot_map< int >::handle h;
}

TEST_CASE( "basic interface" ) {
	slot_map< int > sm;
	auto h0 = sm.insert( 0 );
	auto h1 = sm.insert( 1 );
	REQUIRE( sm.valid( h0 ) );
	sm.erase( h1 );
	sm[ h0 ] = 42;
	for ( auto &x : sm )
		REQUIRE( x == 42 );
	REQUIRE( sm.get( h0 ) != nullptr );
}

TEST_CASE("Container concepts") {
	slot_map<int> sm;	
	for (int i : {0, 1, 2, 3, 4}) {
		sm.insert(i);
	}

	slot_map<int> copy_sm(sm);
	int tmp = 0;
	for (int i : copy_sm) {
		REQUIRE(i == tmp);
		tmp++;
	}
	REQUIRE(sm == copy_sm);

	slot_map<int> sm1;
	sm1 = sm;
	tmp = 0;
	for (int i : sm1) {
		REQUIRE(i == tmp);
		tmp++;
	}

	REQUIRE(sm == sm1);
	REQUIRE(sm.size() == 5);
	REQUIRE(!sm.empty());

	slot_map<int> empty_sm;
	REQUIRE(empty_sm.empty());

	sm.swap(empty_sm);
	REQUIRE(sm.empty());
	REQUIRE(empty_sm == sm1);

	swap(empty_sm, sm);
	REQUIRE(empty_sm.empty());
	REQUIRE(sm == sm1);
}

TEST_CASE("Interface") {
	slot_map<int> sm;
	auto h1 = sm.insert(1);
	auto h2 = sm.insert(2);
	auto h3 = sm.insert(3);
	auto h4 = sm.insert(4);
	auto h5 = sm.insert(5);

	REQUIRE(sm[h1] == 1);
	REQUIRE(sm[h2] == 2);
	REQUIRE(sm[h3] == 3);
	REQUIRE(sm[h4] == 4);
	REQUIRE(sm[h5] == 5);

	REQUIRE(*(sm.get(h1)) == 1);
	REQUIRE(*(sm.get(h5)) == 5);

	REQUIRE(sm.valid(h1));
	REQUIRE(sm.valid(h5));

	sm.erase(h5);
	REQUIRE(sm.size() == 4);
	REQUIRE(!sm.valid(h5));

	sm.erase(h1);
	REQUIRE(sm.size() == 3);
	REQUIRE(!sm.valid(h1));

	REQUIRE(sm.get(h5) == nullptr);
}

TEST_CASE("insert insert insert erase") {
	slot_map<int> sm;
	auto h1 = sm.insert(1);
	auto h2 = sm.insert(2);
	auto h3 = sm.insert(3);

	REQUIRE(sm[h1] == 1);
	REQUIRE(sm[h2] == 2);
	REQUIRE(sm[h3] == 3);

	REQUIRE(sm.valid(h1));
	REQUIRE(sm.valid(h2));
	REQUIRE(sm.valid(h3));

	sm.erase(h2);
	REQUIRE(sm.size() == 2);
	REQUIRE(!sm.valid(h2));
}

TEST_CASE("insert insert insert erase insert") {
	slot_map<std::string> sm;

	auto h1 = sm.insert("a");
	auto h2 = sm.insert("b");
	auto h3 = sm.insert("c");

	REQUIRE(sm[h1] == "a");
	REQUIRE(sm[h2] == "b");
	REQUIRE(sm[h3] == "c");

	REQUIRE(sm.valid(h1));
	REQUIRE(sm.valid(h2));
	REQUIRE(sm.valid(h3));

	sm.erase(h2);
	REQUIRE(sm.size() == 2);
	REQUIRE(!sm.valid(h2));

	auto h4 = sm.insert("d");

	REQUIRE(sm[h4] == "d");
	REQUIRE(sm.valid(h4));

}

TEST_CASE("lots of inserts and erases and inserts") {
	slot_map<int> sm;
	std::vector<slot_map<int>::handle> handles;

	for (int i = 0; i < 100; ++i) {
		handles.emplace_back(sm.insert(i));
	}

	for (int i = 0; i < 100; ++i) {
		REQUIRE(sm[handles[i]] == i);
		REQUIRE(sm.valid(handles[i]));
	}

	for (int i = 0; i < 100; i += 2) {
		sm.erase(handles[i]);
	}

	REQUIRE(sm.size() == 50);

	for (int i = 0; i < 100; ++i) {
		if (i % 2 == 0) {
			REQUIRE(!sm.valid(handles[i]));
			REQUIRE(sm.get(handles[i]) == nullptr);
		} else {
			REQUIRE(sm.valid(handles[i]));
			REQUIRE(*(sm.get(handles[i])) == i);
			REQUIRE(sm[handles[i]] == i);
		}
	}

	for (int i = 100; i < 150; ++i) {
		handles.emplace_back(sm.insert(i));
	}

	for (int i = 100; i < 150; ++i) {
		REQUIRE(sm[handles[i]] == i);
		REQUIRE(sm.valid(handles[i]));
	}

	REQUIRE(sm.size() == 100);
}

TEST_CASE("even more of inserts and erases and inserts and erases and inserts") {
	slot_map<int> sm;
	std::vector<slot_map<int>::handle> handles;

	for (int i = 0; i < 100; ++i) {
		handles.emplace_back(sm.insert(i));
	}

	for (int i = 0; i < 100; ++i) {
		REQUIRE(sm[handles[i]] == i);
		REQUIRE(sm.valid(handles[i]));
	}

	for (int i = 0; i < 100; i += 2) {
		sm.erase(handles[i]);
	}

	REQUIRE(sm.size() == 50);

	for (int i = 0; i < 100; ++i) {
		if (i % 2 == 0) {
			REQUIRE(!sm.valid(handles[i]));
			REQUIRE(sm.get(handles[i]) == nullptr);
		} else {
			REQUIRE(sm.valid(handles[i]));
			REQUIRE(*(sm.get(handles[i])) == i);
			REQUIRE(sm[handles[i]] == i);
		}
	}

	for (int i = 100; i < 150; ++i) {
		handles.emplace_back(sm.insert(i));
	}

	for (int i = 100; i < 150; ++i) {
		REQUIRE(sm[handles[i]] == i);
		REQUIRE(sm.valid(handles[i]));
	}

	REQUIRE(sm.size() == 100);

	for (int i = 1; i < 100; i+=2) {
		sm.erase(handles[i]);
	}

	REQUIRE(sm.size() == 50);

	for (int i = 100; i < 150; ++i) {
		REQUIRE(sm.valid(handles[i]));
		REQUIRE(*(sm.get(handles[i])) == i);
		REQUIRE(sm[handles[i]] == i);
	}

	for (int i = 0; i < 100; ++i) {
		REQUIRE(!sm.valid(handles[i]));
		REQUIRE(sm.get(handles[i]) == nullptr);
	}
	
	for (int i = 150; i < 200; ++i) {
		handles.emplace_back(sm.insert(i));
	}

	for (int i = 150; i < 200; ++i) {
		REQUIRE(sm[handles[i]] == i);
		REQUIRE(sm.valid(handles[i]));
	}
}
