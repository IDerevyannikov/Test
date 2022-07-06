#include <utility>
#include <functional>
#include <deque>
#include <numeric>
#include<list>
#include<vector>
#include<string>
#include<thread>
#include<iostream>

template<typename Iterator, typename FuncUn, typename FuncBin>
auto map_reduce(Iterator it_begin, Iterator it_end, FuncUn func_unar, FuncBin func_binar, size_t num_threads)
-> decltype(std::declval<FuncBin>()(std::declval<FuncUn>()(*std::declval<Iterator>()),
    std::declval<FuncUn>()(*std::declval<Iterator>()))) {
    using ResultType = decltype(std::declval<FuncBin>()(std::declval<FuncUn>()(*std::declval<Iterator>()),
        std::declval<FuncUn>()(*std::declval<Iterator>())));

    auto lambda_func = [&func_unar, &func_binar](Iterator it_begin, Iterator it_end, ResultType& result) {
        result = func_unar(*it_begin);
        while (++it_begin != it_end) { result = func_binar(result, func_unar(*it_begin)); }
    };

    size_t size_sequence = static_cast<size_t>(std::abs(std::distance(it_begin, it_end)));
    //if (size_sequence == 0)                    { return ResultType();                    }
    //if (num_threads > size_sequence) { num_threads = size_sequence; }
    //if (num_threads == 0)                       { num_threads = 1;                         }
    size_t size_partition = size_sequence / num_threads;
    size_t size_delta = size_sequence % num_threads;

    std::deque<std::thread>  vect_thread;
    std::deque<ResultType> vect_result(num_threads);

    for(size_t i=0;i<num_threads;++i){
        Iterator it_temp = std::next(it_begin, size_partition + ((static_cast<int>(size_delta--) > 0)));
        vect_thread.emplace_back(lambda_func, it_begin, it_temp, std::ref(vect_result[i]));
        it_begin = std::move(it_temp);
    }

    for (std::thread& thread : vect_thread) { if (thread.joinable()) { thread.join(); } }

    return std::accumulate(std::next(vect_result.begin()), vect_result.end(), vect_result.front(), func_binar);
}


int main()
{
    std::list< int > const l1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    // параллельное суммирование в 3 потока

    auto sum = map_reduce(

        l1.begin()

        , l1.end()

        , [](int i) { return i; }

        , std::plus< int >()

        , 3

    );



    std::cout << sum << std::endl;

    //assert( sum == 55 );



    // проверка наличия чётных чисел в четыре потока

    auto has_even = map_reduce(

        l1.begin()

        , l1.end()

        , [](int i) { return i % 2 == 0; }

        , std::logical_or< bool >()

        , 4

    );



    std::cout << has_even << std::endl;

    //assert( has_even == true );



    std::vector< std::string > const v1 =

    { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11" };

    for (std::size_t j = 1; j <= v1.size(); ++j)

    {

        auto ssum = map_reduce(

            v1.begin()

            , v1.end()

            , [](std::string s) { return s; }

            , std::plus< std::string >()

            , j

        );



        std::cout << ssum << std::endl;

        //assert( ssum == "1234567891011" );

    }



    std::list< int > const l2 =

    { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };



    for (int i = 1; i < 20; ++i)

    {

        auto ssum = map_reduce(

            l2.begin()

            , l2.end()

            , [](int j) { return j; }

            , std::plus< int >()

            , i

        );



        std::cout << ssum << std::endl;

        //assert( ssum == 190 );

    }



    std::vector< std::string > const v2 =

    { "multithread", "and", "async", "in", "C++", "is", "total", "sh!t" };



    // If "i <= v2.size()": on Stepik OK, there NOT OK

    for (std::size_t i = 1; i <= 8; ++i)

    {

        auto size_sum = map_reduce(

            v2.begin()

            , v2.end()

            , [](std::string s) { return s.size(); }

            , std::plus< std::size_t >()

            , i

        );



        std::cout << size_sum << std::endl;

        //assert( size_sum == 35 );

    }


    return 0;
}