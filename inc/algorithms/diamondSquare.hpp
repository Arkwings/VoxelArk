#pragma once

namespace Generation {
	//functional ((j * size + i) * chans + channel)

	template<typename T>
	T* DiamondSquare(int& size, const std::vector<T> min_heights, const std::vector<T> max_heights, const std::vector<bool>& range_attenuation, const double range_factor, const unsigned int seed, unsigned int channels = 1, const bool seamless_x = false, const bool seamless_y = false) {
		if (size < 1) {
			std::cerr << "DiamondSquare: size must be higher than 1, size = " << size << std::endl;
			exit(-1);
		}
		if (size > 4097) {
			std::cerr << "DiamondSquare: size must be lower than 4097, size = " << size << std::endl;
			exit(-1);
		}

		if (channels <= 0) {
			std::cerr << "DiamondSquare: channels must be higher than 0, channels = " << channels << std::endl;
			exit(-1);
		}

		if (min_heights.size() < channels || max_heights.size() < channels) {
			std::cerr << "DiamondSquare: min_heights && max_heights size must be equal or higher than channels = " << channels << std::endl;
			exit(-1);
		}

		for (int channel = 0; channel < channels; ++channel) {
			if (min_heights[channel] >= max_heights[channel]) {
				std::cerr << "DiamondSquare: min_heights[channel] must be lower than max_height, min_heights[channel] = " << min_heights[channel] << " and max_heights[channel] = " << max_heights[channel] << std::endl;
				exit(-1);
			}
		}

		for (int n = 2; ; n <<= 1) {
			if (n >= size - 1) {
				size = n + 1;
				break;
			}
		}
		std::cout << "DiamondSquare: size must be of the form 2^n+1, new size = " << size << std::endl;

		T* generated_points = new T[size * size * channels];
		std::vector<bool> visited_points(size * size);

		srand(seed);

		for (int channel = 0; channel < channels; ++channel) {
			generated_points[channel] = ((double)rand() / RAND_MAX) * (max_heights[channel] - min_heights[channel]) + min_heights[channel];
			generated_points[(size - 1) * channels + channel] = ((double)rand() / RAND_MAX) * (max_heights[channel] - min_heights[channel]) + min_heights[channel];
			generated_points[((size - 1) * size) * channels + channel] = ((double)rand() / RAND_MAX) * (max_heights[channel] - min_heights[channel]) + min_heights[channel];
			generated_points[(size * size - 1) * channels + channel] = ((double)rand() / RAND_MAX) * (max_heights[channel] - min_heights[channel]) + min_heights[channel];

			if (seamless_x) {
				generated_points[(size - 1) * channels + channel] = (((double)rand() / RAND_MAX) * 0.1 - 0.05) * generated_points[channel];
				generated_points[(size * size - 1) * channels + channel] = (((double)rand() / RAND_MAX) * 0.1 - 0.05) * generated_points[((size - 1) * size) * channels + channel];
			}
			if (seamless_y) {
				generated_points[((size - 1) * size) * channels + channel] = (((double)rand() / RAND_MAX) * 0.1 - 0.05) * generated_points[channel];
				generated_points[(size * size - 1) * channels + channel] = (((double)rand() / RAND_MAX) * 0.1 - 0.05) * generated_points[(size - 1) * channels + channel];
			}
		}
		visited_points[0] = true;
		visited_points[size - 1] = true;
		visited_points[(size - 1) * size] = true;
		visited_points[size * size - 1] = true;

		double temp = 0; //useful for calculations for small types
		double value = 0;
		double low_cap = 0;
		double high_cap = 0;
		double final_cap = 0;
		std::vector<double> found_min(channels);
		std::vector<double> found_max(channels);
		for (int channel = 0; channel < channels; ++channel) {
			found_min[channel] = max_heights[channel];
			found_max[channel] = min_heights[channel];
		}

		int nb_sum = 0;

		for (int pas = (size - 1) / 2; pas != 0; pas /= 2) {
			//DIAMOND
			for (int j = pas; j < size; j += pas * 2) {
				for (int i = pas; i < size; i += pas * 2) {
					if (!visited_points[j * size + i]) {
						for (int channel = 0; channel < channels; ++channel) {
							temp
								= generated_points[((j - pas) * size + i - pas) * channels + channel]
								+ generated_points[((j - pas) * size + i + pas) * channels + channel]
								+ generated_points[((j + pas) * size + i - pas) * channels + channel]
								+ generated_points[((j + pas) * size + i + pas) * channels + channel];
							temp /= 4;

							generated_points[(j * size + i) * channels + channel] = temp + ((double)rand() / RAND_MAX) * range_factor * 2 - range_factor;

							if (range_attenuation[channel]) {
								if (found_min[channel] > generated_points[(j * size + i) * channels + channel]) found_min[channel] = generated_points[(j * size + i) * channels + channel];
								if (found_max[channel] < generated_points[(j * size + i) * channels + channel]) found_max[channel] = generated_points[(j * size + i) * channels + channel];
							}
						}
						visited_points[j * size + i] = true;
					}
				}
			}
			//SQUARE
			for (int j = 0; j < size; j += pas) {
				for (int i = 0; i < size; i += pas) {
					if (!visited_points[j * size + i]) {
						for (int channel = 0; channel < channels; ++channel) {
							nb_sum = 0;
							temp = 0;

							if (j - pas > -1) {
								temp += generated_points[((j - pas) * size + i) * channels + channel];
								++nb_sum;
							}
							if (i - pas > -1) {
								temp += generated_points[(j * size + i - pas) * channels + channel];
								++nb_sum;
							}
							if (j + pas < size) {
								temp += generated_points[((j + pas) * size + i) * channels + channel];
								++nb_sum;
							}
							else if (seamless_y) {
								if (visited_points[i]) {
									temp += generated_points[i * channels + channel] * (log2((size - 1) / 2) - log2(pas)) * 5;
									nb_sum += static_cast<int>(log2((size - 1) / 2) - log2(pas)) * 5;
								}
							}
							if (i + pas < size) {
								temp += generated_points[(j * size + i + pas) * channels + channel];
								++nb_sum;
							}
							else if (seamless_x) {
								if (visited_points[j]) {
									temp += generated_points[(j * size) * channels + channel] * (log2((size - 1) / 2) - log2(pas)) * 5;
									nb_sum += static_cast<int>(log2((size - 1) / 2) - log2(pas)) * 5;
								}
							}
							temp /= nb_sum;

							generated_points[(j * size + i) * channels + channel] = temp + ((double)rand() / RAND_MAX) * range_factor * 2 - range_factor;

							if (range_attenuation[channel]) {
								if (found_min[channel] > generated_points[(j * size + i) * channels + channel]) found_min[channel] = generated_points[(j * size + i) * channels + channel];
								if (found_max[channel] < generated_points[(j * size + i) * channels + channel]) found_max[channel] = generated_points[(j * size + i) * channels + channel];
							}
						}
						visited_points[j * size + i] = true;
					}
				}
			}
		}

		for (int channel = 0; channel < channels; ++channel) {
			if (range_attenuation[channel]) {
				double real_size_factor = (found_max[channel] - found_min[channel]) / (max_heights[channel] - min_heights[channel]);
				for (int z = 0; z < size * size; ++z) {
					generated_points[z * channels + channel] = (generated_points[z * channels + channel] - found_min[channel]) / real_size_factor + min_heights[channel];
				}
			}
		}

		//print
		//{
		//	for (int channel = 0; channel < channels; ++channel)
		//	{
		//		for (int count = 0; count < size * size; ++count)
		//		{
		//			if (count % size == 0)	std::cout << std::endl << "   |";
		//			std::cout << std::setfill(' ') << std::setw(3) << (double)generated_points[count * channels + channel] << "|";
		//		}
		//		std::cout << std::endl;
		//	}
		//}

		return generated_points;
	}

	template<typename T>
	T* PointsToVertices(T* points, const int sizeX, const int sizeY, const int channels) {
		T* vertices = new T[(sizeX - 1) * (sizeY - 1) * 18]; //y*x*points(3*2)*channels

		for (int j = 0; j < sizeY - 1; ++j) {
			for (int i = 0; i < sizeX - 1; ++i) {
				for (int channel = 0; channel < channels; ++channel) {
					//triangle 1
					vertices[((j + 1) * sizeX + i) * channels * 6 + channel] = points[((j + 1) * sizeX + i) * channels + channel];
					vertices[((j + 1) * sizeX + i + 1) * channels * 6 + channel] = points[(j * sizeX + i) * channels + channel];
					vertices[((j + 1) * sizeX + i + 2) * channels * 6 + channel] = points[((j + 1) * sizeX + i + 1) * channels + channel];
					//triangle 2
					vertices[((j + 1) * sizeX + i + 3) * channels * 6 + channel] = points[((j + 1) * sizeX + i + 1) * channels + channel];
					vertices[((j + 1) * sizeX + i + 4) * channels * 6 + channel] = points[(j * sizeX + i) * channels + channel];
					vertices[((j + 1) * sizeX + i + 5) * channels * 6 + channel] = points[(j * sizeX + i + 1) * channels + channel];
				}
			}
		}

		return vertices;
	}
}