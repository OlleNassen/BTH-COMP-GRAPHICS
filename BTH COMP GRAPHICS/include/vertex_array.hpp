#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

class vertex_array
{
public:
    vertex_array();
    ~vertex_array();

    void bind() const;

    void attribute_pointer(
        const unsigned int index,
        const int size,
        const unsigned int type,
        const unsigned char normalized,
        const int stride,
        const void* pointer) const;

    void attribute_pointer(
        const unsigned int index,
        const int size,
        const unsigned int type,
        const int stride,
        const void* pointer) const;

    void attribute_divisor(
        const unsigned int index,
        const unsigned int divisor) const;


private:
    unsigned int id;
};

#endif // VERTEX_ARRAY_HPP
