#include "CRT.hxx"

mpz_class mpz_crt_worker (std::vector<mpz_class>, std::vector<mpz_class>, int, mpz_class);
mpz_class mpz_product(std::vector<mpz_class>, int, int);

// uses the chinese remainder theorem to calculate a y that satisfies:
// y = r_i (mod m_i) for all 1 <= i <= length(rs)
mpz_class mpz_crt (const std::vector<mpz_class> rs, const std::vector<mpz_class> ms)
{
    int rlen = (int) rs.size();
    int mlen = (int) ms.size();
    int len = rlen <= mlen ? rlen : mlen;

    return mpz_crt_worker(rs, ms, len - 1, mpz_product(ms, 0, len));
}

// recursive, TODO: make iterative
mpz_class mpz_crt_worker (const std::vector<mpz_class> rs,
                          const std::vector<mpz_class> ms,
                          int index,
                          mpz_class product)
{
    if (index < 0)
    {
        return 0;
    }

    mpz_class a = rs[index];
    mpz_class b = ms[index];

    if (index == 0)
    {
        return mpz_class_fdiv_r(a, b);
    }

    product = product / b;
    mpz_class previous_result = mpz_crt_worker(rs, ms, index-1, product);

    mpz_class d = 0, s = 0, t = 0;
    mpz_eea(&d, &s, &t, product, b);

    mpz_class v = s * product;
    mpz_class u = t * b;
    mpz_class x = previous_result * u + a * v;

    return mpz_class_fdiv_r(x, b * product);
}

// use the chinese remainder theorem to calculate y = a^x mod (m1 * ... * mn)
mpz_class mpz_crt_exp (mpz_class a,
                       mpz_class x,
                       const std::vector<mpz_class> ms)
{
    std::vector<mpz_class> rs;
    rs.reserve(ms.size());

    std::vector<mpz_class>::const_iterator i;

    // calculate residue for each m_i
    for(i = ms.begin(); i != ms.end(); ++i)
    {
        mpz_class result = 0;
        mpz_powm(result.get_mpz_t(), a.get_mpz_t(), x.get_mpz_t(), (*i).get_mpz_t());
        rs.push_back(result);
    }

    // use chinese remainder theorem to calculate y = a^x mod (m1 * ... * mn)
    return mpz_crt(rs, ms);
}

// end is exclusive
mpz_class mpz_product(const std::vector<mpz_class> v, int start, int end)
{
    mpz_class result = 1;
    for(int i = start; i < end; i++)
    {
        result *= v[i];
    }
    return result;
}
