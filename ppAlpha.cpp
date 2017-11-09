#include <iostream>

std::string ppAlpha(std::string source) {
	std::string result = source;

	result[source.length()-1]++;
	for (int i = source.length() - 1; i >= 0; i--) {
		if (result[i] == 'z'+1) {
			result[i] = 'a';

			if (i)
				result[i-1] = result[i-1]+1;
		} else if (result[i] == 'Z'+1) {
			result[i] = 'A';

			if (i)
				result[i-1] = result[i-1]+1;
		} else if (result[i] == '9'+1) {
			result[i] = '0';

			if (i)
				result[i-1] = result[i-1]+1;
		}
	}

	return result;
}