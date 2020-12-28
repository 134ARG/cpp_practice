#ifndef AMD_ENCODER_H
#define AMD_ENCODER_H

#include <array>
#include <cmath>
#include <random>
#include <iostream>


class AMDCode
{
public:
    AMDCode();
    AMDCode(std::size_t word_size, std::size_t seg_len)
        : word_size(word_size), seg_len(seg_len * word_size / 16) {}

    /*
     * error probability: (8+1)/2^16=0.0001373291015625
     * overall code lengh: $seg_len * (word_size / 8) bytes original data
     *                      + (word_size / 8) bytes pivot
     *                      + (word_size / 8) bytes check segment
     * In this case, the code length  for one segment is 8 * 2 + 2 + 2 = 20 bytes, R = 0.4
     *
     * word_size: Word size for AMD code in bit. This number of bits
     *            will be treated as one unit.
     * seg_len:   Segment length. Input data will be broke into segments
     *            with the length *seg_len*.
     */
    const std::size_t word_size = 16;
    const std::size_t seg_len = 8;

    // encoding result
    unsigned char *result = nullptr;
    std::size_t result_len = 0;

    // return the error probability of the AMD code characterized by word_size and seg_len
    double error_probability();

    const unsigned char *encode(const unsigned char raw_data[], std::size_t size);
    const unsigned char *decode(const unsigned char raw_data[], std::size_t size);

    // delete previous result
    void release_result_mem();
};

inline double AMDCode::error_probability()
{
    return (double)(seg_len + 1) / std::pow(2, word_size);
}

inline const unsigned char *AMDCode::encode(const unsigned char raw_data[], std::size_t size)
{
    // generate x(pivot) used in encoding process
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<std::size_t> dis(0, (int)std::pow(2, 16));

    std::size_t check_len = word_size / 16;
    unsigned char x[check_len];
    unsigned char check[check_len];

    // create x(pivot)
    for (std::size_t i = 0; i < word_size/16; i++) {
        x[i] = dis(gen);
    }

    // calculate check segment & generate the encoding result
    std::size_t char_seg_len = seg_len * (word_size / 16);

    release_result_mem();
    result_len = (size / char_seg_len) * (char_seg_len + word_size / 8);
    result = new unsigned char[result_len];

    for (std::size_t i = 0; i < check_len; i++) {
        check[i] = 0;
    }

    std::size_t result_index = 0;
    for (std::size_t i = 0; i < size+1; i++) {
        if (i % seg_len == 0 && i != 0) {
            for (std::size_t j = 0; j < check_len; j++) {
                result[result_index++] = x[j];
            }
            for (std::size_t j = 0; j < check_len; j++) {
                result[result_index++] = check[j];
            }

        }
        if (i != size) {
            check[i%check_len] += x[i%check_len] * raw_data[i];
            result[result_index++] = raw_data[i];
        }

    }

    return result;

}

inline const unsigned char *AMDCode::decode(const unsigned char raw_data[], std::size_t size)
{
    bool is_valid = true;
    std::size_t check_len = word_size / 16;
    std::size_t code_seg_len = seg_len + 2 * check_len;

    const unsigned char *x;
    const unsigned char *check;
    unsigned char new_check[check_len];

    release_result_mem();
    result_len = size / code_seg_len * seg_len;
    result = new unsigned char[result_len];

    for (std::size_t i = 0; i < check_len; i++) {
        new_check[i] = 0;
    }

    std::size_t result_index = 0;
    for (std::size_t i = 0; i < size; i+=code_seg_len) {
        x = raw_data + i + seg_len;
        check = raw_data + i + seg_len + check_len;
        for (std::size_t j = 0; j < seg_len; j++) {
            new_check[j%check_len] += x[j%check_len] * raw_data[j+i];
            result[result_index++] = raw_data[i+j];
        }
        for (std::size_t j = 0; j < check_len; j++) {
            if (new_check[j] != check[j]) {
                is_valid = false;
                break;
            }
        }
    }

    if (is_valid)
        return result;
    else
        return nullptr;

}

inline void AMDCode::release_result_mem()
{
    if (result) {
        delete [] result;
        result = nullptr;
    }
    result_len = 0;
}





#endif // AMD_ENCODER_H
