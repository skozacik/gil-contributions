/*
    Copyright 2008 Christian Henning, Andreas Pokorny, Lubomir Bourdev
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_IO_DETAIL_GIL_EXTENSIONS_HPP_INCLUDED
#define BOOST_GIL_EXTENSION_IO_DETAIL_GIL_EXTENSIONS_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief Definitions of is_bit_aligned, is_homogeneous, and is_similar metafunctions and
///        some other goodies.
/// \author Christian Henning, Andreas Pokorny, Lubomir Bourdev \n
///         
/// \date   2008 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <boost/gil/gil_all.hpp>

namespace boost { namespace gil {

/// is_bit_aligned metafunctions
/// \brief Determines whether the given type is bit_aligned.

template< typename PixelRef >
struct is_bit_aligned : mpl::false_{};

template <typename B, typename C, typename L, bool M>  
struct is_bit_aligned<bit_aligned_pixel_reference<B,C,L,M> > : mpl::true_{};

template <typename B, typename C, typename L, bool M>  
struct is_bit_aligned<const bit_aligned_pixel_reference<B,C,L,M> > : mpl::true_{};

template <typename B, typename C, typename L>  
struct is_bit_aligned<packed_pixel<B,C,L> > : mpl::true_{};

template <typename B, typename C, typename L>  
struct is_bit_aligned<const packed_pixel<B,C,L> > : mpl::true_{};


/// is_similar metafunctions
/// \brief Determines if two pixel types are similar.

template< typename A, typename B >
struct is_similar : mpl::false_ {};

template<typename A>
struct is_similar< A, A > : mpl::true_ {};

template<typename B,int I, int S, bool M, int I2>
struct is_similar< packed_channel_reference< B,  I, S, M >
                 , packed_channel_reference< B, I2, S, M >
                 > : mpl::true_ {};

/// is_homogeneous metafunctions
/// \brief Determines if a pixel types are homogeneous.

template<typename C,typename CMP, int Next, int Last> struct is_homogeneous_impl;

template<typename C,typename CMP, int Last>
struct is_homogeneous_impl<C,CMP,Last,Last> : mpl::true_{};

template<typename C,typename CMP, int Next, int Last>
struct is_homogeneous_impl : mpl::and_< is_homogeneous_impl< C, CMP,Next + 1, Last >
                                      , is_same< CMP, typename mpl::at_c<C,Next>::type
                                      > > {};

template < typename P > struct is_homogeneous : mpl::false_ {};

// pixel
template < typename C, typename L > struct is_homogeneous< pixel<C,L> > : mpl::true_ {};
template < typename C, typename L > struct is_homogeneous<const pixel<C,L> > : mpl::true_ {};
template < typename C, typename L > struct is_homogeneous< pixel<C,L>& > : mpl::true_ {};
template < typename C, typename L > struct is_homogeneous<const pixel<C,L>& > : mpl::true_ {};

// planar pixel reference
template <typename Channel, typename ColorSpace>
struct is_homogeneous< planar_pixel_reference< Channel, ColorSpace > > : mpl::true_ {};
template <typename Channel, typename ColorSpace>
struct is_homogeneous< const planar_pixel_reference< Channel, ColorSpace > > : mpl::true_ {};

template<typename C,typename CMP, int I,int Last>
struct is_homogeneous_impl_p;

// for packed_pixel
template <typename B, typename C, typename L >
struct is_homogeneous<packed_pixel< B, C, L > > 
	: is_homogeneous_impl_p< C 
	                               , typename mpl::at_c< C, 0 >::type
	                               , 1
	                               , mpl::size< C >::type::value
	                               > {};

template <typename B, typename C, typename L>  
struct is_homogeneous<const packed_pixel<B,C,L> > 
	: is_homogeneous_impl_p<C,typename mpl::at_c<C,0>::type,1,mpl::size<C>::type::value>
{};

// for bit_aligned_pixel_reference
template <typename B, typename C, typename L, bool M>  
struct is_homogeneous<bit_aligned_pixel_reference<B,C,L,M> > 
	: is_homogeneous_impl<C,typename mpl::at_c<C,0>::type,1,mpl::size<C>::type::value>
{};

template <typename B, typename C, typename L, bool M>  
struct is_homogeneous<const bit_aligned_pixel_reference<B,C,L,M> > 
	: is_homogeneous_impl<C,typename mpl::at_c<C,0>::type,1,mpl::size<C>::type::value>
{};

//////////////////////
/// other goodies

/// get_num_bits metafunctions
/// \brief Determines the numbers of bits for the given channel type.

template <typename T>
struct get_num_bits;

template< typename B, int I, int S, bool M >
struct get_num_bits< packed_channel_reference< B, I, S, M > >
{
    BOOST_STATIC_CONSTANT( int, value = S );
};

template<typename B,int I, int S, bool M>
struct get_num_bits< const packed_channel_reference< B, I, S, M > >
{
    BOOST_STATIC_CONSTANT( int, value = S );
};


/// channel_type metafunction
/// \brief Generates the channel type for 


template <typename B, typename C, typename L, bool M>  
struct gen_chan_ref
{
	typedef packed_dynamic_channel_reference<B,mpl::at_c<C,0>::type::value,M> type;
};


//! This implementation works for bit_algined_pixel_reference 
//! with a homegeneous channel layout. 
//! The result type will be a packed_dynamic_channel_reference, since the 
//! offset info will be missing. 

// bit_aligned_pixel_reference
template <typename B, typename C, typename L, bool M>  
struct channel_type<bit_aligned_pixel_reference<B,C,L,M> > 
	: boost::lazy_enable_if< 
		is_homogeneous<bit_aligned_pixel_reference<B,C,L,M> >,
		gen_chan_ref<B,C,L,M>
		>
{};

template <typename B, typename C, typename L, bool M>  
struct channel_type<const bit_aligned_pixel_reference<B,C,L,M> > 
	: boost::lazy_enable_if< 
		is_homogeneous<bit_aligned_pixel_reference<B,C,L,M> >,
		gen_chan_ref<B,C,L,M>
		>
{};

template <typename B, typename C, typename L>  
struct gen_chan_ref_p
{
	typedef packed_dynamic_channel_reference< B
	                                        , get_num_bits< typename mpl::at_c<C,0>::type>::value
	                                        , true
	                                        > type;
};

// packed_pixel
template <typename B, typename C, typename L>  
struct channel_type<packed_pixel<B,C,L> > : boost::lazy_enable_if< 
		is_homogeneous<packed_pixel<B,C,L> >,
		gen_chan_ref_p<B,C,L>
		>
{};

template <typename B, typename C, typename L>  
struct channel_type< const packed_pixel< B, C, L > > 
	: boost::lazy_enable_if< is_homogeneous<packed_pixel< B, C, L> >
	                       , gen_chan_ref_p<B,C,L>
		                   >
{};

namespace detail {

/// - performance specialization double
/// - to eliminate compiler warning 4244
template <typename GrayChannelValue>
struct rgb_to_luminance_fn< double, double, double, GrayChannelValue >
{
    GrayChannelValue operator()( const double& red
                               , const double& green
                               , const double& blue    ) const
   {
      return channel_convert<GrayChannelValue>( red * 0.30 + green * 0.59 + blue * 0.11 );
   }
};

} // namespace detail
} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_IO_DETAIL_GIL_EXTENSIONS_HPP_INCLUDED
