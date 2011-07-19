
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "AudioSample.h"
#include "PracticalSocket.h"


int main(int argc, char **argv)
{
	char usage_str[128] = {0};
	snprintf(usage_str, sizeof(usage_str), "Usage: %s wav_file host port\n", argv[0]);
	bool print_usage = true;

	if(argc == 4)
	{
		std::string file = argv[1] != NULL ? std::string(argv[1]) : "";
		std::string ip = argv[2] != NULL ? std::string(argv[2]) : "";
		unsigned short port = argv[2] != NULL ? atoi(argv[3]) : 0;

		if(!file.empty() && !ip.empty() && (port != 0))
		{
			try
			{
				TCPSocket sock(ip.c_str(), port);

				printf("Audio Client playing [%s] to host [%s] on port [%u]\n", file.c_str(), ip.c_str(), port);

				FILE *pFile = fopen(file.c_str(), "rb");

				if(pFile)
				{
					fseek(pFile, 40, SEEK_SET);

					int32_t data_size = 0;
					fread(&data_size, sizeof(int32_t), 1, pFile);

					AudioSample_t *samples = new AudioSample_t[data_size/sizeof(AudioSample_t)];

					if(samples)
					{
						fseek(pFile, 44, SEEK_SET);
						fread(samples, data_size, 1, pFile);
					}

					fclose(pFile);
					pFile = NULL;

					print_usage = false;
					size_t samples_to_push = SAMPLE_BUFFER_SIZE;

					for(size_t i = 0; i < data_size/sizeof(AudioSample_t); i+=samples_to_push)
					{
						size_t actual_samples_pushed = (i + samples_to_push < data_size/sizeof(AudioSample_t)) ? samples_to_push : data_size/sizeof(AudioSample_t)-i;

						sock.send(&samples[i], actual_samples_pushed*sizeof(AudioSample_t));
						usleep(1000*1000*1);
					}

					delete[] samples;
					samples = NULL;
				}
			}
			catch(SocketException &e)
			{
				printf("Unable to send data...caught exception[%s]\n", e.what());
				print_usage = true;
			}
		}
	}

	if(print_usage)
	{
		printf("%s", usage_str);;
	}

	return 0;
}

