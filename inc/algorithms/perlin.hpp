#pragma once

namespace Generation {

    // template <typename T>
    // T Interpolate(const T& value1, const T& value2, const T& multiplier) {
    //     return value1 + (value2 - value1) * Fade<T>(multiplier);
    // }

    // template <typename T>
    // T* PerlinNoise2D(const unsigned int& sizeX, const unsigned int& sizeY, const T& minH, const T& maxH, const T& multiplier, const unsigned int& seed = time(NULL)) {

    //     //checks
    //     if (sizeX < 1) { std::cerr << "PerlinNoise2D: this sizeX(" << sizeX << ") won't work pal" << std::endl; exit(-1); }
    //     if (sizeY < 1) { std::cerr << "PerlinNoise2D: this sizeY(" << sizeY << ") won't work pal" << std::endl; exit(-1); }
    //     if (minH >= maxH) { std::cerr << "PerlinNoise2D: minH(" << minH << ") can't be superior or identical to maxH(" << maxH << ")" << std::endl; exit(-1); }

    //     //set seed
    //     srand(seed);

    //     //random unit vectors
    //     std::vector<glm::vec<2, T>> randomVectors((sizeX + 1) * (sizeY + 1));
    //     T diffH(maxH - minH);
    //     for (auto it(randomVectors.begin()); it != randomVectors.end(); ++it) {
    //         it->x = (static_cast<T>(rand()) * diffH / static_cast<T>(RAND_MAX)) + minH;
    //         it->y = (static_cast<T>(rand()) * diffH / static_cast<T>(RAND_MAX)) + minH;
    //         (*it) = glm::normalize(*it);
    //     }

    //     //dot products
    //     std::vector<glm::vec<4, T>> dotProducts(sizeX * sizeY);
    //     for (unsigned int i = 0; i != dotProducts.size(); ++i) {
    //         dotProducts[i].x = glm::dot(randomVectors[i], randomVectors[i + 1]);
    //         dotProducts[i].y = glm::dot(randomVectors[i + sizeX], randomVectors[i + 1 + sizeX]);
    //         dotProducts[i].z = glm::dot(randomVectors[i], randomVectors[i + sizeX]);
    //         dotProducts[i].w = glm::dot(randomVectors[i + 1], randomVectors[i + 1 + sizeX]);
    //     }

    //     //interpolate
    //     std::vector<T> interpolations(sizeX * sizeY);
    //     T inter1, inter2;
    //     for (unsigned int i = 0; i != interpolations.size(); ++i) {
    //         inter1 = Interpolate<T>(dotProducts[i].x, dotProducts[i].y, multiplier);
    //         inter2 = Interpolate<T>(dotProducts[i].z, dotProducts[i].w, multiplier);
    //         interpolations[i] = Interpolate<T>(inter1, inter2, multiplier);
    //     }

    //     //recreate coords
    //     // T* pointPositions = new T[sizeX * sizeY * 3];
    //     // for (unsigned int i = 0; i != sizeX * sizeY * 3; i += 3) {
    //     //     pointPositions[i] = (i / 3) % sizeX;
    //     //     pointPositions[i + 1] = interpolations[i / 3];
    //     //     pointPositions[i + 2] = (i / 3) / sizeX;
    //     // }

    //     //recreate coords
    //     T* pointPositions = new T[sizeX * sizeY * 3];
    //     for (unsigned int i = 0; i != sizeX * sizeY * 3; i += 3) {
    //         pointPositions[i] = 0.0f;
    //         pointPositions[i + 1] = interpolations[i / 3];
    //         pointPositions[i + 2] = 0.0f;
    //     }

    //     return pointPositions;
    // }

    constexpr unsigned int PERLIN_NOISE = 0U;                       //perlin noise [-1, 1]
    constexpr unsigned int PERLIN_NOISE_REMAP = 1U;                 //perlin noise [ 0, 1]
    constexpr unsigned int PERLIN_OCTAVE_NOISE = 2U;                //octave noise out of [-1, 1]
    constexpr unsigned int PERLIN_OCTAVE_NOISE_CLAMP = 3U;          //octave noise clamped to [-1, 1]
    constexpr unsigned int PERLIN_OCTAVE_NOISE_CLAMP_REMAP = 4U;    //octave noise clamped to [ 0, 1]
    constexpr unsigned int PERLIN_OCTAVE_NOISE_NORMAL = 5U;         //octave noise normalzied to [-1, 1]
    constexpr unsigned int PERLIN_OCTAVE_NOISE_NORMAL_REMAP = 6U;   //octave noise normalzied to [ 0, 1]

    //arbitrary values for less than 3D
    constexpr float DEFAULT_Y = 0.12345f;
    constexpr float DEFAULT_Z = 0.34567f;

    std::array<unsigned int, 256> permutations({ 151U,160U,137U,91U,90U,15U,
                131U,13U,201U,95U,96U,53U,194U,233U,7U,225U,140U,36U,103U,30U,69U,142U,8U,99U,37U,240U,21U,10U,23U,
                190U, 6U,148U,247U,120U,234U,75U,0U,26U,197U,62U,94U,252U,219U,203U,117U,35U,11U,32U,57U,177U,33U,
                88U,237U,149U,56U,87U,174U,20U,125U,136U,171U,168U, 68U,175U,74U,165U,71U,134U,139U,48U,27U,166U,
                77U,146U,158U,231U,83U,111U,229U,122U,60U,211U,133U,230U,220U,105U,92U,41U,55U,46U,245U,40U,244U,
                102U,143U,54U, 65U,25U,63U,161U, 1U,216U,80U,73U,209U,76U,132U,187U,208U, 89U,18U,169U,200U,196U,
                135U,130U,116U,188U,159U,86U,164U,100U,109U,198U,173U,186U, 3U,64U,52U,217U,226U,250U,124U,123U,
                5U,202U,38U,147U,118U,126U,255U,82U,85U,212U,207U,206U,59U,227U,47U,16U,58U,17U,182U,189U,28U,42U,
                223U,183U,170U,213U,119U,248U,152U, 2U,44U,154U,163U, 70U,221U,153U,101U,155U,167U, 43U,172U,9U,
                129U,22U,39U,253U, 19U,98U,108U,110U,79U,113U,224U,232U,178U,185U, 112U,104U,218U,246U,97U,228U,
                251U,34U,242U,193U,238U,210U,144U,12U,191U,179U,162U,241U, 81U,51U,145U,235U,249U,14U,239U,107U,
                49U,192U,214U, 31U,181U,199U,106U,157U,184U, 84U,204U,176U,115U,121U,50U,45U,127U, 4U,150U,254U,
                138U,236U,205U,93U,222U,114U,67U,29U,24U,72U,243U,141U,128U,195U,78U,66U,215U,61U,156U,180U });

    template <typename T>
    T Modulo(const T& x, const T& y) {
        const int quo = static_cast<int>(floor(abs(x / y)));
        return x - y * static_cast<T>(quo);
    }

    template <typename T>
    T Fade(const T& value) {
        return ((6 * value - 15) * value + 10) * value * value * value;
    }

    template <typename T>
    T Grad(const unsigned int& hash, const T& x, const T& y, const T& z) {
        const unsigned int h = hash & 15;
        const T u = h < 8 ? x : y;
        const T v = h < 4 ? y : h == 12 || h == 14 ? x : z;
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }

    template <typename T>
    T Lerp(const T& a, const T& b, const T& t) {
        return (a + (b - a) * t);
    }

    template <typename T>
    T MaxAmplitude(const unsigned int& octaves, const T& persistence) {
        T result(0), amplitude(1);
        for (unsigned int i = 0; i < octaves; ++i) {
            result += amplitude;
            amplitude *= persistence;
        }
        return result;
    }

    template <typename T>
    T PerlinNoise(const glm::vec<3, T>& vec) {
        const T x0 = std::floor(vec.x);
        const T y0 = std::floor(vec.y);
        const T z0 = std::floor(vec.z);

        const int ix = static_cast<int>(x0) & 255;
        const int iy = static_cast<int>(y0) & 255;
        const int iz = static_cast<int>(z0) & 255;

        const T fx = vec.x - x0;
        const T fy = vec.y - y0;
        const T fz = vec.z - z0;

        const T u = Fade<T>(fx);
        const T v = Fade<T>(fy);
        const T w = Fade<T>(fz);

        const unsigned int A = (permutations[ix & 255] + iy) & 255;
        const unsigned int B = (permutations[(ix + 1) & 255] + iy) & 255;

        const unsigned int AA = (permutations[A] + iz) & 255;
        const unsigned int AB = (permutations[(A + 1) & 255] + iz) & 255;

        const unsigned int BA = (permutations[B] + iz) & 255;
        const unsigned int BB = (permutations[(B + 1) & 255] + iz) & 255;

        const T p0 = Grad<T>(permutations[AA], fx, fy, fz);
        const T p1 = Grad<T>(permutations[BA], fx - 1, fy, fz);
        const T p2 = Grad<T>(permutations[AB], fx, fy - 1, fz);
        const T p3 = Grad<T>(permutations[BB], fx - 1, fy - 1, fz);
        const T p4 = Grad<T>(permutations[(AA + 1) & 255], fx, fy, fz - 1);
        const T p5 = Grad<T>(permutations[(BA + 1) & 255], fx - 1, fy, fz - 1);
        const T p6 = Grad<T>(permutations[(AB + 1) & 255], fx, fy - 1, fz - 1);
        const T p7 = Grad<T>(permutations[(BB + 1) & 255], fx - 1, fy - 1, fz - 1);

        const T q0 = Lerp<T>(p0, p1, u);
        const T q1 = Lerp<T>(p2, p3, u);
        const T q2 = Lerp<T>(p4, p5, u);
        const T q3 = Lerp<T>(p6, p7, u);

        const T r0 = Lerp<T>(q0, q1, v);
        const T r1 = Lerp<T>(q2, q3, v);

        return Lerp<T>(r0, r1, w);
    }

    template <typename T>
    T PerlinNoise(const glm::vec<2, T>& vec) {
        return PerlinNoise<T>(glm::vec<3, T>(vec, DEFAULT_Z));
    }

    template <typename T>
    T PerlinNoise(const glm::vec<1, T>& vec) {
        return PerlinNoise<T>(glm::vec<2, T>(vec, DEFAULT_Y));
    }

    template <typename T, unsigned int Dim>
    T OctaveNoise(const glm::vec<Dim, T>& vec, const unsigned int& octaves, const T& persistence);

    template <typename T, unsigned int Dim>
    T OctaveNoise(const glm::vec<Dim, T>& vec, const unsigned int& octaves, const T& persistence) {
        T result = 0;
        T amplitude = 1;
        glm::vec<Dim, T> vecCopy = vec;

        for (unsigned int i = 0; i != octaves; ++i) {
            result += (PerlinNoise<T>(vecCopy) * amplitude);
            for (int j = 0; j < Dim; ++j)   vecCopy[j] *= 2;
            amplitude *= persistence;
        }

        return result;
    }

    template <typename T, unsigned int Dim, unsigned int NoiseID>
    T Noise(const glm::vec<Dim, T>& vec, const unsigned int seed, const unsigned int& octaves, const T& persistence) {
        std::iota(permutations.begin(), permutations.end(), 0U);
        shuffle(permutations.begin(), permutations.end(), std::default_random_engine(seed));

        if constexpr (NoiseID == PERLIN_NOISE)                      return PerlinNoise<T>(vec);
        if constexpr (NoiseID == PERLIN_NOISE_REMAP)                return PerlinNoise<T>(vec) * static_cast<T>(0.5) + static_cast<T>(0.5);
        if constexpr (NoiseID == PERLIN_OCTAVE_NOISE)               return OctaveNoise<T>(vec, octaves, persistence);
        if constexpr (NoiseID == PERLIN_OCTAVE_NOISE_CLAMP)         return std::clamp(OctaveNoise<T, Dim>(vec, octaves, persistence), static_cast<T>(-1.0), static_cast<T>(1.0));
        if constexpr (NoiseID == PERLIN_OCTAVE_NOISE_CLAMP_REMAP)   return (std::clamp(OctaveNoise<T, Dim>(vec, octaves, persistence), static_cast<T>(-1.0), static_cast<T>(1.0))) * static_cast<T>(0.5) + static_cast<T>(0.5);
        if constexpr (NoiseID == PERLIN_OCTAVE_NOISE_NORMAL)        return OctaveNoise<T, Dim>(vec, octaves, persistence) / MaxAmplitude<T>(octaves, persistence);
        if constexpr (NoiseID == PERLIN_OCTAVE_NOISE_NORMAL_REMAP)  return (OctaveNoise<T, Dim>(vec, octaves, persistence) / MaxAmplitude<T>(octaves, persistence)) * static_cast<T>(0.5) + static_cast<T>(0.5);
        std::cerr << "Noise: " << NoiseID << " NoiseID not implemented" << std::endl;
        exit(-1);
    }

    template <typename T, unsigned int Dim, unsigned int NoiseID>
    T* NoiseMap(const glm::vec<Dim, T>& maxSize, const unsigned int seed, const unsigned int& octaves, const T& persistence, const T& frequency) {
        unsigned int size = 1;
        for (unsigned int i = 0; i < Dim; ++i)  size *= static_cast<int>(maxSize[i]);
        T* generatedPoints = new T[size * (Dim + 1)];
        glm::vec<Dim, T> position;

        if constexpr (Dim == 1) {
            for (unsigned int i = 0; i < size; ++i) {
                position[0] = i / static_cast<T>(maxSize[0]) * frequency;

                generatedPoints[i * 2] = 0;
                // generatedPoints[i * 2] = i;
                generatedPoints[i * 2 + 1] = Noise<T, Dim, NoiseID>(position, seed, octaves, persistence);
            }
        }
        if constexpr (Dim == 2) {
            for (unsigned int i = 0; i < size; ++i) {
                position[0] = (Modulo<T>(static_cast<float>(i), maxSize[0])) / static_cast<T>(maxSize[0]) * frequency;
                position[1] = (static_cast<T>(i / static_cast<int>(maxSize[0]))) / static_cast<T>(maxSize[1]) * frequency;

                generatedPoints[i * 3] = 0;
                // generatedPoints[i * 3] = position[0];
                generatedPoints[i * 3 + 1] = Noise<T, Dim, NoiseID>(position, seed, octaves, persistence);
                generatedPoints[i * 3 + 2] = 0;
                // generatedPoints[i * 3 + 2] = position[1];
            }
        }
        if constexpr (Dim == 3) {
            std::cerr << "NoiseMap: " << Dim << " dimensions not implemented yet" << std::endl;
            exit(-1);
        }

        return generatedPoints;
    }
};