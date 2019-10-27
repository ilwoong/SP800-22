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
    std::random_device rd;
    std::mt19937_64 engine(rd());
    std::uniform_int_distribution<uint8_t> dist(0, 255);
    auto generator = std::bind(dist, engine);

    for (auto i = 0; i < length; ++i) {
        buffer[i] = generator();
    }
}

static void generate_random(std::ofstream& ofs, size_t length)
{
    uint8_t buffer[1024] = {0};

    size_t n = length / 1024;
    size_t q = length % 1024;
    
    for (auto i = 0; i < n ; ++i) {
        fill_random_buffer(buffer, 1024);
        ofs.write((char*)buffer, 1024);
    }

    if (q > 0) {
        fill_random_buffer(buffer, q);
        ofs.write((char*)buffer, q);
    }
}

static void write_random_file(const char* filepath, size_t length)
{
    std::ofstream ofs(filepath, std::ios::binary);

    if (ofs.is_open()) {
        generate_random(ofs, length);
    }

    ofs.close();
}

int main(int argc, const char** argv)
{
    if (argc != 3) {
        print_usage();
        return -1;
    }

    auto filepath = argv[1];
    auto length = atoi(argv[2]);

    write_random_file(filepath, length);

    return 0;
}