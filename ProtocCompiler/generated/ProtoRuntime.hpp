#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <cstring>
#include <type_traits>

namespace ProtoRuntime {

    // --- INTERNAL HELPERS ---

    // Standard Varint writer
    inline void WriteVarint(std::vector<uint8_t>& buf, uint64_t val) {
        do {
            uint8_t b = static_cast<uint8_t>(val & 0x7F);
            val >>= 7;
            if (val > 0) b |= 0x80;
            buf.push_back(b);
        } while (val > 0);
    }

    // ZigZag encoding for signed integers (keeps small negative numbers small)
    inline uint64_t ZigZagEncode(int64_t n) {
        return (static_cast<uint64_t>(n) << 1) ^ static_cast<uint64_t>(n >> 63);
    }

    // --- MAIN API ---

    // 1. For Integers (Signed 32-bit)
    inline void WriteField(std::vector<uint8_t>& buf, int num, int32_t val) {
        if (val == 0) return;
        buf.push_back(static_cast<uint8_t>((num << 3) | 0)); // Wire 0
        WriteVarint(buf, ZigZagEncode(val));
    }

    // 1b. For Enums (treated as varint)
    template<typename T, typename std::enable_if<std::is_enum<T>::value, int>::type = 0>
    inline void WriteField(std::vector<uint8_t>& buf, int num, const T& val) {
        if (static_cast<int>(val) == 0) return;
        buf.push_back(static_cast<uint8_t>((num << 3) | 0)); // Wire 0 for varint
        WriteVarint(buf, static_cast<uint64_t>(val));
    }

    // 2. For Floats (Fixed 32-bit)
    inline void WriteField(std::vector<uint8_t>& buf, int num, float val) {
        if (val == 0.0f) return;
        buf.push_back(static_cast<uint8_t>((num << 3) | 1)); // Wire 1
        uint32_t bits;
        std::memcpy(&bits, &val, 4); // The safe way to bit-cast in C++
        for (int i = 0; i < 4; ++i) {
            buf.push_back(static_cast<uint8_t>((bits >> (i * 8)) & 0xFF));
        }
    }

    inline void WriteField(std::vector<uint8_t>& buf, int num, double val) {
        if (val == 0.0f) return;
        buf.push_back(static_cast<uint8_t>((num << 3) | 2)); // Wire 2
        uint32_t bits;
        std::memcpy(&bits, &val, 8); // The safe way to bit-cast in C++
        for (int i = 0; i < 8; ++i) {
            buf.push_back(static_cast<uint8_t>((bits >> (i * 8)) & 0xFF));
        }
    }

    // 3. For Strings
    inline void WriteField(std::vector<uint8_t>& buf, int num, const std::string& val) {
        if (val.empty()) return;
        buf.push_back(static_cast<uint8_t>((num << 3) | 2)); // Wire 3
        WriteVarint(buf, val.size());
        buf.insert(buf.end(), val.begin(), val.end());
    }

    // 4. For Vectors of Varints (Packed Logic)
    // Works for std::vector<int32_t>, int64_t, etc.
    template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    void WriteField(std::vector<uint8_t>& buf, int num, const std::vector<T>& vec) {
        if (vec.empty()) return;

        // Tag for the whole packed field (Wire 2)
        buf.push_back(static_cast<uint8_t>((num << 3) | 2));

        // We must write all elements to a temp buffer to calculate total length
        std::vector<uint8_t> temp;
        for (auto item : vec) {
            WriteVarint(temp, ZigZagEncode(item));
        }

        // Write total length then the data
        WriteVarint(buf, temp.size());
        buf.insert(buf.end(), temp.begin(), temp.end());
    }

    // 5. For Vectors of Floats (Packed Logic)
    inline void WriteField(std::vector<uint8_t>& buf, int num, const std::vector<float>& vec) {
        if (vec.empty()) return;

        buf.push_back(static_cast<uint8_t>((num << 3) | 2)); // Wire 2
        WriteVarint(buf, vec.size() * 4); // Total bytes is elements * 4

        for (float f : vec) {
            uint32_t bits;
            std::memcpy(&bits, &f, 4);
            for (int i = 0; i < 4; ++i) {
                buf.push_back(static_cast<uint8_t>((bits >> (i * 8)) & 0xFF));
            }
        }
    }


    inline void WriteField(std::vector<uint8_t>& buf, int num, const std::vector<double>& vec) {
        if (vec.empty()) return;

        buf.push_back(static_cast<uint8_t>((num << 3) | 2)); // Wire 2
        WriteVarint(buf, vec.size() * 8); // Total bytes is elements * 8

        for (float f : vec) {
            uint32_t bits;
            std::memcpy(&bits, &f, 8);
            for (int i = 0; i < 8; ++i) {
                buf.push_back(static_cast<uint8_t>((bits >> (i * 8)) & 0xFF));
            }
        }
    }

    // 6. For Vectors of Strings (Unpacked Logic)
    inline void WriteField(std::vector<uint8_t>& buf, int num, const std::vector<std::string>& vec) {
        for (const auto& s : vec) {
            WriteField(buf, num, s); // Reuse the single string logic
        }
    }

    // 7. For Single User-Defined Objects (not vectors, not arithmetic, not string, not enum)
    template <typename T, typename std::enable_if<!std::is_arithmetic<T>::value && !std::is_enum<T>::value && !std::is_same<T, std::string>::value, int>::type = 0>
    inline void WriteField(std::vector<uint8_t>& buf, int num, const T& obj) {
        // 1. Ask the object to serialize itself
        std::vector<uint8_t> msg_data = obj.Serialize();
        if (msg_data.empty()) return;

        // 2. Write Tag (Wire Type 2)
        buf.push_back(static_cast<uint8_t>((num << 3) | 2));

        // 3. Write Length then Data
        WriteVarint(buf, msg_data.size());
        buf.insert(buf.end(), msg_data.begin(), msg_data.end());
    }

    // 8. For Vector of User-Defined Objects (Unpacked)
    template <typename T, typename std::enable_if<!std::is_arithmetic<T>::value && !std::is_enum<T>::value && !std::is_same<T, std::string>::value, int>::type = 0>
    inline void WriteField(std::vector<uint8_t>& buf, int num, const std::vector<T>& vec) {
        for (const auto& item : vec) {
            // Just call the single object version for every item in the vector
            WriteField(buf, num, item);
        }
    }
}



namespace ProtoRuntime {
    // --- DESERIALIZATION HELPERS ---

    inline uint64_t ReadVarint(const std::vector<uint8_t>& buf, size_t& pos) {
        uint64_t result = 0;
        uint64_t shift = 0;
        while (pos < buf.size()) {
            uint8_t byte = buf[pos++];
            result |= static_cast<uint64_t>(byte & 0x7F) << shift;
            if (!(byte & 0x80)) return result;
            shift += 7;
        }
        return result;
    }

    inline int64_t ZigZagDecode(uint64_t n) {
        return (n >> 1) ^ -(static_cast<int64_t>(n & 1));
    }

    // Used to skip fields we don't recognize
    inline void SkipField(const std::vector<uint8_t>& buf, size_t& pos, int wireType) {
        if (wireType == 0) ReadVarint(buf, pos);
        else if (wireType == 1) pos += 8;
        else if (wireType == 2) {
            uint64_t len = ReadVarint(buf, pos);
            pos += len;
        }
        else if (wireType == 5) pos += 4;
    }

    // --- READ FIELD API ---

    // 1. For Integers
    inline void ReadField(const std::vector<uint8_t>& buf, size_t& pos, int32_t& val) {
        val = static_cast<int32_t>(ZigZagDecode(ReadVarint(buf, pos)));
    }

    // 2. For Strings
    inline void ReadField(const std::vector<uint8_t>& buf, size_t& pos, std::string& val) {
        uint64_t len = ReadVarint(buf, pos);
        val.assign(reinterpret_cast<const char*>(&buf[pos]), len);
        pos += len;
    }

    // 3. For Enums
    template <typename T, typename std::enable_if<std::is_enum<T>::value, int>::type = 0>
    inline void ReadField(const std::vector<uint8_t>& buf, size_t& pos, T& val) {
        val = static_cast<T>(ReadVarint(buf, pos));
    }

    // 4. For Nested Objects (not vector, not string)
    template <typename T, typename std::enable_if<std::is_class<T>::value && !std::is_same<T, std::string>::value && !std::is_same<typename std::decay<T>::type, std::vector<typename T::value_type>>::value, int>::type = 0>
    inline void ReadField(const std::vector<uint8_t>& buf, size_t& pos, T& obj) {
        uint64_t len = ReadVarint(buf, pos);
        std::vector<uint8_t> sub_buffer(buf.begin() + pos, buf.begin() + pos + len);
        obj.Deserialize(sub_buffer); // Recursively call Deserialize
        pos += len;
    }

    // 5. For Vector of Objects (has Deserialize method)
    template <typename T>
    inline void ReadField(const std::vector<uint8_t>& buf, size_t& pos, std::vector<T>& vec) {
        uint64_t len = ReadVarint(buf, pos);
        std::vector<uint8_t> sub_buffer(buf.begin() + pos, buf.begin() + pos + len);
        T obj;
        obj.Deserialize(sub_buffer);
        vec.push_back(obj);
        pos += len;
    }
}