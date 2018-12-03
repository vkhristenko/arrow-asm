#include "arrow/api.h"

#include "classes.h"

template<typename T>
void fill_builder(arrow::StructBuilder*, T const&);
template<>
void fill_builder(arrow::StructBuilder* pbuilder, v1::A const& s) {
	pbuilder->Append();
	static_cast<arrow::Int32Builder*>(pbuilder->field_builder( 0 ))->Append( s.a );
	static_cast<arrow::Int32Builder*>(pbuilder->field_builder( 1 ))->Append( s.b );
	static_cast<arrow::Int32Builder*>(pbuilder->field_builder( 2 ))->Append( s.c );
	static_cast<arrow::FloatBuilder*>(pbuilder->field_builder( 3 ))->Append( s.x );
	static_cast<arrow::FloatBuilder*>(pbuilder->field_builder( 4 ))->Append( s.y );
	static_cast<arrow::FloatBuilder*>(pbuilder->field_builder( 5 ))->Append( s.z );
	static_cast<arrow::Int32Builder*>(pbuilder->field_builder( 6 ))->Append( s.q );
	static_cast<arrow::Int32Builder*>(pbuilder->field_builder( 7 ))->Append( s.t );
	static_cast<arrow::Int32Builder*>(pbuilder->field_builder( 8 ))->Append( s.v );
}

template<>
void fill_builder(arrow::StructBuilder* pbuilder, v1::B const& s) {
	pbuilder->Append();
	static_cast<arrow::Int32Builder*>(pbuilder->field_builder( 0 ))->Append( s.a );
	static_cast<arrow::Int32Builder*>(pbuilder->field_builder( 1 ))->Append( s.b );
	static_cast<arrow::Int32Builder*>(pbuilder->field_builder( 2 ))->Append( s.c );
	static_cast<arrow::FloatBuilder*>(pbuilder->field_builder( 3 ))->Append( s.x );
	static_cast<arrow::FloatBuilder*>(pbuilder->field_builder( 4 ))->Append( s.y );
	static_cast<arrow::FloatBuilder*>(pbuilder->field_builder( 5 ))->Append( s.z );
	static_cast<arrow::Int32Builder*>(pbuilder->field_builder( 6 ))->Append( s.q );
	static_cast<arrow::Int32Builder*>(pbuilder->field_builder( 7 ))->Append( s.t );
	static_cast<arrow::Int32Builder*>(pbuilder->field_builder( 8 ))->Append( s.v );
}

template<>
void fill_builder(arrow::StructBuilder* pbuilder, v1::s_1 const& s) {
	pbuilder->Append();
	static_cast<arrow::Int32Builder*>(pbuilder->field_builder( 0 ))->Append( s.x );
	static_cast<arrow::Int32Builder*>(pbuilder->field_builder( 1 ))->Append( s.y );
	static_cast<arrow::Int32Builder*>(pbuilder->field_builder( 2 ))->Append( s.z );
}

template<>
void fill_builder(arrow::StructBuilder* pbuilder, v1::s_2 const& s) {
	pbuilder->Append();
	static_cast<arrow::FloatBuilder*>(pbuilder->field_builder( 0 ))->Append( s.q );
	static_cast<arrow::FloatBuilder*>(pbuilder->field_builder( 1 ))->Append( s.y );
	static_cast<arrow::FloatBuilder*>(pbuilder->field_builder( 2 ))->Append( s.t );
}

template<>
void fill_builder(arrow::StructBuilder* pbuilder, v1::s_3 const& s) {
	pbuilder->Append();
	fill_builder(static_cast<arrow::StructBuilder*>(pbuilder->field_builder( 0 )), s.s1 );
	fill_builder(static_cast<arrow::StructBuilder*>(pbuilder->field_builder( 1 )), s.s2 );
}

template<>
void fill_builder(arrow::StructBuilder* pbuilder, v2::C const& s) {
	pbuilder->Append();
	fill_builder(static_cast<arrow::StructBuilder*>(pbuilder->field_builder( 0 )), s.b );
	fill_builder(static_cast<arrow::StructBuilder*>(pbuilder->field_builder( 1 )), s.s3 );
	static_cast<arrow::Int32Builder*>(pbuilder->field_builder( 2 ))->Append( s.x );
	static_cast<arrow::FloatBuilder*>(pbuilder->field_builder( 3 ))->Append( s.q );
}






template<typename T>
struct to_arrow_type {
	static std::shared_ptr<arrow::StructType> get();
};
template<>
struct to_arrow_type<v1::A> {
	static std::shared_ptr<arrow::StructType> get() {
		return std::make_shared<arrow::StructType>(std::vector<std::shared_ptr<arrow::Field>>{
			std::make_shared<arrow::Field>(" a ", std::make_shared<arrow::Int32Type>(), false)
			,std::make_shared<arrow::Field>(" b ", std::make_shared<arrow::Int32Type>(), false)
			,std::make_shared<arrow::Field>(" c ", std::make_shared<arrow::Int32Type>(), false)
			,std::make_shared<arrow::Field>(" x ", std::make_shared<arrow::FloatType>(), false)
			,std::make_shared<arrow::Field>(" y ", std::make_shared<arrow::FloatType>(), false)
			,std::make_shared<arrow::Field>(" z ", std::make_shared<arrow::FloatType>(), false)
			,std::make_shared<arrow::Field>(" q ", std::make_shared<arrow::Int32Type>(), false)
			,std::make_shared<arrow::Field>(" t ", std::make_shared<arrow::Int32Type>(), false)
			,std::make_shared<arrow::Field>(" v ", std::make_shared<arrow::Int32Type>(), false)
		});
	}
};

template<>
struct to_arrow_type<v1::B> {
	static std::shared_ptr<arrow::StructType> get() {
		return std::make_shared<arrow::StructType>(std::vector<std::shared_ptr<arrow::Field>>{
			std::make_shared<arrow::Field>(" a ", std::make_shared<arrow::Int32Type>(), false)
			,std::make_shared<arrow::Field>(" b ", std::make_shared<arrow::Int32Type>(), false)
			,std::make_shared<arrow::Field>(" c ", std::make_shared<arrow::Int32Type>(), false)
			,std::make_shared<arrow::Field>(" x ", std::make_shared<arrow::FloatType>(), false)
			,std::make_shared<arrow::Field>(" y ", std::make_shared<arrow::FloatType>(), false)
			,std::make_shared<arrow::Field>(" z ", std::make_shared<arrow::FloatType>(), false)
			,std::make_shared<arrow::Field>(" q ", std::make_shared<arrow::Int32Type>(), false)
			,std::make_shared<arrow::Field>(" t ", std::make_shared<arrow::Int32Type>(), false)
			,std::make_shared<arrow::Field>(" v ", std::make_shared<arrow::Int32Type>(), false)
		});
	}
};

template<>
struct to_arrow_type<v1::s_1> {
	static std::shared_ptr<arrow::StructType> get() {
		return std::make_shared<arrow::StructType>(std::vector<std::shared_ptr<arrow::Field>>{
			std::make_shared<arrow::Field>(" x ", std::make_shared<arrow::Int32Type>(), false)
			,std::make_shared<arrow::Field>(" y ", std::make_shared<arrow::Int32Type>(), false)
			,std::make_shared<arrow::Field>(" z ", std::make_shared<arrow::Int32Type>(), false)
		});
	}
};

template<>
struct to_arrow_type<v1::s_2> {
	static std::shared_ptr<arrow::StructType> get() {
		return std::make_shared<arrow::StructType>(std::vector<std::shared_ptr<arrow::Field>>{
			std::make_shared<arrow::Field>(" q ", std::make_shared<arrow::FloatType>(), false)
			,std::make_shared<arrow::Field>(" y ", std::make_shared<arrow::FloatType>(), false)
			,std::make_shared<arrow::Field>(" t ", std::make_shared<arrow::FloatType>(), false)
		});
	}
};

template<>
struct to_arrow_type<v1::s_3> {
	static std::shared_ptr<arrow::StructType> get() {
		return std::make_shared<arrow::StructType>(std::vector<std::shared_ptr<arrow::Field>>{
			std::make_shared<arrow::Field>(" s1 ", to_arrow_type< struct v1::s_1 >::get(), false)
			,std::make_shared<arrow::Field>(" s2 ", to_arrow_type< struct v1::s_2 >::get(), false)
		});
	}
};

template<>
struct to_arrow_type<v2::C> {
	static std::shared_ptr<arrow::StructType> get() {
		return std::make_shared<arrow::StructType>(std::vector<std::shared_ptr<arrow::Field>>{
			std::make_shared<arrow::Field>(" b ", to_arrow_type< v1::B >::get(), false)
			,std::make_shared<arrow::Field>(" s3 ", to_arrow_type< v1::s_3 >::get(), false)
			,std::make_shared<arrow::Field>(" x ", std::make_shared<arrow::Int32Type>(), false)
			,std::make_shared<arrow::Field>(" q ", std::make_shared<arrow::FloatType>(), false)
		});
	}
};

