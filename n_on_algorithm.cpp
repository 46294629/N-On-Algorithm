#include <algorithm> 
#include <stdio.h>
#include <map>


/*
对于一个N（N为奇数）进制数字（可以以0开头，所有数字长度都是N-1，如5进制，则2为0002），求出所有美丽数字的数量。
美丽数字即前（N-1）/2的数字之和=后（N-1）个数字之和，如0110为美丽数字，因为0+1=1+0
*/

struct N_On_key
{
    N_On_key()
    {
        sum = 0;
        end = 0;
        length = 0;
    }
    N_On_key(unsigned int sum_, unsigned int end_, unsigned int length_) : sum(sum_), end(end_), length(length_) {}
    bool operator<(const N_On_key &other) const
    {
        if (sum != other.sum)
        {
            return (sum < other.sum);
        }
        if (end != other.end)
        {
            return (end < other.end);
        }
        return (length < other.length);
    }
    bool operator==(const N_On_key &other) const
    {
        return (sum == other.sum && end == other.end && length == other.length); 
    }
    unsigned int sum;
    unsigned int end;
    unsigned int length;
};

class N_On_Algorithm
{
public:
    unsigned long cal(unsigned int n);
private:
    unsigned int find_all_permutation2(N_On_key key, std::map<N_On_key, unsigned int> &record_map1, 
                                                        std::map<N_On_key, unsigned int> &record_map2);
    unsigned int find_all_permutation(N_On_key key, std::map<N_On_key, unsigned int> &record_map1, 
                                                        std::map<N_On_key, unsigned int> &record_map2);  
    unsigned int find_first_value(N_On_key key, std::map<N_On_key, unsigned int> &record_map1, 
                                                std::map<N_On_key, unsigned int> &record_map2);    
    unsigned int combine(unsigned int a, unsigned int b);
};

unsigned int N_On_Algorithm::find_first_value(N_On_key key, std::map<N_On_key, unsigned int> &record_map1, 
                                                std::map<N_On_key, unsigned int> &record_map2)
{
    key.end = std::min(key.sum, key.end) - 1;
    std::map<N_On_key, unsigned int>::iterator it = record_map1.find(key);
    if (it == record_map1.end())
    {
        unsigned int value = find_all_permutation(key, record_map1, record_map2);
        record_map1.insert(std::pair<N_On_key, unsigned int>(key, value));
        return value;
    }
    else
    {
        return it->second;
    }
}

/*
s(n+i*m, m, k) = sum(C(k, j) * f(n+(i-j)*m), m-1, k-j)) (n<m, i>=j>0)

f(n, m, k) = f(n, n - 1, k) + k (n <= m）(f(m, m, k) = f(m, m-1, k) + k)
f(n, m, k) = f(n, m-1, k) + s(n, m, k)

f(n, m, 1) = (n > m ? 0 : k)
f(0, m, k) = 1
f(1, m, k) = k
f(n, 0, k) = 1
*/
unsigned int N_On_Algorithm::combine(unsigned int a, unsigned int b)
{
    if (a <= b || 0 == a)
    {
        return -1;
    }
    unsigned int c = a - b, aa = 1, bb = 1;
    while (a > c)
    {
        aa *= a--;
    }
    while (1 < b)
    {
        bb *= b--;
    }
    return aa / bb;
}

unsigned int N_On_Algorithm::find_all_permutation2(N_On_key key, std::map<N_On_key, unsigned int> &record_map1, 
                                                        std::map<N_On_key, unsigned int> &record_map2)
{
    unsigned int i = key.sum / key.end, n = key.sum % key.end;

    std::map<N_On_key, unsigned int>::iterator it;
    N_On_key second_key(key.sum, key.end - 1, key.length);
    unsigned int second_value = 0, value = 0;
    for (unsigned int j = 1; j <= i; ++j)
    {
        if (0 == --second_key.length || second_key.sum < key.end)
        {
            break;
        }
        second_key.sum -= key.end;
        
        if ((it = record_map1.find(second_key)) == record_map1.end())
        {
            value = find_all_permutation(second_key, record_map1, record_map2);
            record_map1.insert(std::pair<N_On_key, unsigned int>(second_key, value));

        }
        else
        {
            value = it->second;
        }
        second_value += (combine(key.length, j) * value);
    }
    return second_value;
}

unsigned int N_On_Algorithm::find_all_permutation(N_On_key key, std::map<N_On_key, unsigned int> &record_map1, std::map<N_On_key, unsigned int> &record_map2)
{
    if (0 == key.sum)
    {
        return 1;
    }
    if (key.sum > key.end * key.length)
    {
        return 0;
    }
    if (key.sum == key.end * key.length)
    {
        return 1;
    }
    if (1 == key.sum)
    {
        return key.length;
    }
    if (1 == key.length)
    {
        if (key.sum > key.end)
        {
            return 0;
        }
        return key.length;
    }

    unsigned int first_value = find_first_value(key, record_map1, record_map2);
    if (key.sum <= key.end)
    {
        return first_value + key.length;
    }
    std::map<N_On_key, unsigned int>::iterator it = record_map2.find(key);
    if (it != record_map2.end())
    {
        return first_value + it->second;
    }
    unsigned int value = find_all_permutation2(key, record_map1, record_map2);
    record_map2.insert(std::pair<N_On_key, unsigned int>(key, value));
    return first_value + value;
}

unsigned long N_On_Algorithm::cal(unsigned int n)
{
    unsigned long total_num = 0;
    if (0 == n % 2)
    {
        printf("%u is not odd\n", n);
        return total_num;
    }
    unsigned int length = (n - 1) / 2;
    if (0 == length)
    {
        return total_num;
    }
    unsigned int max_sum = (n - 1) * length / 2;
    unsigned long count = 0;
    std::map<N_On_key, unsigned int> record_map1, record_map2;
    N_On_key key(0, n - 1, length);
    for (unsigned int i = 0; i < max_sum; ++i)
    {
        key.sum = i;
        count = find_all_permutation(key, record_map1, record_map2);
        total_num += count * count;
    }
    total_num *= 2;
    key.sum = max_sum;
    count = find_all_permutation(key, record_map1, record_map2);
    return total_num + count * count;
} 

int main(int argc,char **argv)
{
    if (2 != argc)
    {
        printf("number of input should be 2\n");
        return -1;
    }
    unsigned int n = atoi(argv[1]);
    N_On_Algorithm test_algorithm;
    printf("input n is %u, total number is %lu\n", n, test_algorithm.cal(n));
    return 1;
}

