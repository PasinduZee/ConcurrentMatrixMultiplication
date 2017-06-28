#include <array>
#include <utility>
#include <iostream>

namespace detail {

template<typename IntType, IntType(*Step)(IntType), IntType Start, IntType ...Is> 
constexpr auto make_integer_array(std::integer_sequence<IntType,Is...>)
{
    return std::array<IntType,sizeof...(Is)>{{Step(Start + Is)...}};
}

template<typename IntType, IntType(*Step)(IntType), IntType Start, std::size_t Length> 
constexpr auto make_integer_array()
{
    return make_integer_array<IntType,Step,Start>(
        std::make_integer_sequence<IntType,Length>());
}


template<
    typename IntType, std::size_t Cols, 
    IntType(*Step)(IntType),IntType Start, std::size_t ...Rs
> 
constexpr auto make_integer_matrix(std::index_sequence<Rs...>)
{
    return std::array<std::array<IntType,Cols>,sizeof...(Rs)> 
        {{make_integer_array<IntType,Step,Start + (Rs * Cols),Cols>()...}};
}

} 
// namespace detail

/*
    Return a compiletime initialized matrix (`std::array` of std::array`)
    of `Cols` columns by `Rows` rows. Ascending elements from [0,0] 
    in row-first order are populated with successive values of the
    constexpr function `IntType Step(IntType i)` for `i` in
    `[Start + 0,Start + (Rows * Cols))` 
*/
template<
    typename IntType, std::size_t Cols, std::size_t Rows, 
    IntType(*Step)(IntType), IntType Start
> 
constexpr auto make_integer_matrix()
{
    return detail::make_integer_matrix<IntType,Cols,Step,Start>(
        std::make_index_sequence<Rows>());
}

/*
    Return a compiletime initialized matrix (`std::array` of std::array`)
    of `Cols` columns by `sizeof...(Starts)` rows. Successive rows are populated
    with successive values of the constexpr function `IntType Step(IntType i)` 
    for `i` in `[start + 0,start + Cols)`, for `start` successively in `...Starts`.  
*/
template<typename IntType, std::size_t Cols, IntType(*Step)(IntType), IntType ...Starts> 
constexpr auto make_integer_matrix()
{
    return std::array<std::array<IntType,Cols>,sizeof...(Starts)> 
        {{detail::make_integer_array<IntType,Step,Starts,Cols>()...}};
}

template<typename IntType>
constexpr auto times_3(IntType i)
{
    return i * 3;
}

static constexpr auto m4x6 = make_integer_matrix<int,4,6,&times_3<int>,4>();
static constexpr auto m5x1 = make_integer_matrix<int,5,&times_3<int>,7>();
static constexpr auto m6x5 = make_integer_matrix<int,6,&times_3<int>,11,13,17,19,23>();
static_assert(m4x6[0][0] == 12,"");

using namespace std;
int main()
{
    cout << "A 4 x 6 matrix that wraps around in steps of `3i` from `i` = 4" << endl; 
    for (auto const & ar  : m4x6) {
        for (auto const & i : ar) {
            cout << i << ' ';
        }
        cout << endl;
    }
    cout << endl;
    cout << "A 6 x 5 matrix with rows of `3i` for initial `i` in <11,13,17,19,23>" 
        << endl;
    for (auto const & ar  : m6x5) {
        for (auto const & i : ar) {
            cout << i << ' ';
        }
        cout << endl;
    }
    cout << endl;
    cout << "A 5 x 1 matrix with rows of of ` 3i` for initial `i` in <7>" 
        << endl;
    for (auto const & ar  : m5x1) {
        for (auto const & i : ar) {
            cout << i << ' ';
        }
        cout << endl;
    }

    
    return 0;
}