#include <iostream>
#include<future>
#include<iterator>
#include<list>
#include<vector>
#include <functional>
#include <utility>
#include<string>
#include<assert.h>


// реализация функции mapreduce
template<typename Iter, typename func1, typename func2>
auto map_reduce(Iter p, Iter q, func1 f1, func2 f2, size_t threads) -> decltype(f2(f1(*p), f1(*p)))
{
    using ResType = decltype(f2(f1(*p), f1(*p)));

    size_t size_sequence = static_cast<size_t>(std::abs(std::distance(p, q)));
    size_t size_partition = size_sequence / threads;
    size_t size_delta = size_sequence % threads;

    auto lambda_func = [&f1, &f2](Iter p, Iter q)->ResType {
        auto result = f1(*p);
        while (++p != q) { result = f2(result, f1(*p)); }
        return result;
    };

    std::vector<std::future<ResType>> result(threads);

    for (size_t i = 0; i < threads; ++i) {
        Iter it_temp = std::next(p, size_partition + ((static_cast<int>(size_delta--) > 0)));
        result[i] = std::async(std::launch::async, lambda_func, p, it_temp);
        p = it_temp;
    }
    auto res = result[0].get();
    ResType answer =res;
    for (size_t i = 1; i < threads; ++i) {
        res = result[i].get();
        answer = f2(answer, res);
    }
    return answer;

}

int main()
{
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

        assert(ssum == "1234567891011");

    }

 
    return 0;
}