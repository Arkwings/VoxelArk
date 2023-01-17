#pragma once

namespace Generation {
    template <typename T>
    class ArkNoise {
        public:
        ArkNoise(const unsigned int& sizeX, const unsigned int& sizeZ, const T& frequency)
            : gradients_(static_cast<unsigned int>((frequency + 1.0f)* (frequency + 1.0f)))
            , points_(new T[sizeX_ * sizeZ_ * 3U])
            , sizeX_(sizeX)
            , sizeZ_(sizeZ)
            , frequency_(static_cast<unsigned int>(frequency)) {}
        ~ArkNoise() {}

        void GenerateGradients(const glm::vec<4, unsigned int>& verticesSeeds, const glm::vec<4, unsigned int>& edgesSeeds, const unsigned int& innerSeed) {
            //populate 4 outer gradient points 

            std::mt19937 generator(verticesSeeds[0]);
            std::uniform_real_distribution<float> distribution(-1000.0f, 1000.0f);
            gradients_[0] = glm::vec<2, T>(distribution(generator), distribution(generator));
            generator.seed(verticesSeeds[1]);
            gradients_[frequency_] = glm::vec<2, T>(distribution(generator), distribution(generator));
            generator.seed(verticesSeeds[2]);
            gradients_[gradients_.size() - frequency_ - 1] = glm::vec<2, T>(distribution(generator), distribution(generator));
            generator.seed(verticesSeeds[3]);
            gradients_[gradients_.size() - 1] = glm::vec<2, T>(distribution(generator), distribution(generator));

            generator.seed(edgesSeeds[0]);
            for (unsigned int i = 1U; i < frequency_; ++i)                                             gradients_[i] = glm::vec<2, T>(distribution(generator), distribution(generator));
            generator.seed(edgesSeeds[1]);
            for (unsigned int i = gradients_.size() - frequency_; i < gradients_.size() - 1; ++i)      gradients_[i] = glm::vec<2, T>(distribution(generator), distribution(generator));
            generator.seed(edgesSeeds[2]);
            for (unsigned int i = 1U; i < frequency_; ++i)                                             gradients_[(frequency_ + 1) * i] = glm::vec<2, T>(distribution(generator), distribution(generator));
            generator.seed(edgesSeeds[3]);
            for (unsigned int i = 2U; i < frequency_ + 1; ++i)                                         gradients_[(frequency_ + 1) * i - 1] = glm::vec<2, T>(distribution(generator), distribution(generator));

            //populate inner gradients 
            generator.seed(innerSeed);
            for (unsigned int j = 1U; j < frequency_; ++j) {
                for (unsigned int i = 1U; i < frequency_; ++i) {
                    gradients_[i + j * ((frequency_ + 1))] = glm::vec<2, T>(distribution(generator), distribution(generator));
                    // std::cout << gradients_[i + j * ((frequency_ + 1))] << std::endl;
                }
            }

            // std::srand(verticesSeeds[0]);
            // gradients_[0] = glm::vec<2, T>(std::rand() / static_cast<T>(RAND_MAX), std::rand() / static_cast<T>(RAND_MAX));
            // std::srand(verticesSeeds[1]);
            // gradients_[frequency_] = glm::vec<2, T>(std::rand() / static_cast<T>(RAND_MAX), std::rand() / static_cast<T>(RAND_MAX));
            // std::srand(verticesSeeds[2]);
            // gradients_[gradients_.size() - frequency_ - 1] = glm::vec<2, T>(std::rand() / static_cast<T>(RAND_MAX), std::rand() / static_cast<T>(RAND_MAX));
            // std::srand(verticesSeeds[3]);
            // gradients_[gradients_.size() - 1] = glm::vec<2, T>(std::rand() / static_cast<T>(RAND_MAX), std::rand() / static_cast<T>(RAND_MAX));

            //populate outer gradient edges 
            // std::srand(edgesSeeds[0]);
            // for (unsigned int i = 1U; i < frequency_; ++i)                                             gradients_[i] = glm::vec<2, T>(std::rand() / static_cast<T>(RAND_MAX), std::rand() / static_cast<T>(RAND_MAX));
            // std::srand(edgesSeeds[1]);
            // for (unsigned int i = gradients_.size() - frequency_; i < gradients_.size() - 1; ++i)      gradients_[i] = glm::vec<2, T>(std::rand() / static_cast<T>(RAND_MAX), std::rand() / static_cast<T>(RAND_MAX));
            // std::srand(edgesSeeds[2]);
            // for (unsigned int i = 1U; i < frequency_; ++i)                                             gradients_[(frequency_ + 1) * i] = glm::vec<2, T>(std::rand() / static_cast<T>(RAND_MAX), std::rand() / static_cast<T>(RAND_MAX));
            // std::srand(edgesSeeds[3]);
            // for (unsigned int i = 2U; i < frequency_ + 1; ++i)                                         gradients_[(frequency_ + 1) * i - 1] = glm::vec<2, T>(std::rand() / static_cast<T>(RAND_MAX), std::rand() / static_cast<T>(RAND_MAX));

            // //populate inner gradients 
            // std::srand(innerSeed);
            // for (unsigned int j = 1U; j < frequency_; ++j) {
            //     for (unsigned int i = 1U; i < frequency_; ++i) {
            //         gradients_[i + j * ((frequency_ + 1))] = glm::vec<2, T>(std::rand() / static_cast<T>(RAND_MAX), std::rand() / static_cast<T>(RAND_MAX));
            //     }
            // }

            for (unsigned int j = 0U; j < frequency_ + 1; ++j) {
                for (unsigned int i = 0U; i < frequency_ + 1; ++i) {
                    gradients_[i + j * ((frequency_ + 1))] = glm::normalize(gradients_[i + j * ((frequency_ + 1))] - 0.5f);
                }
            }
        }

        void GeneratePoints(const bool& isTexture, const unsigned int& octaves, const T& persistence, const T& amplitude) {

            // T res;
            // unsigned int index;
            isTexture_ = isTexture;
            octaves_ = octaves;
            persistence_ = persistence;
            amplitude_ = amplitude;

#pragma omp parallel for
            for (int j = 0; j < sizeZ_; ++j) {
                for (unsigned int i = 0U; i < sizeX_; ++i) {
                    unsigned int index = (i + j * sizeX_) * 3;

                    //res = generatePoint(i, j);
                    T res = octave(static_cast<float>(i), static_cast<float>(j), octaves_, persistence_, amplitude_);

                    if (!isTexture_) {
                        points_[index] = static_cast<float>(i);
                        points_[index + 1] = res;
                        points_[index + 2] = static_cast<float>(j);
                    }
                    else
                        points_[index + 1] = res * 0.5 + 0.5;
                }
            }
        }

        T* GetPoints() {
            return points_;
        }

        private:

        T octave(const T& i, const T& j, const unsigned int& octaves, const T& persistence, const T& amplitude) {
            T result = 0;
            T xcopy = i, zcopy = j, amplicopy = amplitude;

            for (unsigned int i = 0; i != octaves; ++i) {
                result += (generatePoint(xcopy, zcopy) * amplicopy);
                xcopy *= 2;
                zcopy *= 2;
                xcopy = xcopy >= sizeX_ ? xcopy - sizeX_ : xcopy;
                zcopy = zcopy >= sizeZ_ ? zcopy - sizeZ_ : zcopy;
                amplicopy *= persistence;
            }

            return result;
        }

        T generatePoint(T& i, T& j) {
            unsigned int index;
            int x0, z0;
            glm::vec<2, T> g00, g10, g01, g11;
            T t, r, s, g0, g1;

            T x = static_cast<T>(i) / sizeX_ * frequency_;
            T z = static_cast<T>(j) / sizeZ_ * frequency_;

            x0 = static_cast<int>(std::floor(x));
            z0 = static_cast<int>(std::floor(z));

            x -= x0;
            z -= z0;

            g00 = gradients_[x0 + z0 * (frequency_ + 1)];
            g10 = gradients_[x0 + 1 + z0 * (frequency_ + 1)];
            g01 = gradients_[x0 + (z0 + 1) * (frequency_ + 1)];
            g11 = gradients_[x0 + 1 + (z0 + 1) * (frequency_ + 1)];

            t = fade(x);

            r = g00.x * x + g00.y * z;
            s = g10.x * (x - 1) + g10.y * z;
            g0 = lerp(r, s, t);

            r = g01.x * x + g01.y * (z - 1);
            s = g11.x * (x - 1) + g11.y * (z - 1);
            g1 = lerp(r, s, t);

            t = fade(z);

            return lerp(g0, g1, t);
        }

        T fade(const T& value) {
            return ((6 * value - 15) * value + 10) * value * value * value;
        }

        T lerp(const T& a, const T& b, const T& t) {
            return a + (b - a) * t;
        }

        std::vector<glm::vec<2, T>> gradients_;
        const unsigned int sizeX_;
        const unsigned int sizeZ_;
        const unsigned int frequency_;
        T* points_;

        bool isTexture_;
        unsigned int octaves_;
        T persistence_;
        T amplitude_;
    };
};