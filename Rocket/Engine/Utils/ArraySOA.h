#pragma once

namespace Rocket {
    struct Item {
        int m_int;
        float m_float;
    };

    template <typename T, size_t N>
    struct GetPointerToMemberType {
        typedef typename GetType<T, N>::Type T::*Type;
    };
    
    template <typename T, size_t N>
    static typename GetPointerToMemberType<T, N>::Type GetPointerToMember(void);
    
    template <>
    static typename GetPointerToMemberType<Item, 0>::Type GetPointerToMember<Item, 0>(void) {
        return &Item::m_int;
    }
    
    template <>
    static typename GetPointerToMemberType<Item, 1>::Type GetPointerToMember<Item, 1>(void) {
        return &Item::m_float;
    }

    template <size_t N>
    class ArraySOA {
    public:
        void Add(const Item& item);
        void RemoveAt(size_t index);
    
    private:
        int* m_data0;
        float* m_data1;
        
        size_t m_size;
    };
    
    template <size_t N>
    void ArraySOA<N>::Add(const Item& item) {
        for (size_t i=0; i < N; ++i) {
            typedef typename GetType<Item, i>::Type DataType;
            DataType* data = static_cast<DataType*>(m_array[i]);
            data[m_size] = item.*(GetPointerToMember<Item, i>());
        }
        
        ++m_size;
    }
    
    template <size_t N>
    void ArraySOA<N>::RemoveAt(size_t index) {
        const bool isLast = (index+1u == m_size);
        if (!isLast) {
            m_data0[index] = m_data0[m_size-1u];
            m_data1[index] = m_data1[m_size-1u];
        }
        
        --m_size;
    }

    template <int N>
    struct IntToType {};
    
    typedef IntToType<true> ForEachDoNotTerminateLoop;
    typedef IntToType<false> ForEachTerminateLoop;
    
    template <size_t INDEX, size_t COUNT>
    void ForEach(ForEachDoNotTerminateLoop) {
        //MyFunction();
        ForEach<INDEX+1, COUNT>(IntToType<(INDEX+1 < COUNT)>());
    }
    
    template <size_t INDEX, size_t COUNT>
    void ForEach(ForEachTerminateLoop) {
    }

    template <size_t COUNT>
    void ForEachFunction(void) {
        ForEach<0, COUNT>(IntToType<(0 < COUNT)>());
    }
}