#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

class vertex_array
{
public:
    vertex_array();
    ~vertex_array();

    void bind() const;

    void attribute_pointer(
        unsigned int index,
        int size,
        unsigned int type,
        unsigned char normalized,
        int stride,
        const void* pointer) const;

    void attribute_pointer(
        unsigned int index,
        int size,
        unsigned int type,
        int stride,
        const void* pointer) const;


private:
    unsigned int id;
};

#endif // VERTEX_ARRAY_HPP
