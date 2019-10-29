/**
 * The MIT License
 *
 * Copyright (c) 2019 Ilwoong Jeong
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <cstdint>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <random>

static void print_usage()
{
    std::cout << "Usage: random_sample_generator <filename> <length in bytes>" << std::endl;
}

static void fill_random_buffer(uint8_t* buffer, size_t length) 
{
    std::random_device rand_dev;
    std::mt19937_64 engine(rand_dev());
    std::uniform_int_distribution<uint8_t> uni_dist(0, 255);
    auto generator = std::bind(uni_dist, engine);

    for (auto i = 0; i < length; ++i) {
        buffer[i] = generator();
    }
}

static void generate_random(std::ofstream& ofs, size_t length)
{
    const size_t buf_size = 1024;
    uint8_t buffer[buf_size] = {0};

    size_t quotient = length / buf_size;
    size_t remainder = length % buf_size;
    
    for (auto i = 0; i < quotient ; ++i) {
        fill_random_buffer(buffer, buf_size);
        ofs.write((char*)buffer, buf_size);
    }

    if (remainder > 0) {
        fill_random_buffer(buffer, remainder);
        ofs.write((char*)buffer, remainder);
    }
}

static bool is_file_exists(const char* filepath)
{
    return static_cast<bool>(std::ifstream(filepath));
}

static bool confirm_overwrite(const char* filepath)
{
    if(is_file_exists(filepath)) {
        char answer;
        while (true) {
            std::cout << filepath << " is exists, overwrite it? (Y/n)" << std::endl;
            std::cin >> answer;

            if (answer == 'Y' || answer == 'y') {
                return true;
            } else if (answer == 'N' || answer == 'n') {
                return false;
            }
        }
    }

    return true;
}

static bool write_random_file(const char* filepath, size_t length)
{
    if (confirm_overwrite(filepath) == false) {
        return false;
    }
    
    std::ofstream ofs(filepath, std::ios::binary);

    if (ofs.is_open()) {
        generate_random(ofs, length);
    }

    ofs.close();

    return true;
}

int main(int argc, const char** argv)
{
    if (argc != 3) {
        print_usage();
        return -1;
    }

    auto filepath = argv[1];
    auto length = atoi(argv[2]);

    if (write_random_file(filepath, length)) {
        std::cout << length << " bytes of random data were written to " << filepath << std::endl;

    } else {
        std::cout << "CANCELED" << std::endl;
    }

    return 0;
}