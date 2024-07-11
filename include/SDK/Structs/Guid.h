#pragma once

#include <cstdint>

namespace Palworld {

/**
 * Implements a globally unique identifier.
 * Copied from UE4SS with extra additions to work with unordered_map and unordered_set
 */
struct FGuid
{
    public:
        /** Default constructor. */
        FGuid() : A(0), B(0), C(0), D(0) {}

        /**
         * Creates and initializes a new GUID from the specified components.
         */
        FGuid(uint32_t InA, uint32_t InB, uint32_t InC, uint32_t InD) : A(InA), B(InB), C(InC), D(InD) {}

    public:
        /**
         * Compares two GUIDs for equality.
         * @return true if the GUIDs are equal, false otherwise.
         */
        friend bool operator==(const FGuid& X, const FGuid& Y)
        {
            return ((X.A ^ Y.A) | (X.B ^ Y.B) | (X.C ^ Y.C) | (X.D ^ Y.D)) == 0;
        }

        /**
         * Compares two GUIDs for inequality.
         * @return true if the GUIDs are not equal, false otherwise.
         */
        friend bool operator!=(const FGuid& X, const FGuid& Y)
        {
            return ((X.A ^ Y.A) | (X.B ^ Y.B) | (X.C ^ Y.C) | (X.D ^ Y.D)) != 0;
        }

        /**
         * Compares two GUIDs.
         * @return true if the first GUID is less than the second one.
         */
        friend bool operator<(const FGuid& X, const FGuid& Y)
        {
            return    ((X.A < Y.A) ? true : ((X.A > Y.A) ? false :
                    ((X.B < Y.B) ? true : ((X.B > Y.B) ? false :
                    ((X.C < Y.C) ? true : ((X.C > Y.C) ? false :
                    ((X.D < Y.D) ? true : ((X.D > Y.D) ? false : false))))))));
        }

        /**
         * Compares two GUIDs.
         * @return true if the first GUID is less than the second one.
         */
        bool operator<(const FGuid& Other) const
        {
            return    ((this->A < Other.A) ? true : ((this->A > Other.A) ? false :
                    ((this->B < Other.B) ? true : ((this->B > Other.B) ? false :
                    ((this->C < Other.C) ? true : ((this->C > Other.C) ? false :
                    ((this->D < Other.D) ? true : ((this->D > Other.D) ? false : false))))))));
        }

        /**
         * Provides access to the GUIDs components.
         * @param Index The index of the component to return (0...3).
         * @return The component.
         */
        uint32_t& operator[](int32_t index)
        {
            switch (index)
            {
                case 0: return A;
                case 1: return B;
                case 2: return C;
                case 3: return D;
            }
            return A;
        }

        /**
         * Provides read-only access to the GUIDs components.
         *
         * @param Index The index of the component to return (0...3).
         * @return The component.
         */
        const uint32_t& operator[](int32_t index) const
        {
            switch (index)
            {
                case 0: return A;
                case 1: return B;
                case 2: return C;
                case 3: return D;
            }
            return A;
        }

    public:
        /**
         * Invalidates the GUID.
         */
        void invalidate()
        {
            A = B = C = D = 0;
        }

        /**
         * Checks whether this GUID is valid or not.
         * A GUID that has all its components set to zero is considered invalid.
         */
        bool is_valid() const
        {
            return ((A | B | C | D) != 0);
        }

        uint32_t GetA()
        {
            return A;
        }

        uint32_t GetB()
        {
            return B;
        }

        uint32_t GetC()
        {
            return C;
        }

        uint32_t GetD()
        {
            return D;
        }

    public:
        /** Holds the first component. */
        uint32_t A;
        /** Holds the second component. */
        uint32_t B;
        /** Holds the third component. */
        uint32_t C;
        /** Holds the fourth component. */
        uint32_t D;
    };
}

namespace std {
    template <>
    struct hash<Palworld::FGuid> {
        std::size_t operator()(const Palworld::FGuid& Guid) const noexcept {
            std::size_t h1 = std::hash<uint32_t>()(Guid.A);
            std::size_t h2 = std::hash<uint32_t>()(Guid.B);
            std::size_t h3 = std::hash<uint32_t>()(Guid.C);
            std::size_t h4 = std::hash<uint32_t>()(Guid.D);
            return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
        }
    };
}